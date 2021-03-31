#pragma once
#include "hardwarebase.h" //DLLEXPORT
#include <v8.h>
using namespace v8;

class DLLEXPORT HardWareApp
{
public:
	HardWareApp();
	~HardWareApp();

	static bool start(v8::Isolate* isolate, Local<Context> context, const char *program, void *log, const char* callbackJSFile);
	
	static void callback_CoinBtnPushHandler();
	static void callback_CoinHandler(int type);
	static void callback_CashHandler(int cash_in_event_code, const char *event_code_description, int money, int len);
	static void callback_ReportHandler(const char *status); //vmc
	static void callback_MoneyRecvHandler(int type, int money, int total_money); //vmc
	static void callback_BtnPushHandler(int except_money, int actual_money); //vmc
	static void callback_PosHandler(int iEventValue, const char * strEventMsg);
	static void callback_PollICcardHandler(void *extra, const char* blockData);
};

