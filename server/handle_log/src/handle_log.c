/*==================================
*  Copyright (C) 2015 All rights reserved.
*   Filename：      handle_log.c
*   Author：        Mr.Lu
*   Version：       1.0
*   Date：          2015/07/23
*   Description：	登录处理
*   Function List： void handle_log()
*					int check_log_info()
* 
==========================================*/

#include "../../include/myhead.h"

//检查登录信息
int check_log_info(Message msg, Message *back_msg, int sockfd)
{
	int rc;
	int row;
	int column;
	char *errmsg = NULL;
	char **result = NULL;
	char sql[1024];
	sqlite3 *db;         
	Client_Info *new_client;
	Client_Info *temp = client_head->next;

	//检查帐号密码是否正确
	if(sqlite3_open("user_info.db", &db) != SQLITE_OK)
	{
	    perror("Login open error!\n");
		exit(1);
	}
	
	sprintf(sql, "select id, user, password, signature from user_info where id = '%s' or user = '%s'", msg.user, msg.user);
	if((rc = sqlite3_get_table(db, sql, &result, &row, &column, &errmsg)) != SQLITE_OK)
	{
	    perror("Login get table error!\n");
		exit(1);
	}

	printf("检查帐号密码中...\n");
	if(row == 0)
	{
		printf("帐号不存在！\n");
		return ACCOUNT_NOT_EXIST;     //返回帐号不存在
	}
	else
	{
		printf("密码正确！\n");
	  	if(my_strcmp(msg.password, result[6]) == 0)
		{
			//检查用户是否在线
			while(temp != NULL)
			{
			    if(my_strcmp(msg.user, temp->user) == 0)
				{
					sqlite3_close(db);
					printf("用户已登录!\n");
				    return LOGIN_ALREADY;   
				}
				temp = temp->next;
			}

			//插入新的客户端信息到链表中
			new_client = (Client_Info *)malloc(sizeof(Client_Info));
			new_client->new_fd = sockfd;
			my_strcpy(new_client->id, result[4]);
			my_strcpy(new_client->user, result[5]);
			insert_new_client(&new_client);
			
			//回传用户的用户名和个性签名
			my_strcpy(back_msg->user, result[5]);      
			if(result[7] != NULL)
			{
				my_strcpy(back_msg->signature, result[7]); 
			}
			
			printf("%s成功登录！\n", back_msg->user);
			sqlite3_close(db);
			return LOGIN_OK;          
		}
		else
		{
			sqlite3_close(db);
			printf("密码错误!\n");
		    return PASSWORD_ERROR;    
		}
	}
	
	sqlite3_close(db);
	printf("服务器处理失败!\n");
   	return SERVER_FAILURE;             
}
	

