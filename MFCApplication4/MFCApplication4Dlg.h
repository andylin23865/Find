
// MFCApplication4Dlg.h: 头文件
//

#pragma once


// CMFCApplication4Dlg 对话框
class CMFCApplication4Dlg : public CDialogEx
{
// 构造
public:
	CMFCApplication4Dlg(CWnd* pParent = nullptr);	// 标准构造函数
	void updateProAll();
	void updateProOne();
	LRESULT OnMsg(WPARAM wp, LPARAM lp);

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION4_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	void save();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	BOOL PreTranslateMessage(MSG* pMsg);
public:
	CString m_inputFile1;
	CString m_inputFile2;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedOk();
	CEdit m_output;
	CString m_outStr;
	afx_msg void OnEnChangeInputFile1();
	afx_msg void OnEnChangeInputFile2();
	afx_msg void OnEnChangeEdit3();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	// 总进度条
	CProgressCtrl m_progressAll;
	// 单个任务进度
	CProgressCtrl m_progressOne;
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CComboBox m_comInput;
	CComboBox m_comInput2;
};
