/*========================================
*  Copyright (C) 2015 All rights reserved.
*   Filename：      handle_group_msg
*   Author：        Mr.Lu
*   Version：       2.0
*   Date：          2015/8/6
*   Description：	处理群组信息
*   Function List： handle_group_msg()
==========================================*/
#include "../../../include/myhead.h"

//处理群组聊天信息
void handle_group_msg(Message msg, int sockfd)
{
    int i;
	int row;
	int column;
	int user_mark = 0;
	char *errmsg = NULL;
	char **result = NULL;
	char sql[1024];
	sqlite3 *db;
	Message back_msg;
	Client_Info *temp = client_head->next;
	
	//发送给所有用户
	if(sqlite3_open("user_info.db", &db) != SQLITE_OK)
	{
	    error("Motify nickname sqlite3_open error!");
	}

	if(sqlite3_get_table(db, "select user from user_info", &result, &row, &column, &errmsg) != SQLITE_OK)
	{
	    error("Handle group chat sqlite3_get_table error!");
	}

	for(i = 1; i <= row; i++)
	{
		user_mark = 0;

		if(strcmp(result[i], msg.user) == 0)
		{
		    continue;
		}

		//发送给在线用户
		while(temp != NULL)
		{
	    	if(strcmp(temp->user, result[i]) == 0)
			{
				bzero(&back_msg, sizeof(back_msg));
				back_msg.action = SEND_GROUP_MSG;
				strcpy(back_msg.user, msg.user);
				strcpy(back_msg.message, msg.message);
				get_current_time(back_msg.time);

				printf("发送群聊信息给%s\n", result[i]);
				if(send(temp->new_fd, &back_msg, sizeof(back_msg), 0) == -1)
				{
	  		    	error("Handle group msg error!");
				}

				user_mark = 1;
				break;
			}
			temp = temp->next;
		}

		//用户离线,保存离线消息
		if(!user_mark)
		{
			Offline_Group_Msg *offline_msg;	
			offline_msg = (Offline_Group_Msg *)malloc(sizeof(Offline_Group_Msg));
			strcpy(offline_msg->user, msg.user);
			strcpy(offline_msg->target, result[i]);
			get_current_data_time(offline_msg->time);
			strcpy(offline_msg->message, msg.message);
			insert_offline_group_msg(&offline_msg);
		}
	}

	//保存聊天记录
	get_current_time(msg.time);
	save_group_msg_record(msg);
	
	//回传客户端发送群聊消息成功
	bzero(&back_msg, sizeof(back_msg));
	back_msg.action = SEND_GROUP_MSG_OK;
	get_current_time(back_msg.time);
	strcpy(back_msg.user, msg.user);
	strcpy(back_msg.message, msg.message);
	if(send(sockfd, &back_msg, sizeof(back_msg), 0) == -1)
	{
	    error("Send group msg error!");
	}
	printf("发送群聊信息成功！\n");
}


//处理用户获取群通知的操作
void handle_group_notice(Message msg, int sockfd)
{
	int rc;
	int row;
	int column;
	char *errmsg = NULL;
	char **result = NULL;
	char sql[1024];
	sqlite3 *db;
	Message back_msg;
	
	if(sqlite3_open("MsgEx.db", &db) != SQLITE_OK)
	{
    	error("Get group notice sqlite3_open error!");
	}	
	
	bzero(sql, sizeof(sql));
	sprintf(sql, "select notice from group_notice");
	if((rc = sqlite3_get_table(db, sql, &result, &row, &column, &errmsg)) != SQLITE_OK)
	{
	    error("Get group notice sqlite3_get_table error!");
	}

	bzero(&back_msg, sizeof(back_msg));
	back_msg.action = GROUP_NOTICE;
	
	if(row != 0)
	{
	    strcpy(back_msg.message, result[1]);
	}
	printf("群通知: %s\n", back_msg.message);
	
	if(send(sockfd, &back_msg, sizeof(back_msg), 0) == -1)
	{
	    error("Get group notice send error!");
	}

	sqlite3_close(db);
}

