/*========================================
*  Copyright (C) 2015 All rights reserved.
*   Filename：      handle_change_password
*   Author：        Mr.Lu
*   Version：       2.0
*   Date：          2015/7/31
*   Description：	处理密码修改
*   Function List： handle_change_password()
==========================================*/
#include "../../include/myhead.h"

//处理用户修改密码的请求
void handle_change_password(Message msg, int sockfd)
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
	    perror("Login open error!\n");
		exit(1);
	}
	
	sprintf(sql, "select password from user_info where user = '%s'", msg.user);
	if((rc = sqlite3_get_table(db, sql, &result, &row, &column, &errmsg)) != SQLITE_OK)
	{
	    perror("Login get table error!\n");
		exit(1);
	}

	printf("检查当前密码中...\n");
  	if(my_strcmp(msg.message, result[1]) == 0)	//旧密码正确
	{
		bzero(sql, sizeof(sql));
		sprintf(sql, "update user_info set password = '%s' where user = '%s'", msg.password, msg.user);
		sqlite3_exec(db, sql, NULL, NULL, &errmsg);

		back_msg.action = CHANGE_PASSWORD_OK;	
		if(send(sockfd, &back_msg, sizeof(back_msg), 0) == -1)
		{
		    perror("Change password send error!\n");
			exit(1);
		}
	}
	else										//旧密码错误
	{
		back_msg.action = CHANGE_PASSWORD_FAIL;
		if(send(sockfd, &back_msg, sizeof(back_msg), 0) == -1)
		{
		    perror("Change password send error!\n");
			exit(1);
		}
	}

	sqlite3_close(db);
}
