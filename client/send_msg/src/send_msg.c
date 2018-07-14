/*========================================
*  Copyright (C) 2015 All rights reserved.
*   Filename：      send_msg
*   Author：        Mr.Lu
*   Version：       2.0
*   Date：          2015/07/27
*   Description：   发送消息
*   Function List： send_msg()
==========================================*/

#include "../../include/myhead.h"

//发送消息
void send_msg()
{
	Message msg;

    echo();
	curs_set(true);

	bzero(&msg, sizeof(msg));
	mvwgetstr(chat_win[1], 0, 0, msg.message);

	if(strlen(msg.message) > 0)
	{
		msg.action = CHAT;
		strcpy(msg.user, current_user);
		strcpy(msg.target, current_target);
	
		if(send(sockfd, &msg, sizeof(msg), 0) == -1)
		{
		    perror("Send msg error!\n");
			exit(1);
		}
	
		wclear(chat_win[1]);	//清掉发送消息窗
		wrefresh(chat_win[1]);
	}

	noecho();
	curs_set(false);
}



//发送表情
void send_expression()
{
	FILE *fp;
	char expression[5];
	
	//窗口转储
	if((fp = fopen("SAVE.txt", "w")) == NULL)
	{
	    perror("无法打开SAVE.txt\n");
		exit(1);
	}
	putwin(chat_win[0], fp);
	putwin(chat_win[2], fp);
	putwin(chat_win[9], fp);
	fclose(fp);
	
	//显示表情窗口
	box(chat_win[4], 0, 0);
	mvwprintw(chat_win[4], 1, 2, "默认                                         X ");
	mvwprintw(chat_win[4], 3, 1, "    (^_^)    (0_0)    (=_=)    (>_<)    (+_+)  ");
	mvwprintw(chat_win[4], 5, 1, "    (T_T)    ($_$)    (~_~)    (w_w)    (Q_Q)  ");
	mvwprintw(chat_win[4], 12, 2, "表情管理          添加表情       1/1 上页 下页");
	wrefresh(chat_win[4]);

	//表情选择
	int cur_x;
	int cur_y;

	int y = 3;
	int x = 3;
	
	int ch;

	int expression_choose_mark = EMPTY;  //将记录选择标志置空
    
	cbreak();
	noecho();
	keypad(chat_win[4], true);
	curs_set(false);

	wrefresh(chat_win[4]);
	wmove(chat_win[4], y, x);
	mvwaddstr(chat_win[4], y, x, "＊");
	wrefresh(chat_win[4]);

	do
	{
	    ch = wgetch(chat_win[4]);

		switch(ch)
		{
			case KEY_DOWN:
			{
				if(y == 1)
				{
					y = 3;
					x = 39;
					break;
				}

				if(y == 3)
				{
					y = 5;
					break;
				}

				break;
			}

			case KEY_UP:
			{
				if(y == 3)
				{
					y = 1;
					x = 44;
				}

				if(y == 5)
				{
				    y = 3;
				}

				break;
			}

			case KEY_RIGHT:
			{
				if(x == 3)
				{
					x = 12;
					break;
				}

				if(x == 12)
				{
					x = 21;
					break;
				}

				if(x == 21)
				{
				    x = 30;
					break;
				}

				if(x == 30)
				{
				    x = 39;
					break;
				}
				break;
			}
			
			case KEY_LEFT:
			{
				if(x == 39)
				{
				    x = 30;
					break;
				}

				if(x == 30)
				{
				    x = 21;
					break;
				}

				if(x == 21)
				{
				    x = 12;
					break;
				}

				if(x == 12)
				{
				    x = 3;
					break;
				}
				break;
			}

			//表情选择
			case '\n':
			{
				if(y == 1)
				{
					expression_choose_mark = EXIT;
					break;
				}

				if(y == 3)
				{
					switch(x)
					{
						case 3:
						{	
							strcpy(expression, "(^_^)");
							expression_choose_mark = EXIT;
							break;
						}
						case 12:
						{
							strcpy(expression, "(0.0)");
							expression_choose_mark = EXIT;
							break;
						}
						case 21:
						{
							strcpy(expression, "(=_=)");
							expression_choose_mark = EXIT;
							break;
						}
						case 30:
						{
							strcpy(expression, "(>_<)");
							expression_choose_mark = EXIT;
							break;
						}
						case 39:
						{
							strcpy(expression, "(+_+)");
							expression_choose_mark = EXIT;
							break;
						}
						default:
						{
							break;
						}
					}

					break;
				}

				if(y == 5)
				{
					switch(x)
					{
						case 3:
						{
							strcpy(expression, "(T_T)");
							expression_choose_mark = EXIT;
							break;
						}
						case 12:
						{
							strcpy(expression, "($_$)");
							expression_choose_mark = EXIT;
							break;
						}
						case 21:
						{
							strcpy(expression, "(~_~)");
							expression_choose_mark = EXIT;
							break;
						}
						case 30:
						{
							strcpy(expression, "(W_W)");
							expression_choose_mark = EXIT;
							break;
						}
						case 39:
						{
							strcpy(expression, "(Q_Q)");
							expression_choose_mark = EXIT;
							break;
						}
						default:
						{
							break;
						}
					}
					break;
				}
				break;
			}
			default:
			{
				break;
			}
		}
		
		getyx(chat_win[4], cur_y, cur_x);
		mvwaddstr(chat_win[4], cur_y, cur_x - 2, "  ");
	
		mvwaddstr(chat_win[4], y, x, "＊");
		wrefresh(chat_win[4]);

	}while(expression_choose_mark != EXIT); 


	//显示原来的窗口
	if((fp = fopen("SAVE.txt", "r")) == NULL)
	{
	    perror("无法打开SAVE.txt\n");
		exit(1);
	}

	wclear(chat_win[4]);
	wrefresh(chat_win[4]);
	chat_win[0] = getwin(fp);
	chat_win[2] = getwin(fp);
	chat_win[9] = getwin(fp);
	wrefresh(chat_win[0]);
	wrefresh(chat_win[2]);
	wrefresh(chat_win[9]);
	fclose(fp);

	//选择完表情之后的消息输入
	if(strlen(expression) > 0)
	{
		char message[1024];
		Message msg;

  	 	echo();
		curs_set(true);

		bzero(&msg, sizeof(msg));
		mvwprintw(chat_win[1], 0, 0, "%s", expression);
		wgetstr(chat_win[1], message);
		sprintf(msg.message, "%s%s", expression, message);

		if(strlen(msg.message) > 0)
		{
			msg.action = CHAT;
			strcpy(msg.user, current_user);
			strcpy(msg.target, current_target);
	
			if(send(sockfd, &msg, sizeof(msg), 0) == -1)
			{
		    	perror("Send msg error!\n");
				exit(1);
			}
		}
	}

	wclear(chat_win[1]);	//清掉发送消息窗
	wrefresh(chat_win[1]);

	noecho();
	curs_set(false);
}


