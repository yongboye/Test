// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� SDK2_1_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// SDK2_1_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef _WINDOWS
#ifdef SDK2_1_EXPORTS
#define SDK2_1_API __declspec(dllexport)
#else
#define SDK2_1_API __declspec(dllimport)
#endif
#else
#define SDK2_1_API
#endif

// �����Ǵ� sdk2_1.dll ������
class SDK2_1_API Csdk2_1 {
public:
	Csdk2_1(void);
	// TODO:  �ڴ�������ķ�����
};

typedef void (*HW_CALLBACK1)(const char * strEventMsg);
class sdk {
public:
	static SDK2_1_API void Test(char* arr);
	static SDK2_1_API void Test2(char* arr, HW_CALLBACK1 cb);
};

extern SDK2_1_API int nsdk2_1;

SDK2_1_API int fnsdk2_1(void);
