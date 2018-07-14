/*========================================
*  Copyright (C) 2015 All rights reserved.
*   Filename：      detail_func.c
*   Author：        Mr.Lu
*   Version：       2.0
*   Date：          2015/07/25
*   Description：   小功能
*   Function List： void password_input()
*					void get_auth_code()
*					void current_time()
*					void continue_choose()
* 
==========================================*/

#include "../../include/myhead.h"

//延时函数
void delay(int n)
{
	int i;
	int j;

	for(i = 0; i < n; i++)
	{
	    for(j = 0; j < 99999999; j++)
		{
			;
		}
	}
}


//获取验证码
void get_auth_code(char *auth_code)
{
    srand((unsigned)time(NULL));
	int num = rand() % 1000 + 1000;
	
	while(num > 0)
	{
	    *auth_code = num % 10 + 'A';
		num /= 10;
		auth_code++;
	}

	*auth_code = '\0';
}


//获取当前时间
void get_current_time(char *tm)
{
	struct tm *now;
	long t;
	t = time(NULL);
	now = localtime(&t);

//	sprintf(tm, "%d/%02d/%02d %02d:%02d", now->tm_year+1900, now->tm_mon + 1,now->tm_mday, now->tm_hour, now->tm_min);
	sprintf(tm, "%02d:%02d:%02d", now->tm_hour, now->tm_min, now->tm_sec);
}

//更新时间
int update_time(char *now_time)
{
	if((now_time[3] == last_time[3]) && (now_time[4] == last_time[4]))
	{
		return NO;
	} 
	    
	strcpy(last_time, now_time);
	return YES;
}

//更新日期
int update_data(char *now_data)
{
	if(strcmp(now_data, last_data) == 0)
	{
		return NO;
	}

	strcpy(last_data, now_data);
	return YES;
}

//密码输入，显示星号
void password_input(char *password)
{
    int x;
	int y;
	int i = 0;
	int ch;
	char buf[20];

	noecho();
	cbreak();
	intrflush(stdscr, false);
	refresh();

	while((ch = getch()) != '\n')
	{
	    if(ch == 127)
		{
		    if(i == 0)
			{
			    continue;
			}
			else if(i > 0)
			{
			    echo();
				getyx(stdscr, y, x);
				mvaddch(y, --x, ' ');
				move(y, x);
				refresh();
				buf[i] = '\0';
				i = i - 1;
				noecho();
				continue;
			}
		}

		buf[i] = (char)ch;
		i++;

		echo();
		printw("*");
		refresh();
		noecho();
	}

	echo();
	nocbreak();
	buf[i] = '\0';
	strcpy(password, buf);
}

//继续或退出
void continue_choose(WINDOW *win)
{
	int ch;
    
	cbreak();
	noecho();
	keypad(reg_win[0], true);
	curs_set(false);

	do
	{
	    ch = wgetch(win);

		switch(ch)
		{
		    case 32:
			{
			    continue_mark = CONTINUE;
				break;
			}

			case 27:
			{
			    continue_mark = EXIT;
				break;
			}

			default:
			{
				continue_mark = EMPTY;
			    break;
			}
		}
	} while(!continue_mark);
}

//获取聊天目标的个性签名
void search_signature(char *target, char *target_signature)
{
	Friend *temp = friend_head;

	while(temp != NULL)
	{
	    if(strcmp(target, temp->user) == 0)
		{
		    strcpy(target_signature, temp->signature);
			break;
		}

		temp = temp->next;
	}
}

//获取好友的状态
int get_friend_state(char *friend)
{
    Friend *temp = friend_head;

	while(temp != NULL)
	{
	    if(strcmp(temp->user, friend) == 0)
		{
		    if(temp->state == FRIEND_ONLINE)
			{
			    return FRIEND_ONLINE;
			}
			else
			{
			    return FRIEND_OFFLINE;
			}
		}

		temp = temp->next;
	}

	return CLIENT_FAIL; 
}

//获得群通知
void get_group_notice()
{
    Message msg;

	msg.action = GROUP_NOTICE;
	strcpy(msg.user, current_user);

	if(send(sockfd, &msg, sizeof(msg), 0) == -1)
	{
	    perror("Get group notice send error!\n");
		exit(1);
	}
}

//获取广告信息
void get_advert_info()
{
    Message msg;

	msg.action = ADVERT_INFO;
	strcpy(msg.user, current_user);

	if(send(sockfd, &msg, sizeof(msg), 0) == -1)
	{
	    perror("Get advert info send error!\n");
		exit(1);
	}
}

//开启匿名聊天
void open_anonymity()
{
    Message msg;

	msg.action = OPEN_ANONYMITY;
	strcpy(msg.user, current_user);

	if(send(sockfd, &msg, sizeof(msg), 0) == -1)
	{
	    perror("Open anonymity send error!\n");
		exit(1);
	}
}

//关闭匿名聊天
void close_anonymity()
{
    Message msg;

	msg.action = CLOSE_ANONYMITY;
	strcpy(msg.user, current_user);

	if(send(sockfd, &msg, sizeof(msg), 0) == -1)
	{
	    perror("Close anonymity send error!\n");
		exit(1);
	}
}


//密码输入，显示星号(窗口)
void password_input_win(WINDOW *win, char *password)
{
    int x;
	int y;
	int i = 0;
	int ch;
	char buf[20];

	noecho();
	cbreak();
	intrflush(win, false);
	wrefresh(win);

	while((ch = wgetch(win)) != '\n')
	{
	    if(ch == 127)
		{
		    if(i == 0)
			{
			    continue;
			}
			else if(i > 0)
			{
			    echo();
				getyx(win, y, x);
				mvwaddch(win, y, --x, ' ');
				wmove(win, y, x);
				wrefresh(win);
				buf[i] = '\0';
				i = i - 1;
				noecho();
				continue;
			}
		}

		buf[i] = (char)ch;
		i++;

		echo();
		wprintw(win, "*");
		wrefresh(win);
		noecho();
	}

	echo();
	nocbreak();
	buf[i] = '\0';
	strcpy(password, buf);
}