//发送短语
void send_phrase()
{
	FILE *fp;
	char phrase[1024];
	
	//窗口转储
	if((fp = fopen("SAVE.txt", "w")) == NULL)
	{
	    perror("无法打开SAVE.txt\n");
		exit(1);
	}
	putwin(chat_win[0], fp);
	putwin(chat_win[2], fp);
	putwin(chat_win[9], fp);
	fclose(fp);

	//显示短语窗口
	box(chat_win[5], 0, 0);
	mvwprintw(chat_win[5], 1, 2, "默认                                         X ");
	mvwprintw(chat_win[5], 3, 10, "1.You bet!");
	mvwprintw(chat_win[5], 4, 10, "2.Sure thing.");
	mvwprintw(chat_win[5], 5, 10, "3.Take it easy.");
	mvwprintw(chat_win[5], 6, 10, "4.Good for you!");
	mvwprintw(chat_win[5], 7, 10, "5.You are welcome.");
	mvwprintw(chat_win[5], 8, 10, "6.Have a good of it.");
	mvwprintw(chat_win[5], 9, 10, "7.I swear by the god.");
	mvwprintw(chat_win[5], 10, 10,"8.I will play it by ear.");
	mvwprintw(chat_win[5], 12, 2, "短语管理          添加短语       1/1 上页 下页");
	wrefresh(chat_win[5]);

	
	//短语选择
	int cur_x;
	int cur_y;

	int y = 3;
	int x = 6;
	
	int ch;

	int phrase_choose_mark = EMPTY;  //将短语选择标志置空
 	bzero(phrase, sizeof(phrase));	 //清空短语

	cbreak();
	noecho();
	keypad(chat_win[5], true);
	curs_set(false);

	wrefresh(chat_win[5]);
	wmove(chat_win[5], y, x);
	mvwaddstr(chat_win[5], y, x, "＊");
	wrefresh(chat_win[5]);

	do
	{
	    ch = wgetch(chat_win[5]);

		switch(ch)
		{
			case KEY_DOWN:
			{
				if(y == 1)
				{
					y = 3;
					x = 4;
					break;
				}
				if(y == 3)
				{
					y = 4;
					break;
				}
				if(y == 4)
				{
					y = 5;
					break;
				}
				if(y == 5)
				{
					y = 6;
					break;
				}
				if(y == 6)
				{
					y = 7;
					break;
				}
				if(y == 7)
				{
					y = 8;
					break;
				}
				if(y == 8)
				{
					y = 9;
					break;
				}
				if(y == 9)
				{
					y = 10;
					break;
				}
				break;
			}

			case KEY_UP:
			{
				if(y == 10)
				{
					y = 9;
					break;
				}
				if(y == 9)
				{
					y = 8;
					break;
				}
				if(y == 8)
				{
					y = 7;
					break;
				}
				if(y == 7)
				{
					y = 6;
					break;
				}
				if(y == 6)
				{
					y = 5;
					break;
				}
				if(y == 5)
				{
					y = 4;
					break;
				}
				if(y == 4)
				{
					y = 3;
					break;
				}
				if(y == 3)
				{
					y = 1;
					x = 44;
					break;
				}
				break;
			}

			case '\n':
			{
				if(y == 1)
				{
					phrase_choose_mark = EXIT;
					break;
				}

				if(y == 3)
				{
					strcpy(phrase,"You bet!");
					phrase_choose_mark = EXIT;
					break;
				}
				if(y == 4)
				{
					strcpy(phrase, "Sure thing.");
					phrase_choose_mark = EXIT;
					break;
				}
				if(y == 5)
				{
					strcpy(phrase, "Take it easy.");
					phrase_choose_mark = EXIT;
					break;
				}
				if(y == 6)
				{
					strcpy(phrase, "Good for you!");
					phrase_choose_mark = EXIT;
					break;
				} 
				if(y == 7)
				{
					strcpy(phrase, "You are welcome.");
					phrase_choose_mark = EXIT;
					break;
				}
				if(y == 8)
				{
					strcpy(phrase, "I swear by the god.");
					phrase_choose_mark = EXIT;
					break;
				} 
				if(y == 9)
				{
					strcpy(phrase, "Have a good of it.");
					phrase_choose_mark = EXIT;
					break;
				}
				if(y == 10)
				{
					strcpy(phrase, "I will play it by ear.");
					phrase_choose_mark = EXIT;
					break;
				} 
				break;
			}

			default:
			{
				break;
			}
		}

		getyx(chat_win[5], cur_y, cur_x);
		mvwaddstr(chat_win[5], cur_y, cur_x - 2, "  ");
	
		mvwaddstr(chat_win[5], y, x, "＊");
		wrefresh(chat_win[5]);

	}while(phrase_choose_mark != EXIT); 


	//显示原来的窗口
	if((fp = fopen("SAVE.txt", "r")) == NULL)
	{
	    perror("无法打开SAVE.txt\n");
		exit(1);
	}

	wclear(chat_win[5]);
	wrefresh(chat_win[5]);
	chat_win[0] = getwin(fp);
	chat_win[2] = getwin(fp);
	chat_win[9] = getwin(fp);
	wrefresh(chat_win[0]);
	wrefresh(chat_win[2]);
	wrefresh(chat_win[9]);
	fclose(fp);

	if(strlen(phrase) > 0)
	{
		Message msg;
		msg.action = CHAT;
		strcpy(msg.user, current_user);
		strcpy(msg.target, current_target);
		strcpy(msg.message, phrase);
	
		if(send(sockfd, &msg, sizeof(msg), 0) == -1)
		{
		    perror("Send msg error!\n");
			exit(1);
		}
	}
	
	wclear(chat_win[1]);	//清掉发送消息窗
	wrefresh(chat_win[1]);
}

