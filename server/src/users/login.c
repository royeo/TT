#include "../../include/myhead.h"

// 检查用户帐号是否存在
void check_account(Message msg, int sockfd)
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

	if(sqlite3_open("user_info.db", &db) != SQLITE_OK)
	{
	    error("Check account exist sqlite3_open error!");
	}
				
	sprintf(sql, "select user from user_info where user = '%s'", msg.user);
	if((rc = sqlite3_get_table(db, sql, &result, &row, &column, &errmsg)) != SQLITE_OK)
	{
	    error("Check account exist sqlite3_get_table error!");
	}

	if(row == 0)
	{
		back_msg.action = ACCOUNT_NOT_EXIST;
		
		if(send(sockfd, &back_msg, sizeof(back_msg), 0) == -1)
		{
		    error("Check account send error!");
		}
	}
	else
	{
		back_msg.action = ACCOUNT_EXIST;
		
		if(send(sockfd, &back_msg, sizeof(back_msg), 0) == -1)
		{
		    error("Check account send error!");
		}
	}
}

//处理用户修改密码的请求
void change_password(Message msg, int sockfd)
{
	int rc;
	int row;
	int column;
	char *errmsg = NULL;
	char **result = NULL;
	char sql[1024];
	sqlite3 *db;   
	Message back_msg;

	//检查当前密码是否正确
	if(sqlite3_open("user_info.db", &db) != SQLITE_OK)
	{
	    error("Login open error!");
	}
	
	sprintf(sql, "select password from user_info where user = '%s'", msg.user);
	if((rc = sqlite3_get_table(db, sql, &result, &row, &column, &errmsg)) != SQLITE_OK)
	{
	    error("Login get table error!");
	}

	printf("检查当前密码中...\n");
  	if(strcmp(msg.message, result[1]) == 0)	//旧密码正确
	{
		bzero(sql, sizeof(sql));
		sprintf(sql, "update user_info set password = '%s' where user = '%s'", msg.password, msg.user);
		sqlite3_exec(db, sql, NULL, NULL, &errmsg);

		back_msg.action = CHANGE_PASSWORD_OK;	
		if(send(sockfd, &back_msg, sizeof(back_msg), 0) == -1)
		{
		    error("Change password send error!");
		}
	}
	else										//旧密码错误
	{
		back_msg.action = CHANGE_PASSWORD_FAIL;
		if(send(sockfd, &back_msg, sizeof(back_msg), 0) == -1)
		{
		    error("Change password send error!");
		}
	}

	sqlite3_close(db);
}


