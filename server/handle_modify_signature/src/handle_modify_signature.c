/*==================================
*  Copyright (C) 2015 All rights reserved.
*   Filename：      handle_modify_signature.c
*   Author：        Mr.Lu
*   Version：       2.0
*   Date：          2015/07/23
*   Description：	登录处理
*   Function List： handle_log()
*					handle_modify_signature()
==========================================*/

#include "../../include/myhead.h"

//保存用户个性签名
void handle_modify_signature(Message msg, int sockfd)
{
	char *errmsg = NULL;
	char sql[1024];
	sqlite3 *db;  

	//将注册信息保存到数据库
	if(sqlite3_open("user_info.db", &db) != SQLITE_OK)
	{
	    perror("Open error!\n");
		exit(1);
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
		my_strcpy(back_msg.target, msg.user);
		my_strcpy(back_msg.signature, msg.signature);

		printf("发送给用户%s : %s的个性签名修改为%s\n", temp->user, back_msg.target, back_msg.signature);
		if(send(temp->new_fd, &back_msg, sizeof(back_msg), 0) == -1)
		{
	    	perror("Modify signature send error!\n");
			exit(1);
		}
		temp = temp->next;
	}
}
