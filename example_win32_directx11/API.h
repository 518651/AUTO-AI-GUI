#pragma once
//#include <iostream>
#include <string>
#include <winsock.h>
#include "include/mysql.h"
#pragma comment(lib,"libmysql.lib")


using namespace std;

BOOL getBoisIDByCmd(char* lpszBaseBoard); //获取BIOS号
int Connectmysql();//连接数据库
