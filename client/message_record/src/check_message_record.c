/*========================================
*  Copyright (C) 2015 All rights reserved.
*   Filename：      check_message_record
*   Author：        Mr.Lu
*   Version：       2.0
*   Date：          2015/07/29
*   Description：   查看消息记录
*   Function List： void check_message_record()
==========================================*/

#include "../../include/myhead.h"

//查看好友聊天记录
void check_message_record()
{
	FILE *fp;
	
	//窗口转储
	if((fp = fopen("SAVE.txt", "w")) == NULL)
	{
	    perror("无法打开SAVE.txt\n");
		exit(1);
	}
	putwin(friend_win[0], fp);
	putwin(friend_win[1], fp);
	putwin(friend_win[2], fp);
	putwin(chat_win[0], fp);
	putwin(chat_win[2], fp);
	putwin(chat_win[9], fp);
	fclose(fp);
	
	//打开消息记录界面
	box(chat_win[6], 0, 0);
	mvwprintw(chat_win[6], 1, 1, "                             -  []  X");
	mvwprintw(chat_win[6], 4, 1, "--------------------------------------");
	mvwprintw(chat_win[6], 5, 1, " 消息记录 X｜");
	mvwprintw(chat_win[6], 6, 1, "--------------------------------------");
	mvwprintw(chat_win[6], 7, 1, "   全部  图片  文件");
	mvwprintw(chat_win[6], 25, 1, "--------------------------------------");
	mvwprintw(chat_win[6], 26, 1," 删除聊天记录｜          按左右键翻页");
	wrefresh(chat_win[6]);	
	
	//删除聊天记录	
	sprintf(sql, "delete from user_%s", current_user);
	if(sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK)
	{
	    perror("sqlite3_exec error!\n");
		exit(1);
	}
	
	//获取聊天记录
	Message msg;
	bzero(&msg, sizeof(msg));
	msg.action = CHECK_MSG_RECORD;
	strcpy(msg.user, current_user);
	strcpy(msg.target, current_target);

	if(send(sockfd, &msg, sizeof(msg), 0) == -1)
	{
		perror("Check message record send error!\n");
		exit(1);
	}

	//等待聊天记录接收完毕
	mvwprintw(chat_win[7], 6, 10, "正在加载中...");
	wrefresh(chat_win[7]);
	
	pthread_mutex_lock(&mutex);
	
	if(chat_record_mark != 1)
	{
	    pthread_cond_wait(&cond, &mutex);
	}
	pthread_mutex_unlock(&mutex);

	//打印最后一页聊天记录
	char sql[1024];
	bzero(sql, sizeof(sql));
	sprintf(sql, "select data, message from user_%s", current_user);
	if(sqlite3_get_table(db, sql, &result, &row, &column, &errmsg) != SQLITE_OK)
	{
	    perror("Select chat_record sqlite3_get_table error!\n");
		exit(1);
	}
			
	int i;
	int chat_record_site = 0;
	int record_row = 0;
	wclear(chat_win[7]);
	if(row <= 5 && row > 0)
	{
		mvwprintw(chat_win[7], record_row, 0, "---------  %s  ---------", result[column]);
		strcpy(last_data, result[column]);
		wrefresh(chat_win[7]);
		record_row += 1;
		
		for(i = 1; i <= row; i++)
		{
			if(update_data(result[i * column]) == YES)
			{
				mvwprintw(chat_win[7], record_row, 0, "---------  %s  ---------", result[i * column]);
				record_row += 1;
			}
					
			mvwprintw(chat_win[7], record_row, 0, "%s", result[i * column + 1]);
			record_row += 3;
			wrefresh(chat_win[7]);
		}
		
		chat_record_site = 1;
	}
	else if(row > 5)
	{
		mvwprintw(chat_win[7], record_row, 0, "---------  %s  ---------", result[(row - row % 5 + 1)* column]);
		strcpy(last_data, result[(row - row % 5 + 1) * column]);
		wrefresh(chat_win[7]);
		record_row += 1;

	    for(i = row - row % 5 + 1; i <= row; i++)
		{
			if(update_data(result[i * column]) == YES)
			{
				mvwprintw(chat_win[7], record_row, 0, "---------  %s  ---------", result[i * column]);
				record_row += 1;
			}
			
			mvwprintw(chat_win[7], record_row, 0, "%s", result[i * column + 1]);
			record_row += 3;
			wrefresh(chat_win[7]);
		}

		chat_record_site = row - row % 5 + 1;
	}
	else if(row == 0)
	{
		mvwprintw(chat_win[7], 6, 10, "聊天记录为空！");
		wrefresh(chat_win[7]);
	}


	//功能选择
	int cur_x;
	int cur_y;

	int y = 5;
	int x = 14;
	
	int ch;

	int record_choose_mark = EMPTY;  //将记录选择标志置空
    
	cbreak();
	noecho();
	keypad(chat_win[6], true);
	curs_set(false);

	wrefresh(chat_win[6]);
	wmove(chat_win[6], y, x);
	mvwaddstr(chat_win[6], y, x, "＊");
	wrefresh(chat_win[6]);

	do
	{
	    ch = wgetch(chat_win[6]);

		switch(ch)
		{
		    case KEY_UP:
			{
				if(y == 26)
				{
					y = 5; 
					x = 14;
					break;
				}
				break;
			}
			case KEY_DOWN:
			{
				if(y == 5)
				{
				    y = 26;
					x = 16;
					break;
				}

				break;
			}

			case KEY_LEFT:
			{
				if(chat_record_site <= 1)
				{
				    break;
				}

				record_row = 0;
				wclear(chat_win[7]);
				mvwprintw(chat_win[7], record_row, 0, "---------  %s  ---------", result[(chat_record_site - 5) * column]);
				strcpy(last_data, result[(chat_record_site - 5) * column]);
				record_row += 1;
	    			
				for(i = chat_record_site - 5; i <= chat_record_site - 1; i++)
				{
					if(result[i * column] == NULL)
					{
					    break;
					}

					if(update_data(result[i * column]) == YES)
					{
						mvwprintw(chat_win[7], record_row, 0, "---------  %s  ---------", result[i * column]);
						record_row += 1;
					}
							
					mvwprintw(chat_win[7], record_row, 0, "%s", result[i * column + 1]);
					record_row += 3;
					wrefresh(chat_win[7]);
				}

				chat_record_site -= 5;
			    
			    break;
			}

			case KEY_RIGHT:
			{
				if(chat_record_site >= row - 4)
				{
				    break;
				}
			
				record_row = 0;
				wclear(chat_win[7]);
				mvwprintw(chat_win[7], record_row, 0, "---------  %s  ---------", result[(chat_record_site + 5) * column]);
				strcpy(last_data, result[(chat_record_site + 5) * column]);
				record_row += 1;
	    			
				for(i = chat_record_site + 5; i <= chat_record_site + 9; i++)
				{
					if(result[i * column] == NULL)
					{
					    break;
					}
						
					if(update_data(result[i * column]) == YES)
					{
						mvwprintw(chat_win[7], record_row, 0, "---------  %s  ---------", result[i * column]);
						record_row += 1;
					}
						
					mvwprintw(chat_win[7], record_row, 0, "%s", result[i * column + 1]);
					record_row += 3;
					wrefresh(chat_win[7]);
				}
					
				chat_record_site += 5;
			    
				break;
			}
			
			case '\n':
			{
			    if(y == 5)				//退出聊天记录
				{
					record_choose_mark = EXIT;
					break;
				}

				if(y == 26)				//删除聊天记录
				{
					Message msg;

					msg.action = DEL_MSG_RECORD;
					strcpy(msg.user, current_user);
					strcpy(msg.target, current_target);

					if(send(sockfd, &msg, sizeof(msg), 0) == -1)
					{
						perror("Delete message record error!\n");
						exit(1);
					}
					break;
				}
			}
			default:
			{
				break;
			}
		}
		
		getyx(chat_win[6], cur_y, cur_x);
		mvwaddstr(chat_win[6], cur_y, cur_x - 2, "  ");
	
		mvwaddstr(chat_win[6], y, x, "＊");
		wrefresh(chat_win[6]);

	}while(record_choose_mark != EXIT);

	//显示原来的窗口
	if((fp = fopen("SAVE.txt", "r")) == NULL)
	{
	    perror("无法打开SAVE.txt\n");
		exit(1);
	}

	//显示聊天记录的数据初始化
	bzero(&last_data, sizeof(last_data));
	chat_record_mark = 0;

	wclear(chat_win[6]);
	wrefresh(chat_win[6]);
	friend_win[0] = getwin(fp);
	friend_win[1] = getwin(fp);
	friend_win[2] = getwin(fp);
	chat_win[0] = getwin(fp);
	chat_win[2] = getwin(fp);
	chat_win[9] = getwin(fp);
	wrefresh(friend_win[0]);
	wrefresh(friend_win[1]);
	wrefresh(friend_win[2]);
	wrefresh(chat_win[0]);
	wrefresh(chat_win[2]);
	wrefresh(chat_win[9]);

	fclose(fp);
}








