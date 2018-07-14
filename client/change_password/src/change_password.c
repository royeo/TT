/*========================================
*  Copyright (C) 2015 All rights reserved.
*   Filename：      change_password
*   Author：        Mr.Lu
*   Version：       2.0
*   Date：          2015/07/31
*   Description：   修改密码
*   Function List： change_password()
==========================================*/

#include "../../include/myhead.h"


//修改密码界面
void change_password_ui()
{
	manage_win[0] = newwin(32, 70, 0, 25);   //主窗口
	manage_win[1] = newwin(3, 30, 5, 45);    //当前密码
    manage_win[2] = newwin(3, 30, 8, 45);    //新密码
    manage_win[3] = newwin(3, 30, 14, 45);   //确认新密码
    manage_win[4] = newwin(3, 30, 17, 45);   //输验证码
    manage_win[5] = newwin(4, 15, 20, 45);   //验证码
	manage_win[6] = newwin(3, 14, 25, 50);   //下一步窗口
   	
	box(manage_win[0], 0, 0);
   	box(manage_win[1], 0, 0);
   	box(manage_win[2], 0, 0);
   	box(manage_win[3], 0, 0);
   	box(manage_win[4], 0, 0);
   	box(manage_win[5], 0, 0);
   	box(manage_win[6], 0, 0);

	mvwprintw(manage_win[0], 1, 4, "密码管理 > 修改密码");
	mvwprintw(manage_win[0], 2, 4, "--------------------------------------------------------------");

	mvwprintw(manage_win[0], 6, 11,  "当前密码");
	mvwprintw(manage_win[0], 9, 13,  "新密码");
	mvwprintw(manage_win[0], 12, 11, "密码强度");
	mvwprintw(manage_win[0], 12, 21, "[    ][    ][    ]");
	mvwprintw(manage_win[0], 15, 9, "确认新密码");
	mvwprintw(manage_win[0], 18, 13, "验证码");
	mvwprintw(manage_win[0], 22, 36, "看不清楚,换一张");

	mvwprintw(manage_win[6], 1, 4, "确  定");
	
	
	wrefresh(manage_win[0]);
	wrefresh(manage_win[1]);
	wrefresh(manage_win[2]);
	wrefresh(manage_win[3]);
	wrefresh(manage_win[4]);
	wrefresh(manage_win[5]);
	wrefresh(manage_win[6]);
}

//修改密码操作
void change_password_opt()
{
	int change_password_mark = EMPTY;

	char password[20];
	char auth_code[5];
	char auth_code_input[4];

	Message msg;
	Message back_msg;

	echo();          //开回显
	curs_set(true);  //显示光标
	
	//获取验证码
	get_auth_code(auth_code);  
	mvwprintw(manage_win[5], 2, 5, "%s", auth_code);
	wrefresh(manage_win[5]);
	
	
	//输入当前密码
	wmove(manage_win[1], 1, 2);
	password_input_win(manage_win[1], msg.message);
	
	if(strlen(msg.message) == 0)
	{
		change_password_mark = CHANGE_PASSWORD_FAIL;
		mvwprintw(manage_win[0], 6, 52, "当前密码为空");
		wrefresh(manage_win[0]);
	}

	//输入新密码
	wmove(manage_win[2], 1, 2);
	password_input_win(manage_win[2], msg.password);
	
	int len = strlen(msg.password);
	
	if(strcmp(msg.message, msg.password) != 0)
	{
		if(len <= 6 && len >= 1)
		{
			mvwprintw(manage_win[0], 12, 23, "低");
			wrefresh(manage_win[0]);
		}
		else if(len > 6 && len <= 12)
		{
			mvwprintw(manage_win[0], 12, 29, "中");
			wrefresh(manage_win[0]);
		}
		else if(len > 12)
		{
			mvwprintw(manage_win[0], 12, 35, "强");
			wrefresh(manage_win[0]);
		}
		else
		{
			change_password_mark = CHANGE_PASSWORD_FAIL;
			mvwprintw(manage_win[0], 9, 52, "新密码为空");
			wrefresh(manage_win[0]);
		}
	}
	else
	{
		change_password_mark = CHANGE_PASSWORD_FAIL;
		mvwprintw(manage_win[0], 9, 52, "新旧密码不能相同");
		wrefresh(manage_win[0]);
	}

	//再次确认新密码
	wmove(manage_win[3], 1, 2);
	password_input_win(manage_win[3], password);

	if(strcmp(password, msg.password) == 0 && strlen(msg.password) > 0)
	{
		mvwprintw(manage_win[0], 15, 52, "密码正确");
		wrefresh(manage_win[0]);
	}
	else
	{
		change_password_mark = CHANGE_PASSWORD_FAIL;
		mvwprintw(manage_win[0], 15, 52, "密码不一致");
		wrefresh(manage_win[0]);
	}
	
	//输入验证码
	mvwscanw(manage_win[4], 1, 2, "%s", auth_code_input);

	if(((auth_code[0] == auth_code_input[0]) || (auth_code[0] == auth_code_input[0] - 32))
	&& ((auth_code[1] == auth_code_input[1]) || (auth_code[1] == auth_code_input[1] - 32))
	&& ((auth_code[2] == auth_code_input[2]) || (auth_code[2] == auth_code_input[2] - 32))
	&& ((auth_code[3] == auth_code_input[3]) || (auth_code[3] == auth_code_input[3] - 32)))
	{
		mvwprintw(manage_win[0], 18, 52, "验证码正确");
		wrefresh(manage_win[0]);
	}
	else
	{
		change_password_mark = CHANGE_PASSWORD_FAIL;  //将找回密码标记设为失败
		mvwprintw(manage_win[0], 18, 52, "验证码错误");
		wrefresh(manage_win[0]);
	}

	
	//如果前面操作有错误
	if(change_password_mark == CHANGE_PASSWORD_FAIL)
	{
		wclear(manage_win[6]);
		wrefresh(manage_win[6]);
		mvwprintw(manage_win[0], 26, 28, "＊操作失败＊");
		mvwprintw(manage_win[0], 30, 14, "      按空格键重新操作,按ESC键退出           ");
	    wrefresh(manage_win[0]);
	}
	else  //操作成功，向服务器发送修改密码的请求
	{
		msg.action = CHANGE_PASSWORD;
		strcpy(msg.user, current_user);

		if(send(sockfd, &msg, sizeof(msg), 0) == -1)
		{
			perror("Password back send error!\n");
			exit(1);
		}
	}
		
	noecho();         //关闭回显
	curs_set(false);  //不显示光标
}


//修改密码
void change_password()
{
	continue_mark = EMPTY;

    while(continue_mark != EXIT)
	{
		change_password_ui();
	    change_password_opt();
		continue_choose(manage_win[0]);  
	}

	wclear(manage_win[0]);
	wrefresh(manage_win[0]);
}




	




