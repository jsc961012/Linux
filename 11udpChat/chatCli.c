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
#include <unistd.h>

#include "public.h"

#define ERR_EXIT(m) \
	do \
	{ \
		perror(m); \
		exit(EXIT_FAILURE); \
	} while(0)




// 当前用户名
char username[16];

// 聊天室成员列表
UserList_t clientList;

void doSomeoneLogin(Message_t& msg);
void doSomeoneLogout(Message_t& msg);
void doGetlist();
void doChat();
void parseCmd(char* cmdline, int sockfd, struct sockaddr_in *servaddr);
bool sendmsgto(int sockfd, char* username, char *msg);

void parseCmd(char* cmdline, int sockfd, struct sockaddr_in* servaddr)
{/*{{{*/
	char cmd[10] = {0};
	char *p;
	p = strchr(cmdline, ' ');
	if (p != NULL)
	{
		*p = '\0';
	}
	strcpy(cmd, cmdline);

	if (strcmp(cmd, "exit") == 0)  // 登出
	{
		Message_t msg;
		memset(&msg, 0, sizeof(msg));
		msg.cmd = htonl(C2S_LOGOUT);
		strcpy(msg.body, username);

		if (sendto(sockfd, &msg, sizeof(msg), 0, (struct sockaddr*)servaddr, sizeof(struct sockaddr)) < 0)
		{
			ERR_EXIT("sendto");
		}

		printf("user %s has logout server\n", username);
		exit(EXIT_FAILURE);
	}
	else if (strcmp(cmd, "send") == 0)
	{
		char peername[16] = {0};
		char msg[MSG_LEN] = {0};

		/* send user msg */
		/*      p    p2  */
		while (*p++ == ' ')
		{
			;
		}
		char *p2;
		p2 = strchr(p, ' ');
		if (p2 == NULL)
		{
			printf("bad command\n");
			printf("\nCommands are:\n");
			printf("send username msg\n");
			printf("list\n");
			printf("exit\n");
			printf("\n");
			return;
		}

		*p2 = '\0';
		strcpy(peername, p);

		while(*p2++ == ' ');
		strcpy(msg, p2);
		sendmsgto(sockfd, peername, msg);
	}
	else if (strcmp(cmd, "list") == 0)
	{
		Message_t msg;
		memset(&msg, 0, sizeof(msg));
		msg.cmd = htonl(C2S_ONLINE_USER);

		if (sendto(sockfd, &msg, sizeof(msg), 0, (struct sockaddr*)servaddr, sizeof(struct sockaddr)) < 0)
		{
			ERR_EXIT("sendto");
		}

	}
	else
	{
		printf("bad command\n");
		printf("\nCommands are:\n");
		printf("send username msg\n");
		printf("list\n");
		printf("exit\n");
		printf("\n");

	}
}/*}}}*/

void doGetlist(int sockfd)
{
	int count;
	recvfrom(sockfd, &count, sizeof(int), 0, NULL, NULL);
	printf("has %d users logined server\n", ntohl(count));
	clientList.clear();

	int n = ntohl(count);
	for (int i=0; i<n; i++)
	{
		UserInfo_t user;
		recvfrom(sockfd, &user, sizeof(UserInfo_t), 0, NULL, NULL);
		clientList.push_back(user);
		in_addr tmp;
		tmp.s_addr = user.ip;
		printf("%d %s -> %s:%d\n", i, user.username, inet_ntoa(tmp), ntohs(user.port));
	}
}

void doSomeoneLogin(Message_t& msg)
{
	UserInfo_t *user = (UserInfo_t*)msg.body;
	in_addr tmp;
	tmp.s_addr = user->ip;
	printf("%s <-> %s:%d has logined server\n", user->username, inet_ntoa(tmp), ntohs(user->port));
	clientList.push_back(*user);   // 添加该用户到在线用户列表中
}


void doSomeoneLogout(Message_t& msg)
{
	UserList_t::iterator it;
	for (it=clientList.begin(); it!=clientList.end(); ++it)
	{
		if(strcmp(it->username, msg.body) == 0)
		{
			break;
		}
	}
	if (it != clientList.end())
	{
		clientList.erase(it);   // 清除该用户
	}

	printf("user %s has logout server\n", msg.body);
}

void doChat(const Message_t& msg)
{
	ChatMsg_t *cm = (ChatMsg_t*)msg.body;
	printf("recv a msg [%s] from [%s]\n", cm->msg, cm->username);
	// recvfrom(sockfd, &count, sizeof(int), 0, NULL, NULL);
}


