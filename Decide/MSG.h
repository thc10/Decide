#pragma once
const int MSG_LOGIN = 0;	//加入群聊
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

//发起投票：由集群中的某一个节点发起投票，包括问题和答案，发送给集群中的所有节点（包括发起投票的节点）
typedef struct lchvote
{
	char question[100];  //需要投票进行选择的问题
	char answer1[50];  //选择1
	char answer2[50];  //选择2
}LCHVOTE;

//做出选择：每个节点随机选择一个节点（节点不能重复)发送自己的选择
typedef struct choice
{
	int answer;
	int flag;  //标识这是一次普通的选择还是最终选择，如果flag为1，说明该节点已经收到了2f+1个相同的选择,产生了最终结果
	int count;  //最终结果经过的节点数，如果不是最终结果，则始终为0，当最终结果经过的节点数达到阈值时，停止发送
}CHOICE;

//每个节点维护一个结构，记录他所收到的其他节点的选择，当收到选择为2f+1个时，根据多数节点的选择改变自己的选择
typedef struct record
{
	int count1;  //该节点收到其他节点的选择中，选择选项1的节点个数
	int count2;  //该节点收到其他节点的选择中，选择选项2的节点个数
}RECORD;