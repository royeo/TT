/*========================================
*  Copyright (C) 2015 All rights reserved.
*   Filename：      handle_chat
*   Author：        Mr.Lu
*   Version：       2.0
*   Date：          2015/7/17
*   Description：	处理聊天信息
*   Function List： void handle_chat()
==========================================*/
#include "../../include/myhead.h"

//获取当前时间
void get_current_time(char *tm)
{
	struct tm *now;
	long t;
	t = time(NULL);
	now = localtime(&t);

//	sprintf(tm, "%d/%02d/%02d %02d:%02d", now->tm_year+1900, now->tm_mon + 1,now->tm_mday, now->tm_hour, now->tm_min);
	sprintf(tm, "%02d:%02d:%02d", now->tm_hour, now->tm_min, now->tm_sec);
}

//获取当前时间日期
void get_current_data_time(char *tm)
{
	struct tm *now;
	long t;
	t = time(NULL);
	now = localtime(&t);

	sprintf(tm, "%d/%02d/%02d %02d:%02d", now->tm_year+1900, now->tm_mon + 1,now->tm_mday, now->tm_hour, now->tm_min);
}
//处理聊天信息
void handle_chat(Message msg, int sockfd)
{
	int send_msg_mark = EMPTY;
	Message back_msg;
	Client_Info *temp = client_head->next;

			printf("%s对%s说：%s\n", msg.user, msg.target, msg.message);
	//搜索目标客户端套接字
	while(temp != NULL)
	{
	    if(my_strcmp(temp->user, msg.target) == 0)
		{
			//发送信息给目标客户端
			bzero(&back_msg, sizeof(back_msg));
			back_msg.action = CHAT;
			my_strcpy(back_msg.user, msg.user);
			my_strcpy(back_msg.target, msg.target);
			my_strcpy(back_msg.message, msg.message);
			get_current_time(back_msg.time);

			printf("user = %s\n", back_msg.user);
			printf("target = %s\n", back_msg.target);
			printf("time = %s\n", back_msg.time);
			printf("message = %s\n", back_msg.message);

			if(send(temp->new_fd, &back_msg, sizeof(back_msg), 0) == -1)
			{
	  		    perror("Chat send msg error!\n");
				exit(1);
			}
			printf("发送消息成功！\n");
			send_msg_mark = SEND_MSG_OK;
			break;
		}
		temp = temp->next;
	}

	//用户离线,保存离线消息
	if(send_msg_mark != SEND_MSG_OK)
	{
		Offline_Msg *offline_msg;	
		offline_msg = (Offline_Msg *)malloc(sizeof(Offline_Msg));
		my_strcpy(offline_msg->user, msg.user);
		my_strcpy(offline_msg->target, msg.target);
		get_current_data_time(offline_msg->time);
		my_strcpy(offline_msg->message, msg.message);
		insert_offline_msg(&offline_msg);
	}

	//保存聊天记录
	get_current_time(msg.time);
	save_message_record(msg);
	
	//回传客户端发送消息成功
	bzero(&back_msg, sizeof(back_msg));
	back_msg.action = SEND_MSG_OK;
	get_current_time(back_msg.time);
	my_strcpy(back_msg.user, msg.user);
	my_strcpy(back_msg.message, msg.message);
	if(send(sockfd, &back_msg, sizeof(back_msg), 0) == -1)
	{
	    perror("Chat send msg error!\n");
		exit(1);
	}
}

