#pragma once
#include "Resource.h"
#include <iostream>
using namespace std;

// ShowDlg 对话框

class ShowDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ShowDlg)

public:
	ShowDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~ShowDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnCancel();

	DECLARE_MESSAGE_MAP()
public:
	void showFile1(string str);
	void showFile2(string str);
	void clean();
	CEdit m_file1;
	CEdit m_file2;
	CString m_inputfile1;
	CString m_inputfile2;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedCancel();
	CString m_file1Name;
	CStatic m_file2Name;
};
