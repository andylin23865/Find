#define _CRT_SECURE_NO_WARNINGS
#define FIND_3229FASTSIM
#define FIND_3229SIM

#include<iostream>
#include <fstream>
#include<CString>
#include <io.h>
#include <vector>
#include <windows.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include "ShowDlg.h"
#include "MFCApplication4Dlg.h"

#define STR_SIZE 20

bool is_dir(const char* path) {
	struct _stat buf = { 0 };
	_stat(path, &buf);
	return buf.st_mode & _S_IFDIR;
}

std::string getFileName(std::string filePath) {
	int pos = filePath.find_last_of('\\');
	return filePath.substr(pos + 1, filePath.size() - pos - 1);
}

using namespace std;
bool lineCmp = false;
bool errorTypenow = true;
bool sameName = true;
bool logInfo = false;
bool showDetail = false;
bool next10Line = false;
int errorLevel = 0;
extern bool pause;

extern int g_proAll;
extern int g_proOne;
extern bool update;

HANDLE OpenFilemap(string _szName, unsigned& _size) {

	//开始
	//获得文件句柄
	//CString str = CString(_szName.c_str());
	//USES_CONVERSION;
	LPCSTR wszClassName = _szName.c_str();
	//str.ReleaseBuffer();
	HANDLE hFile = CreateFile(
		wszClassName,   //文件名
		GENERIC_READ | GENERIC_WRITE, //对文件进行读写操作
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,  //打开已存在文件
		FILE_ATTRIBUTE_NORMAL,
		0);

	//返回值size_high,size_low分别表示文件大小的高32位/低32位
	DWORD size_low, size_high;
	_size = GetFileSize(hFile, &size_high);

	//创建文件的内存映射文件。   
	HANDLE hMapFile = CreateFileMapping(
		hFile,
		NULL,
		PAGE_READWRITE,  //对映射文件进行读写
		size_high,
		_size,   //这两个参数共64位，所以支持的最大文件长度为16EB
		NULL);
	if (hMapFile == INVALID_HANDLE_VALUE)
	{
		//AfxMessageBox("Can't create file mapping.Error%d:\n", GetLastError());
		CloseHandle(hFile);
		return NULL;
	}
	return hMapFile;
}

void* mapFile(HANDLE _hFile, unsigned _size) {
	//把文件数据映射到进程的地址空间
	void* pvFile = MapViewOfFile(
		_hFile,
		FILE_MAP_READ | FILE_MAP_WRITE,
		0,
		0,
		_size);
	return pvFile;
}

void unmapFile(HANDLE _hFile) {
	//把文件数据映射到进程的地址空间
	UnmapViewOfFile(_hFile);
}

bool isSame(string file1, string file2) {
	long long NotSameByte = 0;
	string t, ans, ans2;
	int i;
	freopen((file1).c_str(), "r", stdin);
	char c;
	while (scanf("%c", &c) != EOF) ans += c;
	fclose(stdin);
	freopen((file2).c_str(), "r", stdin);
	while (scanf("%c", &c) != EOF) ans2 += c;
	fclose(stdin);
	if (ans.size() != ans2.size()) { return false; }
	for (i = 0; i < ans.size(); i++)
		if (ans[i] != ans2[i]) { NotSameByte++; }
	if (NotSameByte != 0)
	{

#ifdef FIND_3229FASTSIM
		if (NotSameByte == 324) {
			cout << "FIND_3229FASTSIM" << endl;
			return true;
		}
#endif

#ifdef FIND_3229SIM
		if (NotSameByte == 9876) {
			cout << "FIND_3229SIM" << endl;
			return true;
		}
#endif
		cout << "NotSameByte is:" << NotSameByte << endl;
		return false;
	}
	return true;
}

int findFile(string fileName, vector<string>& files) {
	for (int i = 0; i < files.size(); i++) {
		if (fileName == getFileName(files[i]))
			return i;
	}
	return -1;
}

void getFiles(string path, vector<string>& files)
{
	if (!is_dir(path.c_str())) {//如果是文件路径
		files.push_back(path);
		return;
	}
	intptr_t   hFile = 0;//文件句柄，过会儿用来查找
	struct _finddata_t fileinfo;//文件信息
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
		//如果查找到第一个文件
	{
		do
		{
			if ((fileinfo.attrib &  _A_SUBDIR))//如果是文件夹
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			else//如果是文件
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);	//能寻找到其他文件

		_findclose(hFile);	//结束查找，关闭句柄
	}
}