bool sendmsgto(int sockfd, char *name, char *msg)
{/*{{{*/
	if (strcmp(name, username) == 0)
	{
		printf("can't send message to self\n");
		return false;
	}

	UserList_t::iterator it;
	for (it=clientList.begin(); it!=clientList.end(); ++it)
	{
		if (strcmp(it->username, name) == 0)
		{
			break;
		}
	}

	if(it == clientList.end())
	{
		printf("user %s has not logined server\n", name);
		return false;
	}

	Message_t m;
	memset(&m, 0, sizeof(m));
	m.cmd = htonl(C2C_CHAT);

	ChatMsg_t cm;
	strcpy(cm.username, username);
	strcpy(cm.msg, msg);

	memcpy(m.body, &cm, sizeof(cm));

	struct sockaddr_in peeraddr;
	memset(&peeraddr, 0, sizeof(peeraddr));
	peeraddr.sin_family = AF_INET;
	peeraddr.sin_addr.s_addr = it->ip;
	peeraddr.sin_port = it->port;

	in_addr tmp;
	tmp.s_addr = it->ip;

	printf("sending message [%s] to user [%s] <-> %s:%d\n", msg, name, inet_ntoa(tmp), ntohs(peeraddr.sin_port));

	sendto(sockfd, (const char*)&m, sizeof(m), 0, (struct sockaddr*)&peeraddr, sizeof(peeraddr));
	return true;
}/*}}}*/

void chatClient(int sockfd)
{/*{{{*/
	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_port = htons(5188);
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	struct sockaddr_in peeraddr;
	socklen_t peerlen;

	Message_t msg;
	while(1)
	{
		memset(username, 0, sizeof(username));
		printf("please input you name:\n");
		fflush(stdout);
		scanf("%s", username);

		memset(&msg, 0, sizeof(msg));
		msg.cmd = htonl(C2S_LOGIN);
		strcpy(msg.body, username);

		sendto(sockfd, &msg, sizeof(msg), 0, (struct sockaddr*)&servaddr, sizeof(servaddr));

		memset(&msg, 0, sizeof(msg));
		recvfrom(sockfd, &msg, sizeof(msg), 0, NULL, NULL);
		int cmd = ntohl(msg.cmd);
		if (cmd == S2C_ALREADY_LOGINED)
		{
			printf("user %s already logined server, please use another username\n", username);
		}
		else if (cmd == S2C_LOGIN_OK)
		{
			printf("user %s has logined server\n", username);
			break;
		}
	}

	// 登入成功后
	int count;
	recvfrom(sockfd, &count, sizeof(count), 0, NULL, NULL);
	int n = ntohl(count);
	printf("has %d users logined server\n", n);

	for (int i=0; i<n; i++)
	{
		UserInfo_t user;
		recvfrom(sockfd, &user, sizeof(UserInfo_t), 0, NULL, NULL);
		clientList.push_back(user);
		in_addr tmp;
		tmp.s_addr = user.ip;

		printf("%d %s <-> %s:%d\n", i, user.username, inet_ntoa(tmp), ntohs(user.port));
	}

	//
	printf("\nCommands are:\n");
	printf("send username masg\n");
	printf("list\n");
	printf("exit\n");
	printf("\n");

	fd_set rset;
	FD_ZERO(&rset);
	int nready;
	while(1)
	{
		FD_SET(STDIN_FILENO, &rset);
		FD_SET(sockfd, &rset);
		nready = select(sockfd+1, &rset, NULL, NULL, NULL);
		if (nready == -1)
		{
			ERR_EXIT("select");
		}
		if (nready == 0)
		{
			continue;
		}

		if (FD_ISSET(sockfd, &rset))
		{
			peerlen = sizeof(peeraddr);
			memset(&msg, 0, sizeof(msg));
			recvfrom(sockfd, &msg, sizeof(msg), 0, (struct sockaddr*)&peeraddr, &peerlen);
			int cmd = ntohl(msg.cmd);
			switch(cmd)
			{
			case S2C_SOMEONE_LOGIN:
				doSomeoneLogin(msg);
				break;
			case S2C_SOMEONE_LOGOUT:
				doSomeoneLogout(msg);
				break;
			case S2C_ONLINE_USER:
				doGetlist(sockfd);
				break;
			case C2C_CHAT:
				doChat(msg);
				break;
			default:
				break;
			}
		}

		if (FD_ISSET(STDIN_FILENO, &rset))
		{
			char cmdline[100] = {0};
			if (fgets(cmdline, sizeof(cmdline), stdin) == NULL)
			{
				break;
			}

			if (cmdline[0] == '\n')
			{
				continue;
			}
			cmdline[strlen(cmdline)-1] = '\0';
			parseCmd(cmdline, sockfd, &servaddr);
		}
	}

	memset(&msg, 0, sizeof(msg));
	msg.cmd = htonl(C2S_LOGOUT);


}/*}}}*/


int main(int argc, const char *argv[])
{
	int sockfd;
	if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
	{
		ERR_EXIT("socket");
	}

	chatClient(sockfd);

	return 0;
}
