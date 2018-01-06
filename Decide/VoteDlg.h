#pragma once
#include "afxwin.h"


// CVoteDlg 对话框

class CVoteDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVoteDlg)

public:
	CVoteDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CVoteDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VOTE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();
	void OnReceiveRequest();  //收到发起投票的请求
	void OnSendOption(CString &choice);  //向其他节点发送自己的选择
	void DisplayVote(CString &question, CString &answer1, CString &answer2);  //显示投票的问题和选项
	void DisplayResult(CString &result) const;  //显示最终投票结果
	CComboBox *m_option = (CComboBox *)GetDlgItem(IDC_COMBO1);

};
