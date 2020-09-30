// ShowDlg.cpp: 实现文件
//

#include "pch.h"
#include "ShowDlg.h"
#include "afxdialogex.h"


// ShowDlg 对话框

IMPLEMENT_DYNAMIC(ShowDlg, CDialogEx)

ShowDlg::ShowDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

ShowDlg::~ShowDlg()
{
}

void ShowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ShowDlg, CDialogEx)
END_MESSAGE_MAP()


// ShowDlg 消息处理程序
