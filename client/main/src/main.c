/*========================================
*  Copyright (C) 2015 All rights reserved.
*   Filename：      main
*   Author：        Mr.Lu
*   Version：       1.0
*   Date：          2015/07/17
*   Description：   主函数
*   Function List： int main() read_msg()
* 
==========================================*/

#include "../../include/myhead.h"

//线程函数,用来提示并显示聊天信息
void show_msg(void *arg)
{
	int i;
	int msg_prompt_mark = -1;
	char user[20];

	Chat_Msg *temp;
	Chat_Msg *ptr;
	
	my_strcpy(user, (char *)arg);

    while(1)
	{
		while(chat_msg_mark == 0)
		{
		
		}

		temp = chat_msg_head->next;

		//如果有该好友的消息
		while(temp != NULL)
		{
			if(my_strcmp(temp->user, user) == 0)
			{
				msg_prompt_mark = 1;

				while(friend_list_mark != OPEN)
				{
					usleep(500000);
					mvwprintw(friend_win[0], 10, 3, "                       ");
					wrefresh(friend_win[0]);
					usleep(500000);
					mvwprintw(friend_win[0], 10, 1, " >  我的好友           ");
					wrefresh(friend_win[0]);
				}
					
				while(my_strcmp(current_target, user) != 0)
				{
					for(i = 1 ; i < 30; i++)
					{
						if(friend_list[i] == NULL)
						{
							continue;
						}

		    			if(my_strcmp(friend_list[i], user) == 0)
						{
							break;
						}
					}		 

					mvwprintw(friend_win[0], i - 1, 1, "                      ");
					wrefresh(friend_win[0]);
					if(friend_list_mark == CLOSE)
					{
						msg_prompt_mark = 0;
						refresh_friend_list();
						break;
					}
					usleep(500000);
					mvwprintw(friend_win[0], i - 1, 3, "%s", user);
					if(get_friend_state(user) == FRIEND_ONLINE)
					{
						wprintw(friend_win[0], " [在线]");
					}
					else
					{
						wprintw(friend_win[0], " [离线]");
					}
					wrefresh(friend_win[0]);
					if(friend_list_mark == CLOSE)
					{
						msg_prompt_mark = 0;
						refresh_friend_list();
						break;
					}	
					usleep(500000);
				}

				break;
			}

			temp = temp->next;
		}

		if(msg_prompt_mark == 1)
		{
			temp = chat_msg_head->next;
			while(temp != NULL)
			{
				if(my_strcmp(temp->user, user) == 0)
				{
					if(my_strlen(temp->target) > 0)			//如果是文件
					{
						if(chat_row == 14 || chat_row == 15)
						{
							wmove(chat_win[2], 0, 0);
							winsdelln(chat_win[2], -2);
							chat_row = 13;
						}
						if(update_time(temp->time) == YES)
						{
							chat_row -= 1;
							mvwprintw(chat_win[2], chat_row++, (49 - my_strlen(temp->time)) / 2, "%s", temp->time);
						}
						mvwprintw(chat_win[2], chat_row++, 0, "[^_^] [%s] (成功接收文件)", temp->target);
						chat_row++;
						wrefresh(chat_win[2]);
					
						ptr = temp;
						temp = temp->next;
						delete_chat_msg(&ptr);
						continue;
					}

					if(chat_row == 14 || chat_row == 15)	//如果是消息
					{
						wmove(chat_win[2], 0, 0);
						winsdelln(chat_win[2], -2);
						chat_row = 13;
					}

					if(update_time(temp->time) == YES)
					{
						chat_row -= 1;
						mvwprintw(chat_win[2], chat_row++, (49 - my_strlen(temp->time)) / 2, "%s", temp->time);
					}	
					mvwprintw(chat_win[2], chat_row++, 0, "[^_^] %s", temp->message);
					chat_row++;
					wrefresh(chat_win[2]);
					
					ptr = temp;
					temp = temp->next;
					delete_chat_msg(&ptr);
					continue;
				}
			
				temp = temp->next;
			}
		}
			
		if(msg_prompt_mark == 1)
		{
			msg_prompt_mark = -1;
		}
		
		usleep(100000);
		chat_msg_mark = 0;
	}
}

