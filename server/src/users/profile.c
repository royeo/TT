#include "../../../include/myhead.h"

// 修改用户昵称
void modify_nickname(Message msg, int sockfd)
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
	    error("Motify nickname sqlite3_open error!");
	}
				
	sprintf(sql, "select user from user_info where user = '%s'", msg.target);
	if((rc = sqlite3_get_table(db, sql, &result, &row, &column, &errmsg)) != SQLITE_OK)
	{
	    error("Motify nickname sqlite3_get_table error!");
	}

	if(row == 0)   	//昵称可修改
	{
		printf("成功修改昵称:%s！\n", msg.target);
		back_msg.action = MODIFY_NICKNAME_OK;

		sprintf(sql, "update user_info set user = '%s' where user = '%s'", msg.target, msg.user);
		sqlite3_exec(db, sql, NULL, NULL, &errmsg);
	
		strcpy(back_msg.user, msg.target);	
		if(send(sockfd, &back_msg, sizeof(back_msg), 0) == -1)
		{
		    error("Motify nickname send error!");
		}

		//修改在线链表中用户昵称,并发送给所有在线用户
		Client_Info *temp = client_head->next;
		while(temp != NULL)
		{
			if(strcmp(temp->user, msg.user) == 0)
			{
				printf("修改在线链表中%s昵称为%s\n", temp->user, msg.target);
				strcpy(temp->user, msg.target);
				continue;
			}
			
			bzero(&back_msg, sizeof(back_msg));
			back_msg.action = MODIFY_NICKNAME;
			strcpy(back_msg.user, msg.user);
			strcpy(back_msg.target, msg.target);

			printf("发送%s修改昵称通知给%s\n", msg.user, temp->user);
			if(send(temp->new_fd, &back_msg, sizeof(back_msg), 0) == -1)
			{
				error("Modify nickname error!");
			}

			temp = temp->next;
		}

		//修改保存聊天记录数据库中的名字
		bzero(sql, sizeof(sql));
    	if(sqlite3_get_table(db, "select user from user_info", &result, &row, &column, &errmsg) != SQLITE_OK)
		{
		    error("Modify nickname sqlite3_get_table error!");
		}
		sqlite3_close(db);
		
		if(sqlite3_open("MsgEx.db", &db) != SQLITE_OK)
		{
		    error("Modify nickname MsgEx sqlite3_open error!");
		}

		bzero(sql, sizeof(sql));   	//先修改当前修改昵称用户的聊天记录数据库
		printf("msg.user = %s, msg.target = %s\n", msg.user, msg.target);
		sprintf(sql, "alter table user_%s rename to user_%s", msg.user, msg.target);
		printf("sql = %s\n", sql);
		if(sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK)
		{
		    error("Modify nickname alter sqlite3_exec error!");
		}

		for(i = 1; i <= row; i++)	//修改每个好友聊天记录里的对象
		{
    		if(strcmp(result[i], msg.target) == 0)
			{
				continue;
			}

		    bzero(sql, sizeof(sql));
			sprintf(sql, "update user_%s set target = '%s' where target = '%s'", result[i], msg.target, msg.user);
			printf("sql = %s\n", sql);
			if(sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK)
			{
			    error("Modify nickname updata sqlite3_exec error!");
			}
		}

	}
	else			//昵称已存在
	{
		printf("昵称已存在,修改失败！\n");
		back_msg.action = MODIFY_NICKNAME_FAIL;
		
		if(send(sockfd, &back_msg, sizeof(back_msg), 0) == -1)
		{
		    error("Motify nickname send error!");
		}
	}

	sqlite3_close(db);
}

// 修改用户个性签名
void modify_signature(Message msg, int sockfd)
{
	char *errmsg = NULL;
	char sql[1024];
	sqlite3 *db;  

	//将注册信息保存到数据库
	if(sqlite3_open("user_info.db", &db) != SQLITE_OK)
	{
	    error("Open error!");
	}

	//保存个性签名
	memset(sql, 0, sizeof(sql));
	sprintf(sql, "update user_info set signature = '%s' where user = '%s'", msg.signature, msg.user);
	printf("修改的个性签名：%s\n", msg.signature);
	printf("修改个性签名成功！\n");
	sqlite3_exec(db, sql, NULL, NULL, &errmsg);
	sqlite3_close(db);

	//发送给每个在线用户
	Message back_msg;
	Client_Info *temp = client_head->next;

	while(temp != NULL)
	{
		if(temp->new_fd == sockfd)
		{
		    temp = temp->next;
			continue;
		}

		bzero(&back_msg, sizeof(back_msg));
		back_msg.action = MODIFY_SIGNATURE;
		strcpy(back_msg.target, msg.user);
		strcpy(back_msg.signature, msg.signature);

		printf("发送给用户%s : %s的个性签名修改为%s\n", temp->user, back_msg.target, back_msg.signature);
		if(send(temp->new_fd, &back_msg, sizeof(back_msg), 0) == -1)
		{
	    	error("Modify signature send error!");
		}
		temp = temp->next;
	}
}

//找回用户密码
void password_back(Message msg, int sockfd)
{
	int rc;
	int row;
	int column;
	char *errmsg = NULL;
	char **result = NULL;
	char sql[1024];
	sqlite3 *db; 
	Message back_msg;
	bzero(&back_msg, sizeof(back_msg));

	//搜索密保问题
	if(sqlite3_open("user_info.db", &db) != SQLITE_OK)
	{
	    error("Password back sqlite3_open error!");
	}
	
	sprintf(sql, "select password, question, answer from user_info where id = '%s' or user = '%s'", msg.user, msg.user);
	if((rc = sqlite3_get_table(db, sql, &result, &row, &column, &errmsg)) != SQLITE_OK)
	{
	    error("Password back sqlite3_get_table error!");
	}

	if(result[4] == NULL)
	{
		printf("尚未设置密保问题\n");
		back_msg.action = PASSWORD_BACK_FAIL;     
		
		if(send(sockfd, &back_msg, sizeof(back_msg), 0) == -1)
		{
		    error("Password back send error!");
		}
	}
	else
	{
		printf("搜索到用户密保问题！\n");
		back_msg.action = PASSWORD_BACK_OK;
		strcpy(back_msg.password, result[3]);
		strcpy(back_msg.question, result[4]);
		strcpy(back_msg.answer, result[5]);

		if(send(sockfd, &back_msg, sizeof(back_msg), 0) == -1)
		{
		    error("Password back send error!");
		}
    }
}
