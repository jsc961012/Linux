#include "public.h"

/*
 * 现在的linux中，在SIGRTMIN（34）之前的都是不可靠信号。
 *
 *
 * 可靠信号：多个信号发送给进程时（收到信号的速度超过了进程处理信号的速度），
 *			这些没有来得及处理的信号，就会放入进程队列中，等待有机会处理时，依次处理
 * 不可靠信号：不支持信号队列，没来得及处理的信号会丢弃
 *
 * SIGCHLD 是不可靠信号。
 * */

//获取当前时间进行拼接
int getNowTime(char *nowTime)
{/*{{{*/
	char acYear[5] = {0};
	char acMonth[5] = {0};
	char acDay[5] = {0};
	char acHour[5] = {0};
	char acMin[5] = {0};
	char acSec[5] = {0};
	time_t now;
	struct tm* timenow;
	time(&now);
	timenow = localtime(&now);
	strftime(acYear,sizeof(acYear),"%Y",timenow);
	strftime(acMonth,sizeof(acMonth),"%m",timenow);
	strftime(acDay,sizeof(acDay),"%d",timenow);
	strftime(acHour,sizeof(acHour),"%H",timenow);
	strftime(acMin,sizeof(acMin),"%M",timenow);
	strftime(acSec,sizeof(acSec),"%S",timenow);
	strncat(nowTime, acYear, 4);
	strncat(nowTime,"-",1);
	strncat(nowTime, acMonth, 2);
	strncat(nowTime,"-",1);
	strncat(nowTime, acDay, 2);
	strncat(nowTime," ",1);
	strncat(nowTime, acHour, 2);
	strncat(nowTime,":",1);
	strncat(nowTime, acMin, 2);
	strncat(nowTime,":",1);
	strncat(nowTime, acSec, 2);
	return 0;
}/*}}}*/

void sigHandler(int signo)
{/*{{{*/
	if(signo == SIGINT)
	{
		printf("process :%d caught SIGINT.\n",getpid());
		exit(0);
	}
	else if(SIGCHLD == signo)
	{
		//wait(NULL);
		while(waitpid(-1,NULL,WNOHANG) > 0)
		{
			;		//等待所有的子进程退出，不挂起
		}
	}
	else
	{}

}/*}}}*/

sqlite3* connSQL()
{/*{{{*/
	sqlite3 * db = 0;
	int ret = sqlite3_open("./dict.db", &db);
	if( ret != SQLITE_OK ){
		fprintf(stderr, "无法打开数据库: %s", sqlite3_errmsg(db));
		return;
	}
	return db;
}/*}}}*/


void doRegister(int connfd,char username[],char password[])
{
	struct XProtocol *revbuf;
	unsigned char buffer[sizeof(struct XProtocol)];
	struct XProtocol *pxp;
	unsigned short cmd_type,rel_val;
	unsigned long data_len;
	//回复
	//注册操作，写入数据库
	sqlite3 *db = connSQL();
	sql=sqlite3_mprintf("INSERT INTO user VALUES(NULL , '%q' , '%q','n')", username,password);
	int ret = sqlite3_exec( db , sql , 0 , 0 , &pErrMsg);
	if(ret != SQLITE_OK)
	{
		memset(buffer,0x00,sizeof(struct XProtocol));
		pxp = (struct XProtocol *)buffer;
		strncpy(pxp->username,username,strlen(username));
		pxp->cmd_type = htons(RPL_Register);
		pxp->data_len = htonl(strlen("register failure"));
		pxp->ret_val = htons(1);
		strncpy((char *)pxp->data,"register failure",strlen("register failure"));
		write(connfd,buffer,sizeof(struct XProtocol));
		printf("用户：%s  注册失败\n",pxp->username);
		return;
	}
	else
	{
		memset(buffer,0x00,sizeof(struct XProtocol));
		pxp = (struct XProtocol *)buffer;
		strncpy(pxp->username,username,strlen(username));
		pxp->cmd_type = htons(RPL_Register);
		pxp->data_len = htonl(strlen("register success"));
		pxp->ret_val = htons(0);
		strncpy((char *)pxp->data,"register success",strlen("register success"));
		write(connfd,buffer,sizeof(struct XProtocol));
		printf("用户：%s  注册成功\n",pxp->username);
		return;
	}
}