//开始发送文件
void start_send_file(char *file_name)
{
	FILE *fp;
	char buf[1024];
	Message msg;
	
	bzero(&msg, sizeof(msg));
	msg.action = SEND_FILE;
	strcpy(msg.user, current_user);
	strcpy(msg.target, current_target);
	strcpy(msg.file_name, file_name);

	if((fp = fopen(file_name, "r")) == NULL)
	{
		perror("无法打开要发送的文件！\n");
		exit(1);
	}

	while(fread(msg.message, 1, sizeof(msg.message), fp))
	{
		if(send(sockfd, &msg, sizeof(msg), 0) == -1)
		{
			perror("Send file error!\n");
			exit(1);
		}
		bzero(&msg, sizeof(msg));
	}
}


//发送文件
void send_file()
{
	FILE *fp;
	
	//窗口转储
	if((fp = fopen("SAVE.txt", "w")) == NULL)
	{
	    perror("无法打开SAVE.txt\n");
		exit(1);
	}
	putwin(chat_win[0], fp);
	putwin(chat_win[2], fp);
	putwin(chat_win[9], fp);
	fclose(fp);
	
	//显示文件窗口
	box(chat_win[8], 0, 0);
	mvwprintw(chat_win[8], 1, 1, " []                  T T                      X ");
	mvwprintw(chat_win[8], 2, 1, "------------------------------------------------");
	mvwprintw(chat_win[8], 3, 1, " 文件(F) 编辑(E) 查看(V) 位置(P) 帮助(H)");
	mvwprintw(chat_win[8], 4, 1, "------------------------------------------------");
	mvwprintw(chat_win[8], 6, 1, "        a.txt         b.txt         c.txt");
	wrefresh(chat_win[8]);
	
	//文件选择
	int cur_x;
	int cur_y;

	int y = 6;
	int x = 6;
	
	int ch;

	int file_choose_mark = EMPTY;  //将文件选择标志置空

	cbreak();
	noecho();
	keypad(chat_win[8], true);
	curs_set(false);

	wrefresh(chat_win[8]);
	wmove(chat_win[8], y, x);
	mvwaddstr(chat_win[8], y, x, "＊");
	wrefresh(chat_win[8]);

	do
	{
	    ch = wgetch(chat_win[8]);

		switch(ch)
		{
			case KEY_LEFT:
			{
				if(x == 34)
				{
					x = 20;
					break;
				}

				if(x == 20)
				{
					x = 6;
					break;
				}
				break;
			}
			case KEY_RIGHT:
			{
				if(x == 6)
				{
					x = 20;
					break;
				}

				if(x == 20)
				{
					x = 34;
					break;
				}
				break;
			}
			case KEY_UP:
			{
				if(y == 6)
				{
					y = 1;
					x = 44;
					break;
				}
				break;
			}
			case KEY_DOWN:
			{
				if(y == 1)
				{
					y = 6;
					x = 34;
					break;
				}
				break;
			}
			case '\n':
			{
				if(y == 1)
				{
					file_choose_mark = EXIT;
					break;
				}

				if(x == 6)
				{
					start_send_file("a.txt");
					file_choose_mark = EXIT;
					break;
				}
				
				if(x == 20)
				{
					start_send_file("b.txt");
					file_choose_mark = EXIT;
					break;
				}

				if(x == 34)
				{
					start_send_file("c.txt");
					file_choose_mark = EXIT;
					break;
				}
				break;
			}
			default:
			{
				break;
			}
		}
		
		getyx(chat_win[8], cur_y, cur_x);
		mvwaddstr(chat_win[8], cur_y, cur_x - 2, "  ");
		
		mvwaddstr(chat_win[8], y, x, "＊");
		wrefresh(chat_win[8]);

	} while(file_choose_mark != EXIT);
	
	//显示原来的窗口
	if((fp = fopen("SAVE.txt", "r")) == NULL)
	{
	    perror("无法打开SAVE.txt\n");
		exit(1);
	}

	wclear(chat_win[8]);
	wrefresh(chat_win[8]);
	chat_win[0] = getwin(fp);
	chat_win[2] = getwin(fp);
	chat_win[9] = getwin(fp);
	wrefresh(chat_win[0]);
	wrefresh(chat_win[2]);
	wrefresh(chat_win[9]);
	fclose(fp);
}

