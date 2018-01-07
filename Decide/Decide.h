
// Decide.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "ServerSocket.h"
#include "ClientSocket.h"
#include "VoteDlg.h"
#include "MSG.h"
#include <vector>
using namespace std;

// CDecideApp: 
// 有关此类的实现，请参阅 Decide.cpp
//

class CDecideApp : public CWinApp
{
private:
	CServerSocket*m_Socket;
public:
	CDecideApp();

// 重写
public:
	virtual BOOL InitInstance();
	CVoteDlg mainDlg;
	CString result;     //final result
	CString m_IP;		//local IP
	UINT m_Port;		//listened port
	CString groupIP;	//IP of the room
	UINT groupPort;		//port of the room
	vector<IPINFO> IPList;	//IP container
	int type;			//the type of gossip message
	int is_start;		//is voting start?
	int voteend;		//is voting end?
	int version;		//verison of IPList
	LCHVOTE VoteQue;	//Info of vote question
	CHOICE MyChoice;	//local choice
	RECORD VoteRecord;	//count the recieved vote msg
// 实现

	CServerSocket* GetServerSocket() const;
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
	BOOL WChar2MByte(LPCWSTR srcBuff, LPSTR destBuff, int nlen);
	static UINT gossip(LPVOID lpParam);
	char* prepareMsg(int type);
public:
	void setVersion(int version);
	void setType(int type);
	void setVoteQue(LCHVOTE Que);
	void setChoice(CHOICE choice);
	void setRecord(RECORD record);
	void setIsStart(int is_start);
	void setVoteEnd(int sign);
	void HandleChoice(CHOICE choice);
	void HandleRecord();
	void VersionCompare(int receive_version, char* ip, int port);
	void SendListMsg(char* ip, int port);
	void SendVote();
	void StartVote();
};

extern CDecideApp theApp;
