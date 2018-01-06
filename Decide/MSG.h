#pragma once
const int MSG_INFO = 1;		//本机IP以及侦听端口信息
const int MSG_VOTE = 2;		//投票信息
const int MSG_LIST = 3;		//用户列表
const int MSG_VERSION = 4;	//版本信息

//消息头
typedef struct msg {
	int type;
	int version;
	int length;
}MSGHEAD;

// IP及侦听端口信息
typedef struct ipinfo {
	char ip[20];
	 int port;
}IPINFO;

// 投票信息
typedef struct vote {
	int choice;
	int flag;
}VOTE;