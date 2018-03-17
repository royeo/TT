/*========================================
*  Copyright (C) 2015 All rights reserved.
*   Filename：      tcp_connect.c
*   Author：        Mr.Lu
*   Version：       1.0
*   Date：          2015/07/17
*   Description：   tcp连接
*   Function List： tcp_connect(const char *ip)
==========================================*/

#include "../../include/myhead.h"

int tcp_connect(const char *ip)
{
    int sockfd;
    struct sockaddr_in server_addr;

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
	    perror("socket error!\n");
		exit(1);
	}

	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = inet_addr(ip);

	if(connect(sockfd, (struct sockaddr *)(&server_addr), sizeof(server_addr)) == -1)
	{
	    perror("connect error!\n");
		exit(1);
	}

	return sockfd;
}
