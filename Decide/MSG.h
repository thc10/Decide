#pragma once
const int MSG_LOGIN = 0;	//Add into room
const int MSG_VERSION = 1;	//Version Msg
const int MSG_REQUEST = 2;	//Requet for IPList
const int MSG_LIST = 3;		//Send IPList
const int MSG_VOTE = 4;		//Vote Msg
const int MSG_STARTVOTE = 5;	//Strat vote
const int MSG_STOP = 6;  //Stop Vote

// the Message of IP and Port
typedef struct ipinfo {
	char ip[20];
	int port;
}IPINFO;

// th Head of Message
typedef struct msg {
	int type;
	int length;
	IPINFO ipinfo;
}MSGHEAD;
// Start Vote : One of the Nodes Input the Question and Two Answers, Then Send This Struct to Other Nodes 
typedef struct lchvote
{
	char question[100];
	char answer1[50];  
	char answer2[50]; 
}LCHVOTE;

// Make the Choice 
typedef struct choice
{
	int answer;
	int flag;  //Make if Make the Final Choice，if flag equal 1，it means this node has recive 2f+1 choices and get the final result
	int count;  //avoid the message move from one node to another node forever , use count to kill message
}CHOICE;

//count the choice recieved
typedef struct record
{
	int count1;  //该节点收到其他节点的选择中，选择选项1的节点个数
	int count2;  //该节点收到其他节点的选择中，选择选项2的节点个数
}RECORD;