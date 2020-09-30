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

extern int g_proAll;
extern int g_proOne;
extern bool update;

HANDLE OpenFilemap(string _szName, unsigned& _size) {

	//��ʼ
	//����ļ����
	//CString str = CString(_szName.c_str());
	//USES_CONVERSION;
	LPCSTR wszClassName = _szName.c_str();
	//str.ReleaseBuffer();
	HANDLE hFile = CreateFile(
		wszClassName,   //�ļ���
		GENERIC_READ | GENERIC_WRITE, //���ļ����ж�д����
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,  //���Ѵ����ļ�
		FILE_ATTRIBUTE_NORMAL,
		0);

	//����ֵsize_high,size_low�ֱ��ʾ�ļ���С�ĸ�32λ/��32λ
	DWORD size_low, size_high;
	_size = GetFileSize(hFile, &size_high);

	//�����ļ����ڴ�ӳ���ļ���   
	HANDLE hMapFile = CreateFileMapping(
		hFile,
		NULL,
		PAGE_READWRITE,  //��ӳ���ļ����ж�д
		size_high,
		_size,   //������������64λ������֧�ֵ�����ļ�����Ϊ16EB
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
	//���ļ�����ӳ�䵽���̵ĵ�ַ�ռ�
	void* pvFile = MapViewOfFile(
		_hFile,
		FILE_MAP_READ | FILE_MAP_WRITE,
		0,
		0,
		_size);
	return pvFile;
}

void unmapFile(HANDLE _hFile) {
	//���ļ�����ӳ�䵽���̵ĵ�ַ�ռ�
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
	if (!is_dir(path.c_str())) {//������ļ�·��
		files.push_back(path);
		return;
	}
	intptr_t   hFile = 0;//�ļ�������������������
	struct _finddata_t fileinfo;//�ļ���Ϣ
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
		//������ҵ���һ���ļ�
	{
		do
		{
			if ((fileinfo.attrib &  _A_SUBDIR))//������ļ���
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			else//������ļ�
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);	//��Ѱ�ҵ������ļ�

		_findclose(hFile);	//�������ң��رվ��
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

bool isSameFileLine(string file1, string file2, CString& reMsg, CMFCApplication4Dlg* hwnd) {
	g_proOne = 0;
	std::ifstream OsRead1(file1, std::ofstream::app);
	std::ifstream OsRead2(file2, std::ofstream::app);
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
			hwnd->UpdateData(FALSE);
			ret = false;
			if(errorTypenow)
				return ret;
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
					reMsg += "*--------------------------------NOTFINDFILE----------------------------------\r\n";
					reMsg += files1[i].c_str();
					reMsg += "\r\n*";
					reMsg += "*--------------------------------NOTFINDFILE----------------------------------\r\n";
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
			else {
				if (false == isSameFile(file1, file2, reMsg)) {
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
		}
	}
	g_proAll = 100;
}