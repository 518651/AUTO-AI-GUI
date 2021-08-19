#include "API.h"
#include <string>
#include <windows.h>


BOOL getBoisIDByCmd(char* lpszBaseBoard) {
    const long MAX_COMMAND_SIZE = 10000; // ��������������С	

    WCHAR szFetCmd[] = L"wmic csproduct get UUID"; // ��ȡBOIS������	
    const string strEnSearch = "UUID"; // �������кŵ�ǰ����Ϣ

    BOOL   bret = FALSE;
    HANDLE hReadPipe = NULL; //��ȡ�ܵ�
    HANDLE hWritePipe = NULL; //д��ܵ�	
    PROCESS_INFORMATION pi; //������Ϣ	
    memset(&pi, 0, sizeof(pi));
    STARTUPINFO	si;	//���������д�����Ϣ
    memset(&si, 0, sizeof(si));
    SECURITY_ATTRIBUTES sa; //��ȫ����
    memset(&sa, 0, sizeof(sa));

    char szBuffer[MAX_COMMAND_SIZE + 1] = { 0 }; // ���������н�������������
    string	strBuffer;
    unsigned long count = 0;
    long ipos = 0;

    pi.hProcess = NULL;
    pi.hThread = NULL;
    si.cb = sizeof(STARTUPINFO);
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    //1.�����ܵ�
    bret = CreatePipe(&hReadPipe, &hWritePipe, &sa, 0);
    if (!bret) {
        CloseHandle(hWritePipe);
        CloseHandle(hReadPipe);

        return bret;
    }

    //2.���������д��ڵ���ϢΪָ���Ķ�д�ܵ�
    GetStartupInfo(&si);
    si.hStdError = hWritePipe;
    si.hStdOutput = hWritePipe;
    si.wShowWindow = SW_HIDE; //���������д���
    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

    //3.������ȡ�����еĽ���
    bret = CreateProcess(NULL, szFetCmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
    if (!bret) {
        CloseHandle(hWritePipe);
        CloseHandle(hReadPipe);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        return bret;
    }

    //4.��ȡ���ص�����
    WaitForSingleObject(pi.hProcess, 200);
    bret = ReadFile(hReadPipe, szBuffer, MAX_COMMAND_SIZE, &count, 0);
    if (!bret) {
        CloseHandle(hWritePipe);
        CloseHandle(hReadPipe);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        return bret;
    }

    //5.��������ID
    bret = FALSE;
    strBuffer = szBuffer;
    ipos = strBuffer.find(strEnSearch);

    if (ipos < 0) { // û���ҵ�
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

    //ȥ���м�Ŀո� \r \n
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

