/*========================================
*  Copyright (C) 2015 All rights reserved.
*   Filename：      login.c
*   Author：        Mr.Lu
*   Version：       1.0
*   Date：          2015/07/23
*   Description：   登录注册操作
*   Function List： void log_in()
*					void regist()
==========================================*/

#include "../../include/myhead.h"aa


//登录操作
void login()
{
	Message msg;
	Message back_msg;

	bzero(&msg, sizeof(msg));
	bzero(&back_msg, sizeof(back_msg));
	
	echo();          //开回显
	curs_set(true);  //显示光标

	mvwprintw(menu_win[2], 1, 2, "        ");
	mvwscanw(menu_win[2], 1, 2, "%s", msg.user);
	mvwprintw(menu_win[3], 1, 2, "        ");
	wrefresh(menu_win[3]);
	move(20, 57);
	password_input(msg.password);
			
	curs_set(false);  //不显示光标

	//将登录信息发送服务器
	msg.action = LOGIN;
	if(send(sockfd, &msg, sizeof(msg), 0) == -1)
	{
	    perror("login send error!\n");
		exit(1);
	}

	if(recv(sockfd, &back_msg, sizeof(back_msg), 0) == -1)
	{
	    perror("login recv error!\n");
		exit(1);
	}
	
	//检测服务器回传的信息
	switch(back_msg.action)
	{	
	    //登录成功
		case LOGIN_OK:    
		{
			login_mark = LOGIN_OK;   //将登录标志设为成功
			memset(current_signature, 0, sizeof(current_signature));

			my_strcpy(current_user, back_msg.user);             //保存当前用户名
			my_strcpy(current_signature, back_msg.signature);   //保存当前个性签名

			//打开保存好友聊天记录的数据库
			if(sqlite3_open("MsgEx.db", &db) != SQLITE_OK)
			{
			    printf("MsgEx.db open error!\n");
				exit(1);
			}
			
			//创建该用户的聊天记录表
			char sql[1024];
			bzero(sql, sizeof(sql));	//保存和好友的聊天记录
			sprintf(sql, "create table user_%s(target text, data text, message text)", current_user);
			sqlite3_exec(db, sql, NULL, NULL, &errmsg);
			
			bzero(sql, sizeof(sql));	//保存群组聊天记录
			sprintf(sql, "create table group_chat(data text, message text)");
			sqlite3_exec(db, sql, NULL, NULL, &errmsg);
	
			wclear(menu_win[0]);
			box(menu_win[0], 0, 0);
			box(menu_win[4], 0, 0);
			mvwprintw(menu_win[0], 12, 1, "——————————————————————————————————————————————————————————");
			mvwprintw(menu_win[0], 1, 54, "EXIT");
			mvwprintw(menu_win[0], 19, (60 - my_strlen(current_user)) / 2, "%s", current_user);
			mvwprintw(menu_win[4], 1, 8, "  登  录  中     ");  
			
			wrefresh(menu_win[0]);
			wrefresh(menu_win[4]);
			wrefresh(menu_win[5]);
			//sleep(1);
			break;
		}
			
		//用户已登录
		case LOGIN_ALREADY:
		{
			login_mark = LOGIN_FAIL;   //将登录标志设为失败
			
			mvwprintw(menu_win[6], 0, 2, "用户已登录,按任意键继续");
			wrefresh(menu_win[6]);
			getch();
			break;
		}

		//帐号不存在
		case ACCOUNT_NOT_EXIST:
		{
			login_mark = LOGIN_FAIL;   //将登录标志设为失败
			
			mvwprintw(menu_win[6], 0, 2, "帐号不存在,按任意键继续");
			wrefresh(menu_win[6]);
			getch();
			break;
		}
			
		//密码错误
		case PASSWORD_ERROR:
		{
			login_mark = LOGIN_FAIL;   //将登录标志设为失败
			
			mvwprintw(menu_win[6], 0, 2, "密码错误,按任意键继续");
			wrefresh(menu_win[6]);
			getch();
			break;
		}
		
		//服务器未处理
		default:
		{
			login_mark = LOGIN_FAIL;   //将登录标志设为失败
			
			mvwprintw(menu_win[6], 0, 2, "登录超时,按任意键继续");
			wrefresh(menu_win[6]);
			getch();
			break;
		}
	}
}
