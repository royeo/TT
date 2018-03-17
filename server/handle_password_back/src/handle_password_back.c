/*==================================
*  Copyright (C) 2015 All rights reserved.
*   Filename：      handle_password_back.c
*   Author：        Mr.Lu
*   Version：       1.0
*   Date：          2015/7/24
*   Description：	处理用户找回密码的请求
*   Function List： handle_password_back()
==========================================*/
#include "../../include/myhead.h"

//处理用户找回密码的请求
void handle_password_back(Message msg, int sockfd)
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
	    perror("Password back sqlite3_open error!\n");
		exit(1);
	}
	
	sprintf(sql, "select password, question, answer from user_info where id = '%s' or user = '%s'", msg.user, msg.user);
	if((rc = sqlite3_get_table(db, sql, &result, &row, &column, &errmsg)) != SQLITE_OK)
	{
	    perror("Password back sqlite3_get_table error!\n");
		exit(1);
	}

	if(result[4] == NULL)
	{
		printf("尚未设置密保问题\n");
		back_msg.action = PASSWORD_BACK_FAIL;     
		
		if(send(sockfd, &back_msg, sizeof(back_msg), 0) == -1)
		{
		    perror("Password back send error!\n");
			exit(1);
		}
	}
	else
	{
		printf("搜索到用户密保问题！\n");
		back_msg.action = PASSWORD_BACK_OK;
		my_strcpy(back_msg.password, result[3]);
		my_strcpy(back_msg.question, result[4]);
		my_strcpy(back_msg.answer, result[5]);

		if(send(sockfd, &back_msg, sizeof(back_msg), 0) == -1)
		{
		    perror("Password back send error!\n");
			exit(1);
		}
    }
}
