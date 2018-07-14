/*========================================
*  Copyright (C) 2015 All rights reserved.
*   Filename：      start_ui.c
*   Author：        Mr.Lu
*   Version：       1.0
*   Date：          2015/07/17
*   Description：   开始界面
*   Function List： ui_start()
* 
==========================================*/

#include "../../include/myhead.h"

//删除主界面窗口
void del_menu_win()
{
    delwin(menu_win[0]);
    delwin(menu_win[1]);
    delwin(menu_win[2]);
    delwin(menu_win[3]);
    delwin(menu_win[4]);
    delwin(menu_win[5]);
    delwin(menu_win[6]);
}


//主界面
void menu_ui()
{
    pthread_t id;

	clear();
	refresh();

	menu_win[0] = newwin(24, 60, 4, 35);  //主窗口
	menu_win[1] = newwin(6, 13, 17, 40);  //头像窗
	menu_win[2] = newwin(3, 25, 17, 55);  //帐号窗
	menu_win[3] = newwin(3, 25, 19, 55);  //密码窗
	menu_win[4] = newwin(3, 30, 24, 50);  //安全登录窗
	menu_win[5] = newwin(6, 13, 17, 58);  //备用窗
	menu_win[6] = newwin(1, 60, 28, 35);  //提示信息框

	//设置窗口边框
	box(menu_win[0], 0, 0);
	box(menu_win[1], 0, 0);
	box(menu_win[2], 0, 0);
	box(menu_win[3], 0, 0);
	box(menu_win[4], 0, 0);
	box(menu_win[5], 0, 0);

	mvwprintw(menu_win[0], 6, 20, "TT 欢迎你的回归...");

	mvwprintw(menu_win[0], 12, 1, "——————————————————————————————————————————————————————————");
	mvwprintw(menu_win[0], 14, 49, "注册帐号");
	mvwprintw(menu_win[0], 16, 49, "找回密码");
	mvwprintw(menu_win[0], 18, 21, "[]记住密码");
	mvwprintw(menu_win[0], 18, 35, "[]自动登录");
	mvwprintw(menu_win[0], 1, 54, "EXIT");

	
	mvwprintw(menu_win[1], 1, 1, "         ");
	mvwprintw(menu_win[1], 2, 1, "   (^_^) ");
	mvwprintw(menu_win[1], 3, 1, "         ");
	mvwprintw(menu_win[1], 4, 1, "         ");
	
	mvwprintw(menu_win[2], 1, 2, "昵称/ID");
	mvwprintw(menu_win[3], 1, 2, "密码");
	
	mvwprintw(menu_win[4], 1, 8, "安  全  登  录");

	mvwprintw(menu_win[5], 1, 1, "         ");
	mvwprintw(menu_win[5], 2, 1, "   (^_^) ");
	mvwprintw(menu_win[5], 3, 1, "         ");
	mvwprintw(menu_win[5], 4, 1, "         ");


	//刷新窗口
	wrefresh(menu_win[0]);
	wrefresh(menu_win[1]);
	wrefresh(menu_win[2]);
	wrefresh(menu_win[3]);
	wrefresh(menu_win[4]);
	
/*	//创建一个线程打印美文
	if(pthread_create(&id, NULL, (void *)print_word, NULL) != 0)
	{
	    perror("Pthread menu error!\n");
		exit(1);
	} */
}


