
// MFCApplication4Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication4.h"
#include "MFCApplication4Dlg.h"
#include "afxdialogex.h"
#include "same.h"
#include <atlstr.h>
#include <Windows.h>
#include <process.h>
#include <direct.h>
#include <queue>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//#define WM_USER_MSG WM_USER + 1001

int g_proAll = 0;
int g_proOne = 0;
bool update = true;

#define MAX_CNT 3

queue<string> inputList1;
queue<string> inputList2;
string savePath = "path.txt";

DWORD WINAPI UpdateThread(LPVOID lpParam)
{
	//Sleep(5*1000);
	//CControlPage* pControlPage = (CControlPage*)lpParam;
	CMFCApplication4Dlg* hWnd = (CMFCApplication4Dlg*)lpParam;
	while (true)
	{
		hWnd->SetDlgItemText(IDC_TIME, CTime::GetCurrentTime().Format("%Y/%m/%d %H:%M:%S"));        //显示系统时
		Sleep(1000);
	}
	return 0;
}

DWORD WINAPI UpdateThreadAll(LPVOID lpParam)
{
	//Sleep(5*1000);
	//CControlPage* pControlPage = (CControlPage*)lpParam;
	CMFCApplication4Dlg* hWnd = (CMFCApplication4Dlg*)lpParam;
	while (true)
	{
		hWnd->m_progressAll.SetPos(g_proAll);
		Sleep(100);
	}
	return 0;
}

DWORD WINAPI UpdateThreadOne(LPVOID lpParam)
{
	//Sleep(5*1000);
	//CControlPage* pControlPage = (CControlPage*)lpParam;
	CMFCApplication4Dlg* hWnd = (CMFCApplication4Dlg*)lpParam;
	while (true)
	{
		hWnd->m_progressOne.SetPos(g_proOne);
		Sleep(100);
	}
	return 0;
}

DWORD WINAPI CalThread(LPVOID lpParam)
{
	//Sleep(5*1000);
	//CControlPage* pControlPage = (CControlPage*)lpParam;
	USES_CONVERSION;
	CMFCApplication4Dlg* hWnd = (CMFCApplication4Dlg*)lpParam;
	isSamePath(hWnd->m_inputFile1.GetString(), hWnd->m_inputFile2.GetString(), hWnd->m_outStr,hWnd);
	g_proOne = 100;
	//g_proAll = 100;
	//hWnd->UpdateData(FALSE);
	//exit(0);
	return 0;
}
//CreateThread(NULL, 0, UpdateThread, this, 0, 0);
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
	~CAboutDlg() {
	}

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication4Dlg 对话框



CMFCApplication4Dlg::CMFCApplication4Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION4_DIALOG, pParent)
	, m_inputFile1(_T(""))
	, m_inputFile2(_T(""))
	, m_outStr(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CMFCApplication4Dlg::updateProAll()
{
	m_progressAll.SetPos(g_proAll);
}

void CMFCApplication4Dlg::updateProOne()
{
		m_progressOne.SetPos(g_proOne);
}

void CMFCApplication4Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_INPUT_FILE1, m_inputFile1);
	DDX_Text(pDX, IDC_INPUT_FILE2, m_inputFile2);
	DDX_Control(pDX, IDC_EDIT3, m_output);
	DDX_Text(pDX, IDC_EDIT3, m_outStr);
	DDX_Control(pDX, IDC_PROGRESS1, m_progressAll);
	DDX_Control(pDX, IDC_PROGRESS2, m_progressOne);
	DDX_Control(pDX, IDC_COMBO3, m_comInput);
	DDX_Control(pDX, IDC_COMBO5, m_comInput2);
}

BEGIN_MESSAGE_MAP(CMFCApplication4Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication4Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCApplication4Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDOK, &CMFCApplication4Dlg::OnBnClickedOk)
	ON_EN_CHANGE(IDC_INPUT_FILE1, &CMFCApplication4Dlg::OnEnChangeInputFile1)
	ON_EN_CHANGE(IDC_INPUT_FILE2, &CMFCApplication4Dlg::OnEnChangeInputFile2)
	//ON_EN_CHANGE(IDC_EDIT3, &CMFCApplication4Dlg::OnEnChangeEdit3)
	ON_WM_CTLCOLOR()
	ON_WM_DROPFILES()
	//ON_EN_CHANGE(IDC_EDIT1, &CMFCApplication4Dlg::OnEnChangeEdit1)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMFCApplication4Dlg 消息处理程序

