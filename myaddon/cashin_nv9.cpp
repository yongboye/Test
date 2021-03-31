#include "cashin_nv9.h"
#include <string.h>
#include "tools.h"
#include "Commands.h"
//#include "Logger.h"
#include <json/json.h>
//#include <NV9.h> //2021-03-09

cash_in_junpeng* cash_in_junpeng::pThis = NULL;

cash_in_junpeng::cash_in_junpeng(cash_event_callback_t cash_event_handler_junpeng, const char* port, int accept_)
{
	devParam = NULL;
	pThis = this;
	junpeng_ret_buf[0] = 0;
	this->cash_event_handler_junpeng = cash_event_handler_junpeng;	
	m_repeat_event = 0;
	m_last_event_type = 0;
	strcpy(comPort, port);	
	m_cash_accept_1 = accept_;
}
cash_in_junpeng::~cash_in_junpeng()
{
	/*if(devParam != NULL){
		if(!devParam->exit) devParam->exit = true;
	}
	nv9_end();*/
	cash_in_FreeLib();
}

void cash_in_junpeng::destructObj()
{
	/*if(devParam != NULL){
		if(!devParam->exit) devParam->exit = true;
	}
	nv9_end();*/
}

char* cash_in_junpeng::test()
{
	return "hello cash_in_junpeng";
}
char *cash_in_junpeng::cash_in_enable()
{
	Json::Value root;
	Json::FastWriter writer;  
	std::string lib_info = "";
	
	if(devParam->enable == true){
		lib_info = "success";
		root["result_code"] = 0;
		root["result_msg"] = lib_info;
		strcpy(junpeng_ret_buf, writer.write(root).c_str());
		return junpeng_ret_buf;
	}
	
	bool enable_flag = kt_nv9_enable();
	if (!enable_flag)
	{
		lib_info = "Enable Banknote receiver failure";
		root["result_code"] = -7;
		root["result_msg"] = lib_info;
	}
	else
	{
		if(devParam != NULL)
			devParam->enable = true;
		lib_info = "success";
		root["result_code"] = 0;
		root["result_msg"] = lib_info;
	}
	strcpy(junpeng_ret_buf, writer.write(root).c_str());
	return junpeng_ret_buf;
}

char *cash_in_junpeng::cash_in_disable()
{
	Json::Value root;
	Json::FastWriter writer;  
	std::string lib_info = "";
	
	cout<<"nv9 1"<<endl;
	if(devParam->enable == false){
		cout<<"nv9 1"<<endl;
		lib_info = "success";
		cout<<"nv9 1"<<endl;
		root["result_code"] = 0;
		cout<<"nv9 1"<<endl;
		root["result_msg"] = lib_info;
		cout<<"nv9 1"<<endl;
		strcpy(junpeng_ret_buf, writer.write(root).c_str());
		cout<<"nv9 n"<<endl;
		return junpeng_ret_buf;
	}

	cout<<"nv9 2"<<endl;
	bool enable_flag = kt_nv9_disable();
	if (!enable_flag)
	{
		cout<<"nv9 3"<<endl;
		lib_info = "Failed to close the note receiver";
		root["result_code"] = -8;
		root["result_msg"] = lib_info;
	}
	else
	{
		cout<<"nv9 4"<<endl;
		if(devParam != NULL)
			devParam->enable = false;
		lib_info = "success";
		root["result_code"] = 0;
		root["result_msg"] = lib_info;
	}
	strcpy(junpeng_ret_buf, writer.write(root).c_str());
	cout<<"nv9 5"<<endl;
	return junpeng_ret_buf;
}

char *cash_in_junpeng::cash_in_check()
{
	Json::Value root;
	Json::FastWriter writer;  
	std::string lib_info = "";

	int ret = kt_nv9_getStatus();
	{
		lib_info = "success";
		root["result_code"] = 0;
		root["result_msg"] = lib_info;
		root["error_code"] = ret;
		root["error_msg"] = getErrorMsg(ret);
	}
	strcpy(junpeng_ret_buf, writer.write(root).c_str());
	return junpeng_ret_buf;
}

char *cash_in_junpeng::cash_in_reset()
{
	Json::Value root;
	Json::FastWriter writer;  
	std::string lib_info = "";

	bool reset_flag = kt_nv9_reset();
	if (!reset_flag)
	{
		lib_info = "Failed to reset the bank note receiver";
		root["result_code"] = -53;
		root["result_msg"] = lib_info;
	}
	else
	{
		lib_info = "success";
		root["result_code"] = 0;
		root["result_msg"] = lib_info;
	}
	strcpy(junpeng_ret_buf, writer.write(root).c_str());
	return junpeng_ret_buf;
}

