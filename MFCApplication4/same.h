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

bool is_dir(const char* path) {
	struct _stat buf = { 0 };
	_stat(path, &buf);
	return buf.st_mode & _S_IFDIR;
}


using namespace std;

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
	reMsg += "--------------------------------SAME----------------------------------\r\n";
	reMsg += file1.c_str();
	reMsg += "\r\n*";
	reMsg += file2.c_str();
	reMsg += "\r\n";
	return true;
}

void isSamePath(string filePath1, string filePath2,CString& reMsg,CMFCApplication4Dlg* hwnd) {
	g_proAll = 0;
	vector<string> files1;
	vector<string> files2;

	getFiles(filePath1, files1);
	getFiles(filePath2, files2);

	for (int i = 0; i < files1.size(); i++) {
		g_proAll = (double)i / files1.size() * 100;
		if (false == isSameFile(files1[i], files2[i], reMsg)) {
			//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
			//reMsg += "*--------------------------------NOTSAME----------------------------------\r\n*";
			//reMsg += files1[i].c_str();
			//reMsg += "\r\n*";
			//reMsg +=	files2[i].c_str();
			//reMsg += "\r\n";
			reMsg += "*--------------------------------NOTSAME----------------------------------\r\n";
			//hwnd->UpdateData(FALSE);
			//int line = hwnd->m_output.GetLineCount();
			//hwnd->m_output.LineScroll(line - 1);
			//update = false;
			hwnd->SetDlgItemTextA(IDC_EDIT3, reMsg);
			int line = hwnd->m_output.GetLineCount();
			hwnd->m_output.LineScroll(line - 1);
		}
		else {
			//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
			//reMsg += "--------------------------------SAME----------------------------------\r\n";
			//reMsg += files1[i].c_str();
			//reMsg += "\r\n";
			//reMsg += files2[i].c_str();
			//reMsg += "\r\n";
			reMsg += "--------------------------------SAME----------------------------------\r\n";
			//hwnd->UpdateData(FALSE);
			//int line = hwnd->m_output.GetLineCount();
			//hwnd->m_output.LineScroll(line - 1);
			//update = true;
			hwnd->SetDlgItemTextA(IDC_EDIT3, reMsg);
			int line = hwnd->m_output.GetLineCount();
			hwnd->m_output.LineScroll(line - 1);
		}
	}
	g_proAll = 100;
}