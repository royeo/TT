/*========================================
*  Copyright (C) 2015 All rights reserved.
*   Filename：      set_up
*   Author：        Mr.Lu
*   Version：       2.0
*   Date：          2015/07/30
*   Description：   设置操作
*   Function List： set_up()
==========================================*/

#include "../../include/myhead.h"

//设置操作
void set_up()
{
	FILE *fp;
	
	//好友列表窗口转储
	if((fp = fopen("SAVE.txt", "w")) == NULL)
	{
	    perror("无法打开SAVE.txt\n");
		exit(1);
	}
	putwin(friend_win[0], fp);
	putwin(friend_win[1], fp);
	putwin(friend_win[2], fp);
	fclose(fp);


	//显示设置界面
	box(friend_win[4], 0, 0);
	mvwprintw(friend_win[4], 2, 2, "修改密码");
	mvwprintw(friend_win[4], 4, 2, "切换帐号");
	mvwprintw(friend_win[4], 6, 2, "TT会员         >");
	mvwprintw(friend_win[4], 8, 2, "帮助           >");
	mvwprintw(friend_win[4], 10, 2,"软件升级 [new]");
	mvwprintw(friend_win[4], 12, 1,  "------------------");
	mvwprintw(friend_win[4], 13, 14, "退出");
	wrefresh(friend_win[4]);

	//功能选择
	int cur_x;
	int cur_y;

	int y = 17;
	int x = 97;
	
	int ch;
   
	int setup_choose_mark = EMPTY;

	cbreak();
	noecho();
	keypad(stdscr, true);
	curs_set(false);

	getyx(stdscr, cur_y, cur_x);
	mvaddstr(cur_y, cur_x - 2, "  ");
	
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
				if(y == 28)
				{
					y = 25;
					break;
				}
				if(y == 25)
				{
					y = 23;
					break;
				}
				if(y == 23)
				{
					y = 21;
					break;
				}
				if(y == 21)
				{
					y = 19;
					break;
				}
				if(y == 19)
				{
				    y = 17;
					break;
				}
				break;
			}
			case KEY_DOWN:
			{
				if(y == 17)
				{
					y = 19;
					break;
				}
				if(y == 19)
				{
					y = 21;
					break;
				}
				if(y == 21)
				{
					y = 23;
					break;
				}
				if(y == 23)
				{
					y = 25;
					break;
				}
				if(y == 25)
				{
					y = 28;
					break;
				}
				break;
			}
			case '\n':
			{
				//退出
				if(y == 28)
				{
					setup_choose_mark = EXIT;
					break;
				}
				
				//修改密码
				if(y == 17)
				{
					change_password();
					break;
				}

				//切换用户
				if(y == 19)
				{
					setup_choose_mark = EXIT;
					switch_user_mark = SWITCH_USER;
					func_choose_mark = EXIT;
					break;
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
		
		if(cur_y == 17 && cur_x == 97)
		{
			refresh();
		}
		
		mvaddstr(y, x, "＊");
		refresh();

	}while(setup_choose_mark != EXIT);

	wclear(friend_win[4]);
	wrefresh(friend_win[4]);

	//显示原来的窗口
	if((fp = fopen("SAVE.txt", "r")) == NULL)
	{
	    perror("无法打开SAVE.txt\n");
		exit(1);
	}
	
	friend_win[0] = getwin(fp);
	friend_win[1] = getwin(fp);
	friend_win[2] = getwin(fp);
	wrefresh(friend_win[0]);
	wrefresh(friend_win[1]);
	wrefresh(friend_win[2]);
	fclose(fp);

	getyx(stdscr, cur_y, cur_x);
	mvaddstr(cur_y, cur_x - 2, "  ");

	mvwprintw(stdscr, 30, 97, "＊");
	refresh();
}


