#include "public.h"

void doLogin(int sockfd,char username[]){
	struct XProtocol *recvbuf;
	struct XProtocol *sendbuf;
	
	int n;
	int ret;
	fd_set rset;	//读文件描述符集合
	FD_ZERO(&rset);	//清除所有文件描述符
	int maxfd;		//监控文件描述符中的最大值+1
	int nready;		//有事件IO的个数

	int fd_stdin = fileno(stdin);
	if(fd_stdin > sockfd)
	{
		maxfd = fd_stdin;
	}
	else
	{
		maxfd = sockfd;
	}
	while(1){
		unsigned char buffer[sizeof(struct XProtocol)];
		struct XProtocol *pxp;
		unsigned short cmd_type,rel_val;
		unsigned long data_len;
		char data[MSG_LEN];

		//将需要监控的文件描述符加入到集合中
		FD_SET(sockfd,&rset);	//sockfd
		FD_SET(fd_stdin,&rset); //标准输入
		nready = select(maxfd+1,&rset,NULL,NULL,NULL);
		if(-1 == nready)
		{
			ERR_EXIT("select");
		}
		if(0 == nready)
		{
			//继续检测
			continue;
		}
		//判断socket 是否ready
		if(FD_ISSET(sockfd,&rset))
		{
			//sockfd 有输入
			memset(&recvbuf,0,sizeof(recvbuf));
			memset(buffer,0,sizeof(struct XProtocol));
			ret = read(sockfd,recvbuf->username,MAXLEN_USERNAME);
			if(ret == 0 )
			{
				printf("server close");
				break;
			}
			else{}

			read(sockfd,buffer,sizeof(struct XProtocol));
			recvbuf = (struct XProtocol*) buffer;
			cmd_type = ntohs(recvbuf->cmd_type);
			switch(cmd_type){
				case RPL_Query:
					if(ntohs(recvbuf->ret_val) == 0)
						printf("单词解释如下，回车继续: \n %s \n",recvbuf->data);
					else
						printf("查找失败，回车继续\n");
					break;
				case RPL_HiSTORY:
					if(ntohs(recvbuf->ret_val) == 0)
						printf("历史查询结果，回车继续: \n %s\n",recvbuf->data);
					else
						printf("查找失败，无记录\n");
					break;
			}
		}
		if(FD_ISSET(fd_stdin,&rset))
		{
			//标准输入有数据
			memset(&sendbuf,0,sizeof(sendbuf));
			memset(buffer,0,sizeof(struct XProtocol));
			printf("1.查询\t 2.查看历史查询\t 3.退出\n");
			printf("输入服务代号：\n");
			scanf("%o",&cmd_type);
			sendbuf = (struct XProtocol *)buffer;
			strncpy(sendbuf->username,username,strlen(username));
			
			switch(cmd_type){
			case 1:
				printf("输入单词：\n");
				scanf("%s",&data);
				strncpy(sendbuf->data,data,strlen(data));
				//printf("发起查询服务\n");
				sendbuf->cmd_type = htons(REQ_Query);
				write(sockfd,sendbuf,sizeof(struct XProtocol));
				break;
			case 2:
				printf("发起历史查询服务\n");
				sendbuf->cmd_type = htons(REQ_History);
				write(sockfd,sendbuf,sizeof(struct XProtocol));
				break;
			case 3:
				printf("退出系统\n");
				sendbuf->cmd_type = htons(REQ_Exit);
				write(sockfd,sendbuf,sizeof(struct XProtocol));
				return;
				break;
			
			}
		}
	}
}

/*
 * select 管理者
 * 思想：用select来管理多个IO，无事件，阻塞
 * 		一旦有其中一个IO或者多个IO 检测到了我们感兴趣的事件
 *		select 函数返回，返回值是检测到的事件个数
 *
 * */
