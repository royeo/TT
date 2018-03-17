/*==================================
*  Copyright (C) 2015 All rights reserved.
*   Filename：      handle_account.c
*   Author：        Mr.Lu
*   Version：       1.0
*   Date：          2015/7/22
*   Description：	检查帐号是否存在
*   Function List： handle_account()
==========================================*/
#include "../../include/myhead.h"

//检查帐号是否存在
void handle_account(Message msg, int sockfd)
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
	    perror("Check account exist sqlite3_open error!\n");
		exit(1);
	}
				
	sprintf(sql, "select user from user_info where user = '%s'", msg.user);
	if((rc = sqlite3_get_table(db, sql, &result, &row, &column, &errmsg)) != SQLITE_OK)
	{
	    perror("Check account exist sqlite3_get_table error!\n");
		exit(1);
	}

	if(row == 0)
	{
		back_msg.action = ACCOUNT_NOT_EXIST;
		
		if(send(sockfd, &back_msg, sizeof(back_msg), 0) == -1)
		{
		    perror("Check account send error!\n");
			exit(1);
		}
	}
	else
	{
		back_msg.action = ACCOUNT_EXIST;
		
		if(send(sockfd, &back_msg, sizeof(back_msg), 0) == -1)
		{
		    perror("Check account send error!\n");
			exit(1);
		}
	}
}
