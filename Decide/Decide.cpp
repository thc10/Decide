
// Decide.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "Decide.h"
#include "DecideDlg.h"
#include "LinkDlg.h"
#include "ClientSocket.h"
#include "ServerSocket.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDecideApp

BEGIN_MESSAGE_MAP(CDecideApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CDecideApp 构造

CDecideApp::CDecideApp()
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
		AfxMessageBox(_T("动态创建服务器套接字出错！"));
		return false;
	}
	CLinkDlg Linkdlg;
	INT_PTR nResponse = Linkdlg.DoModal();
	if (nResponse == IDOK)
	{
		// 当用户输入IP和端口信息后，创建本机指定端口的服务器套接字
		if (!m_Socket->Create(theApp.m_Port))
		{
			AfxMessageBox(_T("创建套接字错误！"));
			m_Socket->Close();
			return false;
		}
		if (!m_Socket->Listen())
		{
			AfxMessageBox(_T("监听失败！"));
			m_Socket->Close();
			return false;
		}
		// 判断是否加入群聊
		if (!((m_IP.CompareNoCase(groupIP) == 0) && (m_Port == groupPort)))
		{
			CClientSocket *pClient = new CClientSocket();
			if (!pClient)
			{
				AfxMessageBox(_T("内存不足！"));
				return false;
			}
			if (!pClient->Create())
			{
				AfxMessageBox(_T("创建套接字失败！"));
				return false;
			}
			if (!pClient->Connect(theApp.groupIP.GetBuffer(0), theApp.groupPort))
			{
				AfxMessageBox(_T("连接群聊失败！"));
				return false;
			}
			MSGHEAD msg;
			cJSON *root = cJSON_CreateObject();
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
		type = MSG_VERSION;
		version = -1;
		AfxBeginThread(gossip, this);
		CDecideDlg dlg;
		m_pMainWnd = &dlg;
		nResponse = dlg.DoModal();
		if (nResponse == IDCANCEL)
		{

		}
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
		num = theApp.IPList.size();
		if (num == 0) {
			Sleep(1000);
			continue;
		}
		srand((unsigned)time(NULL));
		select = rand() % num;
		char *ip = theApp.IPList[select].ip;
		int port = theApp.IPList[select].port;
		CClientSocket *pClient = new CClientSocket();
		if (!pClient || !pClient->Create() || !pClient->Connect(CString(theApp.IPList[select].ip).GetBuffer(0), (UINT)theApp.IPList[select].port))
			continue;
		MSGHEAD msg;
		char* msg_toBsend = theApp.prepareMsg(theApp.type);
		if (!msg_toBsend)
			continue;
		msg.type = theApp.type;
		msg.length = strlen(msg_toBsend);
		pClient->SendMSG(msg_toBsend, &msg);
		Sleep(2000);
		pClient->Close();
		delete pClient;
	}
}

char* CDecideApp::prepareMsg(int type) {
	if (type == MSG_VERSION) {
		int num = IPList.size();
		int i = 0;
		cJSON *root = cJSON_CreateObject();
		cJSON_AddNumberToObject(root, "version", version);
		return cJSON_PrintUnformatted(root);
	}
	else if (type == MSG_VOTE) {

	}
	else {
		return NULL;
	}
}
