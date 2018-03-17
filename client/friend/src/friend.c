/*========================================
*  Copyright (C) 2015 All rights reserved.
*   Filename：      friend.c
*   Author：        Mr.Lu
*   Version：       1.0
*   Date：          2015/07/24
*   Description：   好友列表界面
*   Function List： void friend()
*					void friend_ui()
*					void friend_choose()
*					void friend_operation()
* 
==========================================*/

#include "../../include/myhead.h"

//好友列表界面
void friend_ui()
{
	clear();
	refresh();
	
    friend_win[0] = newwin(31, 28, 1, 100);  //好友列表主窗口 
	friend_win[1] = newwin(1, 20, 4, 102);   //显示当前用户名窗口
	friend_win[2] = newwin(1, 25, 6, 102);   //显示个性签名窗口
	friend_win[3] = newwin(1, 18, 8, 102);   //搜索好友窗口

	friend_win[4] = newwin(15, 20, 15, 101); //设置窗口

	box(friend_win[0], 0, 0);
	mvwprintw(friend_win[0], 1, 2,  "TT                     X");
	mvwprintw(friend_win[0], 6, 1,  "--------------------------");
	mvwprintw(friend_win[0], 7, 1,  "                    搜索  ");
	mvwprintw(friend_win[0], 8, 1,  "--------------------------");
	mvwprintw(friend_win[0], 10, 1, " >  我的好友              ");
	mvwprintw(friend_win[0], 12, 1, " >  我的群组              ");
	mvwprintw(friend_win[0], 28, 1, "--------------------------");
	mvwprintw(friend_win[0], 29, 1, " = 设置            - 退出 ");

    mvwprintw(friend_win[1], 0, 0, "%s", current_user);
	
	if(my_strlen(current_signature) > 0)
	{
		mvwprintw(friend_win[2], 0, 0, "%s", current_signature);
	}
	else
	{
		mvwprintw(friend_win[2], 0, 0, "设置个性签名");
	} 

	wrefresh(friend_win[0]);
	wrefresh(friend_win[1]);
	wrefresh(friend_win[2]);
}


