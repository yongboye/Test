#pragma once
#include "V8Plugin.h"
#include <map>
#include <string>
using std::string;

class Hardware :public V8Plugin
{
public:
	Hardware(v8::Isolate* isolate, Local<Context> context);
	~Hardware();
			
	int load(const string &fileName);
/*protected:	
	std::unique_ptr<V8Base> v8Base; //体现出v8插件
	v8::Isolate* isolate;*/
public:
	void callJSCoinBtnPushHandler();
	void callJSCoinHandler(int type);
	void callJSCashHandler(int cash_in_event_code, const char *event_code_description, int money, int len);
	void callJSReportHandler(const char *status);
	void callJSMoneyRecvHandler(int type, int money, int total_money);
	void callJSBtnPushHandler(int except_money, int actual_money);
	void callJSPosHandler(int iEventValue, const char * strEventMsg);
	void callJSPollICcardHandler(void *extra, const char* blockData);
};

