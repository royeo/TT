#ifndef MY_HEAD
#define MY_HEAD

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <sqlite3.h>
#include <ncurses.h>
#include <curses.h>
#include <locale.h>
#include <time.h>

#define port 3333     //端口号为8000

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

	GET_FRIEND_INFO, 	//获取好友信息
	FRIEND_STATE,		//好友状态
	FRIEND_ONLINE,		//好友在线
	FRIEND_OFFLINE, 	//好友离线
	NEW_ONLINE_FRIEND, 	//好友上线
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

	CHECK_MSG_RECORD,	//查看好友聊天记录
	SEND_MSG_RECORD,	//发送好友聊天记录
	DEL_MSG_RECORD,		//删除好友聊天记录
	
	CHECK_GROUP_MSG_RECORD,	//查看群组聊天记录
	SEND_GROUP_MSG_RECORD,	//发送群组聊天记录
	MSG_RECORD_OVER,		//聊天记录发送完毕

	CHANGE_PASSWORD,		//修改密码
	CHANGE_PASSWORD_OK,		//修改密码成功	
	CHANGE_PASSWORD_FAIL,	//修改密码失败

	MODIFY_NICKNAME,		//修改昵称
	MODIFY_NICKNAME_OK,		//修改昵称成功
	MODIFY_NICKNAME_FAIL,	//修改昵称失败

	SEND_FILE,			//发送文件
	SEND_FILE_OK,		//发送文件成功
	SEND_FILE_FAIL,		//发送文件失败
	OFFLINE_FILE,		//离线文件

	ADVERT_INFO,		//获取广告信息
	
	OPEN_ANONYMITY,		//开启匿名聊天
	CLOSE_ANONYMITY,	//关闭匿名聊天

	OPEN,            //打开好友列表
	CLOSE,			 //关闭好友列表

	YES,
	NO,
	
	CLIENT_FAIL
};


//客户端传给服务器或从服务器接受的信息
typedef struct _Message
{
    int action;           //动作
	int state;			  //状态
	char id[20];		  //ID
	char user[20];        //用户名
	char password[20];    //密码
	char time[20]; 	  	  //聊天的时间
	char target[20];      //聊天的目标
	char message[1024];   //聊天内容
	char question[1024];  //密保问题
	char answer[1024];	  //密保答案
	char signature[25];	  //个性签名
	char file_name[20];   //文件名
} Message;

//好友状态
typedef struct _Friend
{
    int state;
	char user[20];
	char signature[25];
	struct _Friend *next;
} Friend;

//好友消息链表
typedef struct _Chat_Msg
{
    char user[20];
	char time[20];
	char target[20];
	char message[1024];
	struct _Chat_Msg *next;
} Chat_Msg;

int sockfd;    					//保存和服务器通信的套接字    

int func_choose_mark;			//功能选择标志
int login_mark;					//登录成功标志
int continue_mark;				//继续标志		
int auto_login_mark;			//自动登录标志
int keep_password_mark;			//保存密码标志
int switch_user_mark;			//切换用户标志
int friend_list_mark;			//好友列表打开关闭的标志
int group_list_mark;			//群组列表打开关闭的标志
int chat_msg_mark;				//聊天消息的标志
int group_msg_mark;				//群聊消息的标志
int chat_record_mark;			//聊天记录的标志
int input_mark;					//输入的标志
int chat_row;					//控制聊天信息所在行数

char current_user[20];    	  	//保存当前用户名
char current_signature[25];    	//保存当前用户个性签名
char current_target[20];		//保存当前聊天目标
char current_group_notice[100];	//保存当前群通知
char last_time[20];			    //保存上次发送消息的时间
char last_data[20];				//保存聊天记录日期
char *friend_list[30];			//保存好友在列表中的位置
char casual_user[20];			//保存临时用户名
char chat_target[20];			//发来消息的对象

pthread_t friend_id[10];		//创建线程来提示好友信息
pthread_t group_id[10];			//创建线程来提示群组信息
pthread_t advert_id[10];		//创建线程来打印广告信息

Friend *friend_head;			//保存好友状态信息的链表头结点
Chat_Msg *chat_msg_head;		//保存好友聊天信息的链表头结点
Chat_Msg *group_msg_head;		//保存好友群聊信息的链表头结点

WINDOW *menu_win[10];    //菜单窗口
WINDOW *reg_win[10];     //注册窗口
WINDOW *manage_win[10];  //密码管理窗
WINDOW *friend_win[10];  //好友列表窗口
WINDOW *chat_win[20];    //聊天窗口

sqlite3 *db;		//保存聊天记录的数据库句柄
int row;
int column;
char *errmsg;
char **result;
char sql[1024];

pthread_mutex_t mutex;
pthread_mutex_t mutex1;
pthread_mutex_t mutex2;
pthread_cond_t cond;
pthread_cond_t cond1;
pthread_cond_t cond2;
#endif