//处理用户的登录操作
void handle_log(Message msg, int sockfd)
{
	Message back_msg;

	bzero(&back_msg, sizeof(back_msg));

	//将处理后的信息回传给客户端
	back_msg.action = check_log_info(msg, &back_msg, sockfd);
	if(send(sockfd, &back_msg, sizeof(back_msg), 0) == -1)
	{
	    perror("Login send error!\n");
		exit(1);
	}
	printf("登录信息发送成功！\n");

	//如果用户登录成功后
	if(back_msg.action == LOGIN_OK)
	{
		int i;
		int rc;
		int row;
		int column;
		int online_flag;
		char *errmsg = NULL;
		char **result = NULL;
		char active_user[20];
		sqlite3 *db;         
		Client_Info *temp = client_head->next;
		Offline_Msg *temp1 = offline_head->next;
		Offline_File *temp2 = offline_file_head->next;
		Offline_Group_Msg *temp3 = offline_group_head->next;

		my_strcpy(active_user, back_msg.user);
		bzero(&back_msg, sizeof(back_msg));
		
		//将所有用户信息发给当前用户
		if(sqlite3_open("user_info.db", &db) != SQLITE_OK)
		{
	    	perror("Send online user info sqlite3_open error!\n");
			exit(1);
		}	
	
		if((rc = sqlite3_get_table(db, "select user, signature from user_info", &result, &row, &column, &errmsg)) != SQLITE_OK)
		{
	    	perror("Send online user info sqlite3_get_table error!\n");
			exit(1);
		}
		
		for(i = 1; i <= row; i++)
		{
			online_flag = FRIEND_OFFLINE;
			
			temp = client_head->next;
			while(temp != NULL)
			{
				if(my_strcmp(temp->user, result[i * column]) == 0)
				{
			    	online_flag = FRIEND_ONLINE;
					break;
				}

				temp = temp->next;
			}
				
			if(my_strcmp(result[i * column], active_user) == 0)
			{
				printf("自己不发送\n");
				continue;
			}

			bzero(&back_msg, sizeof(back_msg));
			back_msg.action = FRIEND_STATE;
			back_msg.state = online_flag;
			my_strcpy(back_msg.user, result[i * column]);
			if(result[i * column + 1] != NULL)
			{
				my_strcpy(back_msg.signature, result[i * column + 1]);
			}
			printf("好友：%s\n", back_msg.user);
			printf("个性签名：%s\n", back_msg.signature);
		
			if(send(sockfd, &back_msg, sizeof(back_msg), 0) == -1)
			{
	   			perror("Send online user info error!\n");
				exit(1);
			}
		}
		printf("所有用户信息已发送！\n");
		sqlite3_close(db);

		//发送新用户上线信息给所有在线用户
		temp = client_head->next;

		bzero(&back_msg, sizeof(back_msg));
		back_msg.action = NEW_ONLINE_FRIEND;
		my_strcpy(back_msg.user, msg.user);
		while(temp != NULL)
		{
			if(my_strcmp(temp->user, msg.user) == 0)
			{
			    temp = temp->next;
				continue;
			}
			
			if(send(temp->new_fd, &back_msg, sizeof(back_msg), 0) == -1)
			{
			    perror("Send new user online msg error!\n");
				exit(1);
			}

		    temp = temp->next;
		}

		//检测是否有离线消息				
		printf("检测是否有离线消息\n");
		while(temp1 != NULL)
		{
		    if(my_strcmp(temp1->target, msg.user) == 0)
			{
				bzero(&back_msg, sizeof(back_msg));
				back_msg.action = OFFLINE_MSG;
			    my_strcpy(back_msg.user, temp1->user);
			    my_strcpy(back_msg.message, temp1->message);
			    my_strcpy(back_msg.time, temp1->time);
				
	 			if(send(sockfd, &back_msg, sizeof(back_msg), 0) == -1)
				{
				    perror("Send new user online msg error!\n");
					exit(1);
				}
				delete_offline_msg(&temp1);
				printf("离线消息发送成功!\n");
			}
			temp1 = temp1->next;
		}

		//检查是否有离线文件
		printf("检测是否有离线文件\n");
		while(temp2 != NULL)
		{
		    if(my_strcmp(temp2->target, msg.user) == 0)
			{
				bzero(&back_msg, sizeof(back_msg));
				
				back_msg.action = OFFLINE_FILE;
			    my_strcpy(back_msg.user, temp2->user);
			    my_strcpy(back_msg.file_name, temp2->file_name);
			    my_strcpy(back_msg.message, temp2->file_content);
				
	 			if(send(sockfd, &back_msg, sizeof(back_msg), 0) == -1)
				{
				    perror("Send new user online file error!\n");
					exit(1);
				}
				delete_offline_file(&temp2);
				printf("离线文件发送成功!\n");
			}
			temp2 = temp2->next;
		}
		
		//检查是否有离线群聊信息
		printf("检查是否有离线群聊信息\n");
		while(temp3 != NULL)
		{
		    if(my_strcmp(temp3->target, msg.user) == 0)
			{
				bzero(&back_msg, sizeof(back_msg));
				back_msg.action = OFFLINE_GROUP_MSG;
			    my_strcpy(back_msg.user, temp3->user);
			    my_strcpy(back_msg.message, temp3->message);
			    my_strcpy(back_msg.time, temp3->time);
				
	 			if(send(sockfd, &back_msg, sizeof(back_msg), 0) == -1)
				{
				    perror("Send new user online group msg error!\n");
					exit(1);
				}
				delete_offline_group_msg(&temp3);
				printf("离线消息发送成功!\n");
			}
			temp3 = temp3->next;
		}
	}
}
