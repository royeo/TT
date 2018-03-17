/*========================================
*  Copyright (C) 2015 All rights reserved.
*   Filename：      search.c
*   Author：        Mr.Lu
*   Version：       2.0
*   Date：          2015/07/28
*   Description：   搜索好友操作
*   Function List： void search_friend()
==========================================*/

#include "../../include/myhead.h"

//搜索好友
void search_friend()
{
	char friend_name[20];
	Friend *temp = friend_head;

	echo();
	curs_set(true);

	wclear(friend_win[3]);
	mvwscanw(friend_win[3], 0, 0, "%s", friend_name);
	wclear(friend_win[3]);
	wrefresh(friend_win[3]);
	
	while(temp != NULL)
	{
	    if(my_strcmp(temp->user, friend_name) == 0)
		{
			my_strcpy(current_target, friend_name);
			chat();
			break;
		}

		temp = temp->next;
	}

	curs_set(false);
}
