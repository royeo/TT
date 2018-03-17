/*========================================
*  Copyright (C) 2015 All rights reserved.
*   Filename：      handle_modify_nickname
*   Author：        Mr.Lu
*   Version：       2.0
*   Date：          2015/8/3
*   Description：	处理用户修改昵称操作
*   Function List： handle_modify_nickname
==========================================*/
#include "../../include/myhead.h"

void handle_modify_nickname(Message msg, int sockfd)
{
    int i;
	int rc;
	int row;
	int column;
	char *errmsg = NULL;
	char **result = NULL;
	char sql[1024];
	sqlite3 *db;
	Message back_msg;
	bzero(&back_msg, sizeof(back_msg));

	if(sqlite3_open("user_info.db", &db) != SQLITE_OK)
	{
	    perror("Motify nickname sqlite3_open error!\n");
		exit(1);
	}
				
	sprintf(sql, "select user from user_info where user = '%s'", msg.target);
	if((rc = sqlite3_get_table(db, sql, &result, &row, &column, &errmsg)) != SQLITE_OK)
	{
	    perror("Motify nickname sqlite3_get_table error!\n");
		exit(1);
	}

	if(row == 0)   	//昵称可修改
	{
		printf("成功修改昵称:%s！\n", msg.target);
		back_msg.action = MODIFY_NICKNAME_OK;

		sprintf(sql, "update user_info set user = '%s' where user = '%s'", msg.target, msg.user);
		sqlite3_exec(db, sql, NULL, NULL, &errmsg);
	
		my_strcpy(back_msg.user, msg.target);	
		if(send(sockfd, &back_msg, sizeof(back_msg), 0) == -1)
		{
		    perror("Motify nickname send error!\n");
			exit(1);
		}

		//修改在线链表中用户昵称,并发送给所有在线用户
		Client_Info *temp = client_head->next;
		while(temp != NULL)
		{
			if(my_strcmp(temp->user, msg.user) == 0)
			{
				printf("修改在线链表中%s昵称为%s\n", temp->user, msg.target);
				my_strcpy(temp->user, msg.target);
				continue;
			}
			
			bzero(&back_msg, sizeof(back_msg));
			back_msg.action = MODIFY_NICKNAME;
			my_strcpy(back_msg.user, msg.user);
			my_strcpy(back_msg.target, msg.target);

			printf("发送%s修改昵称通知给%s\n", msg.user, temp->user);
			if(send(temp->new_fd, &back_msg, sizeof(back_msg), 0) == -1)
			{
				perror("Modify nickname error!\n");
				exit(1);
			}

			temp = temp->next;
		}

		//修改保存聊天记录数据库中的名字
		bzero(sql, sizeof(sql));
    	if(sqlite3_get_table(db, "select user from user_info", &result, &row, &column, &errmsg) != SQLITE_OK)
		{
		    perror("Modify nickname sqlite3_get_table error!\n");
			exit(1);
		}
		sqlite3_close(db);
		
		if(sqlite3_open("MsgEx.db", &db) != SQLITE_OK)
		{
		    perror("Modify nickname MsgEx sqlite3_open error!\n");
			exit(1);
		}

		bzero(sql, sizeof(sql));   	//先修改当前修改昵称用户的聊天记录数据库
		printf("msg.user = %s, msg.target = %s\n", msg.user, msg.target);
		sprintf(sql, "alter table user_%s rename to user_%s", msg.user, msg.target);
		printf("sql = %s\n", sql);
		if(sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK)
		{
		    perror("Modify nickname alter sqlite3_exec error!\n");
			exit(1);
		}

		for(i = 1; i <= row; i++)	//修改每个好友聊天记录里的对象
		{
    		if(my_strcmp(result[i], msg.target) == 0)
			{
				continue;
			}

		    bzero(sql, sizeof(sql));
			sprintf(sql, "update user_%s set target = '%s' where target = '%s'", result[i], msg.target, msg.user);
			printf("sql = %s\n", sql);
			if(sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK)
			{
			    perror("Modify nickname updata sqlite3_exec error!\n");
				exit(1);
			}
		}

	}
	else			//昵称已存在
	{
		printf("昵称已存在,修改失败！\n");
		back_msg.action = MODIFY_NICKNAME_FAIL;
		
		if(send(sockfd, &back_msg, sizeof(back_msg), 0) == -1)
		{
		    perror("Motify nickname send error!\n");
			exit(1);
		}
	}

	sqlite3_close(db);
}
