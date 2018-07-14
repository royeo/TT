/*========================================
*  Copyright (C) 2015 All rights reserved.
*   Filename：      regist.c
*   Author：        Mr.Lu
*   Version：       1.0
*   Date：          2015/07/17
*   Description：   注册界面，注册操作
*   Function List： void reg_ui() int regist()
* 
==========================================*/

#include "../../include/myhead.h"

//删除注册界面窗口
void del_reg_win()
{
    delwin(reg_win[0]);
    delwin(reg_win[1]);
    delwin(reg_win[2]);
    delwin(reg_win[3]);
    delwin(reg_win[4]);
    delwin(reg_win[5]);
    delwin(reg_win[6]);
    delwin(reg_win[7]);
}

//注册界面
void reg_ui()
{
	clear();
	refresh();

    reg_win[0] = newwin(32, 70, 0, 30);   //主窗口

    reg_win[1] = newwin(3, 30, 5, 50);    //昵称窗口
    reg_win[2] = newwin(3, 30, 8, 50);    //密码窗口
    reg_win[3] = newwin(3, 30, 11, 50);   //确认密码窗口
    
	reg_win[4] = newwin(3, 30, 14, 50);   //密保问题窗口
    reg_win[5] = newwin(3, 30, 17, 50);   //密保问题答案窗口

	reg_win[6] = newwin(3, 20, 25, 52);   //立即注册窗口

	reg_win[7] = newwin(16, 40, 8, 44);  //注册成功界面

	box(reg_win[0], 0, 0);
	box(reg_win[1], 0, 0);
	box(reg_win[2], 0, 0);
	box(reg_win[3], 0, 0);
	box(reg_win[4], 0, 0);
	box(reg_win[5], 0, 0);
	box(reg_win[6], 0, 0);
	box(reg_win[7], 0, 0);

	mvwprintw(reg_win[0], 2, 6, "|注册帐号");
	mvwprintw(reg_win[0], 3, 6, "-------------------------------------------------------");
	mvwprintw(reg_win[0], 6, 15, "昵称");
	mvwprintw(reg_win[0], 9, 15, "密码");
	mvwprintw(reg_win[0], 12, 11, "确认密码");
	mvwprintw(reg_win[0], 15, 11, "密保问题");
	mvwprintw(reg_win[0], 18, 15, "答案");
	
	mvwprintw(reg_win[0], 15, 52, "(选填)");
	mvwprintw(reg_win[0], 18, 52, "(选填)");
	
	mvwprintw(reg_win[0], 22, 23, "[＠]同时开通TT空间");
	mvwprintw(reg_win[0], 23, 23, "[＠]我已阅读并同意相关服务条款");
	mvwprintw(reg_win[0], 30, 15, "温馨提示：忘记密码可以通过密保问题找回哦！");

	mvwprintw(reg_win[6], 1, 6, "立即注册");

	mvwprintw(reg_win[7], 4, 5, "注册成功，您获得ID：");
	mvwprintw(reg_win[7], 13, 10, "按任意键返回登录界面");

	wrefresh(reg_win[0]);
	wrefresh(reg_win[1]);
	wrefresh(reg_win[2]);
	wrefresh(reg_win[3]);
	wrefresh(reg_win[4]);
	wrefresh(reg_win[5]);
	wrefresh(reg_win[6]);
}

