/*==================================
*  Copyright (C) 2015 All rights reserved.
*   Filename：      link_operation.c
*   Author：        Mr.Lu
*   Version：       1.0
*   Date：          2015/7/16
*   Description：	链表操作
*   Function List： create_client_link()
*					insert_new_client()
*					delete_client()
*					create_offline_link()
*					insert_offline_msg()
					delete_offline_msg()
* 
==========================================*/
#include "../../../include/myhead.h"

//初始化客户端链表
void create_client_link()
{
    client_head = (Client_Info *)malloc(sizeof(Client_Info));
	client_head->next = NULL;
}

//插入新客户端
void insert_new_client(Client_Info **new_client)
{
    Client_Info *temp = client_head;

	while(temp->next != NULL)
	{
	    temp = temp->next;
	}

	temp->next = *new_client;
	(*new_client)->next = NULL;
}

//删除一个客户端并把下线通知发给所有在线用户
void delete_client(int sockfd)
{
	Message back_msg;

    Client_Info *temp = client_head;
	Client_Info *ptr = temp;
	temp = temp->next;

	bzero(&back_msg, sizeof(back_msg));
	
	while(temp != NULL)
	{
	    if(temp->new_fd == sockfd)
		{
			strcpy(back_msg.user, temp->user);
			printf("\n%s已下线.\n", back_msg.user);
		    ptr->next = temp->next;
			free(temp);
			temp = NULL;
			
			continue;
		}
		
		ptr = temp;
		temp = temp->next;
	}
			
	//发送用户下线消息到所有在线用户
	back_msg.action = NEW_OFFLINE_FRIEND;
	temp = client_head->next;
	while(temp != NULL)
	{
		if(send(temp->new_fd, &back_msg, sizeof(back_msg), 0) == -1)
		{
	   	 	error("User exit send error!");
		}

		temp = temp->next;
	}
}

//初始化离线消息链表
void create_offline_link()
{
    offline_head = (Offline_Msg *)malloc(sizeof(Offline_Msg));
	offline_head->next = NULL;
}

//插入一条新的离线消息
void insert_offline_msg(Offline_Msg **new_message)
{
    Offline_Msg *temp = offline_head;

	while(temp->next != NULL)
	{
	    temp = temp->next;
	}
	
	temp->next = *new_message;
	(*new_message)->next = NULL;
}

//删除一条离线消息
void delete_offline_msg(Offline_Msg **offline_msg)
{
	Offline_Msg *temp = offline_head;

	while(temp->next != *offline_msg)
	{
	    temp = temp->next;
	}

	temp->next = (*offline_msg)->next;
	free(*offline_msg);
}


//初始化离线文件链表
void create_offline_file_link()
{
    offline_file_head = (Offline_File *)malloc(sizeof(Offline_File));
	offline_file_head->next = NULL;
}

//插入一个新的离线文件
void insert_offline_file(Offline_File **new_offline_file)
{
    Offline_File *temp = offline_file_head;

	while(temp->next != NULL)
	{
	    temp = temp->next;
	}
	
	temp->next = *new_offline_file;
	(*new_offline_file)->next = NULL;
}

//删除一个离线文件
void delete_offline_file(Offline_File **offline_file)
{
	Offline_File *temp = offline_file_head;

	while(temp->next != *offline_file)
	{
	    temp = temp->next;
	}

	temp->next = (*offline_file)->next;
	free(*offline_file);
}


//初始化离线群组消息链表
void create_offline_group_link()
{
    offline_group_head = (Offline_Group_Msg *)malloc(sizeof(Offline_Group_Msg));
	offline_group_head->next = NULL;
}

//插入一条新的离线群组消息
void insert_offline_group_msg(Offline_Group_Msg **new_message)
{
    Offline_Group_Msg *temp = offline_group_head;

	while(temp->next != NULL)
	{
	    temp = temp->next;
	}
	
	temp->next = *new_message;
	(*new_message)->next = NULL;
}

//删除一条离线群组消息
void delete_offline_group_msg(Offline_Group_Msg **offline_msg)
{
	Offline_Group_Msg *temp = offline_group_head;

	while(temp->next != *offline_msg)
	{
	    temp = temp->next;
	}

	temp->next = (*offline_msg)->next;
	free(*offline_msg);
}