//处理用户修改群通知的操作
void handle_change_group_notice(Message msg, int sockfd)
{
	char *errmsg = NULL;
	char sql[1024];
	sqlite3 *db;
	Message back_msg;
	
	if(sqlite3_open("MsgEx.db", &db) != SQLITE_OK)
	{
    	error("Change group notice sqlite3_open error!");
	}	

	//删除原来的群通知
	bzero(sql, sizeof(sql));
	sprintf(sql, "delete from group_notice");
	if((sqlite3_exec(db, sql, NULL, NULL, &errmsg)) != SQLITE_OK)
	{
	    error("Change group notice sqlite3_exec error!");
	}
	
	//插入新的群通知
	bzero(sql, sizeof(sql));
	sprintf(sql, "insert into group_notice values('%s')", msg.message);
	if((sqlite3_exec(db, sql, NULL, NULL, &errmsg)) != SQLITE_OK)
	{
	    error("Change group notice sqlite3_exec error!");
	}

	bzero(&back_msg, sizeof(back_msg));
	back_msg.action = GROUP_NOTICE;
	strcpy(back_msg.message, msg.message);
	printf("修改群通知: %s\n", back_msg.message);

	Client_Info *temp = client_head->next;

	//发送给每个在线用户
	while(temp != NULL)
	{
		if(send(temp->new_fd, &back_msg, sizeof(back_msg), 0) == -1)
		{
	    	error("Change group notice send error!");
		}
		
		temp = temp->next;
	}

	sqlite3_close(db);
}

//处理用户开启匿名聊天的操作
void open_anonymity(Message msg, int sockfd)
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
	
	if(sqlite3_open("MsgEx.db", &db) != SQLITE_OK)
	{
    	error("Get group anonymity sqlite3_open error!");
	}	
	
	bzero(sql, sizeof(sql));
	sprintf(sql, "select * from group_anonymity");
	if((rc = sqlite3_get_table(db, sql, &result, &row, &column, &errmsg)) != SQLITE_OK)
	{
	    error("Get group anonymity sqlite3_get_table error!");
	}

	srand((unsigned)time(NULL));
	int num; 
	
	bzero(&back_msg, sizeof(back_msg));
	
	while(1)
	{
		num = rand() % row + 1;     
		if(atoi(result[num * column + 1]) ==  0)
		{
		    strcpy(back_msg.user, result[num * column]);

			bzero(sql, sizeof(sql));
			sprintf(sql, "update group_anonymity set state = 1 where anonymity = '%s'", back_msg.user);
			if((sqlite3_exec(db, sql, NULL, NULL, &errmsg)) != SQLITE_OK)
			{
	    		error("Get group anonymity sqlite3_exec error!");
			}
			printf("获得匿名:%s\n", result[num * column]);
			break;
		}
	}
	
	back_msg.action = OPEN_ANONYMITY;
	if(send(sockfd, &back_msg, sizeof(back_msg), 0) == -1)
	{
	    error("Get group anonymity send error!");
	}

	sqlite3_close(db);
}

//处理用户关闭匿名聊天的操作
void handle_close_anonymity(Message msg, int sockfd)
{
	char *errmsg = NULL;
	char sql[1024];
	sqlite3 *db;
	Message back_msg;
	
	if(sqlite3_open("MsgEx.db", &db) != SQLITE_OK)
	{
    	error("Close group anonymity sqlite3_open error!");
	}	
	
	bzero(sql, sizeof(sql));
	sprintf(sql, "update group_anonymity set state = 0 where anonymity = '%s'", msg.user);
	if((sqlite3_exec(db, sql, NULL, NULL, &errmsg)) != SQLITE_OK)
	{
   		error("Close group anonymity sqlite3_exec error!");
	}

	bzero(&back_msg, sizeof(back_msg));
	back_msg.action = CLOSE_ANONYMITY;
	if(send(sockfd, &back_msg, sizeof(back_msg), 0) == -1)
	{
	    error("Close group anonymity send error!");
	}

	sqlite3_close(db);
}