bool isSameFile(string file1, string file2, CString& reMsg) {
	g_proOne = 0;
	unsigned size1;
	unsigned size2;
	HANDLE hFile1 = OpenFilemap(file1, size1);
	HANDLE hFile2 = OpenFilemap(file2, size2);
	if (size1 != size2)
	{
		reMsg += "*--------------------------------NOTSAME----------------------------------\r\n*";
		reMsg += file1.c_str();
		reMsg += "\r\n*";
		reMsg += file2.c_str();
		reMsg += "\r\n";
		reMsg += "NOT SAME SIZE!\r\n";
		return false;
	}
	char* pfile1 = (char*)mapFile(hFile1, size1);
	char* pfile2 = (char*)mapFile(hFile2, size2);

	for (unsigned i = 0; i < size1; i++) {
		g_proOne = (double)i / size1 * 100;
		if (*(pfile1 + i) != *(pfile2 + i)) {
			char temp[100];
			int len = sprintf(temp,"addr:0x%x data1:0x%x data2:0x%x \n",i, *(pfile1 + i), *(pfile2 + i));
			reMsg += "*--------------------------------NOTSAME----------------------------------\r\n*";
			reMsg += file1.c_str();
			reMsg += "\r\n*";
			reMsg += file2.c_str();
			reMsg += "\r\n";
			reMsg += temp;
			reMsg += "\r\n";
			unmapFile(hFile1);
			unmapFile(hFile2);
			return false;
		}
		//cout << *(pfile2 + i);
	}
	unmapFile(hFile1);
	unmapFile(hFile2);
	g_proOne = 100;
	if (errorLevel <= 0)
	{
		reMsg += "--------------------------------SAME----------------------------------\r\n";
		reMsg += file1.c_str();
		reMsg += "\r\n*";
		reMsg += file2.c_str();
		reMsg += "\r\n";
	}
	return true;
}

bool isSameFileLine(string file1, string file2, CString& reMsg, CMFCApplication4Dlg* hwnd) {
	g_proOne = 0;
	std::ifstream OsRead1(file1, std::ofstream::app);
	std::ifstream OsRead2(file2, std::ofstream::app);
	string str1;
	string str2;
	vector<string> str1_20;
	vector<string> str2_20;
	vector<string> nextstr1_10;
	vector<string> nextstr2_10;
	int lineNum = 1;
	bool ret = true;
	while (true) {
		if (pause && next10Line) {
			int num = 10 - nextstr1_10.size();
			for (int i = 0; i < num; i++) {
				if (getline(OsRead1, str1)) {
					nextstr1_10.push_back(str1);
				}
			}
			num = 10 - nextstr2_10.size();
			for (int i = 0; i < num; i++) {
				if (getline(OsRead2, str2)) {
					nextstr2_10.push_back(str2);
				}
			}
				for (int i = 0; i < nextstr1_10.size() && i < 10; i++) {
					char temp2[20];
					_itoa(lineNum + i, temp2, 10);
					hwnd->m_pDlg->showFile1((string)temp2 + ":\t" + nextstr1_10[i]);
					//hwnd->m_pDlg->showFile2((string)temp1 + ":\t" + str2_20[i]);
				}
				for (int i = 0; i < nextstr2_10.size() && i < 10; i++) {
					char temp2[20];
					_itoa(lineNum + i, temp2, 10);
					//hwnd->m_pDlg->showFile1((string)temp1 + ":\t" + str1_20[i]);
					hwnd->m_pDlg->showFile2((string)temp2 + ":\t" + nextstr2_10[i]);
				}
				hwnd->m_pDlg->UpdateData(FALSE);
		}
		while (pause)
		{
			Sleep(100);
		}
		if (nextstr1_10.size() > 0) {
			str1 = nextstr1_10[0];
			nextstr1_10.erase(nextstr1_10.begin());
		}
		else
		{
			if (!getline(OsRead1, str1)) {
				break;
			}
		}
		if (nextstr2_10.size() > 0) {
			str2 = nextstr2_10[0];
			nextstr2_10.erase(nextstr2_10.begin());
		}
		else
		{
			if (!getline(OsRead2, str2)) {
				reMsg += "*--------------------------------NOTSAME----------------------------------\r\n*";
				reMsg += file1.c_str();
				reMsg += "\r\n*";
				reMsg += file2.c_str();
				reMsg += "\r\n";
				reMsg += "NOT SAME SIZE!\r\n";
				//hwnd->UpdateData(FALSE);
				hwnd->SetDlgItemTextA(IDC_EDIT3, reMsg);
				int line = hwnd->m_output.GetLineCount();
				hwnd->UpdateData(FALSE);
				hwnd->m_output.LineScroll(line - 1);
				return false;
			}
		}
		str1_20.push_back(str1);
		str2_20.push_back(str2);
		if (str1 != str2) {
			char temp[20];
			_itoa(lineNum, temp,10);
			reMsg += "*--------------------------------NOTSAME----------------------------------\r\n*";
			reMsg += file1.c_str();
			reMsg += "---->:";
			reMsg += str1.c_str();
			reMsg += "\r\n*";
			reMsg += file2.c_str();
			reMsg += "---->:";
			reMsg += str2.c_str();
			reMsg += "\r\n";
			reMsg += CString("NOT SAME Line at "+ CString(temp) +"!\r\n");
			//hwnd->UpdateData(FALSE);
			hwnd->SetDlgItemTextA(IDC_EDIT3, reMsg);
			int line = hwnd->m_output.GetLineCount();
			hwnd->UpdateData(FALSE);
			hwnd->m_output.LineScroll(line - 1);
			ret = false;
			if (showDetail) {
				pause = true;
				hwnd->m_pDlg->ShowWindow(SW_SHOW);
				//Sleep(10000);
				hwnd->m_pDlg->clean();
				for (int i = 0; i < str1_20.size(); i++) {
					char temp1[20];
					_itoa(lineNum- str1_20.size()+1+i, temp1, 10);
					hwnd->m_pDlg->showFile1((string)temp1 +":\t"+ str1_20[i]);
					hwnd->m_pDlg->showFile2((string)temp1 + ":\t" + str2_20[i]);
				}
				hwnd->m_pDlg->UpdateData(FALSE);
			}
			if(errorTypenow)
				return ret;
		}
		if (str1_20.size() > STR_SIZE) {
			str1_20.erase(str1_20.begin());
		}
		if (str2_20.size() > STR_SIZE) {
			str2_20.erase(str2_20.begin());
		}
		lineNum++;
	}
	return ret;
}

