#include "./include/myhead.h"

int main()
{
	Message msg;

    int lfd;
	int cfd;
	int sfd;
	int rdy;

	struct sockaddr_in sin;
	struct sockaddr_in cin;
	Client_Info *temp;

    int client[FD_SETSIZE];     //客户端连接的套接字描述符数组

	int maxi;
	int maxfd;    //最大连接数

	fd_set rset;
	fd_set allset;  //创建一个文件描述符的集合

	socklen_t addr_len;   //地址结构长度

	int i;
	int n;
	int len;
	int opt = 1;  //套接字选项


	//对sin结构进行赋值
	bzero(&sin, sizeof(struct sockaddr_in));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);

	//调用socket函数创建一个TCP协议套接字
	if((lfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
	    fprintf(stderr, "Socket error:%s\n\a", strerror(errno));
		exit(1);
	}

	//设置套接字选项，使用默认选项
	setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	//调用bind函数，将sin结构绑定到lfd上
	if(bind(lfd, (struct sockaddr *)(&sin), sizeof(struct sockaddr)) == -1)
	{
	    fprintf(stderr, "Bind error:%s\n\a", strerror(errno));
		exit(1);
	}

	//开始监听端口，等待客户请求
	if(listen(lfd, 20) == -1)
	{
	    fprintf(stderr, "Listen error:%s\n\a", strerror(errno));
		exit(-1);
	}

	printf("Accepting connections ......\n");

	maxfd = lfd;       //对最大文件描述符进行初始化
	maxi = -1;

	//初始化客户端连接描述符集合
	for(i = 0; i < FD_SETSIZE; i++)
	{
	    client[i] = -1;
	}

	FD_ZERO(&allset);      //清空文件描述符集合
	FD_SET(lfd, &allset);  //将lfd这个文件描述符加入到集合内

	//初始化在线用户,离线消息,离线文件链表
	create_client_link();
	create_offline_link();
	create_offline_file_link();
	create_offline_group_link();

	//开始服务程序的死循环
	while(1)
	{
	    rset = allset;

		//得到当前可以读的文件描述符数
		rdy = select(maxfd + 1, &rset, NULL, NULL, NULL);

		if(FD_ISSET(lfd, &rset))   //检查一个文件描述符是否可以读写
		{
		    addr_len = sizeof(sin);

			//接受客户端的请求
			if((cfd = accept(lfd, (struct sockaddr *)(&cin), &addr_len)) == -1)
			{
	    		fprintf(stderr, "Accept error:%s\n\a", strerror(errno));
			    exit(1);
			}

			//查找一个空闲位置
			for(i = 0; i < FD_SETSIZE; i++)
			{
			    if(client[i] <= 0)
				{
				    client[i] = cfd;
					break;
				}
			}

			//太多的客户端连接，服务器拒绝连接请求，跳出循环
			if(i == FD_SETSIZE)
			{
			    printf("too many clients");
				exit(1);
			}

			FD_SET(cfd, &allset);  //设置连接集合
			if(cfd > maxfd)        //新的连接描述符
			{
			    maxfd = cfd;
			}

			if(i > maxi)
			{
			    maxi = i;
			}

			if(--rdy <= 0)    //减少一个连接描述符
			{
			    continue;   
			}
		}

		//对每一个连接描述符做处理
		for(i = 0; i < FD_SETSIZE; i++)
		{
		    if((sfd = client[i]) < 0)
			{
			    continue;
			}

			if(FD_ISSET(sfd, &rset))
			{
				bzero(&msg, sizeof(msg));
			    
				n = read(sfd, &msg, sizeof(msg));

				if(n == 0)
				{
				    delete_client(sfd);

					fflush(stdout);          //刷新输出终端
					close(sfd);

					FD_CLR(sfd, &allset);      //将sfd从描述符数组中删除
					client[i] = -1;
				}
				else
				{
					switch(msg.action)
					{	
						// 用户登录
					    case LOGIN:
						{
							printf("\n用户%s请求登录！\n", msg.user);
							login(msg, sfd);
							break;
						}
						
						// 查看昵称能否注册
					    case CHECK_ACCOUNT:
						{
							printf("\n用户%s请求查看昵称是否存在！\n", msg.user);
							check_account(msg, sfd);
							break;
						}

						// 注册账号
						case REGIST:
						{
							printf("\n用户%s注册成功！\n", msg.user);
							register_account(msg,sfd);
						    break;
						}

						// 找回密码
						case PASSWORD_BACK:
						{
						    printf("\n用户%s请求找回密码！\n", msg.user);
							password_back(msg,sfd);
							break;
						}

						// 修改签名
						case MODIFY_SIGNATURE:
						{
						    printf("\n用户%s修改个性签名！\n", msg.user);
							modify_signature(msg,sfd);
							break;
						}

						// 聊天
					    case CHAT:
						{
							printf("用户%s请求和%s聊天！\n", msg.user, msg.target);
							handle_chat(msg, sfd);
							break;
						} 

						// 查看好友聊天记录
						case CHECK_MSG_RECORD:
						{
							printf("用户%s请求查看和%s的聊天记录！\n", msg.user, msg.target);
							handle_check_record(msg, sfd);
							break;
						}

						// 删除好友聊天记录
						case DEL_MSG_RECORD:
						{
							printf("用户%s请求删除和%s的聊天记录！\n", msg.user, msg.target);
							delete_chat_record(msg, sfd);
							break;
						}
						
						// 查看群组聊天记录
						case CHECK_GROUP_MSG_RECORD:
						{
							printf("用户%s请求查看群组聊天记录！\n", msg.user);
							handle_check_group_record(msg, sfd);
							break;
						}

						// 获取群通知
						case GROUP_NOTICE:
						{
						    printf("用户%s请求获取群通知！\n", msg.user);
							handle_group_notice(msg, sfd);
							break;
						}
						
						// 修改群通知
						case CHANGE_GROUP_NOTICE:
						{
							printf("用户%s请求修改群通知！\n", msg.user);
							handle_change_group_notice(msg, sfd);
						    break;
						}

						// 开启匿名聊天
						case OPEN_ANONYMITY:
						{
						    printf("用户%s请求开启匿名聊天！\n", msg.user);
							open_anonymity(msg, sfd);
							break;
						}

						// 关闭匿名聊天
						case CLOSE_ANONYMITY:
						{
						    printf("用户%s请求关闭匿名聊天！\n", msg.user);
							handle_close_anonymity(msg, sfd);
							break;
						}

						// 获取广告信息
						case ADVERT_INFO:
						{
							printf("用户%s请求获取广告信息！\n", msg.user);
							handle_advert_info(msg, sfd);
						    break;
						}

						// 修改密码
						case CHANGE_PASSWORD:
						{
						    printf("用户%s请求修改密码！\n", msg.user);
							change_password(msg, sfd);
							break;
						} 
						
						// 发送文件
						case SEND_FILE:
						{
						    printf("用户%s请求发送文件！\n", msg.user);
							send_file(msg, sfd);
							break;
						}

						// 修改昵称
						case MODIFY_NICKNAME:
						{
							printf("用户%s请求修改昵称！\n", msg.user);
							modify_nickname(msg, sfd);
							break;
						}

						// 发送群组消息
						case SEND_GROUP_MSG:
						{
						    printf("用户%s请求发送群组消息！\n", msg.user);
							handle_group_msg(msg, sfd);
							break;
						}
					}
				}

				//如果没有可读的套接字，退出循环
				if(--rdy <= 0)
				{
				    break;
				}
			}
		}
	}

    close(lfd);
	return 0;
}

