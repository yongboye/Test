// tester.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
using namespace std;
#include "../sdk2_1/sdk2_1.h"
#ifdef WIN32
#pragma comment(lib, "../Debug/sdk2_1.lib")
#else //_WIN64
#pragma comment(lib, "../x64/Debug/sdk2_1.lib")
#endif // WIN32



void func(const char* msg) 
{
	printf("%s\n", msg);
}

int main()
{
	char str[100] = {0};
	sdk::Test(str);
	sdk::Test2(str, func);
	printf("%s\n", str);
	system("pause");
    return 0;
}