BOOL CMFCApplication4Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	m_progressAll.SetRange(0, 100);
	m_progressAll.SetStep(1);
	m_progressAll.SetPos(0);

	m_progressOne.SetRange(0, 100);
	m_progressOne.SetStep(1);
	m_progressOne.SetPos(0);

	//GetDlgItem(IDC_TIME).en

	CreateThread(NULL, 0, UpdateThreadAll, this, 0, 0);
	CreateThread(NULL, 0, UpdateThreadOne, this, 0, 0);
	CreateThread(NULL, 0, UpdateThread, this, 0, 0);

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	//CString strTime;
	//CTime tm;
	//tm = CTime::GetCurrentTime();         //获取当前系统时间
	//strTime = tm.Format("%y/%m/%d %X");   //格式化系统时间。即使系统时 间按照Format中设置的格式显示
	//SetDlgItemText(IDC_TIME, strTime);        //初始化编辑框显示
	SetTimer(1, 1000, NULL);         //启动定时器

	// TODO: 在此添加额外的初始化代码
	ChangeWindowMessageFilter(WM_DROPFILES, MSGFLT_ADD);
	ChangeWindowMessageFilter(0x0049, MSGFLT_ADD);
	m_comInput.ShowWindow(FALSE);
	m_comInput2.ShowWindow(FALSE);

	char buffer[MAX_PATH];
	_getcwd(buffer, MAX_PATH);
	savePath = string(buffer) + "\\"+ savePath;

	std::ifstream	OsRead(savePath, std::ofstream::app);
	int size1 = 0, size2 = 0;
	OsRead >> size1 >> size2;
	for (int i = 0; i < size1; i++) {
		OsRead >> buffer;
		inputList1.push(buffer);
	}
	for (int i = 0; i < size2; i++) {
		OsRead >> buffer;
		inputList2.push(buffer);
	}
	//inputList2.push("555");
	OsRead.close();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCApplication4Dlg::OnCancel()
{
	save();
	CDialogEx::OnCancel();
}

void CMFCApplication4Dlg::save()
{
	remove(savePath.c_str());
	std::ofstream	OsWrite(savePath, std::ofstream::app);
	OsWrite << inputList1.size() << " " << inputList2.size() << endl;
	int size1 = inputList1.size();
	int size2 = inputList2.size();
	for (int i = 0; i < size1; i++) {
		OsWrite << inputList1.front().c_str() << endl;
		inputList1.pop();
	}
	for (int i = 0; i < size2; i++) {
		OsWrite << inputList2.front().c_str() << endl;
		inputList2.pop();
	}
	OsWrite.close();
}

void CMFCApplication4Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCApplication4Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
		//dc.SetTextColor(RGB(0, 255, 0));
		//CString t("This is a test!");
		//dc.DrawText(t, CRect(100, 10, 400, 400), DT_SINGLELINE | DT_LEFT | DT_VCENTER); //显示文本
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCApplication4Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CMFCApplication4Dlg::PreTranslateMessage(MSG * pMsg)
{
	if (pMsg->message == WM_DROPFILES) {
		//pMsg->hwnd->unused;
		CPoint ptMouse;
		GetCursorPos(&ptMouse);
		CWnd * pWnd = WindowFromPoint(ptMouse);
		HDROP hDropInfo = (HDROP)pMsg->wParam;
		TCHAR szPath[MAX_PATH] = { 0 };
		UINT nCount = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
		for (UINT idx = 0; idx < nCount; ++idx)
		{
			DragQueryFile(hDropInfo, idx, szPath, MAX_PATH);
			//MessageBox(szPath);//以消息盒子形式显示路径
			//SetDlgItemText(IDC_INPUT_FILE1, szPath);//在编辑框内显示路径
			pWnd->SetWindowTextA(szPath);
			UpdateData();
		}
		DragFinish(hDropInfo);
		//TranslateMessage(pMsg);
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}



void CMFCApplication4Dlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//选择输出路径  
	TCHAR szDir[100];
	BROWSEINFO bi;
	ITEMIDLIST *pidl;
	bi.hwndOwner = this->m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szDir;//输出缓冲区
	bi.lpszTitle = _T("请选择文件夹："); //选择界面的标题  
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;//新的界面
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	pidl = SHBrowseForFolder(&bi);//弹出对话框   
	if (pidl == NULL)//点了取消，或者选择了无效的文件夹则返回NULL  
		return;

	if (SHGetPathFromIDList(pidl, szDir))
	{
		SetDlgItemText(IDC_INPUT_FILE1, szDir);//szDir就是选择的文件夹路径，IDC_EDIT_DIR是edit控件的ID
	}
}


