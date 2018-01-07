// LinkDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Decide.h"
#include "LinkDlg.h"
#include "afxdialogex.h"


// CLinkDlg 对话框

IMPLEMENT_DYNAMIC(CLinkDlg, CDialogEx)

CLinkDlg::CLinkDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LINKDLG, pParent)
	, localIP(2130706433)
	, groupIP(2130706433)
	, localPort(6666)
	, groupPort(6666)
{

}

CLinkDlg::~CLinkDlg()
{
}

void CLinkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_IPAddress(pDX, IDC_LOCALIP, localIP);
	DDX_IPAddress(pDX, IDC_GROUPIP, groupIP);
	DDX_Text(pDX, IDC_LOCALPORT, localPort);
	DDX_Text(pDX, IDC_GROUPPORT, groupPort);
}


BEGIN_MESSAGE_MAP(CLinkDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CLinkDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CLinkDlg 消息处理程序


void CLinkDlg::OnBnClickedOk()
{
	UpdateData();
	if (localIP == 0 || groupIP == 0 || localPort == 0 || groupPort == 0)
	{
		AfxMessageBox(_T("输入的信息无效!"));
		return;
	}
	char ip[20];
	IN_ADDR addr1, addr2;
	addr1.S_un.S_addr = htonl(localIP);
	addr2.S_un.S_addr = htonl(groupIP);
	//inet_ntoa返回一个char *,而这个char *的空间是在inet_ntoa里面静态分配
	theApp.m_IP.Empty();
	theApp.groupIP.Empty();
	inet_ntop(AF_INET, &addr1, ip, sizeof(ip));
	theApp.m_IP = ip;
	inet_ntop(AF_INET, &addr2, ip, sizeof(ip));
	theApp.groupIP = ip;
	theApp.m_Port = localPort;
	theApp.groupPort = groupPort;

	CDialogEx::OnOK();
}
