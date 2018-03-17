/*==================================
*  Copyright (C) 2015 All rights reserved.
*   Filename：      edit_data
*   Author：        Mr.Lu
*   Version：       2.0
*   Date：          2015/07/31
*   Description：	编辑资料
*   Function List： edit_data() 
==========================================*/

#include "../../include/myhead.h"

//编辑资料界面
void edit_data()
{
	manage_win[0] = newwin(20, 45, 6, 40);
	manage_win[1] = newwin(3, 20, 14, 53);
	box(manage_win[0], 0, 0);
	box(manage_win[1], 0, 0);

	mvwprintw(manage_win[0], 1, 3, "编辑资料 > 昵称修改");
	mvwprintw(manage_win[0], 2, 1, "-------------------------------------------");

	mvwprintw(manage_win[0], 5, 6, "当前昵称");
	mvwprintw(manage_win[0], 9, 6, "新昵称");

	mvwprintw(manage_win[0], 5, 18, "[%s]", current_user);
	
	wrefresh(manage_win[0]);
	wrefresh(manage_win[1]);

	echo();
	curs_set(true);

	Message msg;

	bzero(&msg, sizeof(msg));
	mvwscanw(manage_win[1], 1, 2, "%s", msg.target);

	if(my_strlen(msg.target) > 0)
	{
		msg.action = MODIFY_NICKNAME;
		my_strcpy(msg.user, current_user);

		if(send(sockfd, &msg, sizeof(msg), 0) == -1)
		{
			perror("Change nickname error!\n");
			exit(1);
		}
	}
	else
	{
		curs_set(false);
		mvwprintw(manage_win[0], 13, 19, "昵称为空");
		mvwprintw(manage_win[0], 16, 15, "＊昵称修改失败＊");
		wrefresh(manage_win[0]);
		sleep(2);
		wclear(manage_win[0]);
		wrefresh(manage_win[0]);
	}
	
	curs_set(false);
}

