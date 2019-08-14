#ifndef _PUBLIC_H_
#define _PUBLIC_H_

#include <list>
#include <algorithm>
using namespace std;

// C2S 客户端到服请求消息（命令）
#define C2S_LOGIN		0x01	//登入
#define C2S_LOGOUT		0x02	//登出
#define C2S_ONLINE_USER 0x03	//在线用户列表的请求

#define MSG_LEN			512

// S2C 服务器端到客户端的响应消息
#define S2C_LOGIN_OK		0x01	// 登入成功
#define S2C_ALREADY_LOGINED 0x02	// 已经有用户登入了
#define S2C_SOMEONE_LOGIN 	0x03	// 向其他用户响应有用户登入
#define S2C_SOMEONE_LOGOUT 	0x04	// 向其他用户响应有用户登出
#define S2C_ONLINE_USER		0x05	// 在线用户列表的相应

// C2C
#define C2C_CHAT			0x06	// 客户端与客户端的点对点的聊天


// 消息结构
// 登入消息：cmd=C2S_LOGIN body=username
// 聊天消息：cmd=C2C_CHAT body=ChatMsg_t信息 
typedef struct tagMessage
{
	int cmd;
	char body[MSG_LEN];
}Message_t;

// 用户信息
typedef struct tagUserInfo
{
	char username[16];
	unsigned int ip;   // 4个字节的网络字节的整数
	unsigned int port; // 2个字节的网络字节的整数
}UserInfo_t;

// 客户端与客户端的点对点的聊天信息
typedef struct tagChatMsg
{
	char username[16];
	char msg[100];
}ChatMsg_t;

typedef list<UserInfo_t> UserList_t;  //链表



#endif
