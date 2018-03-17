/*==================================
*  Copyright (C) 2015 All rights reserved.
*   Filename：      friend_list.c
*   Author：        Mr.Lu
*   Version：       2.0
*   Date：          2015/7/25
*   Description：	好友列表
*   Function List： create_friend_list()
*					insert_friend_list()
==========================================*/
#include "../../include/myhead.h"

//初始化好友列表
void create_friend_list()
{
    friend_head = (Friend *)malloc(sizeof(Friend));
	friend_head->next = NULL;
}

//添加到好友列表中
void insert_friend_list(Friend **friend)
{
    Friend *temp = friend_head;

	while(temp->next != NULL)
	{
	    temp = temp->next;
	}
	
	temp->next = *friend;
	(*friend)->next = NULL;
}

//初始化好友聊天消息链表
void create_chat_msg_list()
{
    chat_msg_head = (Chat_Msg *)malloc(sizeof(Chat_Msg));
	chat_msg_head->next = NULL;
}

//添加一条好友聊天信息
void insert_chat_msg(Chat_Msg **chat_msg)
{
    Chat_Msg *temp = chat_msg_head;

	while(temp->next != NULL)
	{
	    temp = temp->next;
	}
	
	temp->next = *(chat_msg);
	(*chat_msg)->next = NULL;
}

/*删除一个结点*/
int delete_chat_msg(Chat_Msg **chat_msg)
{	
	Chat_Msg *temp = chat_msg_head;
    Chat_Msg *ptr = temp;
	temp = temp->next;

	while(temp != NULL)
	{
	    if(temp == *chat_msg)
		{
		    ptr->next = temp->next;
			free(temp);
			temp = NULL;
            return 0;
		}

		ptr = temp;
		temp = temp->next;
	}

	return 1;	
}


//初始化群聊消息链表
void create_group_msg_list()
{
    group_msg_head = (Chat_Msg *)malloc(sizeof(Chat_Msg));
	group_msg_head->next = NULL;
}

//添加一条群聊天信息
void insert_group_msg(Chat_Msg **chat_msg)
{
    Chat_Msg *temp = group_msg_head;

	while(temp->next != NULL)
	{
	    temp = temp->next;
	}
	
	temp->next = *(chat_msg);
	(*chat_msg)->next = NULL;
}

/*删除一条群聊信息*/
int delete_group_msg(Chat_Msg **chat_msg)
{	
	Chat_Msg *temp = group_msg_head;
    Chat_Msg *ptr = temp;
	temp = temp->next;

	while(temp != NULL)
	{
	    if(temp == *chat_msg)
		{
		    ptr->next = temp->next;
			free(temp);
			temp = NULL;
            return 0;
		}

		ptr = temp;
		temp = temp->next;
	}

	return 1;	
}

//刷新好友列表
void refresh_friend_list()
{
    if(friend_list_mark == OPEN)    //好友列表打开
	{
		int y = 12;
   	 	Friend *temp = friend_head->next;

		bzero(friend_list, sizeof(friend_list));
															  
		mvwprintw(friend_win[0], 12, 1, "                         ");
		mvwprintw(friend_win[0], 14, 1, "                         ");
	
		while(temp != NULL)
		{
			if(temp->state == FRIEND_ONLINE)
			{
	    		mvwprintw(friend_win[0], y, 3, "%s [在线]", temp->user);
				friend_list[y + 1] = temp->user;
				y += 2;
			}
			temp = temp->next;
		}
    
		temp = friend_head->next;
		while(temp != NULL)
		{
			if(temp->state == FRIEND_OFFLINE)
			{
	    		mvwprintw(friend_win[0], y, 3, "%s [离线]", temp->user);
				friend_list[y + 1] = temp->user;
				y += 2;
			}
			temp = temp->next;
		}

		friend_list[y + 1] = "my_group";

		mvwprintw(friend_win[0], y, 1, " >  我的群组              ");
		wrefresh(friend_win[0]);
	}
	else  			//好友列表关闭
	{
		int i;

		mvwprintw(friend_win[0], 10, 1, " >  我的好友             ");
		mvwprintw(friend_win[0], 12, 1, " >  我的群组              ");

		//清空好友列表
		bzero(friend_list, sizeof(friend_list));
	
		//初始化好友列表对应功能
		friend_list[11] = "my_friend";	
		friend_list[13] = "my_group";
	
		for(i = 13; i <= 27; i++) 	
		{
			mvwprintw(friend_win[0], i, 1, "                         ");
		}
	
		wrefresh(friend_win[0]);
	}

	if(group_list_mark == OPEN)    //打开群组列表
	{
 	   int i;

		for(i = 0; i < 30; i++)
		{
		    if(friend_list[i] == "my_group")
			{
				break;	
			}
		}

		mvwprintw(friend_win[0], i + 1, 3, "苏嵌96班 [精英群]");
		wrefresh(friend_win[0]);
		friend_list[i + 2] = "group";
	}
	else		//关闭群组列表
	{
	    int i;

		for(i = 0; i < 30; i++)
		{
		    if(friend_list[i] == "my_group")
			{
				break;	
			}
		}

		mvwprintw(friend_win[0], i + 1, 3, "                   ");
		wrefresh(friend_win[0]);
	}
}