void doLogin(int connfd,char username[],char password[])
{/*{{{*/
	struct XProtocol *revbuf;
	unsigned char buffer[sizeof(struct XProtocol)];
	struct XProtocol *pxp;
	unsigned short cmd_type,rel_val;
	unsigned long data_len;

	sqlite3 *db = connSQL(connfd);
	int nrow = 0, ncolumn = 0;
	char** azResult = NULL;
	sql=sqlite3_mprintf("select * from user where uname='%q' and password='%q' and state = 'n'", username,password);
	sqlite3_get_table( db , sql , &azResult , &nrow , &ncolumn , &pErrMsg );
	sql1 = sqlite3_mprintf("update user set state = 'y' where uname = '%q'",username);
	int ret = sqlite3_exec( db , sql1 , 0 , 0 , &pErrMsg);

	if(nrow == 0){
		memset(buffer,0x00,sizeof(struct XProtocol));
		pxp = (struct XProtocol *)buffer;
		strncpy(pxp->username,username,strlen(username));
		pxp->cmd_type = htons(RPL_Login);
		pxp->data_len = htonl(strlen("Login failure"));
		strncpy((char *)pxp->data,"Login failure",strlen("Login failure"));
		pxp->ret_val = htons(1);
		write(connfd,buffer,sizeof(struct XProtocol));
		printf("用户： %s 登陆失败\n",pxp->username);
		return;
	}
	else
	{
		memset(buffer,0x00,sizeof(struct XProtocol));
		pxp = (struct XProtocol *)buffer;
		strncpy(pxp->username,username,strlen(username));
		pxp->cmd_type = htons(RPL_Login);
		pxp->data_len = htonl(strlen("Login success"));
		strncpy((char *)pxp->data,"Login success",strlen("Login success"));
		pxp->ret_val = htons(0);
		write(connfd,buffer,sizeof(struct XProtocol));
		printf("用户： %s 登陆成功\n",pxp->username);
		return;
	}
}/*}}}*/

void doQuery(int connfd,char username[],char word[])
{
	struct XProtocol *revbuf;
	unsigned char buffer[sizeof(struct XProtocol)];
	struct XProtocol *pxp;
	unsigned short cmd_type,rel_val;
	unsigned long data_len;

	sqlite3 *db = connSQL(connfd);
	int nrow = 0, ncolumn = 0;
	char** azResult = NULL;
	char nowTime[32] = {0};//存当前时间
	getNowTime(nowTime);

	sql=sqlite3_mprintf("select remarks from dic where name = '%q'",word);
	sql1 = sqlite3_mprintf("INSERT into history VALUES(NULL,'%q','%q','%q')",username,nowTime,word);
	sqlite3_get_table( db , sql , &azResult , &nrow , &ncolumn , &pErrMsg );
	int ret = sqlite3_exec( db , sql1 , 0 , 0 , &pErrMsg);

	if(nrow == 0){
		memset(buffer,0x00,sizeof(struct XProtocol));
		pxp = (struct XProtocol *)buffer;
		strncpy(pxp->username,username,strlen(username));
		pxp->cmd_type = htons(RPL_Query);
		pxp->data_len = htonl(strlen("Query failure"));
		strncpy((char *)pxp->data,"Query failure",strlen("Query failure"));
		pxp->ret_val = htons(1);
		write(connfd,buffer,sizeof(struct XProtocol));
		printf("用户： %s 查找 %s 失败\n",pxp->username,word);
		return;
	}
	else
	{
		memset(buffer,0x00,sizeof(struct XProtocol));
		pxp = (struct XProtocol *)buffer;
		strncpy(pxp->username,username,strlen(username));
		pxp->cmd_type = htons(RPL_Query);
		pxp->data_len = htonl(strlen(azResult[1]));

		memcpy((char *)pxp->data,(char *)azResult[1],strlen((char *)azResult[1]));

		//strncpy((char *)pxp->data,azResult[5],strlen(azResult[5]));
		pxp->ret_val = htons(0);
		write(connfd,buffer,sizeof(struct XProtocol));
		printf("用户： %s 查找 %s 成功\n",pxp->username,word);
		return;
	}
}

