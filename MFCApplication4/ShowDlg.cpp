// ShowDlg.cpp: 实现文件
//

#include "pch.h"
#include "ShowDlg.h"
#include "afxdialogex.h"
#include "MFCApplication4Dlg.h"

bool pause = false;
extern bool showDetail;

// ShowDlg 对话框

IMPLEMENT_DYNAMIC(ShowDlg, CDialogEx)

ShowDlg::ShowDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, m_inputfile1(_T(""))
	, m_inputfile2(_T(""))
	, m_file1Name(_T(""))
{

}

ShowDlg::~ShowDlg()
{
}

void ShowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_file1);
	DDX_Control(pDX, IDC_EDIT2, m_file2);
	DDX_Text(pDX, IDC_EDIT1, m_inputfile1);
	DDX_Text(pDX, IDC_EDIT2, m_inputfile2);
	DDX_Text(pDX, IDC_STATIC11, m_file1Name);
	DDX_Control(pDX, IDC_STATIC22, m_file2Name);
}

void ShowDlg::OnCancel()
{
	pause = false;
	showDetail = false;
	CMFCApplication4Dlg* pParent = (CMFCApplication4Dlg*)this->GetParent();
	pParent->detail.SetCheck(showDetail);
	pParent->UpdateData(FALSE);
	CDialogEx::OnCancel();
}

void ShowDlg::showFile1(string str)
{
	m_inputfile1 += str.c_str();
	m_inputfile1 += "\r\n";
	//UpdateData(FALSE);
}

void ShowDlg::showFile2(string str)
{
	m_inputfile2 += str.c_str();
	m_inputfile2 += "\r\n";
	//UpdateData(FALSE);
}


void ShowDlg::clean()
{
	m_inputfile1 = "";
	m_inputfile2 = "";
	UpdateData(FALSE);
}

BEGIN_MESSAGE_MAP(ShowDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &ShowDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDCANCEL, &ShowDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// ShowDlg 消息处理程序


void ShowDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	pause = false;
}


void ShowDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}