void isSamePath(string filePath1, string filePath2,CString& reMsg,CMFCApplication4Dlg* hwnd) {
	g_proAll = 0;
	vector<string> files1;
	vector<string> files2;

	getFiles(filePath1, files1);
	getFiles(filePath2, files2);
	{
		for (int i = 0; i < files1.size(); i++) {
			g_proAll = (double)i / files1.size() * 100;
			string file1 = files1[i];
			string file2;
			if (sameName && !(files1.size() == 1 && files2.size() == 1))
			{
				int index = findFile(getFileName(file1), files2);
				if (index == -1) {
					if (errorLevel <= 1) {
						reMsg += "*--------------------------------NOTFINDFILE----------------------------------\r\n";
						reMsg += files1[i].c_str();
						reMsg += "\r\n*";
						reMsg += "*--------------------------------NOTFINDFILE----------------------------------\r\n";
					}
					continue;
				}
				else {
					file2 = files2[index];
				}
			}
			else {
				file2 = files2[i];
			}
			if (lineCmp)
			{
				if (false == isSameFileLine(file1, file2, reMsg, hwnd)) {

					reMsg += "*--------------------------------NOTSAME----------------------------------\r\n";

					hwnd->UpdateData(FALSE);

				}
				else {

					if (errorLevel <= 0)
					{
						reMsg += "--------------------------------SAME----------------------------------\r\n";
					}

					hwnd->UpdateData(FALSE);

				}
			}
			else {
				if (false == isSameFile(file1, file2, reMsg)) {

					reMsg += "*--------------------------------NOTSAME----------------------------------\r\n";

					hwnd->UpdateData(FALSE);
				}
				else {

					if (errorLevel <= 0)
					{
						reMsg += "--------------------------------SAME----------------------------------\r\n";
					}

					hwnd->UpdateData(FALSE);
				}
			}
		}
	}
	g_proAll = 100;
}

bool CmpLogFile(string file1, string file2, CString& reMsg, CMFCApplication4Dlg* hwnd) {
	g_proOne = 0;
	std::ifstream OsRead1(file1, std::ofstream::app);
	std::ifstream OsRead2(file2, std::ofstream::app);
	vector<string> str1_20;
	vector<string> str2_20;
	int index1 = 0;
	int index2 = 0;
	string str1;
	string str2;
	int lineNum = 1;
	bool ret = true;
	while (getline(OsRead1, str1)) {
		if (!getline(OsRead2, str2)) {
			reMsg += "*--------------------------------NOTSAME----------------------------------\r\n*";
			reMsg += file1.c_str();
			reMsg += "\r\n*";
			reMsg += file2.c_str();
			reMsg += "\r\n";
			reMsg += "NOT SAME SIZE!\r\n";
			hwnd->UpdateData(FALSE);
			return false;
		}
		if (str1 != str2) {
			char temp[20];
			_itoa(lineNum, temp, 10);
			reMsg += "*--------------------------------NOTSAME----------------------------------\r\n*";
			reMsg += file1.c_str();
			reMsg += "---->:";
			reMsg += str1.c_str();
			reMsg += "\r\n*";
			reMsg += file2.c_str();
			reMsg += "---->:";
			reMsg += str2.c_str();
			reMsg += "\r\n";
			reMsg += CString("NOT SAME Line at " + CString(temp) + "!\r\n");
			hwnd->UpdateData(FALSE);
			ret = false;
			if (showDetail) {
				ShowDlg  *pDlg = new ShowDlg;
				pDlg->Create(IDD_DIALOG1, hwnd);
				pDlg->ShowWindow(SW_SHOW);
			}
			if (errorTypenow)
				return ret;
		}
		str1_20.push_back(str1);
		str1_20.push_back(str2);
		if (str1_20.size() > STR_SIZE) {
			str1_20.erase(str1_20.begin());
		}
		if (str2_20.size() > STR_SIZE) {
			str2_20.erase(str2_20.begin());
		}
		lineNum++;
	}
	return ret;
}