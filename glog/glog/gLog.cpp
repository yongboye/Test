#include "gLog.h"


CGLog * CGLog::m_pInstance = NULL;


CGLog::CGLog(void)
{
	memset(pLogBuff, 0, 2048);
}

CGLog::~CGLog(void)
{
	//  here, cannot free the object.
	/*if (NULL != m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}*/
}

int32_t CGLog::InitGLog(const char * szLogFile, const char * argv0, int32_t nLogSeverityToPrint, int32_t nLogSeverityToFile)
{
	FLAGS_logtostderr = 0;  //是否打印到控制台
	FLAGS_alsologtostderr = 0;  //打印到日志同时是否打印到控制台 
	//FLAGS_log_dir = szLogFile; //等级文件
	google::InitGoogleLogging(argv0);
	string logfile = szLogFile;
	logfile += "/hardware_"; //./logs/hardware_
	google::SetLogDestination(google::GLOG_INFO, logfile.c_str()); //日志的输出目录和前缀
	//google::rotate
	//google::ShutdownGoogleLogging();
	FLAGS_logbufsecs = 0; //日志实时输出 
	FLAGS_max_log_size = 100; //最大日志文件大小 100M

	return 0;
}

void CGLog::GLogMsg(uint32_t nLogSeverity, const char *format, ...)
{
	va_list arg_ptr;
	va_start(arg_ptr, format);
	vsprintf(pLogBuff, format, arg_ptr);
	switch(nLogSeverity)
	{
	case 0:
		//DLOG(INFO) << pLogBuff;
		LOG(INFO) << pLogBuff;
		break;
	case 1:
		//DLOG(WARNING) << pLogBuff;
		LOG(WARNING) << pLogBuff;
		break;
	case 2:
		//DLOG(ERROR) << pLogBuff;
		LOG(ERROR) << pLogBuff;
		break;
	case 3:
		//DLOG(FATAL) << pLogBuff;
		LOG(FATAL) << pLogBuff;
		break;
	default:
		break;
	}		
	va_end(arg_ptr);

	memset(pLogBuff, 0, 2048);		
}