//线程函数,用来提示并显示群组信息
void show_group_msg()
{
	int i;
	int msg_prompt_mark = -1;

	Chat_Msg *temp;
	Chat_Msg *ptr;
    
	while(1)
	{
		pthread_mutex_lock(&mutex2);

		if(group_msg_mark != 1)
		{
			pthread_cond_wait(&cond2, &mutex2);	
		}

		temp = group_msg_head->next;

		while(temp != NULL)
		{
			if(my_strcmp(temp->target, "group") == 0)
			{
				msg_prompt_mark = 1;
				
				while(group_list_mark != OPEN)			
				{
					for(i = 1 ; i < 30; i++)
					{
						if(friend_list[i] == NULL)
						{
							continue;
						}

				    	if(my_strcmp(friend_list[i], "my_group") == 0)
						{
							break;
						}
					}

					mvwprintw(friend_win[0], i - 1, 3, "                       ");
					wrefresh(friend_win[0]);
					usleep(500000);
				
					for(i = 1 ; i < 30; i++)
					{
						if(friend_list[i] == NULL)
						{
							continue;
						}

				    	if(my_strcmp(friend_list[i], "my_group") == 0)
						{
							break;
						}
					}
					mvwprintw(friend_win[0], i - 1, 1, " >  我的群组           ");
					wrefresh(friend_win[0]);
					usleep(500000);
				}

				while(my_strcmp(current_target, "group") != 0)
				{
					for(i = 1 ; i < 30; i++)
					{
						if(friend_list[i] == NULL)
						{
							continue;
						}

		    			if(my_strcmp(friend_list[i], "group") == 0)
						{
							break;
						}
					}
					
					if(group_list_mark == CLOSE)
					{
						msg_prompt_mark = 0;
						refresh_friend_list();
						break;
					}
					
					mvwprintw(friend_win[0], i - 1, 3, "                      ");
					wrefresh(friend_win[0]);
					usleep(500000);
					
					if(group_list_mark == CLOSE)
					{
						msg_prompt_mark = 0;
						refresh_friend_list();
						break;
					}
					
					mvwprintw(friend_win[0], i - 1, 3, "苏嵌96班 [精英群]");
					wrefresh(friend_win[0]);
					usleep(500000);
				}

				break;
			}

			temp = temp->next;
		}

		if(msg_prompt_mark == 1)
		{
			temp = group_msg_head->next;
			while(temp != NULL)
			{
				if(my_strcmp(temp->target, "group") == 0)
				{
					if(chat_row == 14 || chat_row == 15)
					{
						wmove(chat_win[2], 0, 0);
						winsdelln(chat_win[2], -2);
						chat_row = 13;
					}

					if(update_time(temp->time) == YES)
					{
						chat_row -= 1;
						mvwprintw(chat_win[2], chat_row++, (49 - my_strlen(temp->time)) / 2, "%s", temp->time);
					}
					mvwprintw(chat_win[2], chat_row++, 0, "[^_^] %s : %s", temp->user, temp->message);
					chat_row++;
					wrefresh(chat_win[2]);
					wrefresh(chat_win[0]);
					
					ptr = temp;
					temp = temp->next;
					delete_group_msg(&ptr);
					continue;
				}
			
				temp = temp->next;
			}
		}
		
		if(msg_prompt_mark == 1)
		{
			msg_prompt_mark = -1;
		}
		
		group_msg_mark = 0;
		pthread_mutex_unlock(&mutex2);
	} 
}


