
// Decide.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "ServerSocket.h"
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
	CString m_IP;		//本机IP
	UINT m_Port;		//侦听端口
	CString groupIP;	//群聊房间IP
	UINT groupPort;		//群聊房间端口
	vector<IPINFO> IPList;	//IP容器
	int type;			//gossip同步的类型
	int version;		//版本号
// 实现

	CServerSocket* GetServerSocket() const;
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
	BOOL WChar2MByte(LPCWSTR srcBuff, LPSTR destBuff, int nlen);
	static UINT gossip(LPVOID lpParam);
	char* prepareMsg(int type);

public:
	void setVersion(int version);
};

extern CDecideApp theApp;
