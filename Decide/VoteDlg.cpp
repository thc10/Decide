// VoteDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Decide.h"
#include "VoteDlg.h"
#include "afxdialogex.h"


// CVoteDlg 对话框

IMPLEMENT_DYNAMIC(CVoteDlg, CDialogEx)

CVoteDlg::CVoteDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VOTE_DIALOG, pParent)
{

}

CVoteDlg::~CVoteDlg()
{
}

void CVoteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CVoteDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CVoteDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CVoteDlg 消息处理程序

//收到节点发送的投票请求，发起投票的节点发送问题和选项
void CVoteDlg::OnReceiveRequest()
{
	/*
	收到：
	key: question value:(CString)m_questiong
	key: answer value: (CString)m_answer1
	key: answer value: (CString)m_answer2
	*/
	
	/*
	处理后调用DisplayVote()显示投票的问题和选项
	*/

}

//向其他节点发送自己的选择
void CVoteDlg::OnSendOption(CString &choice)
{
	/*
	发送：key:choice value:(CString)choice
	*/
}

//显示投票的问题和选项
void CVoteDlg::DisplayVote(CString &question, CString &answer1, CString &answer2)
{
	//在IDC_EDIT_QUESTION_R显示投票问题
	((CEdit*)GetDlgItem(IDC_EDIT_QUESTION_R))->ReplaceSel(question);
	//向下拉列表框添加选项
	m_option->InsertString(0, answer1);
	m_option->InsertString(1, answer2);
}

//显示投票最终结果
void CVoteDlg::DisplayResult(CString &result) const
{
	((CEdit*)GetDlgItem(IDC_EDIT_RESULT))->ReplaceSel(result);
}


void CVoteDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//获取节点做出的选择
	CString choice;  //节点做出的选择
	int nIndex = m_option->GetCurSel();  //获取选中的索引
	m_option->GetLBText(nIndex, choice);
	//调用函数OnSendOption()，向其他节点发送本节点的选择
	OnSendOption(choice);
	CDialogEx::OnOK();
}