int doService(int sockfd)
{
	struct XProtocol *recvbuf;
	struct XProtocol *sendbuf;
	//packet_t recvbuf;
	//packet_t sendbuf;
	int n;
	int ret;

	//char recvbuf[1024] = {0};
	//char sendbuf[1024] = {0};


	fd_set rset;	//读文件描述符集合
	FD_ZERO(&rset);	//清除所有文件描述符
	int maxfd;		//监控文件描述符中的最大值+1
	int nready;		//有事件IO的个数

	int fd_stdin = fileno(stdin);
	if(fd_stdin > sockfd)
	{
		maxfd = fd_stdin;
	}
	else
	{
		maxfd = sockfd;
	}
	while(1){
		unsigned char buffer[sizeof(struct XProtocol)];
		struct XProtocol *pxp;
		unsigned short cmd_type,ret_val;
		unsigned long data_len;
		char username[MAXLEN_USERNAME];
		char data[MSG_LEN];


		//将需要监控的文件描述符加入到集合中
		FD_SET(sockfd,&rset);	//sockfd
		FD_SET(fd_stdin,&rset); //标准输入
		nready = select(maxfd+1,&rset,NULL,NULL,NULL);
		if(-1 == nready)
		{
			ERR_EXIT("select");
		}
		if(0 == nready)
		{
			//继续检测
			continue;
		}
		//判断socket 是否ready
		if(FD_ISSET(sockfd,&rset))
		{
			//sockfd 有输入
			memset(&recvbuf,0,sizeof(recvbuf));
			memset(buffer,0,sizeof(struct XProtocol));
			ret = read(sockfd,recvbuf->username,sizeof(recvbuf->username));
			if(ret == 0 )
			{
				printf("server close");
				break;
			}
			else{}

			read(sockfd,buffer,sizeof(struct XProtocol));
			recvbuf = (struct XProtocol*) buffer;
			cmd_type = ntohs(recvbuf->cmd_type);
			switch(cmd_type){
				case RPL_Register:
					if(ntohs(recvbuf->ret_val) == 0)
						printf("注册成功，回车继续\n");
					else
						printf("注册失败,回车继续\n");
					break;
				case RPL_Login:
					if(ntohs(recvbuf->ret_val) == 0)
					{
						printf("登陆成功,回车继续\n");
						//printf("%s",recvbuf->username);
						doLogin(sockfd,recvbuf->username);
					}
					else {
						printf("登陆失败，回车继续\n");
					}
					break;
			}
		}
		if(FD_ISSET(fd_stdin,&rset))
		{
			//标准输入有数据
			memset(&sendbuf,0,sizeof(sendbuf));
			memset(buffer,0,sizeof(struct XProtocol));
			sendbuf = (struct XProtocol *)buffer;
			printf("1.注册\t 2.登陆\n");
			printf("输入服务代号：\n");
			scanf("%o",&cmd_type);
			printf("输入用户名：\n");
			scanf("%s",&username);
			printf("输入密码：\n");
			scanf("%s",&data);
			strncpy(sendbuf->username,username,strlen(username));
			strncpy(sendbuf->data,data,strlen(data));
			switch(cmd_type){
			case 1:
				//注册服务
				printf("发起注册服务\n");
				sendbuf->cmd_type = htons(REQ_Register);
				write(sockfd,sendbuf,sizeof(struct XProtocol));
				break;
			case 2:
				printf("发起登陆服务\n");
				sendbuf->cmd_type = htons(REQ_Login);
				write(sockfd,sendbuf,sizeof(struct XProtocol));
				break;
			}
		}
	}
}


int main(int argc, const char *argv[])
{/*{{{*/
	//创建scoket
	int sockfd = socket(PF_INET,SOCK_STREAM,0);
	if(-1 == sockfd)
	{
		ERR_EXIT("socket");
	}
	//初始化服务器地址
	struct sockaddr_in servaddr;	//ipv4地址结构
	memset(&servaddr ,0,sizeof(servaddr)); 
	servaddr.sin_family = AF_INET;	//地址族
	servaddr.sin_port = htons(5188);	//断口号
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //服务器地址
#if 0

	//将套接字与本地地址绑定
	if(bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr)) == -1)
	{
		ERR_EXIT("bind");
	}

#endif

	if(connect (sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
	{
		ERR_EXIT("connect");
	}
	printf("===========================================================\n");
	doService(sockfd);
	close(sockfd);
	return 0;
}/*}}}*/
