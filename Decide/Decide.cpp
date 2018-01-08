
// Decide.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "Decide.h"
#include "DecideDlg.h"
#include "LinkDlg.h"
#include "VoteDlg.h"
#include "ServerSocket.h"
#include "cJSON.h"
#include <time.h>
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDecideApp

BEGIN_MESSAGE_MAP(CDecideApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CDecideApp 构造

CDecideApp::CDecideApp()
	: is_start(0)
	, voteend(0)
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CDecideApp 对象

CDecideApp theApp;


// CDecideApp 初始化

BOOL CDecideApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}


	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("TALK"));
	
	m_Socket = new CServerSocket();
	if (!m_Socket)
	{
		AfxMessageBox(_T("动态创建服务器套接字出错!"));
		return false;
	}
	CLinkDlg Linkdlg;
	INT_PTR nResponse = Linkdlg.DoModal();
	if (nResponse == IDOK)
	{
		// 当用户输入IP和端口信息后，创建本机指定端口的服务器套接字
		if (!m_Socket->Create(theApp.m_Port))
		{
			AfxMessageBox(_T("创建套接字错误!"));
			m_Socket->Close();
			return false;
		}
		if (!m_Socket->Listen())
		{
			AfxMessageBox(_T("监听失败!"));
			m_Socket->Close();
			return false;
		}
		// 判断是否加入群聊
		if (!((m_IP.CompareNoCase(groupIP) == 0) && (m_Port == groupPort)))
		{
			CClientSocket *pClient = new CClientSocket();
			if (!pClient)
			{
				AfxMessageBox(_T("Error: No enough memory to create a Socket!"));
				return false;
			}
			if (!pClient->Create())
			{
				AfxMessageBox(_T("Error: fail to create a ClientSocket!"));
				return false;
			}
			if (!pClient->Connect(theApp.groupIP.GetBuffer(0), theApp.groupPort))
			{
				AfxMessageBox(_T("Error: fail to connect group!"));
				return false;
			}
			MSGHEAD msg;
			cJSON *root = cJSON_CreateObject();
			if (!root)
			{
				AfxMessageBox(_T("Memory malloc error"));
				return false;
			}
			_bstr_t b(m_IP);
			char* ip = b;
			cJSON_AddStringToObject(root, "ip", ip);
			cJSON_AddNumberToObject(root, "port", m_Port);
			char *pBuf = cJSON_PrintUnformatted(root);

			msg.type = MSG_LOGIN;
			msg.length = strlen(pBuf);
			pClient->SendMSG(pBuf, &msg);

			cJSON_Delete(root);
			free(pBuf);

			pClient->Close();
			delete pClient;
		}
		else {
			_bstr_t b(m_IP);
			char* ip = b;
			IPINFO NewNode;
			strncpy(NewNode.ip, ip, 20);
			NewNode.port = m_Port;
			theApp.IPList.push_back(NewNode);
		}
		type = MSG_VERSION;
		version = -1;
		AfxBeginThread(gossip, this);
		CDecideDlg dlg;
		nResponse = dlg.DoModal();
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

CServerSocket * CDecideApp::GetServerSocket() const
{
	return m_Socket;
}

int CDecideApp::ExitInstance()
{
	if (m_Socket)
	{
		delete m_Socket;
		m_Socket = NULL;
	}
	IPList.clear();
	return CWinApp::ExitInstance();
}

BOOL CDecideApp::WChar2MByte(LPCWSTR srcBuff, LPSTR destBuff, int nlen)
{
	int n = 0;
	n = WideCharToMultiByte(CP_OEMCP, 0, srcBuff, -1, destBuff, 0, 0, FALSE);
	if (n > nlen)
		return FALSE;

	WideCharToMultiByte(CP_OEMCP, 0, srcBuff, -1, destBuff, nlen, 0, FALSE);

	return TRUE;
}

UINT CDecideApp::gossip(LPVOID lpParam) {
	int num = 0, select = 0;
	while (1) {
		if (theApp.voteend)
		{
			continue;
		}
		num = theApp.IPList.size();
		if (num == 0) {
			Sleep(1000);
			continue;
		}
		srand((unsigned)time(NULL));
		select = rand() % num;
		char *ip = theApp.IPList[select].ip;
		int port = theApp.IPList[select].port;
		_bstr_t b(theApp.m_IP);
		char* local = b;
		if (strcmp(ip, local) == 0 && port == theApp.m_Port)
			continue;
		CClientSocket *pClient = new CClientSocket();
		if (!pClient || !pClient->Create() || !pClient->Connect(CString(theApp.IPList[select].ip).GetBuffer(0), (UINT)theApp.IPList[select].port))
			continue;
		MSGHEAD msg;
		char* msg_toBsend = theApp.prepareMsg(theApp.type);
		if (!msg_toBsend)
			continue;
		if (theApp.type == MSG_STOP)
		{
			msg.type = MSG_VOTE;
			theApp.setVoteEnd(1);
		}
		else
		{
			msg.type = theApp.type;
		}
		msg.length = strlen(msg_toBsend);
		pClient->SendMSG(msg_toBsend, &msg);
		Sleep(200);
		pClient->Close();
		delete pClient;
	}
	return 0;
}

char* CDecideApp::prepareMsg(int type) {
	if (type == MSG_VERSION) {
		int num = IPList.size();
		int i = 0;
		_bstr_t b(m_IP);
		char* ip = b;
		cJSON *root = cJSON_CreateObject();
		cJSON_AddNumberToObject(root, "version", version);
		cJSON_AddStringToObject(root, "ip", ip);
		cJSON_AddNumberToObject(root, "port", m_Port);
		char *pBuf = cJSON_PrintUnformatted(root);
		cJSON_Delete(root);
		return pBuf;
	}
	else if (type == MSG_VOTE) {
		cJSON *root = cJSON_CreateObject();
		cJSON_AddNumberToObject(root, "answer", MyChoice.answer);
		cJSON_AddNumberToObject(root, "count", MyChoice.count);
		cJSON_AddNumberToObject(root, "flag", MyChoice.flag);
		char *pBuf = cJSON_PrintUnformatted(root);
		cJSON_Delete(root);
		return pBuf;
	}
	else if (type == MSG_STOP) {
		cJSON *root = cJSON_CreateObject();
		cJSON_AddNumberToObject(root, "answer", MyChoice.answer);
		cJSON_AddNumberToObject(root, "count", MyChoice.count);
		cJSON_AddNumberToObject(root, "flag", MyChoice.flag);
		char *pBuf = cJSON_PrintUnformatted(root);
		cJSON_Delete(root);
		return pBuf;
	}
	else
		return "ad";
}

void CDecideApp::setVersion(int version) {
	this->version = version;
}

void CDecideApp::setType(int type) {
	this->type = type;
}

void CDecideApp::setIsStart(int is_start) {
	this->is_start = is_start;
}

void CDecideApp::setVoteEnd(int sign)
{
	this->voteend = sign;
}

void CDecideApp::setVoteQue(LCHVOTE Que) {
	memset(&VoteQue, 0, sizeof(VoteQue));
	strcpy_s(this->VoteQue.question, Que.question);
	strcpy_s(this->VoteQue.answer1, Que.answer1);
	strcpy_s(this->VoteQue.answer2, Que.answer2);
}

void CDecideApp::setChoice(CHOICE choice) {
	memset(&MyChoice, 0, sizeof(MyChoice));
	this->MyChoice.answer = choice.answer;
	this->MyChoice.count = choice.count;
	this->MyChoice.flag = choice.flag;
}

void CDecideApp::setRecord(RECORD record) {
	memset(&VoteRecord, 0, sizeof(VoteRecord));
	this->VoteRecord.count1 = record.count1;
	this->VoteRecord.count2 = record.count2;
	this->HandleRecord();
}

void CDecideApp::HandleChoice(CHOICE choice) {
	int num = IPList.size();
	if (choice.flag == 1)  //Received 2f+1 same options from other nodes, this is the final result
	{
		if (this->MyChoice.flag == 0)  //未被感染
		{
			CHOICE rechoice;
			rechoice.answer = choice.answer;
			rechoice.flag = choice.flag;
			rechoice.count = num;
			setChoice(rechoice);
			setType(MSG_STOP);
			setVoteEnd(0);
			mainDlg.DisplayResult(MyChoice.answer);
			return;
		}
		else  //已经被感染
		{
			CHOICE rechoice;
			rechoice.answer = choice.answer;
			rechoice.flag = choice.flag;
			rechoice.count = choice.count - 1;
			setChoice(rechoice);
			setType(MSG_STOP);
			setVoteEnd(0);
			mainDlg.DisplayResult(MyChoice.answer);
			return;
		}
	}
	else if (choice.flag == 0)  //说明只是一次普通的选择，不是最终结果
	{
		if (MyChoice.flag == 1)
		{
			return;
		}
		RECORD rerecord;
		rerecord.count1 = VoteRecord.count1;
		rerecord.count2 = VoteRecord.count2;
		if (choice.answer == 1)
		{
			rerecord.count1 += 1;
		}
		else if (choice.answer == 2)
		{
			rerecord.count2 += 1;
		}
		else
		{
			return;
		}
		setRecord(rerecord);
		setType(MSG_VOTE);
		return;
	}	
}

void CDecideApp::HandleRecord()
{
	int num = IPList.size(); 
	int end = ((num * 2 + 1) % 3) ? ((num * 2 + 1) / 3) : (((num * 2 + 1) / 3) + 1);
	if (VoteRecord.count1 + VoteRecord.count2 >= end)  //收到的包不少于2f+1
	{
		CHOICE choice;
		if (VoteRecord.count1 >= end)
		{
			choice.answer = 1;
			choice.flag = 1;
			choice.count = num;
			setChoice(choice);
			setType(MSG_STOP);
			setVoteEnd(0);
		}
		else if (VoteRecord.count2 >= end)
		{
			choice.answer = 2;
			choice.flag = 1;
			choice.count = num;
			setChoice(choice);
			setType(MSG_STOP);
			setVoteEnd(0);
		}
		else if (VoteRecord.count1 > VoteRecord.count2)
		{
			choice.answer = 1;
			choice.flag = 0;
			choice.count = num;
			setChoice(choice);
			setType(MSG_VOTE);
		}
		else if (VoteRecord.count2 > VoteRecord.count1)
		{
			choice.answer = 2;
			choice.flag = 0;
			choice.count = 0;
			setChoice(choice);
			setType(MSG_VOTE);
		}
		else
		{
			return;
		}
	}
	return;
}

void CDecideApp::VersionCompare(int receive_version, char *ip, int port)
{
	//收到版本号相同
	if (theApp.version == receive_version)
	{
		return;
	}
	//收到版本号小于接收的，向对方请求list
	else if (theApp.version < receive_version)
	{
		cJSON *root = cJSON_CreateObject();
		_bstr_t b(m_IP);
		char* ip = b;
		cJSON_AddStringToObject(root, "ip", ip);
		cJSON_AddNumberToObject(root, "port", m_Port);
		char* pBuf = cJSON_PrintUnformatted(root);
		MSGHEAD msg;
		msg.type = MSG_REQUEST;
		msg.length = strlen(pBuf);

		CClientSocket *pClient = new CClientSocket();
		if (pClient && pClient->Create() && pClient->Connect(CString(ip).GetBuffer(0), port)) {
			pClient->SendMSG(pBuf, &msg);
			pClient->Close();
		}
		delete pClient;
		cJSON_Delete(root);
		free(pBuf);
	}
	else
	{
		SendListMsg(ip, port);
	}
}

void CDecideApp::SendListMsg(char* ip, int port) {
	MSGHEAD msg;

	cJSON *json_root = cJSON_CreateObject();
	if (!json_root)
	{
		AfxMessageBox(_T("Memory malloc error"));
		return;
	}

	cJSON *root = cJSON_CreateArray();
	if (!root)
	{
		AfxMessageBox(_T("Memory malloc error"));
		return;
	}

	cJSON_AddNumberToObject(json_root, "version", theApp.version);
	int num = theApp.IPList.size();

	for (int i = 0; i < num; i++)
	{
		cJSON *temp = cJSON_CreateObject();
		cJSON_AddStringToObject(temp, "ip", theApp.IPList[i].ip);
		cJSON_AddNumberToObject(temp, "port", theApp.IPList[i].port);
		cJSON_AddItemToArray(root, temp);
	}
	cJSON_AddItemToObject(json_root, "data", root);

	char *pBuff = cJSON_PrintUnformatted(json_root);

	msg.type = MSG_LIST;
	msg.length = strlen(pBuff);

	CClientSocket *pClient = new CClientSocket();
	if (pClient && pClient->Create() && pClient->Connect(CString(ip).GetBuffer(0), port)) {
		pClient->SendMSG(pBuff, &msg);
		pClient->Close();
	}

	cJSON_Delete(root);
	free(pBuff);
}

void CDecideApp::SendVote() {
	cJSON *root = cJSON_CreateObject();
	if (!root){
		AfxMessageBox(_T("Memory malloc error"));
		return;
	}
	cJSON_AddStringToObject(root, "question", VoteQue.question);
	cJSON_AddStringToObject(root, "answer1", VoteQue.answer1);
	cJSON_AddStringToObject(root, "answer2", VoteQue.answer2);
	char *pBuf = cJSON_PrintUnformatted(root);

	MSGHEAD msg;
	msg.type = MSG_STARTVOTE;
	msg.length = strlen(pBuf);

	int num = IPList.size();

	for (int i = 0; i < num; i++) {
		CClientSocket *pClient = new CClientSocket();
		if (!pClient || !pClient->Create() || !pClient->Connect(CString(IPList[i].ip).GetBuffer(0), IPList[i].port))
			continue;
		pClient->SendMSG(pBuf, &msg);
	}

	cJSON_Delete(root);
	free(pBuf);
	type = MSG_VOTE;
}

void CDecideApp::StartVote() {
	
	//m_pMainWnd = &mainDlg;
	mainDlg.DoModal();
}