void doHistory(int connfd,char username[])
{

	struct XProtocol *revbuf;
	unsigned char buffer[sizeof(struct XProtocol)];
	struct XProtocol *pxp;
	unsigned short cmd_type,rel_val;
	unsigned long data_len;
	char data_history[MSG_LEN];

	sqlite3 *db = connSQL(connfd);
	int nrow = 0, ncolumn = 0;
	char** azResult = NULL;
	char nowTime[32] = {0};//存当前时间
	getNowTime(nowTime);

	sql=sqlite3_mprintf("select uname,timestamp,word from history where uname = '%q'",username);
	sqlite3_get_table( db , sql , &azResult , &nrow , &ncolumn , &pErrMsg );
	int i;
	for(i=0;i<(nrow+1)*ncolumn;i++)
	{
		sprintf(data_history,"%s %s",data_history,azResult[i]);
		//printf("%s\n",azResult[i]);
		if((i+1)%3 == 0)
			sprintf(data_history,"%s %s",data_history,"\n");
		else
			sprintf(data_history,"%s %s",data_history," ");
	}

	if(nrow == 0){
		memset(buffer,0x00,sizeof(struct XProtocol));
		pxp = (struct XProtocol *)buffer;
		strncpy(pxp->username,username,strlen(username));
		pxp->cmd_type = htons(RPL_HiSTORY);
		pxp->data_len = htonl(strlen("Find failure"));
		strncpy((char *)pxp->data,"Find failure",strlen("Find failure"));
		pxp->ret_val = htons(1);
		write(connfd,buffer,sizeof(struct XProtocol));
		printf("用户： %s 查找历史记录失败\n",pxp->username);
		return;
	}
	else
	{
		memset(buffer,0x00,sizeof(struct XProtocol));
		pxp = (struct XProtocol *)buffer;
		strncpy(pxp->username,username,strlen(username));
		pxp->cmd_type = htons(RPL_HiSTORY);
		pxp->data_len = htonl(strlen(data_history));
		memcpy((char *)pxp->data,(char *)data_history,strlen((char *)data_history));
		//strncpy((char *)pxp->data,azResult[5],strlen(azResult[5]));
		pxp->ret_val = htons(0);
		write(connfd,buffer,sizeof(struct XProtocol));
		printf("用户： %s 查找历史记录成功\n",pxp->username);
		return;
	}
}
void doExit(int connfd,char username[])
{

	struct XProtocol *revbuf;
	unsigned char buffer[sizeof(struct XProtocol)];
	struct XProtocol *pxp;
	unsigned short cmd_type,rel_val;
	unsigned long data_len;

	sqlite3 *db = connSQL(connfd);
	sql = sqlite3_mprintf("update user set state = 'n' where uname = '%q'",username);
	int ret = sqlite3_exec( db , sql , 0 , 0 , &pErrMsg);
	if(ret != SQLITE_OK)
	{
		printf("用户 %s 退出失败\n",username);
	}
	else
	{
		printf("用户 %s 退出成功\n",username);
	}
}

int doService(int connfd)
{
	unsigned char buffer[sizeof(struct XProtocol)];
	struct XProtocol *pxp;
	unsigned short cmd_type,rel_val;
	unsigned long data_len;
	int ret;

	//接受一个XProtocol
	struct XProtocol *revbuf;
	memset(&revbuf,0,sizeof(revbuf));
	memset(buffer,0,sizeof(struct XProtocol));

	ret = read(connfd,revbuf->username,sizeof(revbuf->username));
	if(ret == 0)
	{
		printf("client close");
		return 1;
	}
	else{}
	read(connfd,buffer,sizeof(struct XProtocol));
	revbuf = (struct XProtocol *)buffer;
	//printf("收到服务端的命令：%o\n",ntohs(revbuf->cmd_type));
	cmd_type = ntohs(revbuf->cmd_type);
	switch(cmd_type){
	case REQ_Register:
		//注册操作
		doRegister(connfd,revbuf->username,revbuf->data);
		break;
	case REQ_Login:
		doLogin(connfd,revbuf->username,revbuf->data);
		//登陆操作
		break;
	case REQ_Query:
		doQuery(connfd,revbuf->username,revbuf->data);
		//查询操作
		break;
	case REQ_History:
		doHistory(connfd,revbuf->username);
		//查询历史操作
		break;
	case REQ_Exit:
		doExit(connfd,revbuf->username);
		//退出操作
		break;
	}
	return 0;

}