//查看群聊聊天记录
void check_group_msg_record()
{
	FILE *fp;
	
	//窗口转储
	if((fp = fopen("SAVE.txt", "w")) == NULL)
	{
	    perror("无法打开SAVE.txt\n");
		exit(1);
	}
	putwin(friend_win[0], fp);
	putwin(friend_win[1], fp);
	putwin(friend_win[2], fp);
	putwin(chat_win[0], fp);
	putwin(chat_win[2], fp);
	putwin(chat_win[9], fp);
	putwin(chat_win[10], fp);
	fclose(fp);
	
	//打开消息记录界面
	box(chat_win[6], 0, 0);
	mvwprintw(chat_win[6], 1, 1, "                             -  []  X");
	mvwprintw(chat_win[6], 4, 1, "--------------------------------------");
	mvwprintw(chat_win[6], 5, 1, " 消息记录 X｜");
	mvwprintw(chat_win[6], 6, 1, "--------------------------------------");
	mvwprintw(chat_win[6], 7, 1, "   全部 ");
	mvwprintw(chat_win[6], 25, 1, "--------------------------------------");
	mvwprintw(chat_win[6], 26, 1,"              按左右键翻页");
	wrefresh(chat_win[6]);	
	
	//删除聊天记录	
	sprintf(sql, "delete from group_chat");
	if(sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK)
	{
	    perror("sqlite3_exec error!\n");
		exit(1);
	}
	
	//获取聊天记录
	Message msg;
	bzero(&msg, sizeof(msg));
	msg.action = CHECK_GROUP_MSG_RECORD;

	if(send(sockfd, &msg, sizeof(msg), 0) == -1)
	{
		perror("Check message record send error!\n");
		exit(1);
	}

	//等待聊天记录接收完毕
	mvwprintw(chat_win[7], 6, 10, "正在加载中...");
	wrefresh(chat_win[7]);
	
	pthread_mutex_lock(&mutex);
	
	if(chat_record_mark != 1)
	{
	    pthread_cond_wait(&cond, &mutex);
	}
	pthread_mutex_unlock(&mutex);

	//打印最后一页聊天记录
	char sql[1024];
	bzero(sql, sizeof(sql));
	sprintf(sql, "select data, message from group_chat");
	if(sqlite3_get_table(db, sql, &result, &row, &column, &errmsg) != SQLITE_OK)
	{
	    perror("Select group_chat_record sqlite3_get_table error!\n");
		exit(1);
	}
			
	int i;
	int chat_record_site = 0;
	int record_row = 0;
	wclear(chat_win[7]);
	if(row <= 5 && row > 0)
	{
		mvwprintw(chat_win[7], record_row, 0, "---------  %s  ---------", result[column]);
		strcpy(last_data, result[column]);
		wrefresh(chat_win[7]);
		record_row += 1;
		
		for(i = 1; i <= row; i++)
		{
			if(update_data(result[i * column]) == YES)
			{
				mvwprintw(chat_win[7], record_row, 0, "---------  %s  ---------", result[i * column]);
				record_row += 1;
			}
					
			mvwprintw(chat_win[7], record_row, 0, "%s", result[i * column + 1]);
			record_row += 3;
			wrefresh(chat_win[7]);
		}
		
		chat_record_site = 1;
	}
	else if(row > 5)
	{
		mvwprintw(chat_win[7], record_row, 0, "---------  %s  ---------", result[(row - row % 5 + 1)* column]);
		strcpy(last_data, result[(row - row % 5 + 1) * column]);
		wrefresh(chat_win[7]);
		record_row += 1;

	    for(i = row - row % 5 + 1; i <= row; i++)
		{
			if(update_data(result[i * column]) == YES)
			{
				mvwprintw(chat_win[7], record_row, 0, "---------  %s  ---------", result[i * column]);
				record_row += 1;
			}
			
			mvwprintw(chat_win[7], record_row, 0, "%s", result[i * column + 1]);
			record_row += 3;
			wrefresh(chat_win[7]);
		}

		chat_record_site = row - row % 5 + 1;
	}
	else if(row == 0)
	{
		mvwprintw(chat_win[7], 6, 10, "聊天记录为空！");
		wrefresh(chat_win[7]);
	}


	//功能选择
	int cur_x;
	int cur_y;

	int y = 5;
	int x = 14;
	
	int ch;

	int record_choose_mark = EMPTY;  //将记录选择标志置空
    
	cbreak();
	noecho();
	keypad(chat_win[6], true);
	curs_set(false);

	wrefresh(chat_win[6]);
	wmove(chat_win[6], y, x);
	mvwaddstr(chat_win[6], y, x, "＊");
	wrefresh(chat_win[6]);

	do
	{
	    ch = wgetch(chat_win[6]);

		switch(ch)
		{
	/*	    case KEY_UP:
			{
				if(y == 26)
				{
					y = 5; 
					x = 14;
					break;
				}
				break;
			}
			case KEY_DOWN:
			{
				if(y == 5)
				{
				    y = 26;
					x = 16;
					break;
				}

				break;
			}  */

			case KEY_LEFT:
			{
				if(chat_record_site <= 1)
				{
				    break;
				}

				record_row = 0;
				wclear(chat_win[7]);
				mvwprintw(chat_win[7], record_row, 0, "---------  %s  ---------", result[(chat_record_site - 5) * column]);
				strcpy(last_data, result[(chat_record_site - 5) * column]);
				record_row += 1;
	    			
				for(i = chat_record_site - 5; i <= chat_record_site - 1; i++)
				{
					if(result[i * column] == NULL)
					{
					    break;
					}

					if(update_data(result[i * column]) == YES)
					{
						mvwprintw(chat_win[7], record_row, 0, "---------  %s  ---------", result[i * column]);
						record_row += 1;
					}
							
					mvwprintw(chat_win[7], record_row, 0, "%s", result[i * column + 1]);
					record_row += 3;
					wrefresh(chat_win[7]);
				}

				chat_record_site -= 5;
			    
			    break;
			}

			case KEY_RIGHT:
			{
				if(chat_record_site >= row - 4)
				{
				    break;
				}
			
				record_row = 0;
				wclear(chat_win[7]);
				mvwprintw(chat_win[7], record_row, 0, "---------  %s  ---------", result[(chat_record_site + 5) * column]);
				strcpy(last_data, result[(chat_record_site + 5) * column]);
				record_row += 1;
	    			
				for(i = chat_record_site + 5; i <= chat_record_site + 9; i++)
				{
					if(result[i * column] == NULL)
					{
					    break;
					}
						
					if(update_data(result[i * column]) == YES)
					{
						mvwprintw(chat_win[7], record_row, 0, "---------  %s  ---------", result[i * column]);
						record_row += 1;
					}
						
					mvwprintw(chat_win[7], record_row, 0, "%s", result[i * column + 1]);
					record_row += 3;
					wrefresh(chat_win[7]);
				}
					
				chat_record_site += 5;
			    
				break;
			}
			
			case '\n':
			{
			    if(y == 5)				//退出聊天记录
				{
					record_choose_mark = EXIT;
					break;
				}

				break;
			}
			default:
			{
				break;
			}
		}
		
		getyx(chat_win[6], cur_y, cur_x);
		mvwaddstr(chat_win[6], cur_y, cur_x - 2, "  ");
	
		mvwaddstr(chat_win[6], y, x, "＊");
		wrefresh(chat_win[6]);

	}while(record_choose_mark != EXIT);

	//显示原来的窗口
	if((fp = fopen("SAVE.txt", "r")) == NULL)
	{
	    perror("无法打开SAVE.txt\n");
		exit(1);
	}

	//显示聊天记录的数据初始化
	bzero(&last_data, sizeof(last_data));
	chat_record_mark = 0;

	wclear(chat_win[6]);
	wrefresh(chat_win[6]);
	friend_win[0] = getwin(fp);
	friend_win[1] = getwin(fp);
	friend_win[2] = getwin(fp);
	chat_win[0] = getwin(fp);
	chat_win[2] = getwin(fp);
	chat_win[9] = getwin(fp);
	chat_win[10] = getwin(fp);
	wrefresh(friend_win[0]);
	wrefresh(friend_win[1]);
	wrefresh(friend_win[2]);
	wrefresh(chat_win[0]);
	wrefresh(chat_win[2]);
	wrefresh(chat_win[9]);
	wrefresh(chat_win[10]);

	fclose(fp);
}

