#include "HardWareTask.h"
#include <memory>
#include "HardWare.h"

HardWareTask *HardWareTask::inst = new HardWareTask();

HardWareTask::HardWareTask()
{
}

HardWareTask::~HardWareTask()
{
}

int HardWareTask::loadHardWare(v8::Isolate* isolate, Local<Context> context, const string &fileName)
{
	v8Plugin.reset(new Hardware(isolate, context)); 
	if (v8Plugin->load(fileName) != 0) {
		//¼ÓÔØjsÎÄ¼þÊ§°Ü
		v8Plugin.reset();
		return 0;
	}
	return 1;
}

void HardWareTask::callJSCoinBtnPushHandler()
{
	v8Plugin->callJSCoinBtnPushHandler();
}
void HardWareTask::callJSCoinHandler(int type)
{
	v8Plugin->callJSCoinHandler(type);
}
void HardWareTask::callJSCashHandler(int cash_in_event_code, const char *event_code_description, int money, int len)
{
	v8Plugin->callJSCashHandler(cash_in_event_code, event_code_description, money, len);
}
void HardWareTask::callJSReportHandler(const char *status)
{
	v8Plugin->callJSReportHandler(status);
}
void HardWareTask::callJSMoneyRecvHandler(int type, int money, int total_money)
{
	v8Plugin->callJSMoneyRecvHandler(type, money, total_money);
}
void HardWareTask::callJSBtnPushHandler(int except_money, int actual_money)
{
	v8Plugin->callJSBtnPushHandler(except_money, actual_money);
}
void HardWareTask::callJSPosHandler(int iEventValue, const char * strEventMsg)
{
	v8Plugin->callJSPosHandler(iEventValue, strEventMsg);
}
void HardWareTask::callJSPollICcardHandler(void *extra, const char* blockData)
{
	v8Plugin->callJSPollICcardHandler(extra, blockData);
}











//////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
#include <Windows.h>
#include <boost/filesystem.hpp>
#endif
string GetExecutePath()
{
#ifdef _WIN32
	char fileName[512];
	::GetModuleFileName(NULL, fileName, 500);
	boost::filesystem::path ret = fileName;
	return ret.parent_path().parent_path().string();
#else
	return "..";
#endif
}