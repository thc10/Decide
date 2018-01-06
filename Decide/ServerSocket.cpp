// ServerSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "Decide.h"
#include "ServerSocket.h"
#include "ClientSocket.h"

// CServerSocket

CServerSocket::CServerSocket()
{
}

CServerSocket::~CServerSocket()
{
}


// CServerSocket 成员函数


void CServerSocket::OnAccept(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	CClientSocket *Client = new CClientSocket();
	Accept(*Client);
	CSocket::OnAccept(nErrorCode);
}