char *cash_in_junpeng::cash_in_set_notes_accept(int cash_type, bool accept, bool finish)
{
	Json::Value root;
	Json::FastWriter writer;  
	std::string lib_info = "";

	char channel = (char)(cash_type & 0xff);
	bool channel_flag = kt_nv9_set_notes_accept(channel);
	if (!channel_flag)
	{
		lib_info = "Failed to set the denomination received by banknote";
		root["result_code"] = -9;
		root["result_msg"] = lib_info;
	}
	else
	{
		lib_info = "success";
		root["result_code"] = 0;
		root["result_msg"] = lib_info;
	}
	strcpy(junpeng_ret_buf, writer.write(root).c_str());
	return junpeng_ret_buf;
}

char *cash_in_junpeng::cash_in_get_Dataset_Version()
{
	Json::Value root;
	Json::FastWriter writer;  
	std::string lib_info = "";

	char ver[100] = {0};
	bool ret = kt_nv9_getDatasetVersion(ver);
	if (!ret)
	{
		lib_info = "Failed to get packet version";
		root["result_code"] = -48;
		root["result_msg"] = lib_info;
		root["result_version"] = "";
	}
	else
	{
		lib_info = "success";
		root["result_code"] = 0;
		root["result_msg"] = lib_info;
		root["result_version"] = ver;
	}
	strcpy(junpeng_ret_buf, writer.write(root).c_str());
	return junpeng_ret_buf;
}

char *cash_in_junpeng::cash_in_get_Firmware_Version()
{
	Json::Value root;
	Json::FastWriter writer;  
	std::string lib_info = "";

	char ver[100] = { 0 };
	bool ret = kt_nv9_getFirmwareVersion(ver);
	if (!ret)
	{
		lib_info = "获取固件版本失败";
		root["result_code"] = -49;
		root["result_msg"] = lib_info;
		root["result_version"] = "";
	}
	else
	{
		lib_info = "success";
		root["result_code"] = 0;
		root["result_msg"] = lib_info;
		root["result_version"] = ver;
	}
	strcpy(junpeng_ret_buf, writer.write(root).c_str());
	return junpeng_ret_buf;
}

char *cash_in_junpeng::cash_in_upgrade_Firmware_Program(const char *fileName, const char *i_arrPort)
{
	Json::Value root;
	Json::FastWriter writer;  
	std::string lib_info = "";

	bool ret = kt_nv9_programFirmware(fileName, 0, i_arrPort);
	if (!ret)
	{
		lib_info = "升级固件程序失败";
		root["result_code"] = -50;
		root["result_msg"] = lib_info;
	}
	else
	{
		lib_info = "success";
		root["result_code"] = 0;
		root["result_msg"] = lib_info;
	}
	strcpy(junpeng_ret_buf, writer.write(root).c_str());
	return junpeng_ret_buf;
}

void cash_in_junpeng::setParam(void *param)
{
	devParam = (DEVPARAMS *)param;
}

int cash_in_junpeng::cash_in_init(const char* logpath, const char* path)
{
	cout<<"cash_in_init"<<endl;
	//第二步：打开串口	
	cout<<"nv9_init start comPort:"<<comPort<<endl;
	int ret = cash_in_loadDll(path);
	if(ret == 0){
		return 0;
	}else if(ret == -1){
		return 0;
	}
	bool res = kt_nv9_init(0, comPort, logpath);
	cout<<"nv9_init end"<<endl;
	if(res)
	{
		//初始化成功		
		//绑定libNV9需要调用的回调
		kt_nv9_bind_poll_event_handler(cash_in_nv9_callback);
	}
	else
	{
		//初始化失败
		return 0;
	}
	unsigned short note_accept = (unsigned short)m_cash_accept_1;
	res = kt_nv9_set_notes_accept(note_accept);
	if(res)
	{
		//初始化成功
	}
	else
	{
		//初始化失败
		return 0;
	}
	return res;
}

void cash_in_junpeng::cash_in_closeport()
{
	kt_nv9_close_serial();
}

bool cash_in_junpeng::cash_in_sync1(bool bRead)
{
	bool ret = kt_nv9_sync2(bRead);
	return ret;
}

bool cash_in_junpeng::cash_in_poll1()
{
	bool ret = kt_nv9_poll2();
	return ret;
}

void cash_in_junpeng::cash_in_poll2(int *needLen, int *recvLen, int *recvLenLst, unsigned char* recvData, int* sendStatus)
{
	return;
}

