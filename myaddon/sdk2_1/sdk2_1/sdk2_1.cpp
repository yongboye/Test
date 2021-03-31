// sdk2_1.cpp : 定义 DLL 应用程序的导出函数。
//

//#include "stdafx.h"
#include "sdk2_1.h"
#include <string.h>

// 这是导出变量的一个示例
SDK2_1_API int nsdk2_1=0;

// 这是导出函数的一个示例。
SDK2_1_API int fnsdk2_1(void)
{
    return 42;
}

// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 sdk2_1.h
Csdk2_1::Csdk2_1()
{
    return;
}

void sdk::Test(char* arr)
{
	//strcpy_s(arr, 100, "hello world");
	strcpy(arr, "hello world");
}
void sdk::Test2(char* arr, HW_CALLBACK1 cb)
{
	//strcpy_s(arr, 100, "hello world");
	strcpy(arr, "hello world");
	cb("haha");
}