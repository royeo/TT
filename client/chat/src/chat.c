/*========================================
*  Copyright (C) 2015 All rights reserved.
*   Filename：      chat.c
*   Author：        Mr.Lu
*   Version：       2.0
*   Date：          2015/07/26
*   Description：   聊天操作
*   Function List： void chat_ui()
==========================================*/

#include "../../include/myhead.h"

void chat_ui()
{
	int x = 1;
	int y = 1;

	char target_signature[25];

	chat_win[0] = newwin(28, 70, 2, 22);   	//主窗口
	chat_win[1] = newwin(4, 50, 24, 24);	//发送信息窗
	chat_win[2] = newwin(14, 49, 7, 25);	//显示聊天信息窗
	chat_win[3] = newwin(28, 16, 2, 7);  	//多人聊天窗

	chat_win[4] = newwin(14, 50, 8, 10);	//表情窗口
	chat_win[5] = newwin(14, 50, 8, 10);	//短语窗口
	chat_win[6] = newwin(28, 40, 2, 75);	//聊天记录窗口
	chat_win[7] = newwin(16, 34, 11, 79);	//显示聊天记录窗口

	chat_win[8] = newwin(16, 50, 7, 30);	//文件窗
	chat_win[9] = newwin(22, 14, 7, 77); 

	box(chat_win[0], 0, 0);
	mvwprintw(chat_win[0], x++, y, "                                                                  X");
	mvwprintw(chat_win[0], x++, y, "                                                                    ");
	mvwprintw(chat_win[0], x++, y, "                                                                    ");
	mvwprintw(chat_win[0], x++, y, "--------------------------------------------------------------------");
	mvwprintw(chat_win[0], x++, y, "                                                    ｜");
	mvwprintw(chat_win[0], x++, y, "                                                    ｜");
	mvwprintw(chat_win[0], x++, y, "                                                    ｜");
	mvwprintw(chat_win[0], x++, y, "                                                    ｜");
	mvwprintw(chat_win[0], x++, y, "                                                    ｜");
	mvwprintw(chat_win[0], x++, y, "                                                    ｜");
	mvwprintw(chat_win[0], x++, y, "                                                    ｜");
	mvwprintw(chat_win[0], x++, y, "                                                    ｜");
	mvwprintw(chat_win[0], x++, y, "                                                    ｜");
	mvwprintw(chat_win[0], x++, y, "                                                    ｜");
	mvwprintw(chat_win[0], x++, y, "                                                    ｜");
	mvwprintw(chat_win[0], x++, y, "                                                    ｜");
	mvwprintw(chat_win[0], x++, y, "                                                    ｜");
	mvwprintw(chat_win[0], x++, y, "                                                    ｜");
	mvwprintw(chat_win[0], x++, y, "----------------------------------------------------｜");
	mvwprintw(chat_win[0], x++, y, "   聊天   表情   短语   文件               消息记录 ｜");
	mvwprintw(chat_win[0], x++, y, "----------------------------------------------------｜");
	mvwprintw(chat_win[0], x++, y, "                                                    ｜");
	mvwprintw(chat_win[0], x++, y, "                                                    ｜");
	mvwprintw(chat_win[0], x++, y, "                                                    ｜");
	mvwprintw(chat_win[0], x++, y, "                                                    ｜");
	mvwprintw(chat_win[0], x++, y, "                                 ｜关闭｜  ｜发送｜ ｜");

	mvwprintw(chat_win[0], 1, 3, "%s", current_target);       //打印聊天目标

	search_signature(current_target, target_signature);
	if(my_strlen(target_signature) > 0)
	{
		mvwprintw(chat_win[0], 3, 3, "%s", target_signature);  //打印聊天目标的个性签名
	}
	else
	{
		mvwprintw(chat_win[0], 3, 3, "这个人很懒,什么都没留下");  
	}

	get_advert_info();		//获取广告信息
	

	wrefresh(chat_win[0]);
	wrefresh(chat_win[1]);
	wrefresh(chat_win[2]);
}

