/*========================================
*  Copyright (C) 2015 All rights reserved.
*   Filename：      handle_send_file
*   Author：        Mr.Lu
*   Version：       2.0
*   Date：          2015/8/3
*   Description：	处理用户发送文件操作
*   Function List： handle_send_file()
==========================================*/
#include "../../include/myhead.h"

void handle_send_file(Message msg, int sockfd)
{
	int send_file_mark = EMPTY;
	Message back_msg;
	Client_Info *temp = client_head->next;

	//搜索目标用户套接字
	while(temp != NULL)
	{
	    if(my_strcmp(temp->user, msg.target) == 0)
		{
			//用户在线,发送文件给目标客户端
			bzero(&back_msg, sizeof(back_msg));
			
			back_msg.action = SEND_FILE;
			my_strcpy(back_msg.user, msg.user);
			my_strcpy(back_msg.target, msg.target);
			my_strcpy(back_msg.file_name, msg.file_name);
			my_strcpy(back_msg.message, msg.message);
			get_current_time(back_msg.time);

			if(send(temp->new_fd, &back_msg, sizeof(back_msg), 0) == -1)
			{
	  		    perror("Send file error!\n");
				exit(1);
			}
			printf("发送文件成功！\n");
			send_file_mark = SEND_FILE_OK;
			break;
		}
		temp = temp->next;
	}

	//用户不在线,无法发送文件
	if(send_file_mark != SEND_FILE_OK)
	{
	/*	Offline_File *new_offline_file;
		new_offline_file = (Offline_File *)malloc(sizeof(Offline_File));
		get_current_data_time(new_offline_file->time);
		my_strcpy(new_offline_file->user, msg.user);
		my_strcpy(new_offline_file->target, msg.target);
		my_strcpy(new_offline_file->file_name, msg.file_name);
		my_strcpy(new_offline_file->file_content, msg.message);
		insert_offline_file(&new_offline_file);	 */

		bzero(&back_msg, sizeof(back_msg));
		back_msg.action = SEND_FILE_FAIL;
		get_current_time(back_msg.time);
		my_strcpy(back_msg.file_name, msg.file_name);
		if(send(sockfd, &back_msg, sizeof(back_msg), 0) == -1)
		{
		    perror("Send file error!\n");
			exit(1);
		}
	}
	else
	{
		//回传客户端发送文件成功
		bzero(&back_msg, sizeof(back_msg));
		back_msg.action = SEND_FILE_OK;
		get_current_time(back_msg.time);
		my_strcpy(back_msg.file_name, msg.file_name);
		if(send(sockfd, &back_msg, sizeof(back_msg), 0) == -1)
		{
		    perror("Send file error!\n");
			exit(1);
		}
	}
}