//注册操作
void reg_opt()
{
	int regist_mark = EMPTY;     //将注册标记置空
	continue_mark = EMPTY;       //将继续标记置空
	
	char password[20];

	Message msg;
	Message back_msg;

	echo();          //开回显
	curs_set(true);  //显示光标
	
	//输入帐号
	bzero(&msg, sizeof(msg));
	bzero(&back_msg, sizeof(back_msg));
	
	mvwscanw(reg_win[1], 1, 2, "%s", msg.user);
				
	if(strlen(msg.user) > 0)
	{
		msg.action = CHECK_ACCOUNT;
		
		if(send(sockfd, &msg, sizeof(msg), 0) == -1)
		{
			perror("Regist send error!\n");
			exit(1);
		}
			
		if(recv(sockfd, &back_msg, sizeof(back_msg), 0) == -1)
		{
		    perror("Regist recv error!\n");
			exit(1);
		}

		switch(back_msg.action)
		{
		    case ACCOUNT_EXIST:
			{
				regist_mark = REGIST_FAIL;  //昵称已存在将注册标记设为失败
				mvwprintw(reg_win[0], 6, 52, "昵称已存在");
				wrefresh(reg_win[0]);
				break;
			}
	
			case ACCOUNT_NOT_EXIST:
			{
				mvwprintw(reg_win[0], 6, 52, "昵称可用");
				wrefresh(reg_win[0]);
				break;
			}
		}
	}
	else
	{
		regist_mark = REGIST_FAIL;  //昵称为空将注册标记设为失败
		mvwprintw(reg_win[0], 6, 52, "昵称为空");
		wrefresh(reg_win[0]);
	}

	//输入密码
	move(9, 52);
	password_input(password);

	int len = strlen(password);

	if(len <= 6 && len >= 1)
	{
		mvwprintw(reg_win[0], 9, 52, "[=][ ][ ] 弱");
		wrefresh(reg_win[0]);
	}
	else if(len > 6 && len <= 12)
	{
		mvwprintw(reg_win[0], 9, 52, "[=][=][ ] 中");
		wrefresh(reg_win[0]);
	}
	else if(len > 12)
	{
		mvwprintw(reg_win[0], 9, 52, "[=][=][=] 强");
		wrefresh(reg_win[0]);
	}
	else
	{
		regist_mark = REGIST_FAIL;  //密码为空将注册标记设为失败
		mvwprintw(reg_win[0], 9, 52, "密码为空");
		wrefresh(reg_win[0]);
	}

	//再次确认密码
	move(12, 52);
	password_input(msg.password);

	if(strcmp(password, msg.password) == 0 && strlen(msg.password) > 0)
	{
		mvwprintw(reg_win[0], 12, 52, "密码正确");
		wrefresh(reg_win[0]);
	}
	else
	{
		regist_mark = REGIST_FAIL;  //密码不一致将注册标记设为失败
		mvwprintw(reg_win[0], 12, 52, "密码不一致");
		wrefresh(reg_win[0]);
	}

	//密保问题和答案
	mvwscanw(reg_win[4], 1, 2, "%s", msg.question);
	mvwscanw(reg_win[5], 1, 2, "%s", msg.answer);


	//注册信息有错
	if(regist_mark == REGIST_FAIL)
	{
		noecho();         //关闭回显
		curs_set(false);  //不显示光标
		mvwprintw(reg_win[6], 1, 6, "注册失败");
		mvwprintw(reg_win[0], 30, 14, "     按空格键重新注册,按ESC键退出         ");
	    wrefresh(reg_win[6]);
	    wrefresh(reg_win[0]);

	    //继续或退出	
		continue_choose(reg_win[0]);  
	}
	else   
	{
		//设置注册标志为成功
		regist_mark = REGIST_OK;  
		
		//注册信息正确,发送服务器保存
		msg.action = REGIST;
		if(send(sockfd, &msg, sizeof(msg), 0) == -1)
		{
		    perror("Regist send error!\n");
			exit(1);
		}
	
		if(recv(sockfd, &back_msg, sizeof(back_msg), 0) == -1)
		{
		   	perror("Regist recv error!\n");
			exit(1);
		}
		
		curs_set(false);  //不显示光标
		mvwprintw(reg_win[6], 1, 6, "注册中...");
	    wrefresh(reg_win[6]);
	    sleep(1);
		mvwprintw(reg_win[7], 7, 15, "%s", back_msg.id);
		wrefresh(reg_win[7]);
		getch();

		continue_mark = EXIT;   //将继续标志设为退出
	}
}


//注册
void regist()
{
	continue_mark = EMPTY;

    while(continue_mark != EXIT)
	{
		reg_ui();
	    reg_opt();
	}
}




	




