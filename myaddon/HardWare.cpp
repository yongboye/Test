#include "Hardware.h"
#include "V8Base.h"


Hardware::Hardware(v8::Isolate* isolate, Local<Context> context)
{
	isolate = isolate; //
	v8Base.reset(new V8Base(isolate, context, 0, 0)); //
}

Hardware::~Hardware()
{
}

int Hardware::load(const string &fileName)
{
	return v8Base->load(fileName);
}

//js function:void CoinBtnPushHandler()
void Hardware::callJSCoinBtnPushHandler()
{
	char script[1024]; 
	sprintf(script, "CoinBtnPushHandler();");
	//LogInfo(lotCode, "Hardware::CoinBtnPushHandler,javascript:" << script);
	auto isolate = v8Base->getIsolate();
	v8::HandleScope handleScope(isolate);
	v8Base->execute(script);
	return;
}
//js function:void CoinHandler(int type)
void Hardware::callJSCoinHandler(int type)
{
	char script[1024]; 
	sprintf(script, "CoinHandler(%d);", type);
	//LogInfo(lotCode, "Hardware::CoinHandler,javascript:" << script);
	auto isolate = v8Base->getIsolate();
	v8::HandleScope handleScope(isolate);
	v8Base->execute(script);
	return;
}
//js function:void CashHandler(int cash_in_event_code, const char *event_code_description, int money, int len)
void Hardware::callJSCashHandler(int cash_in_event_code, const char *event_code_description, int money, int len)
{
	char script[1024]; 
	sprintf(script, "CashHandler(%d,%s,%d,%d);", cash_in_event_code, event_code_description, money, len);
	//LogInfo(lotCode, "Hardware::CashHandler,javascript:" << script);
	auto isolate = v8Base->getIsolate();
	v8::HandleScope handleScope(isolate);
	v8Base->execute(script);
	return;
}
//js function:void ReportHandler(const char *status)
void Hardware::callJSReportHandler(const char *status)
{
	char script[1024]; 
	sprintf(script, "ReportHandler(%s);", status);
	//LogInfo(lotCode, "Hardware::ReportHandler,javascript:" << script);
	auto isolate = v8Base->getIsolate();
	v8::HandleScope handleScope(isolate);
	v8Base->execute(script);
	return;
}
//js function:void MoneyRecvHandler(int type, int money, int total_money)
void Hardware::callJSMoneyRecvHandler(int type, int money, int total_money)
{
	char script[1024]; 
	sprintf(script, "MoneyRecvHandler(%d,%d,%d);", type, money, total_money);
	//LogInfo(lotCode, "Hardware::MoneyRecvHandler,javascript:" << script);
	auto isolate = v8Base->getIsolate();
	v8::HandleScope handleScope(isolate);
	v8Base->execute(script);
	return;
}
//js function:void BtnPushHandler(int except_money, int actual_money)
void Hardware::callJSBtnPushHandler(int except_money, int actual_money)
{
	char script[1024]; 
	sprintf(script, "BtnPushHandler(%d,%d);", except_money, actual_money);
	//LogInfo(lotCode, "Hardware::BtnPushHandler,javascript:" << script);
	auto isolate = v8Base->getIsolate();
	v8::HandleScope handleScope(isolate);
	v8Base->execute(script);
	return;
}
//js function:void CoinBtnPushHandler(int iEventValue, const char * strEventMsg);
void Hardware::callJSPosHandler(int iEventValue, const char * strEventMsg)
{
#if 0
	char script[1024]; 
	sprintf(script, "CoinBtnPushHandler(%d,%s);", iEventValue, strEventMsg);
	//LogInfo(lotCode, "Hardware::callJSPosHandle,javascript:" << script);
	auto isolate = v8Base->getIsolate();
	v8::HandleScope handleScope(isolate);
	v8Base->execute(script);
#else
	int argc = 1;
	Local<Value> argv[1];
	//threadData->callback->Call(Context::GetCurrent()->Global(), 1, argv);
	//threadData->callback->Call(isolate->GetCurrentContext(), Null(isolate), argc, argv);
#endif
	return;
	
}
//js function:void PollICcardHandler(const char* blockData)
void Hardware::callJSPollICcardHandler(void *extra, const char* blockData)
{
#if 0
	char script[1024]; 
	sprintf(script, "PollICcardHandler(%s);", blockData);
	//LogInfo(lotCode, "Hardware::PollICcardHandler,javascript:" << script);
	auto isolate = v8Base->getIsolate();
	v8::HandleScope handleScope(isolate);
	v8Base->execute(script);
#else
	//ThreadData* data = (ThreadData *) extra;
	//delete data->progress;
	/*ThreadData* data = threadData;
    uv_async_send(data->callbackReq);*/
#endif
	return;
}

/*string Hardware::getFee(int carStyle, int feeType, int beginTime, int timeLong, const string &billId)
{
	char script[1024];
	sprintf(script, "getFee(%d,%d,%d,%d,'%s');", carStyle, feeType, beginTime, timeLong, billId.c_str());
	LogInfo(lotCode, "CouponTicket::getFee,javascript:" << script);
	auto isolate = v8Base->getIsolate();
	v8::HandleScope handleScope(isolate);
	auto result = v8Base->execute(script); //c++ -> js function
	v8::String::Utf8Value utf8(isolate, result);
	LogInfo(lotCode, "CouponTicket::getFee,javascript result:" << *utf8);
	return *utf8;
}*/