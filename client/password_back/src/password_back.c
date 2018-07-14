/*========================================
*  Copyright (C) 2015 All rights reserved.
*   Filename：      password_back.c
*   Author：        Mr.Lu
*   Version：       1.0
*   Date：          2015/07/24
*   Description：   找回密码
*   Function List： void password_back()
* 
==========================================*/

#include "../../include/myhead.h"

//删除找回密码窗口
void del_manage_win()
{
    delwin(manage_win[0]);
    delwin(manage_win[1]);
    delwin(manage_win[2]);
    delwin(manage_win[3]);
    delwin(manage_win[4]);
    delwin(manage_win[5]);
    delwin(manage_win[6]);
    delwin(manage_win[7]);
}

//找回密码
void password_back_ui()
{
    clear();
	refresh();
	
	manage_win[0] = newwin(32, 70, 0, 30);   //主窗口
    
	manage_win[1] = newwin(3, 31, 5, 50);    //帐号窗口
    manage_win[2] = newwin(3, 31, 9, 50);    //验证码输入窗口
    manage_win[3] = newwin(4, 15, 14, 50);   //验证码显示窗口

	manage_win[4] = newwin(3, 14, 20, 57);   //下一步窗口
	
	manage_win[5] = newwin(16, 40, 8, 43);   //密保问题窗口
	manage_win[6] = newwin(3, 25, 12, 53);   //问题窗口
	manage_win[7] = newwin(3, 25, 15, 53);   //答案窗口
   	
	box(manage_win[0], 0, 0);
   	box(manage_win[1], 0, 0);
   	box(manage_win[2], 0, 0);
   	box(manage_win[3], 0, 0);
   	box(manage_win[4], 0, 0);
   	box(manage_win[5], 0, 0);
   	box(manage_win[6], 0, 0);
   	box(manage_win[7], 0, 0);

	mvwprintw(manage_win[0], 1, 4, "密码管理 > 密码找回");
	mvwprintw(manage_win[0], 2, 4, "--------------------------------------------------------------");

	mvwprintw(manage_win[0], 6, 13, "＊帐号");
	mvwprintw(manage_win[0], 10, 11, "＊验证码");
	mvwprintw(manage_win[0], 12, 21, "请输入图中字符,不区分大小写");
	mvwprintw(manage_win[0], 16, 36, "看不清？你眼瞎啊");

	mvwprintw(manage_win[4], 1, 4, "下一步");
	
	mvwprintw(manage_win[5], 1, 2, "密保问题");
	mvwprintw(manage_win[5], 2, 2, "------------------------------------");
	mvwprintw(manage_win[5], 5, 4, "问题：");
	mvwprintw(manage_win[5], 8, 4, "答案：");
	
	wrefresh(manage_win[0]);
	wrefresh(manage_win[1]);
	wrefresh(manage_win[2]);
	wrefresh(manage_win[3]);
	wrefresh(manage_win[4]);
//	wrefresh(manage_win[5]);
//	wrefresh(manage_win[6]);
//	wrefresh(manage_win[7]);
}