//主界面功能选择
void menu_choose()
{
	int cur_x;
	int cur_y;

	int x = 20;
	int y = 21;
	
	int ch;
    
	cbreak();
	noecho();
	keypad(menu_win[0], true);
	curs_set(false);

	wrefresh(menu_win[0]);
	wmove(menu_win[0], y, x);
	mvwaddstr(menu_win[0], y, x, "＊");
	wrefresh(menu_win[0]);

	func_choose_mark = EMPTY;  //功能选择标志置空
	
	do
	{
	    ch = wgetch(menu_win[0]);

		switch(ch)
		{
		    case KEY_UP:
			{
			    if(y == 21)
				{
				    y = 18;
					x = 35;
					break;
				}

				if(y == 18)
				{
				    y = 16;
					x = 46;
					break;
				}

				if(y == 16)
				{
				    y = 14;
					x = 46;
					break;
				}

				if(y == 14)
				{
				    y = 1;
					x = 51;
					break;
				}
				break;
			}

			case KEY_DOWN:
			{
			    if(y == 1)
				{
				    y = 14;
					x = 46;
					break;
				}

				if(y == 14)
				{
				    y = 16;
					x = 46;
					break;
				}

				if(y == 16)
				{
				    y = 18;
					x = 35;
					break;
				}

				if(y == 18)
				{
				    y = 21; 
					x = 20;
					break;
				}
				break;
			}
			
			case KEY_LEFT:
			{
				if(y == 18 && x == 35)
				{
				    y = 18;
					x = 21;
					break;
				}

				break;
			}

			case KEY_RIGHT:
			{
				if(y == 18 && x == 21)
				{
				    y = 18;
					x = 35;
					break;
				}

			    break;
			}

			case '\n':
			{
				//自动登录，记住密码设置
				if(y == 18 && x == 35 && auto_login_mark == 0)
				{
					auto_login_mark = 1;
					break;
				}
				else if(y == 18 && x == 35 && auto_login_mark == 1)
				{
				    auto_login_mark = 0;
					break;
				}
				else if(y == 18 && x == 21 && keep_password_mark == 0)
				{
				    keep_password_mark = 1;
					break;
				}
				else if(y == 18 && x == 21 && keep_password_mark == 1)
				{
				    keep_password_mark = 0;
					break;
				}
				
				//选择操作判断
			    switch(y)
				{
				    case 1:
					{
					    func_choose_mark = EXIT;
						break;
					}
					case 14:
					{
					    func_choose_mark = REGIST;
						break;
					}
					case 16:
					{
					    func_choose_mark = PASSWORD_BACK;
						break;
					}
					case 21:
					{
					    func_choose_mark = LOGIN;
						break;
					}
					default:
					{
					    break;
					}
				}
				break;
			}

			default:
			{
				break;
			}
		}
		
		getyx(menu_win[0], cur_y, cur_x);

		if(cur_y == 18 && cur_x == 37 && auto_login_mark == 0)          //自动登录没被选中
		{
			mvwaddstr(menu_win[0], 18, 35, "[]");
		}
		else if(cur_y == 18 && cur_x == 37 && auto_login_mark == 1)     //自动登录被选中
		{
			mvwaddstr(menu_win[0], 18, 35, "＠");
		}
		else if(cur_y == 18 && cur_x == 23 && keep_password_mark == 0)  //记住密码没被选中
		{
			mvwaddstr(menu_win[0], 18, 21, "[]");
		}
		else if(cur_y == 18 && cur_x == 23 && keep_password_mark == 1)  //记住密码被选中
		{
			mvwaddstr(menu_win[0], 18, 21, "＠");
		}
		else
		{
			mvwaddstr(menu_win[0], cur_y, cur_x - 2, "  ");
		}

		mvwaddstr(menu_win[0], y, x, "＊");
		wrefresh(menu_win[0]);

	}while(!func_choose_mark);
}

//主界面对应功能操作
void menu_operation()
{
	login_mark = EMPTY;   //将登录标记置空

    switch(func_choose_mark)
	{
	    case LOGIN:
		{
			login();
			del_menu_win();
			break;
		}
		case REGIST:
		{
			regist();
			del_reg_win();
			break;
		}
		case PASSWORD_BACK:
		{
		    password_back();
			del_manage_win();
			break;
		}
		case EXIT:
		{
			close(sockfd);
			endwin();
			exit(0);	
		}
		default:
		{
		    break;
		}
	}
}