//开始聊天
void chat_choose()
{
	int cur_x;
	int cur_y;

	int y = 20;
	int x = 2;
	
	int ch;
    
	int chat_choose_mark = EMPTY;  //聊天选择标志置空
	
	cbreak();
	noecho();
	keypad(chat_win[0], true);
	curs_set(false);

	wrefresh(chat_win[0]);
	wmove(chat_win[0], y, x);
	mvwaddstr(chat_win[0], y, x, "＊");
	wrefresh(chat_win[0]);

	do
	{
	    ch = wgetch(chat_win[0]);

		switch(ch)
		{
		    case KEY_LEFT:
			{
				if(y == 26)
				{
				    x = 32;
					break;
				}
				
				if(x == 42)
				{
				    x = 23;
					break;
				}
				if(x == 23)
				{
				    x = 16;
					break;
				}
				if(x == 16)
				{
				    x = 9;
					break;
				}
				if(x == 9)
				{
				    x = 2;
					break;
				}

				if(x == 2)
				{
					x = 42;
					break;
				}
				break;
			}
			case KEY_RIGHT:
			{
				if(y == 26)
				{
				    x = 42;
					break;
				}
				if(x == 2)
				{
					x = 9;
					break;
				}
				if(x == 9)
				{
					x = 16;
					break;
				}
				if(x == 16)
				{
					x = 23;
					break;
				}
				if(x == 23)
				{
					x = 42;
					break;
				}

				if(x == 42)
				{
				    x = 2;
					break;
				}
				break;
			}
			case KEY_UP:
			{
			    if(y == 20)
				{
					y = 1;
				    x = 64;
					break;
				}

				if(y == 26)
				{
				    y = 20;
					x = 2;
					break;
				}
				break;
			}
			case KEY_DOWN:
			{
				if(y == 1)
				{
				    y = 20;
					x = 2;
					break;
				}

				if(y == 20)
				{
				   y = 26;
				   x = 42;
				   break;
				}
				break;
			}

			//功能选择
			case '\n':
			{
			    if(y == 20)
				{
					switch(x)
					{
						//聊天
					    case 2:
						{
							send_msg();
							break;
						}
						//表情
						case 9:
						{
							send_expression();
							break;	
						}
						//短语
						case 16:
						{
							send_phrase();
							break;
						}
						//文件
						case 23:
						{
							send_file();
							break;
						}
						//切换
						case 30:
						{
							break;
						}
						//消息记录
						case 42:
						{
							check_message_record();
							break;
						}
					}
					break;
				}
				
				//发送和关闭
				if(y == 26 && x == 32)
				{
					wclear(chat_win[0]);
					wrefresh(chat_win[0]);
					bzero(&last_time, sizeof(last_time));
					bzero(&last_data, sizeof(last_data));
					bzero(&current_target, sizeof(current_target));
					chat_choose_mark = EXIT;
					chat_row = 1;
					break;
				}
				
				//退出聊天窗口
				if(y == 1)
				{
					wclear(chat_win[0]);
					wrefresh(chat_win[0]);
					bzero(&last_time, sizeof(last_time));
					bzero(&last_data, sizeof(last_data));
					bzero(&current_target, sizeof(current_target));
					chat_choose_mark = EXIT;
					chat_row = 1;
				    break;
				}
				break;
			}
			default:
			{
				break;
			}
		}
		
		getyx(chat_win[0], cur_y, cur_x);
		mvwaddstr(chat_win[0], cur_y, cur_x - 2, "  ");
		
		if(cur_y == 20 && cur_x == 11)
		{
			refresh();
		}
	
		mvwaddstr(chat_win[0], y, x, "＊");
		wrefresh(chat_win[0]);

	}while(chat_choose_mark != EXIT);
		
	getyx(chat_win[0], cur_y, cur_x);
	mvwaddstr(chat_win[0], cur_y, cur_x - 2, "  ");
	wrefresh(chat_win[0]);
}

//聊天操作
void chat()
{
	chat_ui();
	chat_choose();
}




