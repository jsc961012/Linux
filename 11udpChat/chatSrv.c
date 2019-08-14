#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
//#include <signal.h>
//#include <sys/wait.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "public.h"


#define ERR_EXIT(m) \
	do \
	{ \
		perror(m); \
		exit(EXIT_FAILURE); \
	} while(0)


// 聊天室成员列表
UserList_t clientList;

void doLogin(Message_t& msg, int sockfd, struct sockaddr_in *cliAddr);
void doLogout(Message_t& msg, int sockfd, struct sockaddr_in *cliAddr);
void doSendlist(int sockfd, struct sockaddr_in *cliAddr);

void chatSrv(int sockfd)
{/*{{{*/
	struct sockaddr_in cliAddr;
	socklen_t cliLen;
	int n;
	Message_t msg;
	while (1)
	{
		memset(&msg, 0, sizeof(msg));
		cliLen = sizeof(cliAddr);

		int n = recvfrom(sockfd, &msg, sizeof(msg), 0, (struct sockaddr*)&cliAddr, &cliLen);
		if (n < 0)
		{
			if (errno == EINTR)
			{
				continue;
			}
			ERR_EXIT("recvfrom");
		}

		int cmd = ntohl(msg.cmd);
		switch (cmd)
		{
		case C2S_LOGIN:
			doLogin(msg, sockfd, &cliAddr);
			break;
		case C2S_LOGOUT:
			doLogout(msg, sockfd, &cliAddr);
			break;
		case C2S_ONLINE_USER:
			doSendlist(sockfd, &cliAddr);
			break;
		default:
			break;
		}
	}

}/*}}}*/

void doLogout(Message_t& msg, int sockfd, struct sockaddr_in *cliAddr)
{
	printf("has a user logout:%s <-> %s:%d\n", msg.body, inet_ntoa(cliAddr->sin_addr), ntohs(cliAddr->sin_port));
	UserList_t::iterator it;
	for (it=clientList.begin(); it!=clientList.end(); ++it)
	{
		if (strcmp(it->username, msg.body) == 0)
		{
			break;
		}
	}

	if (it != clientList.end())
	{
		clientList.erase(it);
	}

	// 向其他用户通知有用户登出
	for (it=clientList.begin(); it!=clientList.end(); ++it)
	{
		if (strcmp(it->username, msg.body) == 0)
		{
			continue;
		}

		struct sockaddr_in peeraddr;
		memset(&peeraddr, 0, sizeof(peeraddr));
		peeraddr.sin_family = AF_INET;
		peeraddr.sin_port = it->port;
		peeraddr.sin_addr.s_addr = it->ip;

		msg.cmd = htonl(S2C_SOMEONE_LOGOUT);

		if (sendto(sockfd, &msg, sizeof(msg), 0, (struct sockaddr*)&peeraddr, sizeof(peeraddr)) < 0)
		{
			ERR_EXIT("sendto");
		}
	}
}

void doSendlist(int sockfd, struct sockaddr_in *cliAddr)
{/*{{{*/

	Message_t msg;
	msg.cmd = htonl(S2C_ONLINE_USER);
	sendto(sockfd, (const char*)&msg, sizeof(msg), 0, (struct sockaddr*)cliAddr, sizeof(struct sockaddr));

	int count = htonl((int)clientList.size());
	// 发送在线人数
	sendto(sockfd, (const char*)&count, sizeof(int), 0, (struct sockaddr*)cliAddr, sizeof(struct sockaddr));
	// 发送在线用户列表
	for (UserList_t::iterator it=clientList.begin(); it!=clientList.end(); ++it)
	{
		sendto(sockfd, &*it, sizeof(UserInfo_t), 0, (struct sockaddr*)cliAddr, sizeof(struct sockaddr));
	}
}/*}}}*/

void doLogin(Message_t& msg, int sockfd, struct sockaddr_in *cliAddr)
{/*{{{*/
	UserInfo_t user;
	strcpy(user.username, msg.body);
	user.ip = cliAddr->sin_addr.s_addr;
	user.port = cliAddr->sin_port;

	
	// 查找用户是否在在线列表中
	UserList_t::iterator it;
	for (it=clientList.begin(); it!=clientList.end(); ++it)
	{
		if (strcmp(it->username, msg.body) == 0)
		{
			break;
		}
	}

	if (it == clientList.end()) // 没有找到用户
	{
		printf("has a user login: %s <-> %s:%d\n", msg.body, inet_ntoa(cliAddr->sin_addr), ntohs(user.port));
		clientList.push_back(user);  // 将用户插入到列表中

		// 登入成功应答
		Message_t replyMsg;
		memset(&replyMsg, 0, sizeof(replyMsg));
		replyMsg.cmd = htonl(S2C_LOGIN_OK);
		// body为空
		sendto(sockfd, &replyMsg, sizeof(replyMsg), 0, (struct sockaddr *)cliAddr, sizeof(struct sockaddr));

		int count = htonl((int)clientList.size());

		// 发送在线人数
		sendto(sockfd, &count, sizeof(int), 0, (struct sockaddr*)cliAddr, sizeof(struct sockaddr));

		printf("sending user list information to: %s <-> %s: %d\n", msg.body, inet_ntoa(cliAddr->sin_addr), ntohs(cliAddr->sin_port));

		// 发送在线列表
		for (it=clientList.begin(); it != clientList.end(); ++it)
		{
			sendto(sockfd, &*it, sizeof(UserInfo_t), 0, (struct sockaddr*)cliAddr, sizeof(struct sockaddr));
		}

		// 向其用户通知有新用户登入
		for (it=clientList.begin(); it!=clientList.end(); ++it)
		{
			if (strcmp(it->username, msg.body) == 0)
			{
				continue;
			}
			struct sockaddr_in peeraddr;
			memset(&peeraddr, 0, sizeof(peeraddr));
			peeraddr.sin_family = AF_INET;
			peeraddr.sin_port = it->port;
			peeraddr.sin_addr.s_addr = it->ip;

			msg.cmd = htonl(S2C_SOMEONE_LOGIN);
			memcpy(msg.body, &user, sizeof(user));

			if (sendto(sockfd, &msg, sizeof(msg), 0, (struct sockaddr*)&peeraddr, sizeof(peeraddr)) < 0)
			{
				ERR_EXIT("sendto");
			}
		}

	}
	else  // 找到了用户
	{
		printf("user %s has already logined\n", msg.body);

		Message_t replyMsg;
		memset(&replyMsg, 0, sizeof(replyMsg));
		replyMsg.cmd = htonl(S2C_ALREADY_LOGINED);
		sendto(sockfd, &replyMsg, sizeof(replyMsg), 0, (struct sockaddr*)cliAddr, sizeof(struct sockaddr));
	}

}/*}}}*/



int main(int argc, const char *argv[])
{
	int sockfd;
	if((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
	{
		ERR_EXIT("socket");
	}

	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(5188);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
	{
		ERR_EXIT("bind");
	}

	chatSrv(sockfd);

	return 0;
}