/*
//检查登录记录
int check_login_record()
{
	int rc;
	int row;
	int column;
	char *errmsg = NULL;
	char **result = NULL;
	char sql[1024];
	sqlite3 *db;  
	Message msg;
	Message back_msg;
	
	//查看登录记录
	if(sqlite3_open("user_login_record.db", &db) != SQLITE_OK)
	{
	    perror("Open user login record error!\n");
		exit(1);
	}

	if(sqlite3_get_table(db, "select id, user, password from user_login_record", &result, &row, &column, &errmsg) != SQLITE_OK)
	{
	    perror("Login record sqlite3_get_table error!\n");
		exit(1);
	}

	//无登录记录
	if(row == 0)
	{
	    return NO_LOGIN_RECORD;  
	}
	    
	//有登录记录
	if(strlen(result[3]) > 0)
	{
	    strcpy(msg.user, result[3]);
	    strcpy(msg.password, result[5]);
	}
	else
	{
	    strcpy(msg.user, result[4]);
	    strcpy(msg.password, result[5]);
	}

	//将帐号密码显示在主界面上
	mvwprintw(menu_win[2], 1, 2, "        ");
	mvwprintw(menu_win[3], 1, 2, "        ");
	mvwprintw(menu_win[2], 1, 2, "%s", msg.user);
	
	int i;
	wmove(menu_win[3], 1, 2);
	for(i = 0; i < strlen(msg.password); i++)
	{
		wprintw(menu_win[3], "*");
	}

	wrefresh(menu_win[2]);
	wrefresh(menu_win[3]);
//	sleep(1);

	if(send(sockfd, &msg, sizeof(msg), 0) == -1)
	{
	    perror("Login send error!\n");
		exit(1);
	}

	if(recv(sockfd, &back_msg, sizeof(back_msg), 0) == -1)
	{
	    perror("Login recv error!\n");
		exit(1);
	}
	
	switch(back_msg.action)
	{	
	    //登录成功
		case LOGIN_OK:    
		{
			strcpy(active_user, back_msg.user);   //保存当前用户名
			wclear(menu_win[0]);
			box(menu_win[0], 0, 0);
			box(menu_win[4], 0, 0);
			mvwprintw(menu_win[0], 12, 1, "——————————————————————————————————————————————————————————");
			mvwprintw(menu_win[0], 1, 54, "EXIT");
			mvwprintw(menu_win[4], 1, 8, "  登  录  中     ");  
			wrefresh(menu_win[0]);
			wrefresh(menu_win[4]);
			wrefresh(menu_win[5]);
			sleep(3);
			return LOGIN_OK;		
		}
			
		//用户已登录
		case LOGIN_ALREADY:
		{
			mvwprintw(menu_win[6], 0, 2, "用户已登录,按任意键返回");
			wrefresh(menu_win[6]);
			getch();
			return LOGIN_FAIL;
		}

		//帐号不存在
		case ACCOUNT_NOT_EXIST:
		{
			mvwprintw(menu_win[6], 0, 2, "帐号不存在,按任意键返回");
			wrefresh(menu_win[6]);
			getch();
			return LOGIN_FAIL;
		}
			
		//密码错误
		case PASSWORD_ERROR:
		{
			mvwprintw(menu_win[6], 0, 2, "密码错误,按任意键返回");
			wrefresh(menu_win[6]);
			getch();
		 	return LOGIN_FAIL;
		}
		
		//服务器未处理
		default:
		{
			mvwprintw(menu_win[6], 0, 2, "服务器未响应,按任意键返回");
			wrefresh(menu_win[6]);
			getch();
			return LOGIN_FAIL;
		}
	}

	return LOGIN_FAIL; 
}*/
     
//主界面
void menu()
{
	do	
	{
		menu_ui();            //用户界面
		menu_choose();        //功能选择
		menu_operation();     //相应操作
	} while(login_mark != LOGIN_OK);
}