//线程函数,用来读取服务器回传的信息
void recv_msg()
{
	int i;
	int j = 0;
	int k = 0;
	int x;
	int y;
	int n_read;
	FILE *fp;
	char user[20];
	Message back_msg;
	Friend *friend;
	Friend *temp;
	Chat_Msg *chat_msg;

	chat_row = 1;

	pthread_create(&group_id[0], NULL, (void *)show_group_msg, NULL);
	
	while(1)
	{
		bzero(&back_msg, sizeof(back_msg));

		if((n_read = recv(sockfd, &back_msg, sizeof(back_msg), 0)) == 0)
		{
			clear();
		    mvprintw(LINES/2, COLS/2 - 8, "网络连接中断");
			refresh();
			getch();
			endwin();
			exit(1);
		}

	    switch(back_msg.action)
		{
			//当收到好友状态
			case FRIEND_STATE:
			{
				//保存好友状态到链表中
				friend = (Friend *)malloc(sizeof(Friend));
				friend->state = back_msg.state;
				my_strcpy(friend->user, back_msg.user);
				my_strcpy(friend->signature, back_msg.signature);
				insert_friend_list(&friend);
				
				bzero(user, sizeof(user));
				my_strcpy(user, back_msg.user);
				pthread_create(&friend_id[k++], NULL, (void *)show_msg, &user);
				usleep(100000);
				break;
			}
			
			//当收到好友上线通知时
			case NEW_ONLINE_FRIEND:
			{	
				temp = friend_head->next;		//修改好友状态
				while(temp != NULL)
				{
				    if(my_strcmp(temp->user, back_msg.user) == 0)
					{
					    temp->state = FRIEND_ONLINE;
						break;
					}
					temp = temp->next;
				}
				
				if(friend_list_mark == OPEN)  	//如果好友列表打开，刷新列表
				{
				    refresh_friend_list();
				}
			    break;
			}

			//当收到好友下线通知时
			case NEW_OFFLINE_FRIEND:
			{
				temp = friend_head->next;		//修改好友的状态
				while(temp != NULL)
				{
				    if(my_strcmp(temp->user, back_msg.user) == 0)
					{
					    temp->state = FRIEND_OFFLINE;
						break;
					}
					temp = temp->next;
				}
				
				if(friend_list_mark == OPEN)	//如果好友列表打开，刷新列表
				{
				    refresh_friend_list();
				}
			    break;
			}

			//当收到消息发送成功时
			case SEND_MSG_OK:
			{
				if(chat_row == 14 || chat_row == 15)
				{
					wmove(chat_win[2], 0, 0);
					winsdelln(chat_win[2], -2);
					chat_row = 13;
				}

				if(update_time(back_msg.time) == YES)
				{
					chat_row -= 1;
					mvwprintw(chat_win[2], chat_row++, (49 - my_strlen(back_msg.time)) / 2, "%s", back_msg.time);
				}
				mvwprintw(chat_win[2], chat_row++, (49 - my_strlen(back_msg.message) - 6), "%s [^_^]", back_msg.message);
				chat_row++;
				wrefresh(chat_win[2]);
			    break;
			}

			//当收到好友消息时
			case CHAT:
			{
				if(my_strcmp(back_msg.user, current_target) == 0)  	//如果收到当前聊天对象发来消息
				{
					if(chat_row == 14 || chat_row == 15)
					{
						wmove(chat_win[2], 0, 0);
						winsdelln(chat_win[2], -2);
						chat_row = 13;
					}

					if(update_time(back_msg.time) == YES)
					{
						chat_row -= 1;
						mvwprintw(chat_win[2], chat_row++, (49 - my_strlen(back_msg.time)) / 2, "%s", back_msg.time);
					}
					mvwprintw(chat_win[2], chat_row++, 0, "[^_^] %s", back_msg.message);
					chat_row++;
					wrefresh(chat_win[2]);
					break; 
				}
				else												//如果不是当前聊天目标发来的消息
				{
					chat_msg = (Chat_Msg *)malloc(sizeof(Chat_Msg));
					bzero(chat_msg, sizeof(chat_msg));
					my_strcpy(chat_msg->user, back_msg.user);
					my_strcpy(chat_msg->message, back_msg.message);
					my_strcpy(chat_msg->time, back_msg.time);
					insert_chat_msg(&chat_msg);
					chat_msg_mark = 1;
				    pthread_cond_signal(&cond1);
				}
				break;
			}

			//当收到离线消息时
			case OFFLINE_MSG:
			{
				chat_msg = (Chat_Msg *)malloc(sizeof(Chat_Msg));
				bzero(chat_msg, sizeof(chat_msg));
				my_strcpy(chat_msg->user, back_msg.user);
			//	my_strcpy(chat_msg->target, back_msg.target);
				my_strcpy(chat_msg->message, back_msg.message);
				my_strcpy(chat_msg->time, back_msg.time);
				insert_chat_msg(&chat_msg);
				chat_msg_mark = 1;
			    pthread_cond_signal(&cond1);
				break;
			}

			//当收到好友聊天记录时
			case SEND_MSG_RECORD:
			{
				memset(sql, 0, sizeof(sql));
				sprintf(sql, "insert into user_%s values('%s', '%s', '%s')", current_user, back_msg.target, back_msg.time, back_msg.message);
				if(sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK)
				{
				    perror("sqlite3_exec error!\n");
					exit(1);
				}
				break;
			}

			//当收到群组聊天记录时
			case SEND_GROUP_MSG_RECORD:
			{
				memset(sql, 0, sizeof(sql));
				sprintf(sql, "insert into group_chat values('%s', '%s')", back_msg.time, back_msg.message);
				if(sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK)
				{
				    perror("sqlite3_exec error!\n");
					exit(1);
				}
				break;
			}

			//当收到聊天记录发送完毕时
			case MSG_RECORD_OVER:
			{
				chat_record_mark = 1;
			    pthread_cond_signal(&cond);
			    break;
			}
			
			//当收到聊天记录删除的通知时
			case DEL_MSG_RECORD:
			{
				wclear(chat_win[7]);
				mvwprintw(chat_win[7], 6, 10, "聊天记录为空！");
				wrefresh(chat_win[7]);
				break;
			}
			
			//当收到修改密码成功的通知时
			case CHANGE_PASSWORD_OK:
			{
				wclear(manage_win[6]);
				wrefresh(manage_win[6]);
			    mvwprintw(manage_win[0], 26, 23, "＊恭喜您,密码修改成功＊");
				mvwprintw(manage_win[0], 30, 14, "      按空格键重新操作,按ESC键退出           ");
				wrefresh(manage_win[0]);
				break;
			}

			//当收到修改密码失败的通知时
			case CHANGE_PASSWORD_FAIL:
			{
				wclear(manage_win[6]);
				wrefresh(manage_win[6]);
				mvwprintw(manage_win[0], 26, 28, "＊操作失败＊");
				mvwprintw(manage_win[0], 6, 52, "旧密码有误");
				mvwprintw(manage_win[0], 30, 14, "      按空格键重新操作,按ESC键退出           ");
				wrefresh(manage_win[0]);
				break;
			}

			//当收到发送文件成功的通知时
			case SEND_FILE_OK:
			{
				if(chat_row == 14 || chat_row == 15)
				{
					wmove(chat_win[2], 0, 0);
					winsdelln(chat_win[2], -2);
					chat_row = 13;
				}

				if(update_time(back_msg.time) == YES)
				{
					chat_row -= 1;
					mvwprintw(chat_win[2], chat_row++, (49 - my_strlen(back_msg.time)) / 2, "%s", back_msg.time);
				}
					
				mvwprintw(chat_win[2], chat_row++, (49 - my_strlen(back_msg.file_name) - 14 - 9), "[%s] (成功发送文件) [^_^]", back_msg.file_name);
				chat_row++;
				wrefresh(chat_win[2]);
				break;
			}

			//当收到发送文件失败的通知时
			case SEND_FILE_FAIL:
			{
				if(chat_row == 14 || chat_row == 15)
				{
					wmove(chat_win[2], 0, 0);
					winsdelln(chat_win[2], -2);
					chat_row = 13;
				}

				if(update_time(back_msg.time) == YES)
				{
					chat_row -= 1;
					mvwprintw(chat_win[2], chat_row++, (49 - my_strlen(back_msg.time)) / 2, "%s", back_msg.time);
				}
				
				mvwprintw(chat_win[2], chat_row++, (49 - my_strlen(back_msg.file_name) - 23 - 9), "[%s] [对方离线] 发送文件失败 [^_^]", back_msg.file_name);
				chat_row++;
				wrefresh(chat_win[2]);
				break;
			}

			//当收到好友发送的文件时
			case SEND_FILE:
			{
				if((fp = fopen(back_msg.file_name, "a")) == NULL)
				{
					perror("Recv file error!\n");
					exit(1);
				}

				if(fwrite(back_msg.message, 1, sizeof(back_msg.message), fp) == -1)
				{
					perror("File write error!\n");
					exit(1);
				}
					
				if(my_strcmp(back_msg.user, current_target) == 0)  	//如果收到当前聊天对象发来文件
				{
					if(chat_row == 14 || chat_row == 15)
					{
						wmove(chat_win[2], 0, 0);
						winsdelln(chat_win[2], -2);
						chat_row = 13;
					}
					if(update_time(back_msg.time) == YES)
					{
						chat_row -= 1;
						mvwprintw(chat_win[2], chat_row++, (49 - my_strlen(back_msg.time)) / 2, "%s", back_msg.time);
					}
					mvwprintw(chat_win[2], chat_row++, 0, "[^_^] [%s] (成功接收文件)", back_msg.file_name);
					chat_row++;
					wrefresh(chat_win[2]);
					break;
				}
				else
				{
					chat_msg = (Chat_Msg *)malloc(sizeof(Chat_Msg));
					bzero(chat_msg, sizeof(chat_msg));
					my_strcpy(chat_msg->user, back_msg.user);
					my_strcpy(chat_msg->target, back_msg.file_name);
					my_strcpy(chat_msg->message, back_msg.message);
					my_strcpy(chat_msg->time, back_msg.time);
					insert_chat_msg(&chat_msg);
					chat_msg_mark = 1;
				    pthread_cond_signal(&cond1);
				    break;
				}
				break;
			}
				
			//当收到修改昵称成功的通知时
			case MODIFY_NICKNAME_OK:
			{
				my_strcpy(current_user, back_msg.user);
				mvwprintw(manage_win[0], 13, 17, "修改昵称中...");
				wrefresh(manage_win[0]);
				sleep(1);
				mvwprintw(manage_win[0], 16, 15, "＊昵称修改成功＊");
    			mvwprintw(friend_win[0], 3, 2, "%s        ", back_msg.user);
				wrefresh(manage_win[0]);
				wrefresh(friend_win[0]);
				sleep(2);
				wclear(manage_win[0]);
				wrefresh(manage_win[0]);
				refresh();
				break;
			}

			//当收到修改昵称失败的通知时
			case MODIFY_NICKNAME_FAIL:
			{
				mvwprintw(manage_win[0], 13, 18, "昵称已存在");
				mvwprintw(manage_win[0], 16, 15, "＊昵称修改失败＊");
				wrefresh(manage_win[0]);
				sleep(2);
				wclear(manage_win[0]);
				wrefresh(manage_win[0]);
				refresh();
				break;
			}

			//当收到好友修改昵称的通知时
			case MODIFY_NICKNAME:
			{
				temp = friend_head->next;
				while(temp != NULL)
				{
					if(my_strcmp(temp->user, back_msg.user) == 0)
					{
						my_strcpy(temp->user, back_msg.target);
						break;
					}
					temp = temp->next;
				}

				if(friend_list_mark == OPEN)
				{
					refresh_friend_list();	
				}
				
				break;
			}

			//当收到好友修改个性签名的通知时
			case MODIFY_SIGNATURE:
			{
				//如果好友当前正在聊天界面,直接修改
				if(my_strcmp(back_msg.target, current_target) == 0)
				{
					getyx(chat_win[0], y, x);
					mvwprintw(chat_win[0], 3, 3, "                                      "); 
					mvwprintw(chat_win[0], 3, 3, "%s", back_msg.signature); 
					wmove(chat_win[0], y, x);
					wrefresh(chat_win[0]);
				}
				    
				temp = friend_head->next;
				while(temp != NULL)
				{
				    if(my_strcmp(temp->user, back_msg.target) == 0)
					{
					    bzero(temp->signature, sizeof(temp->signature));
						my_strcpy(temp->signature, back_msg.signature);
					}
						
					temp = temp->next;
				}
				break;
			}
			
			//当收到群聊信息发送成功的通知时
			case SEND_GROUP_MSG_OK:
			{
				if(chat_row == 14 || chat_row == 15)
				{
					wmove(chat_win[2], 0, 0);
					winsdelln(chat_win[2], -2);
					chat_row = 13;
				}

				if(update_time(back_msg.time) == YES)
				{
					chat_row -= 1;
					mvwprintw(chat_win[2], chat_row++, (49 - my_strlen(back_msg.time)) / 2, "%s", back_msg.time);
				}
				mvwprintw(chat_win[2], chat_row++, (49 - my_strlen(back_msg.message) - 6), "%s [^_^]", back_msg.message, back_msg.user);
				chat_row++;
				wrefresh(chat_win[2]);
			    break;
			}

			//当接受到群聊信息时
			case SEND_GROUP_MSG:
			{
				if(my_strcmp(back_msg.user, current_user) != 0)
				{
					if(my_strcmp(current_target, "group") == 0)   	//如果当前在群组聊天
					{
						if(chat_row == 14 || chat_row == 15)
						{
							wmove(chat_win[2], 0, 0);
							winsdelln(chat_win[2], -2);
							chat_row = 13;
						}

						if(update_time(back_msg.time) == YES)
						{
							chat_row -= 1;
							mvwprintw(chat_win[2], chat_row++, (49 - my_strlen(back_msg.time)) / 2, "%s", back_msg.time);
						}
						mvwprintw(chat_win[2], chat_row++, 0, "[^_^] %s : %s", back_msg.user, back_msg.message);
						chat_row++;
						wrefresh(chat_win[2]);
					}
					else											//如果当前不在群组聊天
					{
						chat_msg = (Chat_Msg *)malloc(sizeof(Chat_Msg));
						bzero(chat_msg, sizeof(chat_msg));
						my_strcpy(chat_msg->user, back_msg.user);
						my_strcpy(chat_msg->target, "group");
						my_strcpy(chat_msg->message, back_msg.message);
						my_strcpy(chat_msg->time, back_msg.time);
						insert_group_msg(&chat_msg);
						group_msg_mark = 1;
				  		pthread_cond_signal(&cond2);
					}
				}
				break;
			}

			//当收到离线群组信息时
			case OFFLINE_GROUP_MSG:
			{
				chat_msg = (Chat_Msg *)malloc(sizeof(Chat_Msg));
				bzero(chat_msg, sizeof(chat_msg));
				my_strcpy(chat_msg->user, back_msg.user);
				my_strcpy(chat_msg->target, "group");
				my_strcpy(chat_msg->message, back_msg.message);
				my_strcpy(chat_msg->time, back_msg.time);
				insert_group_msg(&chat_msg);
				group_msg_mark = 1;
			    pthread_cond_signal(&cond2);
				break;
			}

			//当收到群通知时
			case GROUP_NOTICE:
			{
				if(my_strlen(back_msg.message) == 0)
				{
					wclear(chat_win[9]);
					mvwprintw(chat_win[9], 0, 0, "暂无群通知")	;
					wrefresh(chat_win[9]);
					my_strcpy(current_group_notice, "暂无群通知");
				}
				else
				{
					wclear(chat_win[9]);
					mvwprintw(chat_win[9], 0, 0, "%s", back_msg.message);
					wrefresh(chat_win[9]);
					my_strcpy(current_group_notice, back_msg.message);
				}
			    break;
			}
			
			//当收到开启群匿名通知时
			case OPEN_ANONYMITY:
			{
				wclear(chat_win[9]);
			    mvwprintw(chat_win[9], 0, 1, "匿名身份");
			    mvwprintw(chat_win[9], 1, 1, " %s", back_msg.user);
				wrefresh(chat_win[9]);
				my_strcpy(casual_user, current_user);
				my_strcpy(current_user, back_msg.user);
			    break;
			}

			//当收到关闭群匿名通知时
			case CLOSE_ANONYMITY:
			{
			    wclear(chat_win[9]);
				mvwprintw(chat_win[9], 0, 0, "%s", current_group_notice);
				wrefresh(chat_win[9]);
				my_strcpy(current_user, casual_user);
				break;
			}

			//当收到广告信息时
			case ADVERT_INFO:
			{
			    mvwprintw(chat_win[9], 0, 0, "%s", back_msg.message);
				wrefresh(chat_win[9]);
				break;
			}

			default:
			{
				break;
			}
		}
	} 
}