int cash_in_junpeng::cash_in_loadDll(const char* path)
{
	char buf[1024];
	sprintf(buf, "%s/libNV9.dll", path);
	hDll = LoadLibrary(buf);
	if (hDll == NULL)
	{
		debug("不存在libNV9.dll");
		return 0;
	}

	kt_nv9_enable = (pKT_nv9_enable)GetProcAddress(hDll, "nv9_enable");
	kt_nv9_disable = (pKT_nv9_disable)GetProcAddress(hDll, "nv9_disable");
	kt_nv9_getStatus = (pKT_nv9_getStatus)GetProcAddress(hDll, "nv9_getStatus");
	kt_nv9_reset = (pKT_nv9_reset)GetProcAddress(hDll, "nv9_reset");
	kt_nv9_set_notes_accept = (pKT_nv9_set_notes_accept)GetProcAddress(hDll, "nv9_set_notes_accept");
	kt_nv9_getDatasetVersion = (pKT_nv9_getDatasetVersion)GetProcAddress(hDll, "nv9_getDatasetVersion");
	kt_nv9_getFirmwareVersion = (pKT_nv9_getFirmwareVersion)GetProcAddress(hDll, "nv9_getFirmwareVersion");
	kt_nv9_programFirmware = (pKT_nv9_programFirmware)GetProcAddress(hDll, "nv9_programFirmware");
	kt_nv9_init = (pKT_nv9_init)GetProcAddress(hDll, "nv9_init");
	kt_nv9_close_serial = (pKT_nv9_close_serial)GetProcAddress(hDll, "nv9_close_serial");
	kt_nv9_sync2 = (pKT_nv9_sync2)GetProcAddress(hDll, "nv9_sync2");
	kt_nv9_poll2 = (pKT_nv9_poll2)GetProcAddress(hDll, "nv9_poll2");
	kt_nv9_bind_poll_event_handler = (pKT_nv9_bind_poll_event_handler)GetProcAddress(hDll, "nv9_bind_poll_event_handler");
	if (!kt_nv9_enable || !kt_nv9_disable || !kt_nv9_getStatus || !kt_nv9_reset || !kt_nv9_set_notes_accept || !kt_nv9_getDatasetVersion ||
	    !kt_nv9_getFirmwareVersion || !kt_nv9_programFirmware || !kt_nv9_init || !kt_nv9_close_serial || !kt_nv9_sync2 || !kt_nv9_poll2 || !kt_nv9_bind_poll_event_handler)
	{
		debug("加载libNV9.dll库函数失败！");
		return -1;
	}
	return 1;
}
BOOL cash_in_junpeng::cash_in_FreeLib()
{
	if (hDll != NULL)
	{
		return FreeLibrary(hDll);
	}
	return FALSE;
}

