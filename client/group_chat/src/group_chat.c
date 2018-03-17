/*==================================
*  Copyright (C) 2015 All rights reserved.
*   Filename：      group_chat
*   Author：        Mr.Lu
*   Version：       2.0
*   Date：          2015/8/7
*   Description：	群组聊天
*   Function List： group_chat()
==========================================*/
#include "../../include/myhead.h"

//群组聊天界面
void group_chat_ui()
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
	chat_win[9] = newwin(2, 10, 11, 79);	//群通知窗
	chat_win[10] = newwin(10, 13, 19, 77);	//群成员窗

	box(chat_win[0], 0, 0);
//	box(chat_win[9], 0, 0);
//	box(chat_win[10], 0, 0);
	mvwprintw(chat_win[0], x++, y, "                                                                  X");
	mvwprintw(chat_win[0], x++, y, "                                                                    ");
	mvwprintw(chat_win[0], x++, y, "                                                                    ");
	mvwprintw(chat_win[0], x++, y, "--------------------------------------------------------------------");
	mvwprintw(chat_win[0], x++, y, "                                                    ｜ 群通知");
	mvwprintw(chat_win[0], x++, y, "                                                    ｜--------------");
	mvwprintw(chat_win[0], x++, y, "                                                    ｜");         
	mvwprintw(chat_win[0], x++, y, "                                                    ｜");
	mvwprintw(chat_win[0], x++, y, "                                                    ｜");
	mvwprintw(chat_win[0], x++, y, "                                                    ｜");
	mvwprintw(chat_win[0], x++, y, "                                                    ｜");
	mvwprintw(chat_win[0], x++, y, "                                                    ｜");
	mvwprintw(chat_win[0], x++, y, "                                                    ｜");
	mvwprintw(chat_win[0], x++, y, "                                                    ｜--------------");
	mvwprintw(chat_win[0], x++, y, "                                                    ｜ 群成员");
	mvwprintw(chat_win[0], x++, y, "                                                    ｜--------------");
	mvwprintw(chat_win[0], x++, y, "                                                    ｜");
	mvwprintw(chat_win[0], x++, y, "                                                    ｜");
	mvwprintw(chat_win[0], x++, y, "----------------------------------------------------｜");
	mvwprintw(chat_win[0], x++, y, "   聊天   表情   短语   匿名   公告        消息记录 ｜");
	mvwprintw(chat_win[0], x++, y, "----------------------------------------------------｜");
	mvwprintw(chat_win[0], x++, y, "                                                    ｜");
	mvwprintw(chat_win[0], x++, y, "                                                    ｜");
	mvwprintw(chat_win[0], x++, y, "                                                    ｜");
	mvwprintw(chat_win[0], x++, y, "                                                    ｜");
	mvwprintw(chat_win[0], x++, y, "                                 ｜关闭｜  ｜发送｜ ｜");

	//打印群组名
	mvwprintw(chat_win[0], 1, 3, "苏嵌96班");       
	mvwprintw(chat_win[0], 3, 3, "＊(10人群)");       

	//打印群成员
	y = 1;
 	Friend *temp = friend_head->next;			
	mvwprintw(chat_win[10], y, 3, "%s", current_user);
	y += 2;
	while(temp != NULL)
	{
		mvwprintw(chat_win[10], y, 3, "%s", temp->user);
		temp = temp->next;
		y += 2;
	}

	//打印群通知
	get_group_notice();
	
	wrefresh(chat_win[0]);
//	wrefresh(chat_win[9]);
	wrefresh(chat_win[10]);
}

//开始群聊
void group_chat_choose()
{
	int cur_x;
	int cur_y;

	int y = 20;
	int x = 2;
	
	int ch;
    
	int chat_choose_mark = EMPTY;  		//聊天选择标志置空
	int anonymity_chat_mark = NO;		//匿名聊天标志置空

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
				    x = 30;
					break;
				}
				if(x == 30)
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
					x = 30;
					break;
				}
				if(x == 30)
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
							send_group_msg();
							break;
						}
						//表情
						case 9:
						{
							send_group_expression();
							break;	
						}
						//短语
						case 16:
						{
							send_group_phrase();
							break;
						}
						//匿名
						case 23:
						{
							if(anonymity_chat_mark == NO)
							{
							    anonymity_chat_mark = YES;
								open_anonymity();
								break;
							}
							
							if(anonymity_chat_mark == YES)
							{
							    anonymity_chat_mark = NO;
							    close_anonymity();
								break;
							}
							break;
						} 
						//公告
						case 30:
						{
							send_group_notice();
							break;
						}
						//消息记录
						case 42:
						{
							check_group_msg_record();
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

	if(anonymity_chat_mark == YES)
	{
		my_strcpy(current_user, casual_user);
	}
}


//群组聊天
void group_chat()
{
    group_chat_ui();
	group_chat_choose();	
}