//主函数
int main(int argc, char *argv[])
{
	int ret;
	pthread_t id;

	//服务端地址
	if(argc != 2)
	{
	    fprintf(stderr, "Usage:%s hostname \a\n", argv[0]);
	    exit(1);
	}

    //连接服务器
	sockfd = tcp_connect(argv[1]);

	//界面初始化
	setlocale(LC_ALL, "");
	initscr();
	raw();
	cbreak();

	//进入主界面
	menu();

	//创建好友链表和聊天信息链表
	create_friend_list();           
	create_chat_msg_list();           
	create_group_msg_list();           

	//初始化互斥锁和条件变量
	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_init(&mutex1, NULL);
	pthread_mutex_init(&mutex2, NULL);
	pthread_cond_init(&cond, NULL);
	pthread_cond_init(&cond1, NULL);
	pthread_cond_init(&cond2, NULL);
    
	//创建一个线程读取服务器发来的信息
	if((ret = pthread_create(&id, NULL, (void *)recv_msg, NULL)) != 0)
	{
	    perror("Pthread error!\n");
		exit(1);
	}
	
	//进入好友列表界面
	friend();

	//切换用户
	if(switch_user_mark == SWITCH_USER)
	{
		close(sockfd);
		system("./client 127.0.0.1");
	}

	pthread_join(id, NULL);  

	close(sockfd);

	endwin();

    
	return 0;
}
