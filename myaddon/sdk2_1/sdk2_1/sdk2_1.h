// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 SDK2_1_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// SDK2_1_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef _WINDOWS
#ifdef SDK2_1_EXPORTS
#define SDK2_1_API __declspec(dllexport)
#else
#define SDK2_1_API __declspec(dllimport)
#endif
#else
#define SDK2_1_API
#endif

// 此类是从 sdk2_1.dll 导出的
class SDK2_1_API Csdk2_1 {
public:
	Csdk2_1(void);
	// TODO:  在此添加您的方法。
};

typedef void (*HW_CALLBACK1)(const char * strEventMsg);
class sdk {
public:
	static SDK2_1_API void Test(char* arr);
	static SDK2_1_API void Test2(char* arr, HW_CALLBACK1 cb);
};

extern SDK2_1_API int nsdk2_1;

SDK2_1_API int fnsdk2_1(void);
