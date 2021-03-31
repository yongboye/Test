#include "ticketsensor_generation.h"
#include <string.h>
#include "tools.h"
#include "Commands.h"
#include <json/json.h>
//#include <libticketsensor.h> //2021-03-09

sensor_generation* sensor_generation::pThis = NULL;

sensor_generation::sensor_generation()
{}

sensor_generation::sensor_generation(ticketsensor_stus_event_handler_t sensor_event_handler_generation, const char* port)
{
	pThis = this;
	devParam = NULL;
	sensor_ret_buf[0] = 0;
	this->sensor_event_handler_generation = sensor_event_handler_generation;
	strcpy(comPort, port);	
}
sensor_generation::~sensor_generation()
{
	/*if(devParam != NULL){
		if(!devParam->exit) devParam->exit = true;
	}
	ITICKET_cleanUp();*/
	sensor_FreeLib();
}

char *sensor_generation::sensor_enable()
{

	Json::Value root;
	Json::FastWriter write;
	std::string lib_info = "";
	//int enable_flag = ITICKET_enable(true);
	int enable_flag = -2;
	if(kt_ITICKET_enable != NULL){
		enable_flag = kt_ITICKET_enable(true);
	}
	if (enable_flag)
	{
		lib_info = "使能票取走传感器失败！";
		root["result_code"] = -59;
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
	std::string strWriter = "";
	strWriter = write.write(root);
	strcpy(sensor_ret_buf, strWriter.c_str());
	debug("sensor_enable返回:%s", sensor_ret_buf);

	return sensor_ret_buf;
}

char *sensor_generation::sensor_disable()
{
	Json::Value root;
	Json::FastWriter write;
	std::string lib_info = "";
	//int enable_flag = ITICKET_enable(false);
	int enable_flag = -2;
	if(kt_ITICKET_enable != NULL){
		enable_flag = kt_ITICKET_enable(false);
	}
	if (enable_flag)
	{
		lib_info = "禁能票取走传感器失败";
		root["result_code"] = -60;
		root["result_msg"] = lib_info;
	}
	else
	{
		if(devParam != NULL)
			devParam->enable = false;
		lib_info = "success";
		root["result_code"] = 0;
		root["result_msg"] = lib_info;
	}
	std::string strWriter = "";
	strWriter = write.write(root);
	strcpy(sensor_ret_buf, strWriter.c_str());
	debug("sensor_disable返回:%s", sensor_ret_buf);
	return sensor_ret_buf;
}

void sensor_generation::sensor_closeport()
{
	//ITICKET_close_serial();
	if(kt_ITICKET_close_serial != NULL){
		kt_ITICKET_close_serial();
	}
}

void sensor_generation::sensor_read_Status()
{
	//ITICKET_read_GetStus();
	if(kt_ITICKET_read_GetStus != NULL){
		kt_ITICKET_read_GetStus();
	}
}

char *sensor_generation::sensor_get_Status()
{
	Json::Value root;
	Json::FastWriter write;
	std::string lib_info = "";
	char oBuf[100] = {0};
	//0succ -2未实例化，调用失败 -4取票传感器获取状态失败(初始化失败、使能状态、驱动不存在、重连失败、发送失败、接收失败、接收非法包)
	//int reset_flag = ITICKET_send_GetStus(oBuf);
	int reset_flag = -2;
	if(kt_ITICKET_send_GetStus != NULL){
		reset_flag	= kt_ITICKET_send_GetStus(oBuf);
	}
	if (reset_flag)
	{
		lib_info = "票取走传感器获取状态失败";
		root["result_code"] = -61;
		root["result_msg"] = lib_info;
		root["sensor_error_code"] = reset_flag;
		if(reset_flag == -2)
			root["sensor_error_msg"] = "未实例化，调用失败";
		else if(reset_flag == -4)
			root["sensor_error_msg"] = "取票传感器获取状态失败(初始化失败、使能状态、驱动不存在、重连失败、发送失败、接收失败、接收非法包)";
		else
			root["sensor_error_msg"] = "other error";
	}
	else
	{
		lib_info = "success";
		root["result_code"] = 0;
		root["result_msg"] = lib_info;
		root["sensor_error_code"] = reset_flag;
		root["sensor_error_msg"] = oBuf;
	}
	std::string strWriter = "";
	strWriter = write.write(root);
	strcpy(sensor_ret_buf, strWriter.c_str());
	debug("sensor_get_Status返回:%s", sensor_ret_buf);
	return sensor_ret_buf;
}

void sensor_generation::setParam(void *param)
{
	devParam = (DEVPARAMS *)param;
}

void sensor_generation::sensor_callback(int method, char* stusData)
{
	//cout<<"sensor_generation::sensor_callback"<<endl;
	if (pThis->sensor_event_handler_generation != NULL)
	{
		//调用c#应用程序的回调
		cout<<"sensor_callback---->"<<stusData<<endl;
		(*pThis->sensor_event_handler_generation)(pThis->devParam, method, stusData);
	}
	else
	{
		//warn(WO_MSG_CASHIN, "应用程序没有注册回调函数");
	}
	return;
}

int sensor_generation::sensor_loadDll(const char* path)
{
	char buf[1024];
	sprintf(buf, "%s/libticketsensor.dll", path);
	hDll = LoadLibrary(buf);
	if (hDll == NULL)
	{
		debug("不存在libticketsensor.dll");
		return 0;
	}

	kt_ITICKET_init = (pKT_ITICKET_init)GetProcAddress(hDll, "ITICKET_init");
	kt_ITICKET_enable = (pKT_ITICKET_enable)GetProcAddress(hDll, "ITICKET_enable");
	kt_ITICKET_send_GetStus = (pKT_ITICKET_send_GetStus)GetProcAddress(hDll, "ITICKET_send_GetStus");
	kt_ITICKET_read_GetStus = (pKT_ITICKET_read_GetStus)GetProcAddress(hDll, "ITICKET_read_GetStus");
	kt_ITICKET_close_serial = (pKT_ITICKET_close_serial)GetProcAddress(hDll, "ITICKET_close_serial");
	kt_ITICKET_cleanUp = (pKT_ITICKET_cleanUp)GetProcAddress(hDll, "ITICKET_cleanUp");
	if (!kt_ITICKET_init || !kt_ITICKET_enable || !kt_ITICKET_send_GetStus || !kt_ITICKET_read_GetStus || !kt_ITICKET_close_serial || !kt_ITICKET_cleanUp)
	{
		debug("加载libticketsensor.dll库函数失败！");
		return -1;
	}
	return 1;
}
BOOL sensor_generation::sensor_FreeLib()
{
	if (hDll != NULL)
	{
		return FreeLibrary(hDll);
	}
	return FALSE;
}

int sensor_generation::sensor_init(const char* logpath, const char* path)
{
	//第二步：打开串口
	//cout<<"7-1"<<endl;
	//int res = ITICKET_init(comPort, sensor_callback, logpath);
	int ret = sensor_loadDll(path);
	if(ret == 0){
		return -5;
	}else if(ret == -1){
		return -6;
	}
	int res = kt_ITICKET_init(comPort, sensor_callback, logpath);
	//cout<<"7-2"<<endl;
	return res; //0:succ
}