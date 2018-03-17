/*==================================
*  Copyright (C) 2015 All rights reserved.
*   Filename：      handle_reg.c
*   Author：        Mr.Lu
*   Version：       1.0
*   Date：          2015/07/16
*   Description：	注册处理
*   Function List： void handle_reg()
* 
==========================================*/

#include "../../include/myhead.h"

//整型转字符串
char *my_itoa(int num, char *str)
{
	int i;
	int len;
	char ch;
	char *temp = str;

	while(num > 0)
	{
	    *temp = num % 10 + '0';
		num /= 10;
		temp++;
	}
	
	len = strlen(str);
	temp = str;

	for(i = 0; i < len / 2; i++)
	{
	    ch = *(temp + i);
		*(temp + i) = *(temp + len - 1 - i);
		*(temp + len - 1 - i) = ch;
	}

	temp[len] = '\0';

	return str;
}

//处理注册信息
void handle_reg(Message msg, int sockfd)
{
	char *errmsg = NULL;
	char sql[1024];
	sqlite3 *db;  
	Message back_msg;
	bzero(&back_msg, sizeof(back_msg));

	//将注册信息保存到数据库
	if(sqlite3_open("user_info.db", &db) != SQLITE_OK)
	{
	    perror("Open error!\n");
		exit(1);
	}

	srand((unsigned)time(NULL));
	int num = rand();            //生成一个随机的ID
	my_itoa(num, back_msg.id);  
	printf("生成id：%s\n", back_msg.id);
	
	if(my_strlen(msg.question) > 0 && my_strlen(msg.answer) > 0)  
	{
		printf("正在保存密保问题...\n");
		memset(sql, 0, sizeof(sql));
		sprintf(sql, "insert into user_info(id, user, password, question, answer) values('%s', '%s', '%s','%s', '%s')", back_msg.id, msg.user, msg.password, msg.question, msg.answer);
		sqlite3_exec(db, sql, NULL, NULL, &errmsg);
	}
	else
	{
		memset(sql, 0, sizeof(sql));
		sprintf(sql, "insert into user_info(id, user, password) values('%s', '%s', '%s')", back_msg.id, msg.user, msg.password);
		sqlite3_exec(db, sql, NULL, NULL, &errmsg);
	}
	sqlite3_close(db);

	//为用户在数据库中创建一个表来保存聊天记录
	if(sqlite3_open("MsgEx.db", &db) != SQLITE_OK)
	{
    	perror("Create message record table sqlite3_open error!\n");
		exit(1);
	}	
	
	bzero(sql, sizeof(sql));
	sprintf(sql, "create table user_%s(target text, data text, message_record text)", msg.user);
	sqlite3_exec(db, sql, NULL, NULL, &errmsg);


	back_msg.action = REGIST_OK;
	if(send(sockfd, &back_msg, sizeof(back_msg), 0) == -1)
	{
	    perror("Regist send error!\n");
		exit(1);
	}

	sqlite3_close(db);

	//发送给每一个在线用户新好友注册的信息
	Client_Info *temp = client_head->next;
	while(temp != NULL)
	{
		bzero(&back_msg, sizeof(back_msg));
		back_msg.action = FRIEND_STATE;
		back_msg.state = FRIEND_OFFLINE;
		my_strcpy(back_msg.user, msg.user);
	
		if(send(temp->new_fd, &back_msg, sizeof(back_msg), 0) == -1)
		{
		    perror("New friend send error!\n");
			exit(1);
		}

		temp = temp->next;
	}
}

	
