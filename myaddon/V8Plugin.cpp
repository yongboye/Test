#include "V8Plugin.h"
#include "V8Base.h"


V8Plugin::V8Plugin()
{
}


V8Plugin::~V8Plugin()
{
}

int V8Plugin::load(const string &fileName)
{
	return -1;
	//return v8Base->load(fileName);
}

//js function:void CoinBtnPushHandler()
void V8Plugin::callJSCoinBtnPushHandler()
{
	return;
}
//js function:void CoinHandler(int type)
void V8Plugin::callJSCoinHandler(int type)
{
	return;
}
//js function:void CashHandler(int cash_in_event_code, const char *event_code_description, int money, int len)
void V8Plugin::callJSCashHandler(int cash_in_event_code, const char *event_code_description, int money, int len)
{
	return;
}
//js function:void ReportHandler(const char *status)
void V8Plugin::callJSReportHandler(const char *status)
{
	return;
}
//js function:void MoneyRecvHandler(int type, int money, int total_money)
void V8Plugin::callJSMoneyRecvHandler(int type, int money, int total_money)
{
	return;
}
//js function:void BtnPushHandler(int except_money, int actual_money)
void V8Plugin::callJSBtnPushHandler(int except_money, int actual_money)
{
	return;
}
//js function:void CoinBtnPushHandler(int iEventValue, const char * strEventMsg);
void V8Plugin::callJSPosHandler(int iEventValue, const char * strEventMsg)
{
	return;
}
//js function:void PollICcardHandler(const char* blockData)
void V8Plugin::callJSPollICcardHandler(void *extra, const char* blockData)
{
	return;
}