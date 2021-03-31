#pragma once
#include <memory>
#include <string>
using std::string;
#include "sdkaddon.h"

class V8Base;

class V8Plugin
{
public:
	V8Plugin();
	virtual ~V8Plugin();

	int load(const string &fileName);

protected:
	std::unique_ptr<V8Base> v8Base; //体现出v8插件
	v8::Isolate* isolate;
	
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