//static 被libNV9调用,libNV9传过来的参数翻译(param*100)给c#的回调
void cash_in_junpeng::cash_in_nv9_callback(unsigned char event_type, int param, int len)
{
	int out_event_type = 0;
	int out_money = 0;
	std::string out_event_msg;
	switch(event_type)
	{
	case SSP_POLL_RESET:
		out_event_type = -41;
		out_event_msg = "Slave reset 0xf1";
		break;
	case SSP_POLL_NOTE_READ:
		out_event_type = -42;
		out_event_msg = "Read Note 0xef";
		out_money = param;
		break;
	case SSP_POLL_CREDIT:
		out_event_type = -43;
		out_event_msg = "Credit Note 0xee";
		out_money = param;
		break;
	case SSP_POLL_REJECTING:
		out_event_type = -44;
		out_event_msg = "Note Rejecting 0xed";
		break;
	case SSP_POLL_REJECTED:
		out_event_type = -45;
		out_event_msg = "Note Rejected 0xec";
		break;
	case SSP_POLL_STACKING:
		out_event_type = -46;
		out_event_msg = "Note Stacking 0xcc";
		break;
	case SSP_POLL_STACKED:
		out_event_type = -47;
		out_event_msg = "Note Stacked 0xeb";
		break;
	case SSP_POLL_SAFE_JAM:
		out_event_type = -48;
		out_event_msg = "Safe Note Jam 0xea";
		break;
	case SSP_POLL_UNSAFE_JAM:
		out_event_type = -49;
		out_event_msg = "Unsafe Note Jam 0xe9";
		break;
	case SSP_POLL_DISABLED:
		out_event_type = -50;
		out_event_msg = "Disabled 0xe8";
		break;
	case SSP_POLL_FRAUD_ATTEMPT:
		out_event_type = -51;
		out_event_msg = "Fraud Attempt 0xe6";
		out_money = param;
		break;
	case SSP_POLL_STACKER_FULL:
		out_event_type = -52;
		out_event_msg = "Stacker Full 0xe7";
		break;
	case SSP_POLL_NOTE_CLEARED_FROM_FRONT:
		out_event_type = -53;
		out_event_msg = "Note Cleared From Front 0xe1";
		out_money = param;
		break;
	case SSP_POLL_NOTE_CLEARED_TO_CASHBOX:
		out_event_type = -54;
		out_event_msg = "Note Cleared To Cashbox 0xe2";
		out_money = param;
		break;
	case SSP_POLL_CHANNEL_DISABLE:
		out_event_type = -55;
		out_event_msg = "Channel Disable 0xb5";
		break;
	case SSP_POLL_INITIALISING:
		out_event_type = -56;
		out_event_msg = "Initialising 0xb6";
		break;
	case SSP_RESPONSE_CMD_OK:
		out_event_type = -57;
		out_event_msg = "idle 0xf0";
		break;
	default:
		out_event_type = -100;
		out_event_msg = "unknown event";
		break;
	}
	char msg[100] = {0};
	strcpy(msg, out_event_msg.c_str());
	
	//所有的钱的单位都是分
	out_money *= 100; //

	//过滤重复事件
	if(pThis->m_last_event_type == event_type && param == 0)
	{
		pThis->m_repeat_event++;
	}
	else
	{
		pThis->m_repeat_event = 0;
	}
	if(pThis->m_repeat_event >= 1)
	{
		//是重复事件
		if(pThis->m_repeat_event == 1)
		{
			//addlog(Device_Cash_In, __FILE__, __LINE__, "callback: ...");
		}
	}
	else
	{
		//不是重复事件
	}
	pThis->m_last_event_type = event_type;
	


	if(pThis->cash_event_handler_junpeng != NULL)
	{
		//调用c#应用程序的回调
		(*pThis->cash_event_handler_junpeng)(pThis->devParam, out_event_type, msg, out_money, len);
	}
	else
	{
	}	
	return ;
}

string cash_in_junpeng::getErrorMsg(int errcode)
{
	string str = "";
	switch (errcode) {
	case -100:
		str = "创建对象失败";
		break;
	case -101:
		str = "固件升级中";
		break;
	case -1:
		str = "未知状态";
		break;
	case -2:
		str = "初始化失败";
		break;
	case -110:
		str = "串口打开成功";
		break;
	case -111:
		str = "串口打开失败";
		break;
	case -116:
		str = "关闭串口";
		break;
	case -112:
		str = "发送失败";
		break;
	case -114:
		str = "接收失败";
		break;
	case -43:
	case 0xee:
		str = "真币";
		break;
	case 0xcc:
		str = "接收中";
		break;
	case -47:
	case 0xeb:
		str = "接收完成";
		break;
	case -45:
	case 0xec:
		str = "退币结束";
		break;
	case 0xed:
		str = "退币中";
		break;
	case -48:
		str = "卡币";
		break;
	case 0xea:
		str = "安全卡币";
		break;
	case 0xe9:
		str = "不安全卡币";
		break;
	case -81:
		str = "电机故障";
		break;
	case -82:
		str = "检验错";
		break;
	case -83:
		str = "bill jam";
		break;
	case -84:
		str = "bill remove";
		break;
	case -85:
		str = "Stacker open";
		break;
	case -86:
		str = "Sensor problem";
		break;
	case -87:
		str = "Bill fish";
		break;
	case -88:
		str = "Stacker problem";
		break;
	case -89:
		str = "Bill reject";
		break;
	case -90:
		str = "Invalid command";
		break;
	case -91:
		str = "reserved";
		break;
	case -92:
		str = "Response when error status is exclusion";
		break;
	case -93:
		str = "enable";
		break;
	case -94:
	case 0xe8:
		str = "inhibit";
		break;
	case 0xb5:
		str = "channel disable";
		break;
	case 0xb6:
		str = "initialising";
		break;
	case -95:
		str = "busy";
		break;
	case 0xe6:
		str = "fraud attempt";
		break;
	case -96:
		str = "power on";
		break;
	case 0xf0:
		str = "response cmd ok";
		break;
	case 0xf1:
		str = "reset";
		break;
	case 0xef:
		str = "note read";
		break;
	case 0xe7:
		str = "钱箱满";
		break;
	case 0xe1:
		str = "CLEARED FROM FRONT";
		break;
	case 0xe2:
		str = "CLEARED TO CASHBOX";
		break;
	}
	return str;
}