#ifndef MY_HEAD_
#define MY_HEAD_

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sqlite3.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <time.h>

#define port 3333     //端口号为3333

enum OPT{
	EMPTY,			    //空
	CONTINUE,			//继续
	
	CHAT,        		//聊天
	CHATALL,			//群聊
	EMOJI,				//发送表情
	PHRASE,				//发送短语
	CHECK_RECORD,		//查看聊天记录
	DEL_RECORD,	  	    //删除聊天记录
	CHANGE_NAME,		//修改昵称
	SWITCH_USER,		//切换用户
	EXIT,        		//退出
	WRONG_CMD,   		//错误命令
	SERVER_FAILURE,  	//未知错误

	LOGIN,       		//登录
	LOGIN_OK,  			//登录成功
	LOGIN_FAIL,      	//登录失败
	LOGIN_ALREADY,   	//用户已登录
	
	PASSWORD_OK,     	//密码正确
	PASSWORD_ERROR,  	//密码错误
	PASSWORD_BACK, 		//忘记密码
	PASSWORD_BACK_OK,   //找回密码成功
	PASSWORD_BACK_FAIL, //找回密码失败
	
	REGIST,      		//注册
	REGIST_OK,       	//注册成功
	REGIST_FAIL,     	//注册失败

	CHECK_ACCOUNT,  	//检查帐号
	ACCOUNT_EXIST,      //帐号存在
	ACCOUNT_NOT_EXIST,	//帐号不存在

	GET_FRIEND_INFO,	//获取好友信息
	FRIEND_STATE,		//好友状态
	FRIEND_ONLINE,		//好友在线
	FRIEND_OFFLINE,		//好友离线
	NEW_ONLINE_FRIEND,  //好友上线
	NEW_OFFLINE_FRIEND, //好友下线

	EDIT_DATA,  		//编辑资料
	MODIFY_SIGNATURE,	//修改签名
	SEARCH_FRIEND,      //搜索好友
	FRIENDS_LIST, 		//好友列表
	
	SEND_MSG_OK,      	//发送信息成功
	OFFLINE_MSG, 	  	//离线消息

	GROUP_NOTICE,			//群通知
	CHANGE_GROUP_NOTICE,	//修改群通知
	CHANGE_GROUP_NOTICE_OK,	//修改群通知成功

	SEND_GROUP_MSG,		//发送群组消息
	SEND_GROUP_MSG_OK,	//发送群组消息成功
	SEND_GROUP_MSG_FAIL,//发送群组消息失败
	OFFLINE_GROUP_MSG,	//离线群组信息
	SEND_GROUP_FILE,	//发送群文件
	
	CHECK_MSG_RECORD,		//查看好友聊天记录
	SEND_MSG_RECORD,		//发送好友聊天记录
	DEL_MSG_RECORD,			//删除好友聊天记录
	
	CHECK_GROUP_MSG_RECORD,	//查看群组聊天记录
	SEND_GROUP_MSG_RECORD,	//发送群组聊天记录
	MSG_RECORD_OVER,		//聊天记录发送完毕

	CHANGE_PASSWORD,		//修改密码
	CHANGE_PASSWORD_OK,	  	//修改密码成功
	CHANGE_PASSWORD_FAIL,	//修改密码失败

	MODIFY_NICKNAME,		//修改昵称
	MODIFY_NICKNAME_OK,		//修改昵称成功
	MODIFY_NICKNAME_FAIL,	//修改昵称失败
	
	SEND_FILE,			//发送文件
	SEND_FILE_OK,		//发送文件成功
	SEND_FILE_FAIL,		//发送文件失败
	OFFLINE_FILE,		//发送离线文件

	ADVERT_INFO,		//广告信息
	
	OPEN_ANONYMITY,		//开启匿名聊天
	CLOSE_ANONYMITY,	//关闭匿名聊天
};

//发送给的客户端的信息
typedef struct _Message
{
    int action;
	int state;
	char id[20];
	char user[20];
	char password[20];
	char time[20];
	char target[20];
	char message[1024];
	char question[1024];
	char answer[1024];
	char signature[25];
	char file_name[20];
} Message;

//保存离线消息
typedef struct _Offline_Msg
{
    char user[20];
	char target[20];
	char message[1024];
	char time[20];
	struct _Offline_Msg *next;
} Offline_Msg;

//保存离线群聊信息
typedef struct _Offline_Group_Msg
{
    char user[20];
	char target[20];
	char message[1024];
	char time[20];
	struct _Offline_Group_Msg *next;
} Offline_Group_Msg;

//保存离线文件
typedef struct _Offline_File
{
	char user[20];
	char target[20];
	char time[20];
	char file_name[100];
	char file_content[1024];
	struct _Offline_File *next;
} Offline_File;

//保存所有客户端的套接字和用户名
typedef struct _Client_Info
{
    int new_fd;
	char id[20];
	char user[20];
	struct _Client_Info *next;
} Client_Info;

Client_Info *client_head;    			//客户端链表的头结点
Offline_Msg *offline_head;  			//离线消息链表的头结点
Offline_File *offline_file_head; 		//离线文件链表的头结点
Offline_Group_Msg *offline_group_head; 	//离线群组消息链表的头结点

#endif
