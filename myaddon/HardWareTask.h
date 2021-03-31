#pragma once

#include <string>
#include <map>
#include <mutex>
using std::map;
using std::string;

#include "hardwarebase.h"
#include "V8Plugin.h"


//class DLLEXPORT HardWareTask
class HardWareTask
{
private:
	HardWareTask();
	~HardWareTask();

public:
	static HardWareTask *instance() {
		return inst;
	}

	int loadHardWare(v8::Isolate* isolate, Local<Context> context, const string &fileName); 
	void callJSCoinBtnPushHandler();
	void callJSCoinHandler(int type);
	void callJSCashHandler(int cash_in_event_code, const char *event_code_description, int money, int len);
	void callJSReportHandler(const char *status);
	void callJSMoneyRecvHandler(int type, int money, int total_money);
	void callJSBtnPushHandler(int except_money, int actual_money);
	void callJSPosHandler(int iEventValue, const char * strEventMsg);
	void callJSPollICcardHandler(void *extra, const char* blockData);
	
	std::unique_ptr<V8Plugin> v8Plugin; 

private:
	static HardWareTask *inst;
};

string DLLEXPORT GetExecutePath();