//好友列表功能选择
void friend_choose()
{
	int i;

	int cur_x;
	int cur_y;

	int x = 97;
	int y = 4;
	
	int ch;
    
	friend_list_mark = CLOSE;   //好友列表的标志(打开/关闭)
	group_list_mark = CLOSE;		//群组列表的标志(打开/关闭)
	
	bzero(friend_list, sizeof(friend_list));
	friend_list[11] = "my_friend";	//初始化好友列表对应功能
	friend_list[13] = "my_group";

	cbreak();
	noecho();
	keypad(stdscr, true);
	curs_set(false);

	refresh();
	move(y, x);
	mvaddstr(y, x, "＊");
	refresh();

	do
	{
	    ch = getch();
		func_choose_mark = EMPTY;		//功能选择标志置空

		switch(ch)
		{
		    case KEY_UP:
			{
				if(y == 30)
				{
					for(i = 29; i >= 0; i--)
					{
					   	if(friend_list[i] != NULL)
						{
					   		break;
						}
					}
					
					y = i;
					x = 97;
					break;
				}

				if(y == 27)
				{
					if(friend_list[25] != NULL)
					{
				    	y = 25;
						break;
					}
					else
					{
					    break;
					}
				}

				if(y == 25)
				{
					if(friend_list[23] != NULL)
					{
				    	y = 23;
						break;
					}
					else
					{
					    break;
					}
				}

				if(y == 23)
				{
					if(friend_list[21] != NULL)
					{
				    	y = 21;
						break;
					}
					else
					{
					    break;
					}
				}
				
				if(y == 21)
				{
					if(friend_list[19] != NULL)
					{
				    	y = 19;
						break;
					}
					else
					{
					    break;
					}
				}
				
				if(y == 19)
				{
					if(friend_list[17] != NULL)
					{
				    	y = 17;
						break;
					}
					else
					{
					    break;
					}
				}
				
				if(y == 17)
				{
					if(friend_list[15] != NULL)
					{
				    	y = 15;
						break;
					}
					else
					{
					    break;
					}
				} 

				if(y == 15)
				{
					if(friend_list[13] != NULL)
					{
				    	y = 13;
						break;
					}
					else
					{
					    break;
					}
				   	y = 13;
					break;
				}

			    if(y == 13)
				{
				   	y = 11;
					break;
				}

				if(y == 11)
				{
				    y = 8;
					break;
				}

				if(y == 8)
				{
				    y = 6;
					break;
				}

				if(y == 6)
				{
				    y = 4;
					break;
				}

				if(y == 4)
				{
				    y = 2;
					x = 122;
					break;
				}

				break;
			}

			case KEY_DOWN:
			{
				if(y == 2)
				{
				    y = 4;
					x = 97;
					break;
				}

			    if(y == 4)
				{
				    y = 6;
					break;
				}

				if(y == 6)
				{
				    y = 8;
					break;
				}

				if(y == 8)
				{
				    y = 11;
					break;
				}

				if(y == 11)
				{
				    y = 13; 
					break;
				}

				if(y == 13)
				{
					if(friend_list[15] != NULL)
					{
				    	y = 15;
						break;
					}
					else
					{
						y = 30;
					    break;
					}
				}
				
				if(y == 15)
				{	
					if(friend_list[17] != NULL)
					{
				    	y = 17;
						break;
					}
					else
					{
						y = 30;
					    break;
					}
				}
				
				if(y == 17)
				{
					if(friend_list[19] != NULL)
					{
				    	y = 19;
					    break;
					}
					else
					{
						y = 30;
					    break;
					}
				}
				
				if(y == 19)
				{
					if(friend_list[21] != NULL)
					{
				    	y = 21;
					    break;
					}
					else
					{
						y = 30;
					    break;
					}
				}
				
				if(y == 21)
				{
					if(friend_list[23] != NULL)
					{
				    	y = 23;
					    break;
					}
					else
					{
						y = 30;
					    break;
					}
				}
				
				if(y == 23)
				{
					if(friend_list[25] != NULL)
					{
				    	y = 25;
					    break;
					}
					else
					{
						y = 30;
					    break;
					}
				}
				
				if(y == 25)
				{
					if(friend_list[27] != NULL)
					{
				    	y = 27;
					    break;
					}
					else
					{
						y = 30;
					    break;
					}
				}
				
				if(y == 27)
				{
				    y = 30;
					break;
				}
				break;
			}

			case KEY_RIGHT:
			{
			    if(y == 30 && x == 97)
				{
				    y = 30;
					x = 120;
					break;
				}
				break;
			}

			case KEY_LEFT:
			{
			    if(y == 30 && x == 120)
				{
				    y = 30;
					x = 97;
					break;
				}
				break;
			}

			case '\n':
			{
				switch(y)
				{
				 	//退出TT	
					case 2:									
					{
						close(sockfd);
						endwin();
						exit(0);
						break;
					}

					//编辑资料
				    case 4:
					{
						edit_data();
						break;
					}

					//修改个性签名
				    case 6:
					{
						modify_signature();					
						break;
					}

					//搜索好友
				    case 8:
					{
						search_friend();	
						break;
					}

					//好友列表
				    case 11:
					{
						if(friend_list_mark == CLOSE)	    //显示好友列表 
						{
						    friend_list_mark = OPEN;
							refresh_friend_list();
						}
						else
						{
						    friend_list_mark = CLOSE;       //关闭好友列表
							refresh_friend_list();
						}
						break;
					}
					
					//聊天对象选择(群组列表打开)
					case 13:
					{
						if(friend_list[13] == "my_group")
						{
							if(group_list_mark == CLOSE)
							{
								group_list_mark = OPEN;
								refresh_friend_list();
							}
							else
							{
							    group_list_mark = CLOSE;
								refresh_friend_list();
							}
						}
						else if(friend_list[13] == "group")
						{
							my_strcpy(current_target, "group");
						    group_chat();
						}
						else
						{
							my_strcpy(current_target, friend_list[13]);
							chat();
						}
						
						y = 13;
						x = 97;
					    break;
					}
					case 15:
					{
						if(friend_list[15] == "my_group")
						{
							if(group_list_mark == CLOSE)
							{
								group_list_mark = OPEN;
								refresh_friend_list();
							}
							else
							{
							    group_list_mark = CLOSE;
								refresh_friend_list();
							}
						}
						else if(friend_list[15] == "group")
						{
							my_strcpy(current_target, "group");
						    group_chat();
						}
						else
						{
							my_strcpy(current_target, friend_list[15]);
							chat();
						}

						y = 15;
						x = 97;
					    break;
					}
					case 17:
					{
						if(friend_list[17] == "my_group")
						{
							if(group_list_mark == CLOSE)
							{
								group_list_mark = OPEN;
								refresh_friend_list();
							}
							else
							{
							    group_list_mark = CLOSE;
								refresh_friend_list();
							}
						}
						else if(friend_list[17] == "group")
						{
							my_strcpy(current_target, "group");
						    group_chat();
						}
						else
						{
							my_strcpy(current_target, friend_list[17]);
							chat();
						}
						
						y = 17;
						x = 97;
					    break;
					}
					case 19:
					{
						if(friend_list[19] == "my_group")
						{
							if(group_list_mark == CLOSE)
							{
								group_list_mark = OPEN;
								refresh_friend_list();
							}
							else
							{
							    group_list_mark = CLOSE;
								refresh_friend_list();
							}
						}
						else if(friend_list[19] == "group")
						{
							my_strcpy(current_target, "group");
						    group_chat();
						}
						else
						{
							my_strcpy(current_target, friend_list[19]);
							chat();
						}
						
						y = 19;
						x = 97;
					    break;
					}
					case 21:
					{
						if(friend_list[21] == "my_group")
						{
							if(group_list_mark == CLOSE)
							{
								group_list_mark = OPEN;
								refresh_friend_list();
							}
							else
							{
							    group_list_mark = CLOSE;
								refresh_friend_list();
							}
						}
						else if(friend_list[21] == "group")
						{
							my_strcpy(current_target, "group");
						    group_chat();
						}
						else
						{
							my_strcpy(current_target, friend_list[21]);
							chat();
						}
						
						y = 21;
						x = 97;
					    break;
					}
					case 23:
					{
						if(friend_list[23] == "my_group")
						{
							if(group_list_mark == CLOSE)
							{
								group_list_mark = OPEN;
								refresh_friend_list();
							}
							else
							{
							    group_list_mark = CLOSE;
								refresh_friend_list();
							}
						}
						else if(friend_list[23] == "group")
						{
							my_strcpy(current_target, "group");
						    group_chat();
						}
						else
						{
							my_strcpy(current_target, friend_list[23]);
							chat();
						}
				
						
						y = 23;
						x = 97;
					    break;
					}

					//设置
					case 30:
					{
						if(x == 97) 						//设置
						{
							set_up();
							break;
						}

						if(x == 120)
						{
							close(sockfd);
							endwin();
							exit(0);
						}
					    break;
					}
					
					//其他
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
		
		getyx(stdscr, cur_y, cur_x);
		mvaddstr(cur_y, cur_x - 2, "  ");
	
		getyx(friend_win[0], cur_y, cur_x);
		if(cur_y == 29 && cur_x == 22)
		{
			mvwprintw(friend_win[0], 29, 20, "-");			
			wrefresh(friend_win[0]);
		}
		
		if(y == 30 && x == 120)
		{
			mvwprintw(friend_win[0], 29, 20, "＊");
			wrefresh(friend_win[0]);
		}
		else
		{
			mvaddstr(y, x, "＊");
			refresh();
		}

	}while(func_choose_mark != EXIT);
}


//TT好友列表
void friend()
{
    friend_ui();        //好友列表界面
	friend_choose();    //好友列表功能选择
}
