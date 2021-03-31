
#include <stdarg.h>
#include <stdint.h>
#include <string>
using namespace std;

 
#include <glog/logging.h>
#include <glog/log_severity.h> 
//libglog_static.lib
#if defined(_WINDOWS)
#pragma comment(lib, "glogd.lib")
#endif
 
class CGLog
{
public:
	CGLog(void);
	~CGLog(void);
 
	int32_t InitGLog(const char * szLogFile, const char * argv0, int32_t nLogSeverityToPrint = google::GLOG_INFO, int32_t nLogSeverityToFile = google::GLOG_INFO);
	void GLogMsg(uint32_t nLogSeverity, const char *format, ...);
 
public:
	static CGLog * Instance()
	{
		if (NULL == m_pInstance)
		{
			m_pInstance = new CGLog();
		}
		return m_pInstance;
	}
 
private:
	static CGLog * m_pInstance;
	char pLogBuff[2048];
};
 
 
#define LOGINIT(szLogFile, ...)  CGLog::Instance()->InitGLog(szLogFile, __VA_ARGS__);
#define LOGINFO(...)  CGLog::Instance()->GLogMsg(0, __VA_ARGS__);
#define LOGWARN(...)  CGLog::Instance()->GLogMsg(1, __VA_ARGS__);
#define LOGERROR(...) CGLog::Instance()->GLogMsg(2, __VA_ARGS__);
#define LOGFATAL(...) CGLog::Instance()->GLogMsg(3, __VA_ARGS__);



/*
int _tmain(int argc, _TCHAR* argv[])
{
 	LOGINIT(argv[0]);
 	LOGINFO("First Number: %d.", 1);
	LOGERROR("Second Number: %d.", 2);
	return 0;
}
*/



