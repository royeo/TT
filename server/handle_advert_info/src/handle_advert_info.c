/*========================================
*  Copyright (C) 2015 All rights reserved.
*   Filename：      handle_advert_info
*   Author：        Mr.Lu
*   Version：       2.0
*   Date：          2015/8/2
*   Description：	处理广告信息
*   Function List： handle_advert_info()
==========================================*/
#include "../../include/myhead.h"

void handle_advert_info(Message msg, int sockfd)
{
    Message back_msg;

	back_msg.action = ADVERT_INFO;

	char advert_1[1024] = "\n[IT]苏嵌教育\n\n\n 嵌入式技术\n   哪家强 ->\n\n  江苏南京\n   找老梁\n\n\n……… ……… …… ……\n\n\n  培训4个月\n\n  高薪offer\n\n  等你来拿";

	char advert_2[1024] = "\n   IPHONE 47\n\n 27寸大屏体验\n\n  只要￥9998\n\n  __________\n |  * ——    |\n |----------|\n |          |\n |          |\n |          |\n |          |\n |          |\n |          |\n |          |\n |----------|\n |    ＊    |\n  ----------";

	char advert_3[1024] = "  JD.COM 京东\n--------------\n   ________\n  |   --   |\n  |        |\n  |        |\n  |        |\n  |        |\n  |--------|\n  |        |\n  |--------|\n  |        |\n  |        |\n  |        |\n  |        |\n  |        |\n  +--------+\n\n\n   低价秒杀";

	char advert_4[1024] = "\n\n\n  TT盒子\n\n 暑假看片神器\n\n\n    ￥199\n    优惠价\n\n     --------\n   /        /)  /   TT   /)\n /        /)\n --------";
	
	char advert_5[1024] = "\n\n  --- --- ---\n |   |   |   | |  --- ---  |  -|   |   |-\n   |  ---- |\n    -|    |-\n     |红牛|\n      ----\n\n\n    立减5元\n\n    ￥34.5\n\n    /6联包";

	srand((unsigned)time(NULL));
	int num = rand() % 5;          
	
	switch(num)
	{
		case 0:
		{
	    	my_strcpy(back_msg.message, advert_1);
			break;
		}
		case 1:
		{
	    	my_strcpy(back_msg.message, advert_2);
		    break;
		}
		case 2:
		{
	    	my_strcpy(back_msg.message, advert_3);
		    break;
		}
		case 3:
		{
	    	my_strcpy(back_msg.message, advert_4);
		    break;
		}
		case 4:
		{
	    	my_strcpy(back_msg.message, advert_5);
		    break;
		}
		default:
		{
		    break;
		}
	}

	if(send(sockfd, &back_msg, sizeof(back_msg), 0) == -1)
	{
	    perror("Send advert info error!\n");
		exit(1);
	}

}

