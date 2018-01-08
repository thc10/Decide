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
	DDX_Control(pDX, IDC_COMBO1, m_option);
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


//显示投票的问题和选项
void CVoteDlg::DisplayVote()
{
	//在IDC_EDIT_QUESTION_R显示投票问题
	((CEdit*)GetDlgItem(IDC_EDIT_QUESTION_R))->ReplaceSel(CString(theApp.VoteQue.question));
	//向下拉列表框添加选项
	m_option.InsertString(0, CString(theApp.VoteQue.answer1));
	m_option.InsertString(1, CString(theApp.VoteQue.answer2));
}

//显示投票最终结果
void CVoteDlg::DisplayResult(int &index) const
{
	CString result;
	m_option.GetLBText(index-1, result);
	((CEdit*)GetDlgItem(IDC_EDIT_RESULT))->SetWindowTextW(result);
}


void CVoteDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//get the choise
	int nIndex = m_option.GetCurSel();  //get the index of the choised question

	//Use gossip to send vote message
	CHOICE CHO;
	CHO.answer = nIndex + 1;
	CHO.flag = 0;
	CHO.count = 0;
	theApp.setIsStart(1);
	theApp.setChoice(CHO);
	theApp.setType(MSG_VOTE);
	theApp.setVoteEnd(0);
	//AfxMessageBox(_T("Vote Succeed!"));
}


BOOL CVoteDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	DisplayVote();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
