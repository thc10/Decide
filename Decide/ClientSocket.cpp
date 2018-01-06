// ClientSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "Decide.h"
#include "MessageHandle.h"
#include "ClientSocket.h"


// CClientSocket

CClientSocket::CClientSocket()
{
}

CClientSocket::~CClientSocket()
{
}


// CClientSocket 成员函数


void CClientSocket::OnClose(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	this->Close();
	delete this;
	CSocket::OnClose(nErrorCode);
}


void CClientSocket::OnReceive(int nErrorCode)
{
	//接收消息头
	MSGHEAD msg;
	char* pMsg = NULL;
	pMsg = new char[sizeof(msg)];
	memset(pMsg, 0, sizeof(msg));
	Receive(pMsg, sizeof(msg));
	msg.type = ((MSGHEAD*)pMsg)->type;
	msg.length = ((MSGHEAD*)pMsg)->length;
	delete pMsg;
	if (msg.length != 0)	//MSG_REQUEST没有消息体
	{
		pMsg = new char[msg.length];
		if (Receive(pMsg, msg.length) != msg.length)
		{
			//AfxMessageBox(_T("收到数据有误"));
			return;
		}
	}
	switch (msg.type)
	{
		case MSG_LOGIN:		//新用户加入
		{
			HandleLoginMsg(pMsg);
			break;
		}
		case MSG_VOTE:		//收到的投票结果
		{
			HandleVoteMsg(pMsg);
			break;
		}
		case MSG_LIST:		//更新本地用户信息表
		{
			HandleListMsg(pMsg);
			break;
		}
		case MSG_VERSION:	//版本信息维护
		{
			HandleVersionMsg(pMsg);
			break;
		}
		case MSG_REQUEST:	//请求用户消息列表
		{
			HandleRequstMsg(pMsg);
		}
	}
	CSocket::OnReceive(nErrorCode);
}

// 发送消息
BOOL CClientSocket::SendMSG(LPSTR pBuff, MSGHEAD* msg)
{
	//生成协议头
	int i = Send(msg, sizeof(MSGHEAD));
	if (i == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送错误！"));
		return FALSE;
	};
	//发送信息
	if (pBuff)
	{
		if (Send(pBuff, msg->length) == SOCKET_ERROR)
		{
			AfxMessageBox(_T("发送错误！"));
			return FALSE;
		};
	}
	return  TRUE;
}

