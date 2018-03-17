/*========================================
*  Copyright (C) 2015 All rights reserved.
*   Filename：      modify_signature.c
*   Author：        Mr.Lu
*   Version：       2.0
*   Date：          2015/07/24
*   Description：   修改个性签名操作
*   Function List： void modify_signature()
==========================================*/

#include "../../include/myhead.h"

//修改个性签名
void modify_signature()
{
	char signature[25];
	Message msg;
		
	echo();
	curs_set(true);

	wclear(friend_win[2]);
	mvwgetstr(friend_win[2], 0, 0, signature);

	curs_set(false);
	
	if(my_strlen(signature) > 0)
	{
		my_strcpy(current_signature, signature);
		mvwprintw(friend_win[2], 0, 0, "%s", signature);
		wrefresh(friend_win[2]);

		msg.action = MODIFY_SIGNATURE;
		my_strcpy(msg.user, current_user);
		my_strcpy(msg.signature, signature);

		if(send(sockfd, &msg, sizeof(msg), 0) == -1)
		{
	    	perror("Modify signature send error!\n");
			exit(1);
		}
	}
	else if(my_strlen(current_signature) > 0)
	{
		mvwprintw(friend_win[2], 0, 0, "%s", current_signature);
		wrefresh(friend_win[2]);
	}
	else
	{
		mvwprintw(friend_win[2], 0, 0, "设置个性签名");
		wrefresh(friend_win[2]);
	}
}
