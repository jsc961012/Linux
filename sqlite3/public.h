#ifndef _PUBLIC_H_
#define _PUBLIC_H_

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/select.h>
#include <sqlite3.h>
#include <time.h>


// C2S 客户端到服请求消息（命令）
#define REQ_Register		0x0001	//注册
#define REQ_Login	    	0x0002	//登陆
#define REQ_Query			0x0003	//查询
#define REQ_History			0x0004	//历史查询
#define REQ_Exit			0x0005	//退出

#define MAXLEN_USERNAME 8
#define MSG_LEN			304

// S2C 服务器端到客户端的响应消息
#define RPL_Register		0x8001	// 用户注册
#define RPL_Login			0x8002	// 已经有用户登入了
#define RPL_Query			0x8003	// 用户查询单词
#define RPL_HiSTORY		 	0x8004	// 用户查询历史

#define ERR_EXIT(m) \
		do {\
					perror(m);\
					exit(EXIT_FAILURE);\
				}while(0)

struct XProtocol
{
	char 			username[MAXLEN_USERNAME];
	unsigned short  cmd_type;
	unsigned short  ret_val;
	unsigned long	data_len;
	unsigned char	data[MSG_LEN];
}__attribute__((aligned(4)));

char * pErrMsg = NULL;
char * sql;
char * sql1;

#endif
