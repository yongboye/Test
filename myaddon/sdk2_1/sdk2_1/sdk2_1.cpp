// sdk2_1.cpp : ���� DLL Ӧ�ó���ĵ���������
//

//#include "stdafx.h"
#include "sdk2_1.h"
#include <string.h>

// ���ǵ���������һ��ʾ��
SDK2_1_API int nsdk2_1=0;

// ���ǵ���������һ��ʾ����
SDK2_1_API int fnsdk2_1(void)
{
    return 42;
}

// �����ѵ�����Ĺ��캯����
// �й��ඨ�����Ϣ������� sdk2_1.h
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