//找回密码操作
void password_back_opt()
{
	int password_back_mark = EMPTY;  //将找回密码标记置空
	continue_mark = EMPTY;		     //将继续标记置空

	char auth_code[5];
	char auth_code_input[4];
	
	Message msg;
	Message back_msg;

	bzero(&msg, sizeof(msg));
	bzero(&back_msg, sizeof(back_msg));
	
	echo();          //开回显
	curs_set(true);  //显示光标
	
	get_auth_code(auth_code);  //获取验证码
	mvwprintw(manage_win[3], 2, 5, "%s", auth_code);
	wrefresh(manage_win[3]);

	//输入帐号
	mvwscanw(manage_win[1], 1, 2, "%s", msg.user);
				
	if(strlen(msg.user) > 0)
	{
		msg.action = CHECK_ACCOUNT;
		
		if(send(sockfd, &msg, sizeof(msg), 0) == -1)
		{
			perror("Password back send error!\n");
			exit(1);
		}
			
		if(recv(sockfd, &back_msg, sizeof(back_msg), 0) == -1)
		{
		    perror("Password back recv error!\n");
			exit(1);
		}

		switch(back_msg.action)
		{
		    case ACCOUNT_NOT_EXIST:
			{
				password_back_mark = PASSWORD_BACK_FAIL;  //将找回密码标记设为失败
				mvwprintw(manage_win[0], 6, 52, "您输入的帐号有误");
				wrefresh(manage_win[0]);
				break;
			}
	
			case ACCOUNT_EXIST:
			{
				break;
			}
		}
	}
	else
	{
		password_back_mark = PASSWORD_BACK_FAIL;  //将找回密码标记设为失败
		mvwprintw(manage_win[0], 6, 52, "请输入帐号");
		wrefresh(manage_win[0]);
	}

	//输入验证码
	mvwscanw(manage_win[2], 1, 2, "%s", auth_code_input);

	if(((auth_code[0] == auth_code_input[0]) || (auth_code[0] == auth_code_input[0] - 32))
	&& ((auth_code[1] == auth_code_input[1]) || (auth_code[1] == auth_code_input[1] - 32))
	&& ((auth_code[2] == auth_code_input[2]) || (auth_code[2] == auth_code_input[2] - 32))
	&& ((auth_code[3] == auth_code_input[3]) || (auth_code[3] == auth_code_input[3] - 32)))
	{
		mvwprintw(manage_win[0], 10, 52, "验证码正确");
		wrefresh(manage_win[0]);
	}
	else
	{
		password_back_mark = PASSWORD_BACK_FAIL;  //将找回密码标记设为失败
		mvwprintw(manage_win[0], 10, 52, "验证码错误");
		wrefresh(manage_win[0]);
	}

	//如果前面操作有错误
	if(password_back_mark == PASSWORD_BACK_FAIL)
	{
	    
		noecho();         //关闭回显
		curs_set(false);  //不显示光标
		wclear(manage_win[4]);
		wrefresh(manage_win[4]);
		mvwprintw(manage_win[0], 22, 28, "＊操作失败＊");
		mvwprintw(manage_win[0], 30, 14, "      按空格键重新操作,按ESC键退出           ");
	    wrefresh(manage_win[0]);

	    //继续或退出	
		continue_choose(manage_win[0]);  
	}
	else  //操作成功，向服务器发送密保问题的请求
	{
		msg.action = PASSWORD_BACK;

		if(send(sockfd, &msg, sizeof(msg), 0) == -1)
		{
			perror("Password back send error!\n");
			exit(1);
		}
			
		if(recv(sockfd, &back_msg, sizeof(back_msg), 0) == -1)
		{
		    perror("Password back recv error!\n");
			exit(1);
		}

		switch(back_msg.action)
		{
			//已设置密保问题
			case PASSWORD_BACK_OK:
			{
				char answer[20];

				wrefresh(manage_win[5]);
				wrefresh(manage_win[6]);
				wrefresh(manage_win[7]);
				
				mvwprintw(manage_win[6], 1, 2, "%s", back_msg.question);
				wrefresh(manage_win[6]);
				
				while(continue_mark != EXIT)
				{
					curs_set(true);
					echo();
					
				    mvwprintw(manage_win[5], 11, 16, "        "); 
				    mvwprintw(manage_win[5], 14, 3, "                                    ");
					wrefresh(manage_win[5]);
					mvwprintw(manage_win[7], 1, 2, "                    ");
					mvwscanw(manage_win[7], 1, 2, "%s", answer);

					if(strcmp(answer, back_msg.answer) == 0)
					{
						curs_set(false);  
				    	mvwprintw(manage_win[5], 11, 14, "您的密码：%s", back_msg.password); 
				    	mvwprintw(manage_win[5], 14, 10, "按任意键返回登录界面"); 
						wrefresh(manage_win[5]);
						getch();
						continue_mark = EXIT;  //将继续标记设为退出
					}
					else
					{
				   		curs_set(false); 
				    	mvwprintw(manage_win[5], 11, 16, "答案错误"); 
				    	mvwprintw(manage_win[5], 14, 3, "按空格键重新输入,按ESC返回登录界面"); 
						wrefresh(manage_win[5]);
						continue_choose(manage_win[5]);
					}
				}

				break;
			}

			//未设置密保问题
			case PASSWORD_BACK_FAIL:
			{
				wclear(manage_win[4]);
				wrefresh(manage_win[4]);
			    mvwprintw(manage_win[0], 22, 23, "＊您尚未设置密保问题＊");
				mvwprintw(manage_win[0], 30, 14, "      按空格键重新操作,按ESC键退出           ");
				wrefresh(manage_win[0]);
				continue_choose(manage_win[0]);
				break;
			}

			default:
			{
				wclear(manage_win[4]);
				wrefresh(manage_win[4]);
			    mvwprintw(manage_win[0], 22, 26, "＊服务器未响应＊");
				mvwprintw(manage_win[0], 30, 14, "      按空格键重新操作,按ESC键退出           ");
				wrefresh(manage_win[0]);
				continue_choose(manage_win[0]);
				break;
			}
	   	} 
	}
}


//找回密码功能
void password_back()
{
    continue_mark = EMPTY;

	while(continue_mark != EXIT)
	{
	    password_back_ui();
		password_back_opt();
	}
}
