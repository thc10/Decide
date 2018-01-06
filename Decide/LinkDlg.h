#pragma once


// CLinkDlg 对话框

class CLinkDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLinkDlg)

public:
	CLinkDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLinkDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LINKDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	DWORD localIP;
	DWORD groupIP;
	UINT localPort;
	UINT groupPort;
	afx_msg void OnBnClickedOk();
};