void CMFCApplication4Dlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
		//选择输出路径  
	TCHAR szDir[100];
	BROWSEINFO bi;
	ITEMIDLIST *pidl;
	bi.hwndOwner = this->m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szDir;//输出缓冲区
	bi.lpszTitle = _T("请选择文件夹："); //选择界面的标题  
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;//新的界面
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	pidl = SHBrowseForFolder(&bi);//弹出对话框   
	if (pidl == NULL)//点了取消，或者选择了无效的文件夹则返回NULL  
		return;

	if (SHGetPathFromIDList(pidl, szDir))
	{
		SetDlgItemText(IDC_INPUT_FILE2, szDir);//szDir就是选择的文件夹路径，IDC_EDIT_DIR是edit控件的ID
	}
}

void Split(CString source, CStringArray& dest, CString division)
{
	dest.RemoveAll();
	int pos = 0;
	int pre_pos = 0;
	while (-1 != pos) {
		pre_pos = pos;
		pos = source.Find(division, (pos + 1));
		dest.Add(source.Mid(pre_pos, (pos - pre_pos)));
	}
}

static HANDLE h = NULL;

void CMFCApplication4Dlg::OnBnClickedOk()
{
	m_outStr = "";
	TerminateThread(h, NULL);
	h = CreateThread(NULL, 0, CalThread, this, 0, 0);
}


void CMFCApplication4Dlg::OnEnChangeInputFile1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	string t = (m_inputFile1.GetString());
	if (inputList1.size() >= MAX_CNT) {
		inputList1.pop();
		inputList1.push(string(CT2A(m_inputFile1.GetString())));
	}
	else {
		inputList1.push(string(CT2A(m_inputFile1.GetString())));
	}

	UpdateData();
	UpdateData(FALSE);
}


void CMFCApplication4Dlg::OnEnChangeInputFile2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	UpdateData(FALSE);
}


HBRUSH CMFCApplication4Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

			//pDC.SetTextColor(RGB(0, 255, 0));
		//CString t("This is a test!");
		//dc.DrawText(t, CRect(100, 10, 400, 400), DT_SINGLELINE | DT_LEFT | DT_VCENTER); //显示文本
		
	if (pWnd->GetDlgCtrlID() == IDC_EDIT3)
	{
		//if (update)
		//{
		//	pDC->SetTextColor(RGB(0, 0, 255));
		//}
		//else
		//{
		//	//pDC->SetTextColor(RGB(255, 0, 0));
		//}
		//pDC->DrawText(t, CRect(100, 10, 400, 400), DT_SINGLELINE | DT_LEFT | DT_VCENTER);
		//pDC->DrawText(t, CRect(100, 10, 400, 400), DT_SINGLELINE | DT_LEFT | DT_VCENTER);
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->SetBkColor(RGB(0, 255, 0));
	}
	//// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CMFCApplication4Dlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	TCHAR szPath[MAX_PATH] = { 0 };
	UINT nCount = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	for (UINT idx = 0; idx < nCount; ++idx)
	{
		DragQueryFile(hDropInfo, idx, szPath, MAX_PATH);
		MessageBox(szPath);//以消息盒子形式显示路径
		SetDlgItemText(IDC_INPUT_FILE1, szPath);//在编辑框内显示路径

		UpdateData();
	}
	DragFinish(hDropInfo);
	CDialogEx::OnDropFiles(hDropInfo);
}


void CMFCApplication4Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//static CString strTime;
	//CTime tm;
	//tm = CTime::GetCurrentTime();
	//strTime = tm.Format("%Y/%m/%d %H:%M:%S");
	//SetDlgItemText(IDC_TIME, strTime);        //显示系统时
	CDialogEx::OnTimer(nIDEvent);
}
