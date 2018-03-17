/*========================================
*  Copyright (C) 2015 All rights reserved.
*   Filename：      handle_message_record
*   Author：        Mr.Lu
*   Version：       2.0
*   Date：          2015/7/29
*   Description：	处理聊天记录
*   Function List： save_message_record()
*					delete_message_record()  
==========================================*/
#include "../../include/myhead.h"

//获取当前日期
void get_current_data(char *tm)
{
	struct tm *now;
	long t;
	t = time(NULL);
	now = localtime(&t);

	sprintf(tm, "%d-%02d-%02d", now->tm_year+1900, now->tm_mon + 1,now->tm_mday);
}

//用户请求查看好友聊天记录
void handle_check_record(Message msg, int sockfd)
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
    	perror("Check message record sqlite3_open error!\n");
		exit(1);
	}	
	
	bzero(sql, sizeof(sql));
	sprintf(sql, "select data, message_record from user_%s where target = '%s'", msg.user, msg.target);
	if((rc = sqlite3_get_table(db, sql, &result, &row, &column, &errmsg)) != SQLITE_OK)
	{
	    perror("Check message record sqlite3_get_table error!\n");
		exit(1);
	}

	printf("聊天记录：\n");
	for(i = 1; i <= row; i++)
	{
		bzero(&back_msg, sizeof(back_msg));
		back_msg.action = SEND_MSG_RECORD;
		my_strcpy(back_msg.target, msg.target);
		my_strcpy(back_msg.time, result[i * column]);
		my_strcpy(back_msg.message, result[i * column + 1]);
		
		printf("%s\n", back_msg.message);
		if(send(sockfd, &back_msg, sizeof(back_msg), 0) == -1)
		{
			perror("Send message record error!\n");
			exit(1);
		}
	}

	bzero(&back_msg, sizeof(back_msg));
	back_msg.action = MSG_RECORD_OVER;
	if(send(sockfd, &back_msg, sizeof(back_msg), 0) == -1)
	{
	    perror("Send message send error!\n");
		exit(1);
	}
	sqlite3_close(db);
}

//用户请求删除好友聊天记录
void handle_delete_record(Message msg, int sockfd)
{
	char *errmsg = NULL;
	char sql[1024];
	sqlite3 *db;      
	Message back_msg;
	
	if(sqlite3_open("MsgEx.db", &db) != SQLITE_OK)
	{
    	perror("Delete message record sqlite3_open error!\n");
		exit(1);
	}	
	
	bzero(sql, sizeof(sql));
	sprintf(sql, "delete from user_%s where target = '%s'", msg.user, msg.target); 
	if(sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK)
	{
		perror("Delete message record sqlite3_exec error!\n");
		exit(1);
	}

	bzero(&back_msg, sizeof(back_msg));
	back_msg.action = DEL_MSG_RECORD;

	if(send(sockfd, &back_msg, sizeof(back_msg), 0) == -1)
	{
		perror("Delete message record send error!\n");
		exit(1);
	}
	sqlite3_close(db);
}

//保存好友聊天记录
void save_message_record(Message msg)
{
	char *errmsg = NULL;
	char sql[1024];
	char data[20];
	sqlite3 *db;         

	if(sqlite3_open("MsgEx.db", &db) != SQLITE_OK)
	{
    	perror("Save message record sqlite3_open error!\n");
		exit(1);
	}	
	
	//保存到发送者数据库
	bzero(sql, sizeof(sql));
	get_current_data(data);
	sprintf(sql, "insert into user_%s(target, data, message_record) values('%s', '%s', '%s %s\n%s')", 
								msg.user, msg.target, data, msg.user, msg.time, msg.message);
	
	if(sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK)
	{
		perror("Save message record sqlite3_exec error!\n");
		exit(1);
	}
	
	//保存到接受者数据库
	bzero(sql, sizeof(sql));
	sprintf(sql, "insert into user_%s(target, data, message_record) values('%s', '%s', '%s %s\n%s')", 
								msg.target, msg.user, data, msg.user, msg.time, msg.message);
	
	if(sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK)
	{
		perror("Save message record sqlite3_exec error!\n");
		exit(1);
	}

	sqlite3_close(db);
}






//用户请求查看群组聊天记录
void handle_check_group_record(Message msg, int sockfd)
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
    	perror("Check group message record sqlite3_open error!\n");
		exit(1);
	}	
	
	bzero(sql, sizeof(sql));
	sprintf(sql, "select data, message_record from group_chat");
	if((rc = sqlite3_get_table(db, sql, &result, &row, &column, &errmsg)) != SQLITE_OK)
	{
	    perror("Check group message record sqlite3_get_table error!\n");
		exit(1);
	}

	printf("聊天记录：\n");
	for(i = 1; i <= row; i++)
	{
		bzero(&back_msg, sizeof(back_msg));
		back_msg.action = SEND_GROUP_MSG_RECORD;
		my_strcpy(back_msg.time, result[i * column]);
		my_strcpy(back_msg.message, result[i * column + 1]);
		
		printf("%s\n", back_msg.message);
		if(send(sockfd, &back_msg, sizeof(back_msg), 0) == -1)
		{
			perror("Send message record error!\n");
			exit(1);
		}
	}

	bzero(&back_msg, sizeof(back_msg));
	back_msg.action = MSG_RECORD_OVER;
	if(send(sockfd, &back_msg, sizeof(back_msg), 0) == -1)
	{
	    perror("Send group message send error!\n");
		exit(1);
	}

	sqlite3_close(db);
}

//保存群聊记录
void save_group_msg_record(Message msg)
{
	char *errmsg = NULL;
	char sql[1024];
	char data[20];
	sqlite3 *db;         

	if(sqlite3_open("MsgEx.db", &db) != SQLITE_OK)
	{
    	perror("Save group message record sqlite3_open error!\n");
		exit(1);
	}	
	
	//保存到数据库
	bzero(sql, sizeof(sql));
	get_current_data(data);
	sprintf(sql, "insert into group_chat(data, message_record) values('%s', '%s %s\n%s')", data, msg.user, msg.time, msg.message);
	if(sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK)
	{
		perror("Save group message record sqlite3_exec error!\n");
		exit(1);
	}
	
	sqlite3_close(db);
}

