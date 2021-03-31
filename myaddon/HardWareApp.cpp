#include "HardWareApp.h"
#include "HardWareTask.h"
#include <iostream>
using namespace std;

/*#include "../nsocketlib/Logger.h"
#include "utils/LotConfig.h"
#include "core/LotManager.h"
#include "redis/RedisHandler.h"*/

void v8init(const char *program);

HardWareApp::HardWareApp()
{
}


HardWareApp::~HardWareApp()
{
}

bool HardWareApp::start(v8::Isolate* isolate, Local<Context> context, const char *program, void *log, const char* callbackJSFile)
{
	//Logger::setLog(log);	//��̬��ģ���ʼ��log
	//v8init(program);	//
	//LotConfig::instance()->loadConfig();
	//if (LotManager::start() == false)
	//	return false;
	//RedisCache::start();
	
	
	HardWareTask::instance()->loadHardWare(isolate, context, callbackJSFile);
	
	
	return true;
}

void HardWareApp::callback_CoinBtnPushHandler()
{
	char buf[4096] = { 0 };
	sprintf(buf, "%s", "�˱Ұ����ص�");
	cout << buf << endl;
	
	//call js function
	HardWareTask::instance()->callJSCoinBtnPushHandler();
}
void HardWareApp::callback_CoinHandler(int type)
{
	char buf[4096] = { 0 };
	sprintf(buf, "Ӳ�ҽ��ջص�[%d]", type);
	cout << buf << endl;
	
	//call js function
	HardWareTask::instance()->callJSCoinHandler(type);
}
void HardWareApp::callback_CashHandler(int cash_in_event_code, const char *event_code_description, int money, int len)
{
	char buf[4096] = { 0 };
	sprintf(buf, "ֽ�ҽ��ջص�[%d][%s][%d][%d]", cash_in_event_code, event_code_description, money, len);
	cout << buf << endl;
	
	//call js function
	HardWareTask::instance()->callJSCashHandler(cash_in_event_code, event_code_description, money, len);
}
void HardWareApp::callback_ReportHandler(const char *status)
{
	char buf[4096] = { 0 };
	sprintf(buf, "vmc״̬�ص�[%s]", status);
	cout << buf << endl;
	
	//call js function
	HardWareTask::instance()->callJSReportHandler(status);
}
void HardWareApp::callback_MoneyRecvHandler(int type, int money, int total_money)
{
	char buf[4096] = { 0 };
	sprintf(buf, "vmc���ص�[%d][%d][%d]", type, money, total_money);
	cout << buf << endl;
	
	//call js function
	HardWareTask::instance()->callJSMoneyRecvHandler(type, money, total_money);
}
void HardWareApp::callback_BtnPushHandler(int except_money, int actual_money)
{
	char buf[4096] = { 0 };
	sprintf(buf, "vmc�˱Ұ����ص�[%d][%d]", except_money, actual_money);
	cout << buf << endl;
	
	//call js function
	HardWareTask::instance()->callJSBtnPushHandler(except_money, actual_money);
}
void HardWareApp::callback_PosHandler(int iEventValue, const char * strEventMsg)
{
	char buf[4096] = { 0 };
	sprintf(buf, "�����ص�[0x%02x][%s]", (unsigned char)iEventValue, strEventMsg);
	cout << buf << endl;
	
	//call js function
	HardWareTask::instance()->callJSPosHandler(iEventValue, strEventMsg);
}
void HardWareApp::callback_PollICcardHandler(void *extra, const char* blockData)
{
	char buf[4096] = { 0 };
	sprintf(buf, "IC���ص�[%s]", blockData);
	cout << buf << endl;
	
	//call js function
	HardWareTask::instance()->callJSPollICcardHandler(extra, blockData);
}