int main(int argc, const char *argv[])
{/*{{{*/
	signal(SIGINT,sigHandler);
	signal(SIGCHLD,sigHandler);
	//创建scoket
	int listenfd = socket(PF_INET,SOCK_STREAM,0);
	if(-1 == listenfd)
	{
		ERR_EXIT("socket");
	}
	//初始化服务器地址
	struct sockaddr_in servaddr;	//ipv4地址结构
	memset(&servaddr ,0,sizeof(servaddr)); 
	servaddr.sin_family = AF_INET;	//地址族
	servaddr.sin_port = htons(5188);	//断口号
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); //推荐绑定本机的任意ip

	//允许本地地址和断口重用
	int on = 1;
	if(setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)) < 0){
		ERR_EXIT("setsockopt");
	}

	//将套接字与本地地址绑定
	if(bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr)) == -1)
	{
		ERR_EXIT("bind");
	}
	//监听套接字，将套接字设置为监听状态（被动状态）,可以用来接受连接
	if(listen(listenfd, SOMAXCONN) < 0)
	{
		ERR_EXIT("listen");
	}
	struct sockaddr_in peeraddr;	//保存接入方的地址
	socklen_t peerlen = sizeof(peeraddr);	//地址长度

	//pid_t pid;
	int connfd; //连接socket(已经连接成功) 或 通信socket

	int nready;
	int maxi = 0;	//最大不空闲位置
	int maxfd = listenfd;	//目前最大文件描述符
	//保存所有已经连接的套接字
	int clientfd[FD_SETSIZE];
	int i;
	for(i=0;i<FD_SETSIZE;i++){
		clientfd[i] = -1;
	}

	fd_set rset;
	fd_set allset;	//监听所有IO
	FD_ZERO(&rset);
	FD_ZERO(&allset);

	FD_SET(listenfd,&allset);
	while(1){
		rset = allset;
		nready = select(maxfd+1,&rset,NULL,NULL,NULL);
		if(nready == -1)
		{
			if(errno == EINTR)
			{
				continue;	//接收被中断
			}
			ERR_EXIT("select");
		}
		if(nready == 0)
		{
			continue;	//超时
		}

		//表示有可读事件
		if(FD_ISSET(listenfd,&rset))
		{
			//监听套接字产生事件

			connfd = accept(listenfd,(struct sockaddr*)&peeraddr,&peerlen);
			if( -1 == connfd){
				ERR_EXIT("accept");
			}
			for(i=0;i<FD_SETSIZE;i++)
			{
				if(clientfd[i] < 0)
				{
					clientfd[i] = connfd;
					if(i>maxi)
					{
						maxi = i;
					}
					break;
				}
			}
			if(i == FD_SETSIZE)
			{
				fprintf(stderr,"too many client\n");
				exit(EXIT_FAILURE);
			}
			printf("clienr ip : %s,port : %d\n",inet_ntoa(peeraddr.sin_addr),ntohs(peeraddr.sin_port));
			printf("===========================================================\n");
			FD_SET(connfd,&allset);
			if(connfd > maxfd)
			{
				maxfd = connfd;
			}
			if(--nready <= 0)
			{
				//表示没有其他等待处理的事件
				continue;
			}
		}
		//连接套接字（通信套接字）有事件
		for(i=0;i<maxi+1;i++)
		{
			connfd = clientfd[i];
			if(-1 == connfd)
			{
				continue;
			}
			if(FD_ISSET(connfd,&rset))
			{
				int ret = doService(connfd);
				if(-1 == ret){
					ERR_EXIT("doService");
				}
				if(1 == ret)
				{
					FD_CLR(connfd,&allset);
					clientfd[i] = -1;
				}
				else{}
			}
		}
	}
	return 0;
}/*}}}*/
