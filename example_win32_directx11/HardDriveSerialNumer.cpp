#include "API.h"
#include <string>
#include <windows.h>


BOOL getBoisIDByCmd(char* lpszBaseBoard) {
    const long MAX_COMMAND_SIZE = 10000; // 命令行输出缓冲大小	

    WCHAR szFetCmd[] = L"wmic csproduct get UUID"; // 获取BOIS命令行	
    const string strEnSearch = "UUID"; // 主板序列号的前导信息

    BOOL   bret = FALSE;
    HANDLE hReadPipe = NULL; //读取管道
    HANDLE hWritePipe = NULL; //写入管道	
    PROCESS_INFORMATION pi; //进程信息	
    memset(&pi, 0, sizeof(pi));
    STARTUPINFO	si;	//控制命令行窗口信息
    memset(&si, 0, sizeof(si));
    SECURITY_ATTRIBUTES sa; //安全属性
    memset(&sa, 0, sizeof(sa));

    char szBuffer[MAX_COMMAND_SIZE + 1] = { 0 }; // 放置命令行结果的输出缓冲区
    string	strBuffer;
    unsigned long count = 0;
    long ipos = 0;

    pi.hProcess = NULL;
    pi.hThread = NULL;
    si.cb = sizeof(STARTUPINFO);
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    //1.创建管道
    bret = CreatePipe(&hReadPipe, &hWritePipe, &sa, 0);
    if (!bret) {
        CloseHandle(hWritePipe);
        CloseHandle(hReadPipe);

        return bret;
    }

    //2.设置命令行窗口的信息为指定的读写管道
    GetStartupInfo(&si);
    si.hStdError = hWritePipe;
    si.hStdOutput = hWritePipe;
    si.wShowWindow = SW_HIDE; //隐藏命令行窗口
    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

    //3.创建获取命令行的进程
    bret = CreateProcess(NULL, szFetCmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
    if (!bret) {
        CloseHandle(hWritePipe);
        CloseHandle(hReadPipe);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        return bret;
    }

    //4.读取返回的数据
    WaitForSingleObject(pi.hProcess, 200);
    bret = ReadFile(hReadPipe, szBuffer, MAX_COMMAND_SIZE, &count, 0);
    if (!bret) {
        CloseHandle(hWritePipe);
        CloseHandle(hReadPipe);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        return bret;
    }

    //5.查找主板ID
    bret = FALSE;
    strBuffer = szBuffer;
    ipos = strBuffer.find(strEnSearch);

    if (ipos < 0) { // 没有找到
        CloseHandle(hWritePipe);
        CloseHandle(hReadPipe);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        return bret;
    }
    else {
        strBuffer = strBuffer.substr(ipos + strEnSearch.length());
    }

    memset(szBuffer, 0x00, sizeof(szBuffer));
    strcpy_s(szBuffer, strBuffer.c_str());

    //去掉中间的空格 \r \n
    int j = 0;
    for (int i = 0; i < strlen(szBuffer); i++) {
        if (szBuffer[i] != ' ' && szBuffer[i] != '\n' && szBuffer[i] != '\r') {
            lpszBaseBoard[j] = szBuffer[i];
            j++;
        }
    }

    CloseHandle(hWritePipe);
    CloseHandle(hReadPipe);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return TRUE;
}

