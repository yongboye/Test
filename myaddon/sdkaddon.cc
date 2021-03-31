#include "sdkaddon.h"
#include <node.h>

#include <sstream>
#include <string>
#include <cstring>
#include <memory>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
 
#include "tools.h"
#include "sdk2_1.h"
//#include <gLog.h>
#include <Logger.h>
#include <json/json.h>
//#include <libRPDCard.h> //2021-03-09
//#include <libRFMCard.h> //2021-03-09
//#include <libSHtransportationcard.h> //2021-03-09
//#include <stru.h>
#include "../libSHtransportationcard/stru.h"
//#include <libGenerationsEpson.h> //2021-03-09
//#include <libEleCtlLock.h> //2020-12-11 2021-03-09
#include "cashin.h"
#include "cashin_nv9.h"
#include "cash_in_v77e.h"
#include "sensor.h"
#include "ticketsensor_generation.h"
//#incldue "ticketsensor_bibasic.h"
//#include "ticketsensor_tert.h"

#if defined(__linux__)
#include <unistd.h>
//dlopen dlsym dlclose
//#include <dlfcn.h> 
#else
#include <windows.h>
#endif

#define MAX_RF_BUFFER     1024

//lock
HINSTANCE lock_hDll;
pKT_IELECTLOCK_init				kt_IELECTLOCK_init;
pKT_IELECTLOCK_send_Unlocking	kt_IELECTLOCK_send_Unlocking;
pKT_IELECTLOCK_close_serial		kt_IELECTLOCK_close_serial;
pKT_IELECTLOCK_cleanUp			kt_IELECTLOCK_cleanUp;
int lock_loadDll(const char* path)
{
	char buf[1024];
	sprintf(buf, "%s/libEleCtlLock.dll", path);
	lock_hDll = LoadLibrary(buf);
	if (lock_hDll == NULL)
	{
		debug("不存在libEleCtlLock.dll");
		return 0;
	}

	kt_IELECTLOCK_init = (pKT_IELECTLOCK_init)GetProcAddress(lock_hDll, "IELECTLOCK_init");
	kt_IELECTLOCK_send_Unlocking = (pKT_IELECTLOCK_send_Unlocking)GetProcAddress(lock_hDll, "IELECTLOCK_send_Unlocking");
	kt_IELECTLOCK_close_serial = (pKT_IELECTLOCK_close_serial)GetProcAddress(lock_hDll, "IELECTLOCK_close_serial");
	kt_IELECTLOCK_cleanUp = (pKT_IELECTLOCK_cleanUp)GetProcAddress(lock_hDll, "IELECTLOCK_cleanUp");
	if (!kt_IELECTLOCK_init || !kt_IELECTLOCK_send_Unlocking || !kt_IELECTLOCK_close_serial || !kt_IELECTLOCK_cleanUp)
	{
		debug("加载libEleCtlLock.dll库函数失败！");
		return -1;
	}
	return 1;
}
BOOL lock_FreeLib()
{
	if (lock_hDll != NULL)
	{
		return FreeLibrary(lock_hDll);
	}
	return FALSE;
}
//rfm libRFMCardV3
HINSTANCE rfm_hDll;
pKT_IRFM_operateAntenna			kt_IRFM_operateAntenna;
pKT_IRFM_searchCard				kt_IRFM_searchCard;
pKT_RFM_preventConflict			kt_RFM_preventConflict;
pKT_RFM_beer					kt_RFM_beer;
pKT_RFM_selectCard				kt_RFM_selectCard;
pKT_RFM_verifyPassWord			kt_RFM_verifyPassWord;
pKT_RFM_readBlock				kt_RFM_readBlock;
pKT_IRFM_cleanUp				kt_IRFM_cleanUp;
pKT_IRFM_Disable				kt_IRFM_Disable;
pKT_IRFM_Enable					kt_IRFM_Enable;
pKT_IRFM_init					kt_IRFM_init;
int rfm_loadDll(const char* path)
{
	char buf[1024];
	sprintf(buf, "%s/libRFMCardV3.dll", path);
	rfm_hDll = LoadLibrary(buf); //E:/keytop-quasar-project/cpspr/src/statics/Hardware/windows/ia32/libRFMCardV3.dll
	if (rfm_hDll == NULL)
	{
		debug("不存在libRFMCardV3.dll");
		return 0;
	}

	kt_IRFM_operateAntenna = (pKT_IRFM_operateAntenna)GetProcAddress(rfm_hDll, "IRFM_operateAntenna");
	kt_IRFM_searchCard = (pKT_IRFM_searchCard)GetProcAddress(rfm_hDll, "IRFM_searchCard");
	kt_RFM_preventConflict = (pKT_RFM_preventConflict)GetProcAddress(rfm_hDll, "RFM_preventConflict");
	kt_RFM_beer = (pKT_RFM_beer)GetProcAddress(rfm_hDll, "RFM_beer");
	kt_RFM_selectCard = (pKT_RFM_selectCard)GetProcAddress(rfm_hDll, "RFM_selectCard");
	kt_RFM_verifyPassWord = (pKT_RFM_verifyPassWord)GetProcAddress(rfm_hDll, "RFM_verifyPassWord");
	kt_RFM_readBlock = (pKT_RFM_readBlock)GetProcAddress(rfm_hDll, "RFM_readBlock");
	kt_IRFM_cleanUp = (pKT_IRFM_cleanUp)GetProcAddress(rfm_hDll, "IRFM_cleanUp");
	kt_IRFM_Disable = (pKT_IRFM_Disable)GetProcAddress(rfm_hDll, "IRFM_Disable");
	kt_IRFM_Enable = (pKT_IRFM_Enable)GetProcAddress(rfm_hDll, "IRFM_Enable");
	kt_IRFM_init = (pKT_IRFM_init)GetProcAddress(rfm_hDll, "IRFM_init");
	if (!kt_IRFM_operateAntenna || !kt_IRFM_searchCard || !kt_RFM_preventConflict || !kt_RFM_beer || !kt_RFM_selectCard ||
	    !kt_RFM_verifyPassWord || !kt_RFM_readBlock || !kt_IRFM_cleanUp || !kt_IRFM_Disable || !kt_IRFM_Enable || !kt_IRFM_init)
	{
		debug("加载libRFMCardV3.dll库函数失败！");
		return -1;
	}
	return 1;
}
BOOL rfm_FreeLib()
{
	if (rfm_hDll != NULL)
	{
		return FreeLibrary(rfm_hDll);
	}
	return FALSE;
}
//rpd220 
HINSTANCE rpd_hDll;
pKT_IRPD_Request				kt_IRPD_Request;
pKT_IRPD_Anticoll				kt_IRPD_Anticoll;
pKT_IRPD_Select					kt_IRPD_Select;
pKT_IRPD_VerifyPassWord			kt_IRPD_VerifyPassWord;
pKT_IRPD_readBlock				kt_IRPD_readBlock;
pKT_IRPD_Beep					kt_IRPD_Beep;
pKT_IRPD_cleanUp				kt_IRPD_cleanUp;
pKT_IRPD_init					kt_IRPD_init;
pKT_IRPD_Disable				kt_IRPD_Disable;
pKT_IRPD_Enable					kt_IRPD_Enable;
int rpd_loadDll(const char* path)
{
	char buf[1024];
	sprintf(buf, "%s/libRPDCardV1.dll", path);
	rpd_hDll = LoadLibrary(buf);
	if (rpd_hDll == NULL)
	{
		debug("不存在libRPDCardV1.dll");
		return 0;
	}

	kt_IRPD_Request = (pKT_IRPD_Request)GetProcAddress(rpd_hDll, "IRPD_Request");
	kt_IRPD_Anticoll = (pKT_IRPD_Anticoll)GetProcAddress(rpd_hDll, "IRPD_Anticoll");
	kt_IRPD_Select = (pKT_IRPD_Select)GetProcAddress(rpd_hDll, "IRPD_Select");
	kt_IRPD_VerifyPassWord = (pKT_IRPD_VerifyPassWord)GetProcAddress(rpd_hDll, "IRPD_VerifyPassWord");
	kt_IRPD_readBlock = (pKT_IRPD_readBlock)GetProcAddress(rpd_hDll, "IRPD_readBlock");
	kt_IRPD_Beep = (pKT_IRPD_Beep)GetProcAddress(rpd_hDll, "IRPD_Beep");
	kt_IRPD_cleanUp = (pKT_IRPD_cleanUp)GetProcAddress(rpd_hDll, "IRPD_cleanUp");
	kt_IRPD_init = (pKT_IRPD_init)GetProcAddress(rpd_hDll, "IRPD_init");
	kt_IRPD_Disable = (pKT_IRPD_Disable)GetProcAddress(rpd_hDll, "IRPD_Disable");
	kt_IRPD_Enable = (pKT_IRPD_Enable)GetProcAddress(rpd_hDll, "IRPD_Enable");
	if (!kt_IRPD_Request || !kt_IRPD_Anticoll || !kt_IRPD_Select || !kt_IRPD_VerifyPassWord || !kt_IRPD_readBlock ||
	    !kt_IRPD_Beep || !kt_IRPD_cleanUp || !kt_IRPD_init || !kt_IRPD_Disable || !kt_IRPD_Enable)
	{
		debug("加载libRPDCardV1.dll库函数失败！");
		return -1;
	}
	return 1;
}
BOOL rpd_FreeLib()
{
	if (rpd_hDll != NULL)
	{
		return FreeLibrary(rpd_hDll);
	}
	return FALSE;
}
//MF800 MasterRD.dll
HINSTANCE mf_hDll;
pKT_rf_init_com				kt_rf_init_com;
pKT_rf_init_device_number	kt_rf_init_device_number;
pKT_rf_get_device_number	kt_rf_get_device_number;
pKT_rf_light				kt_rf_light;
pKT_rf_antenna_sta			kt_rf_antenna_sta;
pKT_rf_request				kt_rf_request;
pKT_rf_anticoll				kt_rf_anticoll;
pKT_rf_select				kt_rf_select;
pKT_rf_halt					kt_rf_halt;
pKT_rf_M1_authentication2	kt_rf_M1_authentication2;
pKT_rf_M1_read				kt_rf_M1_read;
pKT_rf_M1_write				kt_rf_M1_write;
pKT_rf_M1_initval			kt_rf_M1_initval;
pKT_rf_M1_readval			kt_rf_M1_readval;
pKT_rf_M1_decrement			kt_rf_M1_decrement;
pKT_rf_M1_increment			kt_rf_M1_increment;
pKT_rf_M1_restore			kt_rf_M1_restore;
pKT_rf_M1_transfer			kt_rf_M1_transfer;
pKT_rf_ClosePort 			kt_rf_ClosePort;
int mf_loadDll(const char* path)
{
	char buf[1024];
	sprintf(buf, "%s/MasterRD.dll", path);
	mf_hDll = LoadLibrary(buf);
	if (mf_hDll == NULL)
	{
		debug("不存在MasterRD.dll");
		return 0;
	}

	kt_rf_init_com = (pKT_rf_init_com)GetProcAddress(mf_hDll, "rf_init_com");
	kt_rf_init_device_number = (pKT_rf_init_device_number)GetProcAddress(mf_hDll, "rf_init_device_number");
	kt_rf_get_device_number = (pKT_rf_get_device_number)GetProcAddress(mf_hDll, "rf_get_device_number");
	kt_rf_light = (pKT_rf_light)GetProcAddress(mf_hDll, "rf_light");
	kt_rf_antenna_sta = (pKT_rf_antenna_sta)GetProcAddress(mf_hDll, "rf_antenna_sta");
	kt_rf_request = (pKT_rf_request)GetProcAddress(mf_hDll, "rf_request");
	kt_rf_anticoll = (pKT_rf_anticoll)GetProcAddress(mf_hDll, "rf_anticoll");
	kt_rf_select = (pKT_rf_select)GetProcAddress(mf_hDll, "rf_select");
	kt_rf_halt = (pKT_rf_halt)GetProcAddress(mf_hDll, "rf_halt");
	kt_rf_M1_authentication2 = (pKT_rf_M1_authentication2)GetProcAddress(mf_hDll, "rf_M1_authentication2");
	kt_rf_M1_read = (pKT_rf_M1_read)GetProcAddress(mf_hDll, "rf_M1_read");
	kt_rf_M1_write = (pKT_rf_M1_write)GetProcAddress(mf_hDll, "rf_M1_write");
	kt_rf_M1_initval = (pKT_rf_M1_initval)GetProcAddress(mf_hDll, "rf_M1_initval");
	kt_rf_M1_readval = (pKT_rf_M1_readval)GetProcAddress(mf_hDll, "rf_M1_readval");
	kt_rf_M1_decrement = (pKT_rf_M1_decrement)GetProcAddress(mf_hDll, "rf_M1_decrement");
	kt_rf_M1_increment = (pKT_rf_M1_increment)GetProcAddress(mf_hDll, "rf_M1_increment");
	kt_rf_M1_restore = (pKT_rf_M1_restore)GetProcAddress(mf_hDll, "rf_M1_restore");
	kt_rf_M1_transfer = (pKT_rf_M1_transfer)GetProcAddress(mf_hDll, "rf_M1_transfer");
	kt_rf_ClosePort	= (pKT_rf_ClosePort)GetProcAddress(mf_hDll, "rf_ClosePort");
	if ( !kt_rf_init_com || !kt_rf_init_device_number || !kt_rf_get_device_number || !kt_rf_light || !kt_rf_antenna_sta || !kt_rf_request || !kt_rf_anticoll || 
		 !kt_rf_select || !kt_rf_halt || !kt_rf_M1_authentication2 || !kt_rf_M1_read || !kt_rf_M1_write || !kt_rf_M1_initval || !kt_rf_M1_readval || 
		 !kt_rf_M1_decrement || !kt_rf_M1_increment || !kt_rf_M1_restore || !kt_rf_M1_transfer || !kt_rf_ClosePort)
	{
		debug("加载MasterRD.dll库函数失败！");
		return -1;
	}
	return 1;
}
BOOL mf_FreeLib()
{
	if (mf_hDll != NULL)
	{
		return FreeLibrary(mf_hDll);
	}
	return FALSE;
}
//traffic
HINSTANCE traffic_hDll;
pKT_Itcard_init					kt_Itcard_init;
pKT_Itcard_cleanUp				kt_Itcard_cleanUp;
pKT_Itcard_readCard				kt_Itcard_readCard;
pKT_Itcard_readCard_pr			kt_Itcard_readCard_pr;
pKT_Itcard_operateCard			kt_Itcard_operateCard;
int traffic_loadDll(const char* path)
{
	char buf[1024];
	sprintf(buf, "%s/libSHtransportationcard.dll", path);
	traffic_hDll = LoadLibrary(buf);
	if (traffic_hDll == NULL)
	{
		debug("不存在libSHtransportationcard.dll");
		return 0;
	}

	kt_Itcard_init = (pKT_Itcard_init)GetProcAddress(traffic_hDll, "Itcard_init");
	kt_Itcard_cleanUp = (pKT_Itcard_cleanUp)GetProcAddress(traffic_hDll, "Itcard_cleanUp");
	kt_Itcard_readCard = (pKT_Itcard_readCard)GetProcAddress(traffic_hDll, "Itcard_readCard");
	kt_Itcard_readCard_pr = (pKT_Itcard_readCard_pr)GetProcAddress(traffic_hDll, "Itcard_readCard_pr");
	kt_Itcard_operateCard = (pKT_Itcard_operateCard)GetProcAddress(traffic_hDll, "Itcard_operateCard");
	if (!kt_Itcard_init || !kt_Itcard_cleanUp || !kt_Itcard_readCard || !kt_Itcard_readCard_pr || !kt_Itcard_operateCard)
	{
		debug("加载libSHtransportationcard.dll库函数失败！");
		return -1;
	}
	return 1;
}
BOOL traffic_FreeLib()
{
	if (traffic_hDll != NULL)
	{
		return FreeLibrary(traffic_hDll);
	}
	return FALSE;
}
//printer
HINSTANCE prt_hDll;
pKT_lib_printer_end				kt_lib_printer_end;
pKT_lib_printer_init			kt_lib_printer_init;
pKT_lib_printer_get_status		kt_lib_printer_get_status;
pKT_lib_printer_cut				kt_lib_printer_cut;
pKT_lib_printer_set_strong		kt_lib_printer_set_strong;
pKT_lib_printer_set_align		kt_lib_printer_set_align;
pKT_lib_printer_set_size		kt_lib_printer_set_size;
pKT_lib_printer_println			kt_lib_printer_println;
pKT_lib_printer_printBarcode	kt_lib_printer_printBarcode;
pKT_lib_printer_printQr 		kt_lib_printer_printQr;
int prt_loadDll(const char* path)
{
	char buf[1024];
	sprintf(buf, "%s/generationsEpson.dll", path);
	prt_hDll = LoadLibrary(buf);
	if (prt_hDll == NULL)
	{
		debug("不存在generationsEpson.dll");
		return 0;
	}

	kt_lib_printer_end = (pKT_lib_printer_end)GetProcAddress(prt_hDll, "lib_printer_end");
	kt_lib_printer_init = (pKT_lib_printer_init)GetProcAddress(prt_hDll, "lib_printer_init");
	kt_lib_printer_get_status = (pKT_lib_printer_get_status)GetProcAddress(prt_hDll, "lib_printer_get_status");
	kt_lib_printer_cut = (pKT_lib_printer_cut)GetProcAddress(prt_hDll, "lib_printer_cut");
	kt_lib_printer_set_strong = (pKT_lib_printer_set_strong)GetProcAddress(prt_hDll, "lib_printer_set_strong");
	kt_lib_printer_set_align = (pKT_lib_printer_set_align)GetProcAddress(prt_hDll, "lib_printer_set_align");
	kt_lib_printer_set_size = (pKT_lib_printer_set_size)GetProcAddress(prt_hDll, "lib_printer_set_size");
	kt_lib_printer_println = (pKT_lib_printer_println)GetProcAddress(prt_hDll, "lib_printer_println");
	kt_lib_printer_printBarcode = (pKT_lib_printer_printBarcode)GetProcAddress(prt_hDll, "lib_printer_printBarcode");
	kt_lib_printer_printQr = (pKT_lib_printer_printQr)GetProcAddress(prt_hDll, "lib_printer_printQr");
	if (!kt_lib_printer_end || !kt_lib_printer_init || !kt_lib_printer_get_status || !kt_lib_printer_cut || !kt_lib_printer_set_strong ||
	    !kt_lib_printer_set_align || !kt_lib_printer_set_size || !kt_lib_printer_println || !kt_lib_printer_printBarcode || !kt_lib_printer_printQr)
	{
		debug("加载generationsEpson.dll库函数失败！");
		return -1;
	}
	return 1;
}
BOOL prt_FreeLib()
{
	if (prt_hDll != NULL)
	{
		return FreeLibrary(prt_hDll);
	}
	return FALSE;
}

//#include "ICTcoinin1.h"
//#include <boost/thread.hpp>
//#include <boost/date_time/posix_time/posix_time.hpp>


std::string GBKToUTF8(const std::string& strGBK)
{
    std::string strOutUTF8 = "";
    WCHAR * str1;
    int n = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL, 0);
    str1 = new WCHAR[n];
    MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, str1, n); n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);
    char * str2 = new char[n];
    WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, n, NULL, NULL);
    strOutUTF8 = str2;
    delete[]str1;
    str1 = NULL;
    delete[]str2;
    str2 = NULL;
    return strOutUTF8;
}


//////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////

int g_epson_type=1;
bool g_epson_init=false;
bool g_lock_init=false; //2020-12-11
map<string, boost::shared_ptr<ThreadData>> g_map_DevThreadData; //init new\end del
Icash_in *g_cashin = NULL;
bool cashin_InT = false;
Isensor  *g_sensor = NULL;
bool sensor_InT = false; //是否在线程里
bool iccard_InT = false;
bool traffic_InT = false;
int m_serial_Type = 0;
Logger lg;
static bool glogInit = false;

//////////////////////////////////////////////////////////////////////////////////////////////////
static void RunCallback(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  Local<Context> context = isolate->GetCurrentContext();  
  
  Local<Function> cb = Local<Function>::Cast(args[0]); 
  
  const unsigned argc = 1;
  Local<Value> argv[argc] = {
	  String::NewFromUtf8(isolate, "hello world!!!", NewStringType::kNormal).ToLocalChecked() 
  };
  cb->Call(context, Null(isolate), argc, argv).ToLocalChecked();
}

static void RunCallback2(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  Local<Context> context = isolate->GetCurrentContext();  
  
  Local<Function> cb = Local<Function>::Cast(args[0]); //坑，离开区域会爆
  
  v8::String::Utf8Value buf(isolate, args[1]);
  stringstream ss;
  ss << *buf;
  cout << ss.str()<<endl;
		  
  const unsigned argc = 1;
  Local<Value> argv[argc] = {
	  String::NewFromUtf8(isolate, "hello yyb!!!", NewStringType::kNormal).ToLocalChecked() 
  };
  cb->Call(context, Null(isolate), argc, argv).ToLocalChecked();
}

void sendCallbackFunc(uv_async_t *handle) 
{
    HandleScope handle_scope(Isolate::GetCurrent()); //这里是必须的,调用js函数需要一个handle scope
    DevParams*  devParam = (DevParams *) handle->data;
	string devName = devParam->deviceName;
	
	//if(devParam->deviceName == "icCard" || devParam->deviceName == "cashin" || devParam->deviceName == "sensor")
	{
		int argc = 1;
		Local<Value> argv[1] = {String::NewFromUtf8(devParam->isolate, devParam->resultjs.c_str(), NewStringType::kNormal).ToLocalChecked()};
		//if(devParam->deviceName == "sensor") cout<<"sendCallbackFunc->"<<devParam->resultjs.c_str()<<endl;
		Local<Function>::New(devParam->isolate, devParam->oncallback)->Call(devParam->isolate->GetCurrentContext(), Null(devParam->isolate), argc, argv); 		
	}
}

void sensorhandler(void *extra, int method, char* stusData)
{
	if(extra == NULL)
	{
		return ;
	}
	DevParams *data = (DevParams*) extra;
	
	Json::Value root;
	Json::FastWriter writer;  
	root["method"] = method;
	root["stusData"] = stusData;
	data->resultjs = writer.write(root);
	cout<<"sensorhandler-->"<<stusData<<endl;
	uv_async_send(data->callbackSendReq);	
}


void cashinhandler(void *extra, int cash_in_event_code, const char *event_code_description, int money, int len)
{	
	if(extra == NULL)
	{
		return ;
	}
	DevParams *data = (DevParams*) extra;
	
	if(data->subDev == 1){
		if(cash_in_event_code == 0xE8)
		{
			data->enable = false;
		}	
	}else if(data->subDev == 2){
		if(cash_in_event_code == 0x3E)
			data->enable = true;
		else if(cash_in_event_code == 0x5E)
			data->enable = false;
	}	
	
	string str = "";
	switch (len){
	case 1:
		str = "CNY";
		break;
	case 2:
		str = "USD";
		break;
	case 3:
		str = "MXN";
		break;
	case 4:
		str = "default";
		break;
	case 0:
	default:
		str = "unknown";
		break;
	}
	Json::Value root;
	Json::FastWriter writer;  
	cout<<"cash_in_event_code="<<cash_in_event_code<<endl;
	root["eventcode"] = cash_in_event_code;
	root["eventcodeDescription"] = event_code_description;
	root["money"] = money;
	root["currency"] = str;
	data->resultjs = writer.write(root);

	uv_async_send(data->callbackSendReq);	
}











void close_cb(uv_handle_t* handle)
{
}

void callGoThread(uv_work_t* req)
{
    // 从uv_work_t的结构体中获取我们定义的入参结构
    auto* devParam = (DevParams*) req->data;
	devParam->status = 1;
	string devName = devParam->deviceName;
 	char buf[100] = {0}; 
	unsigned char m_recvdata[4096] = {0};

	if(devParam->deviceName == "icCard")
	{
		lg.LogInfo(1, "callGoThread in thread deviceName:icCard");
		while(!devParam->exit)
		{
			iccard_InT = true;
			
			if(devParam->enable)
			{
				if(devParam->subDev == 1)
				{	//rpd
					//cout<<"icCard enable"<<endl;
					bool bRet; int nRet;
					memset(buf, 0x00, sizeof(buf));
					if(kt_IRPD_Request != NULL){
						bRet = kt_IRPD_Request(1, buf);
					}
					//cout<<"IRPD_Request end"<<endl;
					if (bRet){		
						memset(buf, 0x00, sizeof(buf));
						if(kt_IRPD_Anticoll != NULL){
							bRet = kt_IRPD_Anticoll(buf);
						}
						if(bRet){
							if(kt_IRPD_Select != NULL){
								nRet = kt_IRPD_Select(buf);
							}
							if (nRet >= 0){
								if(kt_IRPD_VerifyPassWord != NULL){
									bRet = kt_IRPD_VerifyPassWord(1, devParam->blocknum);
								}
								if (bRet){
									string o_data = "";
									if(kt_IRPD_readBlock != 0){
										o_data = kt_IRPD_readBlock(devParam->blocknum);
									}
									if (o_data != ""){	
										if(kt_IRPD_Beep != NULL){
											kt_IRPD_Beep(10);
										}
										#if defined(__linux__)
										sleep(1); 
										#else
										Sleep(1000);
										#endif

										Json::Value root;
										Json::FastWriter writer;  
										root["cardno"] = o_data;
										devParam->resultjs = writer.write(root);
										uv_async_send(devParam->callbackSendReq); 									
									}
								}
							}
						}
					}
				}
				else if(devParam->subDev == 2)
				{
					int status;
					WORD icdev = 0x0000;
					unsigned char mode = 0x52;
					unsigned short TagType;
					unsigned char bcnt = 0x04;//mifare 卡都用4
					unsigned char Snr[MAX_RF_BUFFER];
					unsigned char len;
					unsigned char Size;
					status = kt_rf_request(icdev, mode, &TagType);					
					if(status == 0){
						status = kt_rf_anticoll(icdev, bcnt, Snr, &len);
						if(status == 0 && len == 4){
							//kt_rf_select(icdev, Snr, len, &Size);
							string o_data = "";
							for(int i=0; i<len; i++){
								char buf[10];
								sprintf(buf, "%02x", Snr[i]);
								o_data.append(buf);
							}
							if(o_data != "00000000"){
								kt_rf_light(icdev, 0);							
								Json::Value root;
								Json::FastWriter writer;  
								root["cardno"] = o_data;
								devParam->resultjs = writer.write(root);
								uv_async_send(devParam->callbackSendReq); 		
								kt_rf_light(icdev, 1);	
							}							
						}
					}
				}
				else if(devParam->subDev == 0)
				{	//rfm
					//cout<<"icCard enable"<<endl;
					bool bRet; char bufdata[33] = {0};
					bRet = kt_IRFM_operateAntenna(true);
					if(bRet){
						string res = kt_IRFM_searchCard(0);
						if(res != "")
						{
							memset(buf, 0x00, sizeof(buf));
							memcpy(buf, res.data(), res.size());
							string serial = kt_RFM_preventConflict();
							if(serial != "")
							{
								if(m_serial_Type == 1){
									memset(bufdata, 0x00, sizeof(bufdata)); 
									//memcpy(bufdata, serial.c_str(), MINFUNCTION(serial.length(), 4));
									for(int q=0; q<serial.length(); q++)
									{
										char tmp[3];
										sprintf(tmp, "%02x", (unsigned char)serial[q]);
										memcpy(bufdata+(q*2), tmp, 2);
									}
									kt_RFM_beer(2);
									kt_RFM_beer(1);
									Json::Value root;
									Json::FastWriter writer;  
									root["cardno"] = bufdata;
									devParam->resultjs = writer.write(root);
									uv_async_send(devParam->callbackSendReq);

									#if defined(__linux__)
									usleep(100); 
									#else
									Sleep(100);
									#endif
								}
								else{
									string str=kt_RFM_selectCard(serial);
									if(str != "")
									{
										bRet = kt_RFM_verifyPassWord(1, devParam->blocknum);
										if(bRet)
										{
											string o_data=kt_RFM_readBlock(devParam->blocknum);
											if(o_data != "")
											{
												memset(bufdata, 0x00, sizeof(bufdata)); 
												memcpy(bufdata, o_data.c_str(), MINFUNCTION(o_data.length(), 32));
												kt_RFM_beer(2);
												kt_RFM_beer(1);
												Json::Value root;
												Json::FastWriter writer;  
												root["cardno"] = bufdata;
												devParam->resultjs = writer.write(root);
												uv_async_send(devParam->callbackSendReq);

												#if defined(__linux__)
												usleep(100); 
												#else
												Sleep(100);
												#endif
											}
										}
									}
								}
							}												
						}
					}
				}	
			} //enable
			else
			{
				//cout<<"icCard disable"<<endl;
				#if defined(__linux__)
				sleep(1); 
				#else
				Sleep(1000);
				#endif
			} //disable			
		} //exit

		if(devParam->subDev == 1){
			lg.LogInfo(1, "callGoThread icCard while break IRPD_cleanUp");
			if(kt_IRPD_cleanUp != 0){
				kt_IRPD_cleanUp();
			}
			rpd_FreeLib();
		}
		else if(devParam->subDev == 0){
			lg.LogInfo(1, "callGoThread icCard while break IRFM_cleanUp");
			kt_IRFM_cleanUp();
			rfm_FreeLib();
		}
		else if(devParam->subDev == 2){
			lg.LogInfo(1, "callGoThread icCard");
			kt_rf_ClosePort();
			mf_FreeLib();
		}
		
		iccard_InT = false;
		lg.LogInfo(1, "callGoThread out thread deviceName:icCard");
	} //icCard
	else if(devParam->deviceName == "traffic")
	{
		lg.LogInfo(1, "callGoThread in thread deviceName:traffic");
		while(!devParam->exit)
		{
			traffic_InT = true;

			Json::Value root;
			Json::FastWriter writer;  

			if(devParam->enable)
			{
				if(!devParam->paylog)
				{
					root["iEventValue"] = 100;
					root["strEventMsg"] = GBKToUTF8("使能中");
					devParam->resultjs = writer.write(root);
					uv_async_send(devParam->callbackSendReq); 	
					
					root["iEventValue"] = 1;
					root["strEventMsg"] = GBKToUTF8("请拍卡... ...");
					devParam->resultjs = writer.write(root);
					uv_async_send(devParam->callbackSendReq); 	
					
					DATACARD dc;
					//lg.LogInfo(0, "读卡命令");
					int ret = -2;
					if(kt_Itcard_readCard != NULL){
						ret = kt_Itcard_readCard(dc);
					}
					if(ret == 0)
					{
						stringstream ss;
						ss.str(""); ss << "卡片类型				 =>" << display1(dc.cardType);
						lg.LogInfo(0, ss.str());
						ss.str(""); ss << "卡内号				 =>" << display2(dc.cardinfo.cardNum, 8);
						lg.LogInfo(0, ss.str());
						//cout<<ss.str()<<endl;
						root["iEventValue"] = 2;
						root["strEventMsg"] = GBKToUTF8(ss.str());
						devParam->resultjs = writer.write(root);
						uv_async_send(devParam->callbackSendReq); 
						ss.str(""); ss << "交通卡类型			 =>" << display1(dc.cardinfo.cardType);
						lg.LogInfo(0, ss.str());
						ss.str(""); ss << "有效期				 =>" << display2(dc.cardinfo.validDate, 4);
						lg.LogInfo(0, ss.str());
						ss.str(""); ss << "卡余额				 =>" << display2(dc.cardinfo.money, 4);
						lg.LogInfo(0, ss.str());
						ss.str(""); ss << "卡状态				 =>" << display1(dc.cardinfo.statu);
						lg.LogInfo(0, ss.str());
						ss.str(""); ss << "进出状态				 =>" << display1(dc.cardinfo.iostatu);
						lg.LogInfo(0, ss.str());
						ss.str(""); ss << "停车场专用进站交易记录 =>" << display2(dc.cardinfo.iTradingrecord, 16);
						lg.LogInfo(0, ss.str());
						ss.str(""); ss << "停车场专用出站交易记录 =>" << display2(dc.cardinfo.oTradingrecord, 16);
						lg.LogInfo(0, ss.str());
						ss.str(""); ss << "RFU					 =>" << display2(dc.cardinfo.rfu, 12);
						lg.LogInfo(0, ss.str());

						root["iEventValue"] = 7;
						root["strEventMsg"] = GBKToUTF8("获取轨道交易记录中");
						devParam->resultjs = writer.write(root);
						uv_async_send(devParam->callbackSendReq); 
						//CARDINFOS infos;
						READCARDPR pr;
						lg.LogInfo(0, "读轨道交易记录命令");
						
						//int ret = Itcard_readLog(0x00, infos);
						int ret = -2;
						if(kt_Itcard_readCard_pr != 0){
							ret = kt_Itcard_readCard_pr(0x00, pr);
						}
						if(ret == 0){							
							Json::Value root1;
							Json::Value row;
							Json::FastWriter outWriter1;  
							string outJson1;
							/*for(int i=0; i<10; i++){
								root1["strEventMsg"].append(display2(infos.cards[i], 16));
							}*/
							char buf[100];
							sprintf(buf, "%02d", pr.itradingrecord[0]);
							row["inFlag"] = buf;
							memset(buf, 0x00, sizeof(buf));
							sprintf(buf, "%02d%02d%02d%02d%02d", pr.itradingrecord[1], pr.itradingrecord[2], pr.itradingrecord[3], pr.itradingrecord[4], pr.itradingrecord[5]);
							row["inTime"] = buf;
							memset(buf, 0x00, sizeof(buf));
							sprintf(buf, "%02d%02d%02d", pr.itradingrecord[6], pr.itradingrecord[7], pr.itradingrecord[8]);
							row["inDevice"] = buf;							
							unsigned short host;
							ntohs1(&pr.itradingrecord[9], host);
							row["inMoney"] = host;
							row["inTimes"] = (int)(pr.itradingrecord[11] && 0xff);
							//row["inAllMoney"] = ;		

							memset(buf, 0x00, sizeof(buf));
							sprintf(buf, "%02d", pr.otradingrecord[0]);							
							row["outFlag"] = buf;
							memset(buf, 0x00, sizeof(buf));
							sprintf(buf, "%02d%02d%02d%02d%02d", pr.otradingrecord[1], pr.otradingrecord[2], pr.otradingrecord[3], pr.otradingrecord[4], pr.otradingrecord[5]);
							row["outTime"] = buf;
							memset(buf, 0x00, sizeof(buf));
							sprintf(buf, "%02d%02d%02d", pr.otradingrecord[6], pr.otradingrecord[7], pr.otradingrecord[8]);
							row["outDevice"] = buf;
							ntohs1(&pr.otradingrecord[9], host);
							row["outMoney"] = host;
							row["outTimes"] = (int)(pr.otradingrecord[11] && 0xff);
							//row["outAllMoney"] = ;
							root1["strEventMsg"] = row;
							root1["iEventValue"] = 8;
							outJson1 = outWriter1.write(root1);
							lg.LogInfo(0, outJson1);
		
							devParam->resultjs = outJson1;
							uv_async_send(devParam->callbackSendReq); 
							devParam->paylog = true;
							lg.LogInfo(0, "获取轨道交易记录成功=>");
							lg.LogInfo(0, outJson1.c_str());
						}
						else{
							root["iEventValue"] = 9;
							root["strEventMsg"] = GBKToUTF8("获取轨道交易记录失败");
							lg.LogInfo(0, "获取轨道交易记录失败");
							devParam->resultjs = writer.write(root);
							uv_async_send(devParam->callbackSendReq); 
						}
					}
					else
					{
						root["iEventValue"] = 3;
						root["strEventMsg"] = GBKToUTF8("拍卡失败");
						devParam->resultjs = writer.write(root);
						uv_async_send(devParam->callbackSendReq); 
					}						
				}
				
				if(!devParam->payed)
				{					
					root["iEventValue"] = 100;
					root["strEventMsg"] = GBKToUTF8("使能中");
					devParam->resultjs = writer.write(root);
					uv_async_send(devParam->callbackSendReq); 	
					
					root["iEventValue"] = 1;
					root["strEventMsg"] = GBKToUTF8("请拍卡... ...");
					devParam->resultjs = writer.write(root);
					uv_async_send(devParam->callbackSendReq); 	
					
					DATACARD dc;
					//lg.LogInfo(0, "读卡命令");
					int ret = -2;
					if(kt_Itcard_readCard != 0){
						ret = kt_Itcard_readCard(dc);
					}
						if(ret == 0){
							stringstream ss;
							ss.str(""); ss << "卡片类型				 =>" << display1(dc.cardType);
							lg.LogInfo(0, ss.str());
							ss.str(""); ss << "卡内号				 =>" << display2(dc.cardinfo.cardNum, 8);
							lg.LogInfo(0, ss.str());
							//cout<<ss.str()<<endl;
							root["iEventValue"] = 2;
							root["strEventMsg"] = GBKToUTF8(ss.str());
							devParam->resultjs = writer.write(root);
							uv_async_send(devParam->callbackSendReq); 
							ss.str(""); ss << "交通卡类型			 =>" << display1(dc.cardinfo.cardType);
							lg.LogInfo(0, ss.str());
							ss.str(""); ss << "有效期				 =>" << display2(dc.cardinfo.validDate, 4);
							lg.LogInfo(0, ss.str());
							ss.str(""); ss << "卡余额				 =>" << display2(dc.cardinfo.money, 4);
							lg.LogInfo(0, ss.str());
							ss.str(""); ss << "卡状态				 =>" << display1(dc.cardinfo.statu);
							lg.LogInfo(0, ss.str());
							ss.str(""); ss << "进出状态				 =>" << display1(dc.cardinfo.iostatu);
							lg.LogInfo(0, ss.str());
							ss.str(""); ss << "停车场专用进站交易记录=>" << display2(dc.cardinfo.iTradingrecord, 16);
							lg.LogInfo(0, ss.str());
							ss.str(""); ss << "停车场专用出站交易记录=>" << display2(dc.cardinfo.oTradingrecord, 16);
							lg.LogInfo(0, ss.str());
							ss.str(""); ss << "RFU					 =>" << display2(dc.cardinfo.rfu, 12);
							lg.LogInfo(0, ss.str());

							//setPaying(1);
							root["iEventValue"] = 4;
							root["strEventMsg"] = GBKToUTF8("交易中");
							devParam->resultjs = writer.write(root);
							uv_async_send(devParam->callbackSendReq); 
							TRADEINFO ti;
							lg.LogInfo(0, "写卡命令");

							time_t tt = time(NULL);
							struct tm t;
							localtime_s(&t, &tt);
							//YyyymmddhhMMSS的7位bcd码
							int year = t.tm_year + 1900;
							char datetime[7];
							datetime[0] = (unsigned char)(((year / 100) / 10) * 16 + ((year / 100) % 10));
							datetime[1] = (unsigned char)((((year % 100) / 10) * 16) + ((year % 100) % 10));
							datetime[2] = (t.tm_mon + 1) / 10 * 16 + (t.tm_mon + 1) % 10;
							datetime[3] = t.tm_mday / 10 * 16 + t.tm_mday % 10;
							datetime[4] = t.tm_hour / 10 * 16 + t.tm_hour % 10;
							datetime[5] = t.tm_min / 10 * 16 + t.tm_min % 10;
							datetime[6] = t.tm_sec / 10 * 16 + t.tm_sec % 10;
							char serial[4];
							htonl(devParam->m_serial, serial);
							int updateFlag = 0x00;
							int ret = -2;
							if(kt_Itcard_operateCard != 0){
								ret = kt_Itcard_operateCard(datetime, serial, devParam->money, updateFlag, ti);
							}
							if(ret == 0){
								devParam->m_serial++;
								if(devParam->m_serial > 0xffffffff){
									devParam->m_serial = 1;
								}

								string strTradeinfo = display2(ti.tradeinfo, 240);
								stringstream ss;
								ss.str(""); ss << "交易信息=>" << strTradeinfo;
								lg.LogInfo(0, ss.str());
								//cout<<strTradeinfo<<endl;
								root["iEventValue"] = 5;
								root["strEventMsg"] = GBKToUTF8(ss.str());;
								devParam->resultjs = writer.write(root);
								uv_async_send(devParam->callbackSendReq); 
								devParam->payed = 1;
							}
							else{
								root["iEventValue"] = 6;
								root["strEventMsg"] = GBKToUTF8("交易失败");
								ss.str(""); ss << "交易失败";
								lg.LogInfo(0, ss.str());
								devParam->resultjs = writer.write(root);
								uv_async_send(devParam->callbackSendReq); 
							}
							//setPaying(0);
						}
						else{
							root["iEventValue"] = 3;
							root["strEventMsg"] = GBKToUTF8("拍卡失败");
							devParam->resultjs = writer.write(root);
							uv_async_send(devParam->callbackSendReq); 
						}
				}				
			} //enable
			else
			{
				root["iEventValue"] = 101;
				root["strEventMsg"] = GBKToUTF8("禁能中");
				//devParam->resultjs = writer.write(root);
				//uv_async_send(devParam->callbackSendReq); 	
			} //disable	
			
			#if defined(__linux__)
			sleep(1); 
			#else
			Sleep(1000);
			#endif
		} //exit

		{
			lg.LogInfo(1, "callGoThread traffic while break Itcard_cleanUp");
			if(kt_Itcard_cleanUp != 0){
				kt_Itcard_cleanUp();
				traffic_FreeLib();
			}
		}		
		
		traffic_InT = false;
		lg.LogInfo(1, "callGoThread out thread deviceName:traffic");
	}
	else if(devParam->deviceName == "sensor")
	{		
		if(devParam->subDev == 0){
			lg.LogInfo(1, "callGoThread in thread deviceName:sensor");
			while(!devParam->exit)
			{
				sensor_InT = true;
				
				if(devParam->enable)
				{
					if(g_sensor != NULL) g_sensor->sensor_read_Status();
				}
			
				#if defined(__linux__)
				usleep(100); 
				#else
				Sleep(100);
				#endif
			}
			
			if(devParam->enable){
				if(g_sensor != NULL) g_sensor->sensor_disable();
			}
			if(g_sensor != NULL) g_sensor->sensor_closeport();
			
			sensor_InT = false;
			lg.LogInfo(1, "callGoThread out thread deviceName:sensor");
		}else{
			return;
		}
	}
	else if(devParam->deviceName == "cashin")
	{
		if(devParam->subDev == 1){ //itl-nv9
			lg.LogInfo(1, "callGoThread in thread deviceName:itl-nv9");
			int count = 0;
			while(!devParam->exit)
			{
				cashin_InT = true;
				
				if(devParam->enable)
				{
					if(count > 100)
					{
						if(g_cashin != NULL) g_cashin->cash_in_sync1(true);
						count = 0;
					}
					
					if(g_cashin != NULL) g_cashin->cash_in_poll1();			
				} //enable
				else
				{
				} //disable
				
				#if defined(__linux__)
				usleep(100); 
				#else
				Sleep(100);
				#endif
			} //exit
			
			if(devParam->enable){
				if(g_cashin != NULL) g_cashin->cash_in_disable();
			}
			if(g_cashin != NULL) g_cashin->cash_in_closeport();
			
			cashin_InT = false;
			lg.LogInfo(1, "callGoThread out thread deviceName:itl-nv9");
		}else if(devParam->subDev == 2){ //ict-v77e		
			lg.LogInfo(1, "callGoThread in thread deviceName:ict-v77e");		
			#if defined(__linux__)
			sleep(3); 
			#else
			Sleep(3000);
			#endif
			int m_nNeedLen = 1;
			int m_tRecvLen = 0;
			int m_tRecvLenLst = 0; 
			memset(m_recvdata, 0x00, sizeof(m_recvdata));
			int m_sendStatus = 1;			
			while(!devParam->exit)
			{
				cashin_InT = true;
				
				if(g_cashin != NULL) g_cashin->cash_in_poll2(&m_nNeedLen, &m_tRecvLen, &m_tRecvLenLst, m_recvdata, &m_sendStatus);			
	
				//20201009 added by yyb
				#if defined(__linux__)
				usleep(200); 
				#else
				Sleep(200);
				#endif				
			}
			
			if(devParam->enable){
				if(g_cashin != NULL) g_cashin->cash_in_disable();
			}
			if(g_cashin != NULL) g_cashin->cash_in_closeport();
			
			cashin_InT = false;
			lg.LogInfo(1, "callGoThread out thread deviceName:ict-v77e");
		}else{
			return;
		}
	} //cashin
}

void freeGoThreadData(DevParams* data) 
{
	if(data == 0){
		lg.LogInfo(1, "freeGoThreadData DevParams is NULL, return");
		return;
	}
	lg.LogInfo(1, "freeGoThreadData 1");
	map<string, boost::shared_ptr<ThreadData>>::iterator it = g_map_DevThreadData.find(data->deviceName);
	if ( it != g_map_DevThreadData.end() ){
		boost::shared_ptr<ThreadData> td = it->second;
		/*ThreadData* td = it->second;
		if(td != 0)
		{
			if(td->req != 0){
				delete td->req;
				td->req = 0; 
			}
			if(td->param != 0){
				if(td->param->callbackSendReq != 0){
					delete td->param->callbackSendReq;
					td->param->callbackSendReq = 0;
				}
				delete td->param;
				td->param = 0;
			}	
			delete td;
			td = 0;
		}*/
	}
	lg.LogInfo(1, "freeGoThreadData 2");
}

void afterGoTread (uv_work_t* req, int status) {
	if(req == 0){
		lg.LogInfo(1, "afterGoTread uv_work_t is NULL, return");
		return;
	}
	lg.LogInfo(1, "afterGoTread 1");
    auto* devParam = (DevParams*) req->data;
    char buf[1024] = {0};
    sprintf(buf, "afterGoTread deviceName:%s", devParam->deviceName.c_str());    
    devParam->status = 2;
    HandleScope handle_scope(Isolate::GetCurrent()); //这里是必须的,调用js函数需要一个handle scope
    uv_close((uv_handle_t*) devParam->callbackSendReq, close_cb); //3.删除通知js的事件
    freeGoThreadData(devParam); //释放内存
	lg.LogInfo(1, "afterGoTread 2");
}

void Add(const FunctionCallbackInfo<Value>& args) 
{
  Isolate* isolate = args.GetIsolate();
  //Local<Context> context = isolate->GetCurrentContext(); 

  if (args.Length() < 2) {
	//isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "参数数量错误", NewStringType::kNormal).ToLocalChecked()));
	return;
  }

  if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
	//isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "参数错误", NewStringType::kNormal).ToLocalChecked()));
	return;
  }

  double value = args[0].As<Number>()->Value() + args[1].As<Number>()->Value();
  Local<Number> num = Number::New(isolate, value);

  char str[100] = {0};
  sdk::Test(str); //call dll/so
  //cout<<str<<endl;
	
  args.GetReturnValue().Set(num);
}

static void HardWareEnd(const FunctionCallbackInfo<Value>& args) 
{
	lg.LogInfo(1, "HardWareEnd =>");
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	//exit thread
	lg.LogInfo(1, "HardWareEnd => 1");
	map<string, boost::shared_ptr<ThreadData>>::iterator it = g_map_DevThreadData.find("icCard");
	if(it != g_map_DevThreadData.end())
	{
		lg.LogInfo(1, "HardWareEnd => ready to exit icCard thread");
		boost::shared_ptr<ThreadData> td = it->second;
		if(td.get() != NULL)
		{
			if(!td->param->exit) td->param->exit = true;
		}
		lg.LogInfo(1, "HardWareEnd => wait for exit icCard thread");
		while(1){
			if(!iccard_InT)break;
			#if defined(__linux__)
			usleep(100); 
			#else
			Sleep(100);
			#endif
		}
		lg.LogInfo(1, "HardWareEnd => exited icCard thread.");
		
		g_map_DevThreadData.erase(it);
		
		#if defined(__linux__)
		usleep(500); 
		#else
		Sleep(500);
		#endif
	}
	
	map<string, boost::shared_ptr<ThreadData>>::iterator itTraffic = g_map_DevThreadData.find("traffic");
	if(itTraffic != g_map_DevThreadData.end())
	{
		lg.LogInfo(1, "HardWareEnd => ready to exit traffic thread");
		boost::shared_ptr<ThreadData> td = itTraffic->second;
		if(td.get() != NULL)
		{
			if(!td->param->exit) td->param->exit = true;
		}
		lg.LogInfo(1, "HardWareEnd => wait for exit traffic thread");
		while(1){
			if(!traffic_InT)break;
			#if defined(__linux__)
			usleep(100); 
			#else
			Sleep(100);
			#endif
		}
		lg.LogInfo(1, "HardWareEnd => exited traffic thread.");
		
		g_map_DevThreadData.erase(itTraffic);
	}
	
	
	//exit thread by delete object
	/*if(g_cashin != NULL){
		delete g_cashin;
		g_cashin = NULL; 
	}
	if(g_sensor != NULL){
		delete g_sensor;
		g_sensor = NULL;
	}*/
	lg.LogInfo(1, "HardWareEnd => 2");
	map<string, boost::shared_ptr<ThreadData>>::iterator it1 = g_map_DevThreadData.find("sensor");
	if(it1 != g_map_DevThreadData.end())
	{
		lg.LogInfo(1, "HardWareEnd => ready to exit sensor thread");
		boost::shared_ptr<ThreadData> td = it1->second;
		if(td != NULL)
		{
			if(!td->param->exit) td->param->exit = true;
		}
		lg.LogInfo(1, "HardWareEnd => wait for exit sensor thread");
		while(1){
			if(!sensor_InT)break;
			#if defined(__linux__)
			usleep(100); 
			#else
			Sleep(100);
			#endif
		}
		lg.LogInfo(1, "HardWareEnd => exited sensor thread.");
		
		g_map_DevThreadData.erase(it1);
		
		#if defined(__linux__)
		usleep(500); 
		#else
		Sleep(500);
		#endif
	}
	if(g_sensor != NULL){
		delete g_sensor; //
		g_sensor = NULL;
	}

	lg.LogInfo(1, "HardWareEnd => 3");
	map<string, boost::shared_ptr<ThreadData>>::iterator it2 = g_map_DevThreadData.find("cashin");
	if(it2 != g_map_DevThreadData.end())
	{
		boost::shared_ptr<ThreadData> td = it2->second;
		lg.LogInfo(1, "HardWareEnd => ready to exit cashin thread");
		if(td != NULL)
		{
			if(td->param->subDev == 2){
				#if defined(__linux__)
				sleep(4); 
				#else
				Sleep(4000);
				#endif
			}
			if(!td->param->exit) td->param->exit = true;
		}
		lg.LogInfo(1, "HardWareEnd => wait for exit cashin thread");
		while(1){
			if(!cashin_InT)break;
			#if defined(__linux__)
			usleep(100); 
			#else
			Sleep(100);
			#endif
		}
		lg.LogInfo(1, "HardWareEnd => exited cashin thread.");
		
		g_map_DevThreadData.erase(it2);
		
		#if defined(__linux__)
		usleep(500); 
		#else
		Sleep(500);
		#endif
	}
	if(g_cashin != NULL){
		delete g_cashin; //
		g_cashin = NULL; 
	}
	
	lg.LogInfo(1, "HardWareEnd => 4");
	if(g_epson_init){
		//cout<<"lib_printer_end"<<endl;
		//lib_printer_end();
		kt_lib_printer_end();
		prt_FreeLib();
	}
	if(g_lock_init){ //2020-12-11
		//IELECTLOCK_cleanUp(); 
		kt_IELECTLOCK_cleanUp();
		lock_FreeLib();
	}
	
	Json::Value root;
	Json::FastWriter write;	
	std::string json_out = "";
	root["result_code"] = 0;
	root["result_msg"] = "success";
	json_out = write.write(root);
	
	char displaybuf[1024];
	sprintf(displaybuf, "HardWareEnd result:%s", json_out.c_str());
	lg.LogInfo(1, displaybuf);

#if defined(__linux__)
#else
	json_out = GBKToUTF8(json_out);
#endif

	auto result = v8::String::NewFromUtf8(isolate, json_out.c_str(), v8::NewStringType::kNormal, (int)json_out.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}

void HardWareInit(const FunctionCallbackInfo<Value>& args) 
{
	/*{
		Json::Value root;
		Json::Value rows;
		Json::Value row;
		Json::FastWriter outWriter;  
		string outJson;
		//for(int i=0; i<10; i++){
		//	row[""] = "00 01 02 30 34 35";
		//	rows.append(row);
		//}
		//root["traffic_error_msg"] = rows;
		root["traffic_error_msg"].append("00 01 02 30 34 35");
		root["traffic_error_msg"].append("00 01 02 30 34 36");
		root["iEventValue"] = 8;
		outJson = outWriter.write(root);
		cout<<outJson<<endl;
		Sleep(50000);
	}*/
				
	/*#if defined(__linux__)
	sleep(5); 
	#else
	Sleep(5000);
	#endif*/
			
  Isolate* isolate = args.GetIsolate();
  Local<Context> context = isolate->GetCurrentContext();  

  if (args.Length() < 9) {
	//isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "参数数量错误", NewStringType::kNormal).ToLocalChecked()));
	return;
  } 
  	
  //out json  
  Json::Value outRoot;
  Json::Value outData;
  Json::Value outdevcoin;
  Json::Value outdevcashin;
  Json::Value outdevsensor;
  Json::Value outdevlock; //2020-12-11
  Json::Value outdevcashout;
  Json::Value outdevprinter;
  Json::Value outdevunion;
  Json::Value outdeviCcard;
  Json::Value outdevtraffic;
  Json::FastWriter outWriter;  
  string outJson;
  outRoot["result_code"] = 0;
  outRoot["result_msg"] = "success";

  //v8::String::Utf8Value execPath(args[0]->ToString());  
  Local<Value> v = args[0]; 
  v8::String::Utf8Value execPath(isolate, v); 
  
  stringstream ss;
  ss << *execPath;
  
  stringstream ss2;
  v8::String::Utf8Value callbackJSFile(isolate, args[1]); 
  ss2 << *callbackJSFile;  
  
  v8::String::Utf8Value dllpath(isolate, args[11]);
  stringstream ss7;
  ss7 << *dllpath;

  v8::String::Utf8Value logpath(isolate, args[3]); 
  stringstream ss4;
  ss4 << *logpath;
  //cout<<"logpath:"<<ss4.str().c_str()<<endl;
  if(!glogInit){
	  glogInit = true;
	  //LOGINIT(ss4.str().c_str(), ""); //sdkaddon.node
	  lg.setParams(ss4.str().c_str(), "");
	  lg.init();
  }
  lg.LogInfo(1, "HardWareInit =>");
  lg.LogInfo(1, "HardWareInit VER:KT_NJGYPCC_WIN_1.0.9");
  
  lg.LogInfo(1, "dll path:");
  lg.LogInfo(1, ss7.str().c_str());
  
  v8::String::Utf8Value serverdatetime(isolate, args[4]); 
  stringstream ss5;
  ss5 << *serverdatetime;
  //cout<<"serverdatetime:"<<ss5.str().c_str()<<endl;

  //v8插件
  //HardWareApp::start(isolate, context, ss.str().c_str(), "", ss2.str().c_str());  
  
  //in json
  v8::String::Utf8Value inijs(isolate, args[2]); 
  Json::Reader reader;
  Json::Value  root;
  Json::Value  jsonVal_coin;  
  int m_coin_type=-1;
  int m_national_cny=-1;
  char m_serial_coin_in[32] = {0};
  int m_include_coin_out_half = -1;
  int m_include_coin_out_one = -1;
  int m_box_coin_out_one = -1;
  int m_serial_coin_out_half = -1;
  int m_serial_coin_out_one = -1;
  Json::Value  jsonVal_cashin;
  int m_cash_in_type = -1;
  char m_serial_cash_in[32] = {0};
  int m_cash_accept = -1;
  int m_cash_model = -1;
  Json::Value	jsonVal_sensor;
  int m_SensorDev_Type = -1;
  char m_serial_sensor[32] = {0};
  Json::Value	jsonVal_lock; //2020-12-11
  int m_LockDev_Type = -1;
  char m_serial_lock[32] = {0};
  Json::Value  jsonVal_cashout;
  int m_cash_out_type = -1;
  char m_serial_cash_out[32] = {0};
  int m_cashbox_num = -1;
  int m_cashbox0_money_type = -1;
  int m_cashbox1_money_type = -1;
  int m_cashbox2_money_type = -1;
  int m_cashbox3_money_type = -1;
  int m_cashbox4_money_type = -1;
  int m_cashbox5_money_type = -1;
  int m_cashbox0_ID = -1;
  int m_cashbox1_ID = -1;
  int m_cashbox2_ID = -1;
  int m_cashbox3_ID = -1;
  int m_cashbox4_ID = -1;
  int m_cashbox5_ID = -1;
  int m_country_currentcy = -1;
  Json::Value  jsonVal_printer;
  int m_printer_type = -1;
  char m_serial_printer[32] = {0};
  int m_baudrate_printer = -1;
  Json::Value  jsonVal_union;
  int m_union_type = -1;
  char m_serial_union[32] = {0};
  int m_union_zhongyu = -1;
  int m_union_BsweepC = -1;
  Json::Value  jsonVal_iccard;
  static int m_iCcard_type = -1;
  char m_serial_iccard[32] = {0};
  Json::Value  jsonVal_traffic;
  int m_traffic_type = -1;
  char m_serial_traffic[32] = {0};
  int m_industrycode_traffic = 1;
  int m_usercode_traffic = 1;
  int m_deviceid_traffic =1;
  int m_units_traffic = 1;
  Json::Value  jsonVal_system;
  string m_authorization_code = "";
  //cout<<"1"<<endl;
  stringstream ss6;
  ss6 << *inijs;
  char displaybuf[10240];
  sprintf(displaybuf, "HardWareInit inijs:%s", ss6.str().c_str());
  lg.LogInfo(1, displaybuf);  
  
  if(reader.parse(*inijs, root))
  {
	  //cout<<"2"<<endl;
	  jsonVal_system = root["System"];
	  m_authorization_code = jsonVal_system["authorization_code"].asString();
	  //coin
	  jsonVal_coin = root["CoinDevType"];
	  m_coin_type = atoi(jsonVal_coin["coin_type"].asString().c_str());
	  m_national_cny = atoi(jsonVal_coin["national_cny"].asString().c_str());
	  if(m_coin_type != -1){
	  }else{
		  //outRoot["result_code"] = -1;
		  //outRoot["result_msg"] = "硬件初始化失败！";
		  outdevcoin["dev_name"] = "coin";
		  outdevcoin["init_result"] = false;
		  outdevcoin["init_msg"] = "不包含硬币器模块"; 
		  outData.append(outdevcoin);
	  }
	  //printer
	  jsonVal_printer = root["PrinterDevType"];
	  m_printer_type = atoi(jsonVal_printer["printer_type"].asString().c_str()); //3一代 4二代	 
	  if(m_printer_type == 3)
		  g_epson_type = 1;
	  else if(m_printer_type == 4)
		  g_epson_type = 2;
	  else if(m_printer_type == 6) //2020-08-12
		  g_epson_type = 3;
      strcpy(m_serial_printer, jsonVal_printer["serial_printer"].asString().c_str());
      m_baudrate_printer = atoi(jsonVal_printer["baudrate_printer"].asString().c_str());
	  if(m_printer_type != -1){
		  int loadPrtDll = prt_loadDll(ss7.str().c_str());
		  if(loadPrtDll == 0){
			  outRoot["result_code"] = -1;
			  outRoot["result_msg"] = "硬件初始化失败！";
			  outdevprinter["dev_name"] = "printer";
			  outdevprinter["init_result"] = false;
			  outdevprinter["init_msg"] = "打印机库加载失败"; 
			  outData.append(outdevprinter);
		  }
		  else if(loadPrtDll == -1){
			  outRoot["result_code"] = -1;
			  outRoot["result_msg"] = "硬件初始化失败！";
			  outdevprinter["dev_name"] = "printer";
			  outdevprinter["init_result"] = false;
			  outdevprinter["init_msg"] = "打印机库函数加载失败"; 
			  outData.append(outdevprinter);
		  }
		  else{
			  g_epson_init = kt_lib_printer_init(0, m_serial_printer, m_baudrate_printer, ss4.str().c_str());
			  if (!g_epson_init)
			  {
				  outRoot["result_code"] = -1;
				  outRoot["result_msg"] = "硬件初始化失败！";
				  outdevprinter["dev_name"] = "printer";
				  outdevprinter["init_result"] = false;
				  outdevprinter["init_msg"] = "打印机初始化失败"; 
				  outData.append(outdevprinter);
			  }
			  else
			  {
				  outdevprinter["dev_name"] = "printer";
				  outdevprinter["init_result"] = true;
				  outdevprinter["init_msg"] = "打印机初始化成功";
				  outData.append(outdevprinter);
			  }
		  }
	  }else{
		  //outRoot["result_code"] = -1;
		  //outRoot["result_msg"] = "硬件初始化失败！";
		  outdevprinter["dev_name"] = "printer";
		  outdevprinter["init_result"] = false;
		  outdevprinter["init_msg"] = "不包含打印机模块"; 
		  outData.append(outdevprinter);
	  }
	  //traffic
	  jsonVal_traffic = root["TrafficDevType"];
	  m_traffic_type = atoi(jsonVal_traffic["traffic_type"].asString().c_str());
	  strcpy(m_serial_traffic, jsonVal_traffic["serial_traffic"].asString().c_str());
	  m_industrycode_traffic = atoi(jsonVal_traffic["industrycode_traffic"].asString().c_str());
      m_usercode_traffic = atoi(jsonVal_traffic["usercode_traffic"].asString().c_str());
      m_deviceid_traffic = atoi(jsonVal_traffic["deviceid_traffic"].asString().c_str());
      m_units_traffic = atoi(jsonVal_traffic["units_traffic"].asString().c_str());
	  
	  if(m_traffic_type == 0){
		int trafficInitDll = traffic_loadDll(ss7.str().c_str());
		if(trafficInitDll == 0){
			outRoot["result_code"] = -1;
			outRoot["result_msg"] = "硬件初始化失败！";
			outdevtraffic["dev_name"] = "traffic";
			outdevtraffic["init_result"] = false;
			outdevtraffic["init_msg"] = "交通卡库加载失败"; 
			outData.append(outdevtraffic);
		}
		else if(trafficInitDll == -1){
			outRoot["result_code"] = -1;
			outRoot["result_msg"] = "硬件初始化失败！";
			outdevtraffic["dev_name"] = "traffic";
			outdevtraffic["init_result"] = false;
			outdevtraffic["init_msg"] = "交通卡库函数加载失败"; 
			outData.append(outdevtraffic);
		}
		else{
			char industrycode = (char)(m_industrycode_traffic&0xff);
			char usercode[] = {0x00, (char)(m_usercode_traffic&0xff)};
			char deviceid = (char)(m_deviceid_traffic&0xff);
			char units[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, (char)(m_units_traffic&0xff)};
			time_t tt = time(NULL);
			struct tm t;
			localtime_s(&t, &tt);
			//YyyymmddhhMMSS的7位bcd码
			int year = t.tm_year + 1900;
			char devicetime[7];
			devicetime[0] = (unsigned char)(((year / 100) / 10) * 16 + ((year / 100) % 10));
			devicetime[1] = (unsigned char)((((year % 100) / 10) * 16) + ((year % 100) % 10));
			devicetime[2] = (t.tm_mon + 1) / 10 * 16 + (t.tm_mon + 1) % 10;
			devicetime[3] = t.tm_mday / 10 * 16 + t.tm_mday % 10;
			devicetime[4] = t.tm_hour / 10 * 16 + t.tm_hour % 10;
			devicetime[5] = t.tm_min / 10 * 16 + t.tm_min % 10;
			devicetime[6] = t.tm_sec / 10 * 16 + t.tm_sec % 10;
			if(kt_Itcard_init(0, m_serial_traffic, ss4.str().c_str(), industrycode, usercode, deviceid, units, devicetime) == 0)
			{
				 outdevtraffic["dev_name"] = "traffic";
				 outdevtraffic["init_result"] = true;
				 outdevtraffic["init_msg"] = "交通卡初始化成功";
				 outData.append(outdevtraffic);
			}
			else
			{
				outRoot["result_code"] = -1;
				outRoot["result_msg"] = "硬件初始化失败！";
				outdevtraffic["dev_name"] = "traffic";
				outdevtraffic["init_result"] = false;
				outdevtraffic["init_msg"] = "交通卡初始化失败"; 
				outData.append(outdevtraffic);
			}
		}
	  }else{
		//outRoot["result_code"] = -1;
	   	//outRoot["result_msg"] = "硬件初始化失败！";
	   	outdevtraffic["dev_name"] = "traffic";
	   	outdevtraffic["init_result"] = false;
	   	outdevtraffic["init_msg"] = "不包含交通卡模块"; 
	   	outData.append(outdevtraffic);	 
	  }
	  //iccard
	  jsonVal_iccard = root["ICcardDevType"];
	  m_iCcard_type = atoi(jsonVal_iccard["iCcard_type"].asString().c_str());
	  strcpy(m_serial_iccard, jsonVal_iccard["serial_iccard"].asString().c_str());	
	  m_serial_Type = atoi(jsonVal_iccard["serial_Type"].asString().c_str());
	  unsigned char *keya_[16]; unsigned char *keyb_[16];	
	  if(m_iCcard_type == 1){
		  int rpdInitDll = rpd_loadDll(ss7.str().c_str());
		  if(rpdInitDll == 0){
			 outRoot["result_code"] = -1;
			 outRoot["result_msg"] = "硬件初始化失败！";
			 outdeviCcard["dev_name"] = "icCard";
			 outdeviCcard["init_result"] = false;
			 outdeviCcard["init_msg"] = "IC读卡器库加载失败"; 
			 outData.append(outdeviCcard);
		  }
		  else if(rpdInitDll == -1){
			 outRoot["result_code"] = -1;
			 outRoot["result_msg"] = "硬件初始化失败！";
			 outdeviCcard["dev_name"] = "icCard";
			 outdeviCcard["init_result"] = false;
			 outdeviCcard["init_msg"] = "IC读卡器库函数加载失败"; 
			 outData.append(outdeviCcard);
		  }
		  else{
			  if(kt_IRPD_init(0, m_serial_iccard, keya_, keyb_, 0, ss4.str().c_str()) == 0)
			  {
				 outdeviCcard["dev_name"] = "icCard";
				 outdeviCcard["init_result"] = true;
				 outdeviCcard["init_msg"] = "IC读卡器初始化成功";
				 outData.append(outdeviCcard);
			  }
			  else
			  {
				 outRoot["result_code"] = -1;
				 outRoot["result_msg"] = "硬件初始化失败！";
				 outdeviCcard["dev_name"] = "icCard";
				 outdeviCcard["init_result"] = false;
				 outdeviCcard["init_msg"] = "IC读卡器初始化失败"; 
				 outData.append(outdeviCcard);
				 //IRPD_cleanUp();
			  }		 
		  }			  
	  }else if(m_iCcard_type == 2){
		int mfInitDll = mf_loadDll(ss7.str().c_str());
		if(mfInitDll == 0){
			outRoot["result_code"] = -1;
			outRoot["result_msg"] = "硬件初始化失败！";
			outdeviCcard["dev_name"] = "icCard";
			outdeviCcard["init_result"] = false;
			outdeviCcard["init_msg"] = "IC读卡器库加载失败"; 
			outData.append(outdeviCcard);
		}
		else if(mfInitDll == -1){
		 outRoot["result_code"] = -1;
		 outRoot["result_msg"] = "硬件初始化失败！";
		 outdeviCcard["dev_name"] = "icCard";
		 outdeviCcard["init_result"] = false;
		 outdeviCcard["init_msg"] = "IC读卡器库函数加载失败"; 
		 outData.append(outdeviCcard);
		}
		else{
			int m_serial_iccard_int = atoi(m_serial_iccard);
			int ret = -1;
			ret = kt_rf_init_com(m_serial_iccard_int, 19200);
			cout<<"kt_rf_init_com ret=" <<ret <<endl;
			if((ret == 0) && (kt_rf_antenna_sta(0x0000, 1) == 0))
			{
				 outdeviCcard["dev_name"] = "icCard";
				 outdeviCcard["init_result"] = true;
				 outdeviCcard["init_msg"] = "IC读卡器初始化成功";
				 outData.append(outdeviCcard);
			}
			else
			{
				 kt_rf_ClosePort();
				 outRoot["result_code"] = -1;
				 outRoot["result_msg"] = "硬件初始化失败！";
				 outdeviCcard["dev_name"] = "icCard";
				 outdeviCcard["init_result"] = false;
				 outdeviCcard["init_msg"] = "IC读卡器初始化失败"; 
				 outData.append(outdeviCcard);
			}
		}
	  }else if(m_iCcard_type == 0){
		int rfmInitDll = rfm_loadDll(ss7.str().c_str());
		if(rfmInitDll == 0){
		 outRoot["result_code"] = -1;
		 outRoot["result_msg"] = "硬件初始化失败！";
		 outdeviCcard["dev_name"] = "icCard";
		 outdeviCcard["init_result"] = false;
		 outdeviCcard["init_msg"] = "IC读卡器库加载失败"; 
		 outData.append(outdeviCcard);
		}
		else if(rfmInitDll == -1){
		 outRoot["result_code"] = -1;
		 outRoot["result_msg"] = "硬件初始化失败！";
		 outdeviCcard["dev_name"] = "icCard";
		 outdeviCcard["init_result"] = false;
		 outdeviCcard["init_msg"] = "IC读卡器库函数加载失败"; 
		 outData.append(outdeviCcard);
		}
		else{
			if(kt_IRFM_init(0, m_serial_iccard, keya_, keyb_, 0, ss4.str().c_str()) == 0)
			{
				 outdeviCcard["dev_name"] = "icCard";
				 outdeviCcard["init_result"] = true;
				 outdeviCcard["init_msg"] = "IC读卡器初始化成功";
				 outData.append(outdeviCcard);
			}
			else
			{
				 outRoot["result_code"] = -1;
				 outRoot["result_msg"] = "硬件初始化失败！";
				 outdeviCcard["dev_name"] = "icCard";
				 outdeviCcard["init_result"] = false;
				 outdeviCcard["init_msg"] = "IC读卡器初始化失败"; 
				 outData.append(outdeviCcard);
				 //IRFM_cleanUp();
			}
		}
	  }else{
		//outRoot["result_code"] = -1;
	   	//outRoot["result_msg"] = "硬件初始化失败！";
	   	outdeviCcard["dev_name"] = "icCard";
	   	outdeviCcard["init_result"] = false;
	   	outdeviCcard["init_msg"] = "不包含读卡器模块"; 
	   	outData.append(outdeviCcard);			   
	  }
	  //lock
	  jsonVal_lock = root["LockDevType"]; //2020-12-11
	  m_LockDev_Type = atoi(jsonVal_lock["LockDev_Type"].asString().c_str());
	  strcpy(m_serial_lock, jsonVal_lock["serial_lock"].asString().c_str());
	  int loadLockDll;
	  if(m_LockDev_Type != -1)
	  {
		  switch(m_LockDev_Type)
		  {
			  case 0:
			  loadLockDll = lock_loadDll(ss7.str().c_str());
			  if(loadLockDll == 0){
				  outRoot["result_code"] = -1;
				  outRoot["result_msg"] = "硬件初始化失败！";
				  outdevlock["dev_name"] = "sensor";
				  outdevlock["init_result"] = false;
				  outdevlock["init_msg"] = "锁库加载失败"; 
				  outData.append(outdevlock);
			  }
			  else if(loadLockDll == -1){
				  outRoot["result_code"] = -1;
				  outRoot["result_msg"] = "硬件初始化失败！";
				  outdevlock["dev_name"] = "sensor";
				  outdevlock["init_result"] = false;
				  outdevlock["init_msg"] = "锁库函数加载失败"; 
				  outData.append(outdevlock);
			  }
			  else{
				  if(kt_IELECTLOCK_init(m_serial_lock, ss4.str().c_str()) == 0)
				  {
					  outdevlock["dev_name"] = "lock";
					  outdevlock["init_result"] = true;
					  outdevlock["init_msg"] = "锁初始化成功"; 
					  outData.append(outdevlock);
					  g_lock_init = true;
				  }
				  else
				  {
					  outRoot["result_code"] = -1;
					  outRoot["result_msg"] = "硬件初始化失败！";
					  outdevlock["dev_name"] = "sensor";
					  outdevlock["init_result"] = false;
					  outdevlock["init_msg"] = "锁初始化失败"; 
					  outData.append(outdevlock);
				  }
			  }
			  break;
		  }
	  }
	  else
	  {
		  //outRoot["result_code"] = -1;
		  //outRoot["result_msg"] = "硬件初始化失败！";
		  outdevlock["dev_name"] = "lock";
		  outdevlock["init_result"] = false;
		  outdevlock["init_msg"] = "不包含锁模块"; 
		  outData.append(outdevlock);
	  }
	  //sensor
	  jsonVal_sensor = root["SensorDevType"];
	  m_SensorDev_Type = atoi(jsonVal_sensor["SensorDev_Type"].asString().c_str());
	  cout<<"SensorDev_Type="<<m_SensorDev_Type<<endl;
	  strcpy(m_serial_sensor, jsonVal_sensor["serial_sensor"].asString().c_str());
	  cout<<"serial_sensor="<<m_serial_sensor<<endl;
	  bool toInitFlag = false;
	  if(m_SensorDev_Type != -1)
	  {
		  //cout<<"3"<<endl;
		  switch(m_SensorDev_Type)
		  {
			  case 0:
			  g_sensor = new sensor_generation(sensorhandler, m_serial_sensor);
			  toInitFlag = true;
			  break;
			  
			  default:
			  outRoot["result_code"] = -1;
			  outRoot["result_msg"] = "硬件初始化失败！";
			  outdevsensor["dev_name"] = "cashin";
			  outdevsensor["init_result"] = false;
			  outdevsensor["init_msg"] = "未知的传感器"; 
			  outData.append(outdevsensor);
			  break;
		  }
		  //cout<<"4"<<endl;
	  }
	  else
	  {
		  //outRoot["result_code"] = -1;
		  //outRoot["result_msg"] = "硬件初始化失败！";
		  outdevsensor["dev_name"] = "sensor";
		  outdevsensor["init_result"] = false;
		  outdevsensor["init_msg"] = "不包含传感器模块"; 
		  outData.append(outdevsensor);
	  }
	  //cout<<"5"<<endl;
	  if(toInitFlag)
	  {
		  //cout<<"6"<<endl;
		  if(g_sensor == NULL)
		  {
			  outRoot["result_code"] = -18;
			  outRoot["result_msg"] = "传感器模块初始化失败(对象实例化失败)";
			  outdevsensor["dev_name"] = "sensor";
			  outdevsensor["init_result"] = false;
			  outdevsensor["init_msg"] = "传感器实例化失败"; 
			  outData.append(outdevsensor);
		  }
		  else
		  {
			  //cout<<"7"<<endl;
			  int sensor_init_res = g_sensor->sensor_init(ss4.str().c_str(), ss7.str().c_str());
			  //cout<<"8"<<endl;
			  if(sensor_init_res == 0)
			  {
				  outdevsensor["dev_name"] = "sensor";
				  outdevsensor["init_result"] = true;
				  outdevsensor["init_msg"] = "传感器初始化成功"; 
				  outData.append(outdevsensor);
				  //cout<<"9"<<endl;
			  }
			  else
			  {
				  outRoot["result_code"] = -1;
				  outRoot["result_msg"] = "硬件初始化失败！";
				  outdevsensor["dev_name"] = "sensor";
				  outdevsensor["init_result"] = false;
				  outdevsensor["init_msg"] = "传感器初始化失败！"; 
				  outData.append(outdevsensor);
				  //cout<<"10"<<endl;
			  }
		  }
	  }
	  //cashin 
	  jsonVal_cashin = root["CashInDevType"];
	  m_cash_in_type = atoi(jsonVal_cashin["cash_in_type"].asString().c_str());
	  strcpy(m_serial_cash_in, jsonVal_cashin["serial_cash_in"].asString().c_str());
	  m_cash_accept = atoi(jsonVal_cashin["cash_accept"].asString().c_str());
	  m_cash_model = atoi(jsonVal_cashin["cash_model"].asString().c_str());
	  toInitFlag = false;
	  if(m_cash_in_type != -1)
	  {			  
		  switch(m_cash_in_type)
		  {
			  case 1:
			  g_cashin = new cash_in_junpeng(cashinhandler, m_serial_cash_in, m_cash_accept);
			  toInitFlag = true;			  
			  break;
		  
			  case 2:
			  g_cashin = new cash_in_v77e(cashinhandler, m_serial_cash_in, m_cash_accept);
			  toInitFlag = true;
			  break;
			  
			  default:
			  outRoot["result_code"] = -1;
			  outRoot["result_msg"] = "硬件初始化失败！";
			  outdevcashin["dev_name"] = "cashin";
			  outdevcashin["init_result"] = false;
			  outdevcashin["init_msg"] = "未知的纸币接收器"; 
			  outData.append(outdevcashin);
			  break;
		  }
	  }else{
		  //outRoot["result_code"] = -1;
		  //outRoot["result_msg"] = "硬件初始化失败！";
		  outdevcashin["dev_name"] = "cashin";
		  outdevcashin["init_result"] = false;
		  outdevcashin["init_msg"] = "不包含纸币接收器模块"; 
		  outData.append(outdevcashin);
	  }	  
	  if(toInitFlag)
	  {
		  if(g_cashin == NULL)
		  {
			  outRoot["result_code"] = -18;
			  outRoot["result_msg"] = "纸币接收模块初始化失败(对象实例化失败)";
			  outdevcashin["dev_name"] = "cashin";
			  outdevcashin["init_result"] = false;
			  outdevcashin["init_msg"] = "纸币接收器实例化失败"; 
			  outData.append(outdevcashin);
		  }
		  else
		  {
			  int cashin_init_res = 0;
			  if(g_cashin != NULL) cashin_init_res = g_cashin->cash_in_init(ss4.str().c_str(), ss7.str().c_str());
			  if(cashin_init_res != 0)
			  {
				  outdevcashin["dev_name"] = "cashin";
				  outdevcashin["init_result"] = true;
				  outdevcashin["init_msg"] = "纸币接收器初始化成功"; 
				  outData.append(outdevcashin);
			  }
			  else
			  {
				  outRoot["result_code"] = -1;
				  outRoot["result_msg"] = "硬件初始化失败！";
				  outdevcashin["dev_name"] = "cashin";
				  outdevcashin["init_result"] = false;
				  outdevcashin["init_msg"] = "纸币接收初始化失败！"; 
				  outData.append(outdevcashin);
			  }
		  }
	  }
  }
     
  //创建线程
  if(outdevsensor["init_result"].asString() == "true")
  {
	  lg.LogInfo(1, "HardWareInit 创建传感器线程");
	  ThreadData* sensorthreadData(new ThreadData);
	  sensorthreadData->inijs = *inijs;
	  sensorthreadData->logpath = *logpath;
	  sensorthreadData->serverdatetime = *serverdatetime;
	  
	  sensorthreadData->deviceName = "sensor";
	  uv_work_t* sensorreq = new uv_work_t();
	  sensorthreadData->req = sensorreq;
	  DevParams* sensorParam(new DevParams);
	  sensorParam->isolate = isolate;
	  sensorParam->oncallback.Reset(isolate, Local<Function>::Cast(args[9]));
	  sensorParam->deviceName = "sensor";
	  sensorParam->subDev = m_SensorDev_Type;
	  uv_async_t* sensorcallbackSendReq(new uv_async_t());
	  sensorParam->callbackSendReq = sensorcallbackSendReq;
	  sensorParam->initFlag = true;
	  sensorParam->exit = false;
	  sensorParam->enable = false;
	  sensorParam->status = 0;
	  sensorParam->resultjs.clear();
	  sensorthreadData->param = sensorParam;
	  sensorthreadData->param->callbackSendReq->data = (void*)sensorthreadData->param; //
	  g_map_DevThreadData["sensor"] = boost::shared_ptr<ThreadData>(sensorthreadData);
	  
	  g_sensor->setParam(sensorthreadData->param);
	  
	  uv_async_init(uv_default_loop(), sensorthreadData->param->callbackSendReq, sendCallbackFunc);
	  
	  sensorthreadData->req->data = sensorthreadData->param; //
	  uv_queue_work(uv_default_loop(), sensorthreadData->req, callGoThread, afterGoTread);
	  
	  #if defined(__linux__)
	  usleep(500); 
	  #else
	  Sleep(500);
	  #endif
  }
  if(outdevcashin["init_result"].asString() == "true")
  {
	  lg.LogInfo(1, "HardWareInit 创建纸币接收器线程");
	  ThreadData* cashInthreadData(new ThreadData);
	  cashInthreadData->inijs = *inijs;
	  cashInthreadData->logpath = *logpath;
	  cashInthreadData->serverdatetime = *serverdatetime;
	  
	  cashInthreadData->deviceName = "cashin";
	  uv_work_t* cashinreq = new uv_work_t();
	  cashInthreadData->req = cashinreq;
	  DevParams* cashinParam(new DevParams);
	  cashinParam->isolate = isolate;
	  cashinParam->oncallback.Reset(isolate, Local<Function>::Cast(args[8]));
	  cashinParam->deviceName = "cashin";
	  cashinParam->subDev = m_cash_in_type; //
	  uv_async_t* cashincallbackSendReq(new uv_async_t());
	  cashinParam->callbackSendReq = cashincallbackSendReq;
	  cashinParam->exit = false;
	  cashinParam->initFlag = true;
	  cashinParam->enable = false;
	  cashinParam->status = 0;
	  cashinParam->resultjs.clear();
	  cashInthreadData->param = cashinParam;
	  cashInthreadData->param->callbackSendReq->data = (void*)cashInthreadData->param;
	  g_map_DevThreadData["cashin"] = boost::shared_ptr<ThreadData>(cashInthreadData);
	  
	  if(g_cashin != NULL) g_cashin->setParam(cashInthreadData->param);
	  
	  uv_async_init(uv_default_loop(), cashInthreadData->param->callbackSendReq, sendCallbackFunc);
	  
	  cashInthreadData->req->data = cashInthreadData->param;
	  uv_queue_work(uv_default_loop(), cashInthreadData->req, callGoThread, afterGoTread);
	  
	  #if defined(__linux__)
	  usleep(500); 
	  #else
	  Sleep(500);
	  #endif
  }
  if(outdevtraffic["init_result"].asString() == "true")
  {
	  lg.LogInfo(1, "HardWareInit 创建交通卡线程");
	  ThreadData* trafficthreadData(new ThreadData);
	  trafficthreadData->inijs = *inijs;
	  trafficthreadData->logpath = *logpath;
	  trafficthreadData->serverdatetime = *serverdatetime;
	  
	  trafficthreadData->deviceName = "traffic";
	  uv_work_t* trafficreq = new uv_work_t(); //多线程uv
	  trafficthreadData->req = trafficreq; 
	  DevParams* trafficParam(new DevParams);
	  trafficParam->isolate = isolate;
	  trafficParam->oncallback.Reset(isolate, Local<Function>::Cast(args[10]));
	  trafficParam->deviceName = "traffic";
	  trafficParam->subDev = m_traffic_type;
	  uv_async_t* trafficcallbackSendReq(new uv_async_t()); //异步uv
	  trafficParam->callbackSendReq = trafficcallbackSendReq; 
	  trafficParam->exit = false;
	  trafficParam->initFlag = true;
	  trafficParam->enable = false;
	  trafficParam->status = 0;
	  trafficParam->money = 0;
	  trafficParam->payed = 1;
	  trafficParam->paylog = true;
	  trafficParam->resultjs.clear();
	  trafficthreadData->param = trafficParam; 
	  trafficthreadData->param->callbackSendReq->data = (void*)trafficthreadData->param; //
	  g_map_DevThreadData["traffic"] = boost::shared_ptr<ThreadData>(trafficthreadData);
	  
	  //2.注册异步uv事件（暂时没用到，因为子库里线程没有到达预计的效果）
	  uv_async_init(uv_default_loop(), trafficthreadData->param->callbackSendReq, sendCallbackFunc); 
	  
	  //3.实现多线程uv
	  trafficthreadData->req->data = trafficthreadData->param; //
	  uv_queue_work(uv_default_loop(), trafficthreadData->req, callGoThread, afterGoTread);    
	  
	  #if defined(__linux__)
	  usleep(500); 
	  #else
	  Sleep(500);
	  #endif
  }
  if(outdeviCcard["init_result"].asString() == "true")
  {	  
	  lg.LogInfo(1, "HardWareInit 创建读卡器线程");
	  //1.对参数格式化
	  ThreadData* icCardthreadData(new ThreadData);
	  icCardthreadData->inijs = *inijs;
	  icCardthreadData->logpath = *logpath;
	  icCardthreadData->serverdatetime = *serverdatetime;
	  
	  icCardthreadData->deviceName = "icCard";
	  uv_work_t* icCardreq = new uv_work_t(); //多线程uv
	  icCardthreadData->req = icCardreq; 
	  DevParams* icCardParam(new DevParams);
	  icCardParam->isolate = isolate;
	  icCardParam->oncallback.Reset(isolate, Local<Function>::Cast(args[7]));
	  icCardParam->deviceName = "icCard";
	  icCardParam->subDev = m_iCcard_type;
	  uv_async_t* icCardcallbackSendReq(new uv_async_t()); //异步uv
	  icCardParam->callbackSendReq = icCardcallbackSendReq; 
	  icCardParam->exit = false;
	  icCardParam->initFlag = true;
	  icCardParam->enable = false;
	  icCardParam->status = 0;
	  icCardParam->resultjs.clear();
	  icCardthreadData->param = icCardParam; 
	  icCardthreadData->param->callbackSendReq->data = (void*)icCardthreadData->param; //
	  g_map_DevThreadData["icCard"] = boost::shared_ptr<ThreadData>(icCardthreadData);
	  
	  //
		
	  //2.注册异步uv事件（暂时没用到，因为子库里线程没有到达预计的效果）
	  uv_async_init(uv_default_loop(), icCardthreadData->param->callbackSendReq, sendCallbackFunc); 
	  
	  //3.实现多线程uv
	  icCardthreadData->req->data = icCardthreadData->param; //
	  uv_queue_work(uv_default_loop(), icCardthreadData->req, callGoThread, afterGoTread);    
	  
	  #if defined(__linux__)
	  usleep(500); 
	  #else
	  Sleep(500);
	  #endif
  }
  
  outRoot["data"] = outData;
  outJson = outWriter.write(outRoot);

  memset(displaybuf, 0x00, sizeof(displaybuf));
  sprintf(displaybuf, "HardWareInit result:%s", outJson.c_str());
  lg.LogInfo(1, displaybuf);
  /*Local<Number> num = Number::New(isolate, 1);
  args.GetReturnValue().Set(num);*/
  
#if defined(__linux__)
#else
  outJson = GBKToUTF8(outJson);
#endif
  
  auto result = v8::String::NewFromUtf8(isolate, outJson.c_str(), v8::NewStringType::kNormal, (int)outJson.size()).ToLocalChecked();
  args.GetReturnValue().Set(result);
}

/*static void CoinInEnable(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	string info = "";
	//threadData->enable = true;
	info = "enable succ";//hd_coin_in_enable();
	
#if defined(__linux__)
#else
	info = GBKToUTF8(info);
#endif

	auto result = v8::String::NewFromUtf8(isolate, info.c_str(), v8::NewStringType::kNormal, (int)info.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}*/
/*static void CoinInDisable(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	string info = "";
	//info = hd_coin_in_disable();
	
#if defined(__linux__)
#else
	info = GBKToUTF8(info);
#endif

	auto result = v8::String::NewFromUtf8(isolate, info.c_str(), v8::NewStringType::kNormal, (int)info.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}*/
/*static void CoinOutCheck(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	string info = "";
	//info = hd_coin_out_check();
	
#if defined(__linux__)
#else
	info = GBKToUTF8(info);
#endif

	auto result = v8::String::NewFromUtf8(isolate, info.c_str(), v8::NewStringType::kNormal, (int)info.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}*/
/*static void CoinOutReset(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	string info = "";
	//info = hd_coin_out_reset();
#if defined(__linux__)
#else
	info = GBKToUTF8(info);
#endif
	auto result = v8::String::NewFromUtf8(isolate, info.c_str(), v8::NewStringType::kNormal, (int)info.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}*/
/*static void CoinOutPayout(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	if (args.Length() < 1) {
		//isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "参数数量错误", NewStringType::kNormal).ToLocalChecked()));
		return;
	}

	if (!args[0]->IsNumber()) {
		//isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "参数错误", NewStringType::kNormal).ToLocalChecked()));
		return;
	}

	int money = args[0].As<Number>()->Value();

	string info = "";
	//info = hd_coin_out_payout(money);
#if defined(__linux__)
#else
	info = GBKToUTF8(info);
#endif
	auto result = v8::String::NewFromUtf8(isolate, info.c_str(), v8::NewStringType::kNormal, (int)info.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}*/
/*static void CoinOutPayoutByNum(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	if (args.Length() < 2) {
		//isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "参数数量错误", NewStringType::kNormal).ToLocalChecked()));
		return;
	}

	if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
		//isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "参数错误", NewStringType::kNormal).ToLocalChecked()));
		return;
	}

	int one = args[0].As<Number>()->Value();
	int half = args[1].As<Number>()->Value();

	string info = "";
	//info = hd_coin_out_payout_by_num(one, half);
#if defined(__linux__)
#else
	info = GBKToUTF8(info);
#endif
	auto result = v8::String::NewFromUtf8(isolate, info.c_str(), v8::NewStringType::kNormal, (int)info.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}*/
/*static void CoinOutPayoutByNum3(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	if (args.Length() < 3) {
		isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "参数数量错误", NewStringType::kNormal).ToLocalChecked()));
		return;
	}

	if (!args[0]->IsNumber() || !args[1]->IsNumber() || !args[2]->IsNumber()) {
		isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "参数错误", NewStringType::kNormal).ToLocalChecked()));
		return;
	}

	int five = args[0].As<Number>()->Value();
	int ten = args[1].As<Number>()->Value();
	int fifty = args[2].As<Number>()->Value();

	string info = "";
	//info = hd_coin_out_payout_by_num3(five, ten, fifty);
#if defined(__linux__)
#else
	info = GBKToUTF8(info);
#endif
	auto result = v8::String::NewFromUtf8(isolate, info.c_str(), v8::NewStringType::kNormal, (int)info.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}*/
/*static void CoinOutClear(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	string info = "";
	//info = hd_coin_out_clear();
#if defined(__linux__)
#else
	info = GBKToUTF8(info);
#endif
	auto result = v8::String::NewFromUtf8(isolate, info.c_str(), v8::NewStringType::kNormal, (int)info.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}*/
/*static void GoodsOut(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	if (args.Length() < 1) {
		isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "参数数量错误", NewStringType::kNormal).ToLocalChecked()));
		return;
	}

	if (!args[0]->IsNumber()) {
		isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "参数错误", NewStringType::kNormal).ToLocalChecked()));
		return;
	}

	int column_num = args[0].As<Number>()->Value();

	string info = "";
	//info = hd_goods_out(column_num);
#if defined(__linux__)
#else
	info = GBKToUTF8(info);
#endif
	auto result = v8::String::NewFromUtf8(isolate, info.c_str(), v8::NewStringType::kNormal, (int)info.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}*/
/*static void GoodsTest(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	if (args.Length() < 1) {
		isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "参数数量错误", NewStringType::kNormal).ToLocalChecked()));
		return;
	}

	if (!args[0]->IsNumber()) {
		isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "参数错误", NewStringType::kNormal).ToLocalChecked()));
		return;
	}

	int column_num = args[0].As<Number>()->Value();

	string info = "";
	//info = hd_goods_test(column_num);
#if defined(__linux__)
#else
	info = GBKToUTF8(info);
#endif
	auto result = v8::String::NewFromUtf8(isolate, info.c_str(), v8::NewStringType::kNormal, (int)info.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}*/
static void PrinterCheckStatus(const FunctionCallbackInfo<Value>& args) 
{
	lg.LogInfo(1, "PrinterCheckStatus =>");
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	Json::Value root;
	int ret = -2;
	if(kt_lib_printer_get_status != 0){
		ret = kt_lib_printer_get_status(); //-1未初始化不能调用接口 -2失败(写串口错误) 0有纸 1纸尽 2纸将尽 3未知状态
	}
	int stu = 5;
	if(ret >= 0 && ret <= 3)
	{
		switch(ret)
		{
			case 0:stu = 1;break;
			case 1:stu = 2;break;
			case 2:stu = 3;break;
			case 3:stu = 5;break;
		}
		root["result_code"] = 0;
		root["result_msg"] = "success";
	}
	else
	{
		root["result_code"] = -54;
		root["result_msg"] = "fail";		
	}
	root["printer_error_code"] =stu;
	//root["printer_error_msg"] = get_status_str(ret);
	
	Json::FastWriter outWriter;  
    string outJson = outWriter.write(root);
	char displaybuf[1024];
	sprintf(displaybuf, "PrinterCheckStatus result:%s", outJson.c_str());
	lg.LogInfo(1, displaybuf);

#if defined(__linux__)
#else
	outJson = GBKToUTF8(outJson);
#endif
	auto result = v8::String::NewFromUtf8(isolate, outJson.c_str(), v8::NewStringType::kNormal, (int)outJson.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}
static void PrinterCut(const FunctionCallbackInfo<Value>& args) 
{
	lg.LogInfo(1, "PrinterCut =>");
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	Json::Value root;
	int ret = -2;
	if(kt_lib_printer_cut != 0){
		ret = kt_lib_printer_cut(1); //1全切 2半切 3进纸半切 4半切
	}
	if(ret == 0)
	{
		root["result_code"] = 0;
		root["result_msg"] = "success";
	}
	else
	{
		root["result_code"] = -54;
		root["result_msg"] = "fail";		
	}
	root["printer_error_code"] =ret;
	//root["printer_error_msg"] = get_status_str(ret);
	
	Json::FastWriter outWriter;  
    string outJson = outWriter.write(root);
	char displaybuf[1024];
	sprintf(displaybuf, "PrinterCut result:%s", outJson.c_str());
	lg.LogInfo(1, displaybuf);

#if defined(__linux__)
#else
	outJson = GBKToUTF8(outJson);
#endif
	auto result = v8::String::NewFromUtf8(isolate, outJson.c_str(), v8::NewStringType::kNormal, (int)outJson.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}
static void PrinterEcho(const FunctionCallbackInfo<Value>& args) 
{
	lg.LogInfo(1, "PrinterEcho =>");
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	if (args.Length() < 5) {
		isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "参数数量错误", NewStringType::kNormal).ToLocalChecked()));
		return;
	}

	int flag = args[0].As<Number>()->Value();
	v8::String::Utf8Value content(isolate, args[1]); //
	stringstream ss;
	ss << *content;
	int size = args[2].As<Number>()->Value();
	int bold1 = args[3].As<Number>()->Value(); 
	int align = args[4].As<Number>()->Value();
	
	string info = "";
	Json::Value root;
	string str2 = "";
	string str = "";
	int ret = 0;
	switch(flag)
	{
		case 1:
			str = "打印文本";
			if(kt_lib_printer_set_strong!=0)kt_lib_printer_set_strong(bold1);
			if(kt_lib_printer_set_align!=0)kt_lib_printer_set_align(align);
			if(kt_lib_printer_set_size!=0)kt_lib_printer_set_size(size);
			if(kt_lib_printer_println!=0)ret = kt_lib_printer_println((char*)ss.str().c_str());
			if(size != 0)
				if(kt_lib_printer_set_size!=0)kt_lib_printer_set_size(0);
			if(bold1 != 0)
				if(kt_lib_printer_set_strong!=0)kt_lib_printer_set_strong(0);

			break;
		case 2:
			str = "打印条码";
			if(kt_lib_printer_set_align!=0)kt_lib_printer_set_align(align);
			if(kt_lib_printer_printBarcode!=0)ret = kt_lib_printer_printBarcode(g_epson_type,100,2,2,align,(char*)ss.str().c_str());
			break;
		case 3: 
			str = "小票打印机打印二维码";
			if(kt_lib_printer_set_align!=0)kt_lib_printer_set_align(align);
			if(kt_lib_printer_printQr!=0)ret = kt_lib_printer_printQr(g_epson_type, (char*)ss.str().c_str(), size, 2);
			break;
	}
	if(align != 0)
		if(kt_lib_printer_set_align!=0)kt_lib_printer_set_align(0);
	
	if(ret == 0){
		str2 = str += "成功";
		root["result_code"] = 0;
		root["result_msg"] = "success";
	}else{
		str2 = str + "失败";
		root["result_code"] = -15;
		root["result_msg"] = str2;
	}
	root["printer_error_code"] =ret;
	//root["printer_error_msg"] = get_status_str(ret);
	
	Json::FastWriter outWriter;  
    string outJson = outWriter.write(root);

	//outJson = hd_printer_echo(flag, ss.str().c_str(), size, bold1, align);
	char displaybuf[1024];
	sprintf(displaybuf, "PrinterEcho result:%s", outJson.c_str());
	lg.LogInfo(1, displaybuf);

#if defined(__linux__)
#else
	outJson = GBKToUTF8(outJson);
#endif
	auto result = v8::String::NewFromUtf8(isolate, outJson.c_str(), v8::NewStringType::kNormal, (int)outJson.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}
/*static void PrinterEcho12(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	if (args.Length() < 5) {
		isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "参数数量错误", NewStringType::kNormal).ToLocalChecked()));
		return;
	}

	int type = args[0].As<Number>()->Value();
	v8::String::Utf8Value content(isolate, args[1]); 
	stringstream ss;
	ss << *content;
	int size = args[2].As<Number>()->Value();
	int bold2 = args[3].As<Number>()->Value(); 
	int bCut = args[4].As<Number>()->Value(); 
	
	string info = "";
	//info = hd_printer_echo12(type, ss.str().c_str(), size, bold2, bCut);
#if defined(__linux__)
#else
	info = GBKToUTF8(info);
#endif
	auto result = v8::String::NewFromUtf8(isolate, info.c_str(), v8::NewStringType::kNormal, (int)info.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}*/
/*static void PrinterEcho2(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	if (args.Length() < 5) {
		isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "参数数量错误", NewStringType::kNormal).ToLocalChecked()));
		return;
	}

	int type = args[0].As<Number>()->Value();
	v8::String::Utf8Value content(isolate, args[1]); 
	stringstream ss;
	ss << *content;
	int size = args[2].As<Number>()->Value();
	int bold3 = args[3].As<Number>()->Value(); 
	int bCut = args[4].As<Number>()->Value(); 
	
	string info = "";
	//info = hd_printer_echo2(type, (char*)ss.str().c_str(), size, bold3, bCut);
#if defined(__linux__)
#else
	info = GBKToUTF8(info);
#endif
	auto result = v8::String::NewFromUtf8(isolate, info.c_str(), v8::NewStringType::kNormal, (int)info.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}*/
/*static void PrinterEcho22(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	if (args.Length() < 10) {
		isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "参数数量错误", NewStringType::kNormal).ToLocalChecked()));
		return;
	}

	//int type = args[0].As<Number>()->Value();
	Local<Number> ltype = Local<Number>::Cast(args[0]);
	int type = ltype->Value();
	int size = args[2].As<Number>()->Value();
	int lmargin = args[4].As<Number>()->Value();
	int scale = args[5].As<Number>()->Value();
	int align = args[6].As<Number>()->Value();
	long witdth = args[7].As<Number>()->Value();
	long height = args[8].As<Number>()->Value();
	int bold4 = args[3].As<Number>()->Value(); 
	int bCut = args[9].As<Number>()->Value(); 
	v8::String::Utf8Value content(isolate, args[1]); 
	stringstream ss;
	ss << *content;
	
	string info = "";
	//info = hd_printer_echo22(type, (char*)ss.str().c_str(), size, bold4, lmargin, scale, align, witdth, height, bCut);
#if defined(__linux__)
#else
	info = GBKToUTF8(info);
#endif
	auto result = v8::String::NewFromUtf8(isolate, info.c_str(), v8::NewStringType::kNormal, (int)info.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}*/
/*static void PrinterFastPic(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	if (args.Length() < 3) {
		isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "参数数量错误", NewStringType::kNormal).ToLocalChecked()));
		return;
	}

	int type = args[0].As<Number>()->Value();
	int bmp_index = args[1].As<Number>()->Value(); //char
	int size = args[2].As<Number>()->Value();
	
	string info = "";	
	//info = hd_printer_fast_pic(type, bmp_index, size);
#if defined(__linux__)
#else
	info = GBKToUTF8(info);
#endif
	auto result = v8::String::NewFromUtf8(isolate, info.c_str(), v8::NewStringType::kNormal, (int)info.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}*/

static void Sensorenable(const FunctionCallbackInfo<Value>& args) 
{
	lg.LogInfo(1, "Sensorenable =>");
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	Json::Value root;
	Json::FastWriter write;	
	std::string json_out = "";
	
	map<string, boost::shared_ptr<ThreadData>>::iterator it = g_map_DevThreadData.find("sensor");
	if(it != g_map_DevThreadData.end())
	{
		boost::shared_ptr<ThreadData> td = it->second;
		json_out = g_sensor->sensor_enable();
	}
	else
	{
		root["result_code"] = -7;
		root["result_msg"] = "传感器初始化失败";
		json_out = write.write(root);
	}
	char displaybuf[1024];
	sprintf(displaybuf, "Sensorenable result:%s", json_out.c_str());
	lg.LogInfo(1, displaybuf);

#if defined(__linux__)
#else
	json_out = GBKToUTF8(json_out);
#endif
	auto result = v8::String::NewFromUtf8(isolate, json_out.c_str(), v8::NewStringType::kNormal, (int)json_out.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}
static void Sensordisable(const FunctionCallbackInfo<Value>& args) 
{
	lg.LogInfo(1, "Sensordisable =>");
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	Json::Value root;
	Json::FastWriter write;	
	std::string json_out = "";
	
	map<string, boost::shared_ptr<ThreadData>>::iterator it = g_map_DevThreadData.find("sensor");
	if(it != g_map_DevThreadData.end())
	{
		boost::shared_ptr<ThreadData> td = it->second;
		json_out = g_sensor->sensor_disable();
	}
	else
	{
		root["result_code"] = -7;
		root["result_msg"] = "传感器初始化失败";
		json_out = write.write(root);
	}
	char displaybuf[1024];
	sprintf(displaybuf, "Sensordisable result:%s", json_out.c_str());
	lg.LogInfo(1, displaybuf);

#if defined(__linux__)
#else
	json_out = GBKToUTF8(json_out);
#endif
	auto result = v8::String::NewFromUtf8(isolate, json_out.c_str(), v8::NewStringType::kNormal, (int)json_out.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}
static void Sensorgetstatus(const FunctionCallbackInfo<Value>& args) 
{
	lg.LogInfo(1, "Sensorgetstatus =>");
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	Json::Value root;
	Json::FastWriter write;	
	std::string json_out = "";
	
	map<string, boost::shared_ptr<ThreadData>>::iterator it = g_map_DevThreadData.find("sensor");
	if(it != g_map_DevThreadData.end())
	{
		boost::shared_ptr<ThreadData> td = it->second;
		json_out = g_sensor->sensor_get_Status();
	}
	else
	{
		root["result_code"] = -61;
		root["result_msg"] = "票取走传感器获取状态失败";
		root["sensor_error_code"] = -1;
		root["sensor_error_msg"] = "传感器初始化失败";
		json_out = write.write(root);
	}
	char displaybuf[1024];
	sprintf(displaybuf, "Sensorgetstatus result:%s", json_out.c_str());
	lg.LogInfo(1, displaybuf);

#if defined(__linux__)
#else
	json_out = GBKToUTF8(json_out);
#endif
	auto result = v8::String::NewFromUtf8(isolate, json_out.c_str(), v8::NewStringType::kNormal, (int)json_out.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}

static void LockSendLock(const FunctionCallbackInfo<Value>& args) 
{	//2020-12-11
	lg.LogInfo(1, "LockSendLock =>");
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 

	Json::Value root;
	//int ret = IELECTLOCK_send_Unlocking();	
	int ret = -2;
	if(kt_IELECTLOCK_send_Unlocking != NULL){
		ret = kt_IELECTLOCK_send_Unlocking();	
	}
	if(ret == 0){
		root["result_code"] = 0;
		root["result_msg"] = "success";
	}else{
		root["result_code"] = -15;
		root["result_msg"] = "开锁失败";
	}
	root["printer_error_code"] =ret;
	//root["printer_error_msg"] = get_status_str(ret);
	
	Json::FastWriter outWriter;  
    string outJson = outWriter.write(root);

	char displaybuf[1024];
	sprintf(displaybuf, "LockSendLock result:%s", outJson.c_str());
	lg.LogInfo(1, displaybuf);

#if defined(__linux__)
#else
	outJson = GBKToUTF8(outJson);
#endif
	auto result = v8::String::NewFromUtf8(isolate, outJson.c_str(), v8::NewStringType::kNormal, (int)outJson.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}
  
static void Cashinenable(const FunctionCallbackInfo<Value>& args) 
{
	lg.LogInfo(1, "Cashinenable =>");
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	Json::Value root;
	Json::FastWriter write;	
	std::string json_out = "";
	
	map<string, boost::shared_ptr<ThreadData>>::iterator it = g_map_DevThreadData.find("cashin");
	if(it != g_map_DevThreadData.end())
	{
		boost::shared_ptr<ThreadData> td = it->second;
		if(g_cashin != NULL) json_out = g_cashin->cash_in_enable();
	}
	else
	{
		root["result_code"] = -7;
		root["result_msg"] = "纸币接收器初始化失败";
		json_out = write.write(root);
	}
	char displaybuf[1024];
	sprintf(displaybuf, "Cashinenable result:%s", json_out.c_str());
	lg.LogInfo(1, displaybuf);

#if defined(__linux__)
#else
	json_out = GBKToUTF8(json_out);
#endif
	auto result = v8::String::NewFromUtf8(isolate, json_out.c_str(), v8::NewStringType::kNormal, (int)json_out.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}
static void Cashincheck(const FunctionCallbackInfo<Value>& args)
{
	lg.LogInfo(1, "Cashincheck =>");
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	Json::Value root;
	Json::FastWriter write;	
	std::string json_out = "";
	
	map<string, boost::shared_ptr<ThreadData>>::iterator it = g_map_DevThreadData.find("cashin");
	if(it != g_map_DevThreadData.end())
	{
		boost::shared_ptr<ThreadData> td = it->second;
		if(g_cashin != NULL) json_out = g_cashin->cash_in_check();
	}
	else
	{
		root["result_code"] = -7;
		root["result_msg"] = "纸币接收器初始化失败";
		json_out = write.write(root);
	}
	char displaybuf[1024];
	sprintf(displaybuf, "Cashincheck result:%s", json_out.c_str());
	lg.LogInfo(1, displaybuf);

#if defined(__linux__)
#else
	json_out = GBKToUTF8(json_out);
#endif
	auto result = v8::String::NewFromUtf8(isolate, json_out.c_str(), v8::NewStringType::kNormal, (int)json_out.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}
static void Cashindisable(const FunctionCallbackInfo<Value>& args) 
{
	lg.LogInfo(1, "Cashindisable =>");
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	Json::Value root;
	Json::FastWriter write;	
	std::string json_out = "";
	
	map<string, boost::shared_ptr<ThreadData>>::iterator it = g_map_DevThreadData.find("cashin");
	if(it != g_map_DevThreadData.end())
	{
		boost::shared_ptr<ThreadData> td = it->second;
		if(g_cashin != NULL) json_out = g_cashin->cash_in_disable();
	}
	else
	{
		root["result_code"] = -7;
		root["result_msg"] = "纸币接收器初始化失败";
		json_out = write.write(root);
	}
	char displaybuf[1024];
	sprintf(displaybuf, "Cashindisable result:%s", json_out.c_str());
	lg.LogInfo(1, displaybuf);

#if defined(__linux__)
#else
	json_out = GBKToUTF8(json_out);
#endif
	auto result = v8::String::NewFromUtf8(isolate, json_out.c_str(), v8::NewStringType::kNormal, (int)json_out.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}
static void Cashinreset(const FunctionCallbackInfo<Value>& args) 
{
	lg.LogInfo(1, "Cashinreset =>");
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	Json::Value root;
	Json::FastWriter write;	
	std::string json_out = "";
	
	map<string, boost::shared_ptr<ThreadData>>::iterator it = g_map_DevThreadData.find("cashin");
	if(it != g_map_DevThreadData.end())
	{
		boost::shared_ptr<ThreadData> td = it->second;
		if(g_cashin != NULL) json_out = g_cashin->cash_in_reset();
	}
	else
	{
		root["result_code"] = -7;
		root["result_msg"] = "纸币接收器初始化失败";
		json_out = write.write(root);
	}
	char displaybuf[1024];
	sprintf(displaybuf, "Cashinreset result:%s", json_out.c_str());
	lg.LogInfo(1, displaybuf);

#if defined(__linux__)
#else
	json_out = GBKToUTF8(json_out);
#endif
	auto result = v8::String::NewFromUtf8(isolate, json_out.c_str(), v8::NewStringType::kNormal, (int)json_out.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}
static void Cashinsetnotesaccept(const FunctionCallbackInfo<Value>& args) 
{
	lg.LogInfo(1, "Cashinsetnotesaccept =>");
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	if (args.Length() < 3) {
		isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "参数数量错误", NewStringType::kNormal).ToLocalChecked()));
		return;
	}

	int cash_type = args[0].As<Number>()->Value();
	int accept = args[1].As<Number>()->Value(); 
	int finish = args[2].As<Number>()->Value(); 

	Json::Value root;
	Json::FastWriter write;	
	std::string json_out = "";
	
	map<string, boost::shared_ptr<ThreadData>>::iterator it = g_map_DevThreadData.find("cashin");
	if(it != g_map_DevThreadData.end())
	{
		boost::shared_ptr<ThreadData> td = it->second;
		if(g_cashin != NULL) json_out = g_cashin->cash_in_set_notes_accept(cash_type, accept, finish);
	}
	else
	{
		root["result_code"] = -7;
		root["result_msg"] = "纸币接收器初始化失败";
		json_out = write.write(root);
	}
	char displaybuf[1024];
	sprintf(displaybuf, "Cashinsetnotesaccept result:%s", json_out.c_str());
	lg.LogInfo(1, displaybuf);

#if defined(__linux__)
#else
	json_out = GBKToUTF8(json_out);
#endif
	auto result = v8::String::NewFromUtf8(isolate, json_out.c_str(), v8::NewStringType::kNormal, (int)json_out.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}
static void CashingetDatasetVersion(const FunctionCallbackInfo<Value>& args) 
{
	lg.LogInfo(1, "CashingetDatasetVersion =>");
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	Json::Value root;
	Json::FastWriter write;	
	std::string json_out = "";
	
	map<string, boost::shared_ptr<ThreadData>>::iterator it = g_map_DevThreadData.find("cashin");
	if(it != g_map_DevThreadData.end())
	{
		boost::shared_ptr<ThreadData> td = it->second;
		if(g_cashin != NULL) json_out = g_cashin->cash_in_get_Dataset_Version();
	}
	else
	{
		root["result_code"] = -7;
		root["result_msg"] = "纸币接收器初始化失败";
		json_out = write.write(root);
	}
	char displaybuf[1024];
	sprintf(displaybuf, "CashingetDatasetVersion result:%s", json_out.c_str());
	lg.LogInfo(1, displaybuf);

#if defined(__linux__)
#else
	json_out = GBKToUTF8(json_out);
#endif
	auto result = v8::String::NewFromUtf8(isolate, json_out.c_str(), v8::NewStringType::kNormal, (int)json_out.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}
static void CashingetFirmwareVersion(const FunctionCallbackInfo<Value>& args) 
{
	lg.LogInfo(1, "CashingetFirmwareVersion =>");
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	Json::Value root;
	Json::FastWriter write;	
	std::string json_out = "";
	
	map<string, boost::shared_ptr<ThreadData>>::iterator it = g_map_DevThreadData.find("cashin");
	if(it != g_map_DevThreadData.end())
	{
		boost::shared_ptr<ThreadData> td = it->second;
		if(g_cashin != NULL) json_out = g_cashin->cash_in_get_Firmware_Version();
	}
	else
	{
		root["result_code"] = -7;
		root["result_msg"] = "纸币接收器初始化失败";
		json_out = write.write(root);
	}
	char displaybuf[1024];
	sprintf(displaybuf, "CashingetFirmwareVersion result:%s", json_out.c_str());
	lg.LogInfo(1, displaybuf);
#if defined(__linux__)
#else
	json_out = GBKToUTF8(json_out);
#endif
	auto result = v8::String::NewFromUtf8(isolate, json_out.c_str(), v8::NewStringType::kNormal, (int)json_out.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}
static void CashinupgradeFirmwareProgram(const FunctionCallbackInfo<Value>& args) 
{
	lg.LogInfo(1, "CashinupgradeFirmwareProgram =>");
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	if (args.Length() < 2) {
		isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "参数数量错误", NewStringType::kNormal).ToLocalChecked()));
		return;
	}

	stringstream ss1;
	v8::String::Utf8Value fileName(isolate, args[0]); 
	ss1 << *fileName;
	stringstream ss2;
	v8::String::Utf8Value arrPort(isolate, args[1]); 
	ss2 << *arrPort;

	Json::Value root;
	Json::FastWriter write;	
	std::string json_out = "";
	
	map<string, boost::shared_ptr<ThreadData>>::iterator it = g_map_DevThreadData.find("cashin");
	if(it != g_map_DevThreadData.end())
	{
		boost::shared_ptr<ThreadData> td = it->second;
		if(g_cashin != NULL) json_out = g_cashin->cash_in_upgrade_Firmware_Program(ss1.str().c_str(), ss2.str().c_str());
	}
	else
	{
		root["result_code"] = -7;
		root["result_msg"] = "纸币接收器初始化失败";
		json_out = write.write(root);
	}
	char displaybuf[1024];
	sprintf(displaybuf, "CashinupgradeFirmwareProgram result:%s", json_out.c_str());
	lg.LogInfo(1, displaybuf);

#if defined(__linux__)
#else
	json_out = GBKToUTF8(json_out);
#endif
	auto result = v8::String::NewFromUtf8(isolate, json_out.c_str(), v8::NewStringType::kNormal, (int)json_out.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}
/*static void Unionconsume(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	if (args.Length() < 11) {
		isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "参数数量错误", NewStringType::kNormal).ToLocalChecked()));
		return;
	}

	int apptype = args[0].As<Number>()->Value();
	int type = args[1].As<Number>()->Value();
	int money = args[2].As<Number>()->Value();
	int left = args[3].As<Number>()->Value();
	int top = args[4].As<Number>()->Value();
	int width = args[5].As<Number>()->Value();
	int height = args[6].As<Number>()->Value();
	int alpha = args[7].As<Number>()->Value();
	int r = args[8].As<Number>()->Value();
	int g = args[9].As<Number>()->Value();
	int b = args[10].As<Number>()->Value();
	
	string info = "";	
	//info = hd_union_consume(apptype, type, money, left, top, width, height, alpha, r, g, b);
#if defined(__linux__)
#else
	info = GBKToUTF8(info);
#endif
	auto result = v8::String::NewFromUtf8(isolate, info.c_str(), v8::NewStringType::kNormal, (int)info.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}*/
/*static void Unionsettle(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	string info = "";
	//info = hd_union_settle();
#if defined(__linux__)
#else
	info = GBKToUTF8(info);
#endif
	auto result = v8::String::NewFromUtf8(isolate, info.c_str(), v8::NewStringType::kNormal, (int)info.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}*/
/*static void Unionsignin(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	string info = "";
	//info = hd_union_signin();
#if defined(__linux__)
#else
	info = GBKToUTF8(info);
#endif
	auto result = v8::String::NewFromUtf8(isolate, info.c_str(), v8::NewStringType::kNormal, (int)info.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}*/
/*static void Unioncancel(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	string info = "";
	//info = hd_union_cancel();
#if defined(__linux__)
#else
	info = GBKToUTF8(info);
#endif
	auto result = v8::String::NewFromUtf8(isolate, info.c_str(), v8::NewStringType::kNormal, (int)info.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}*/

/*static void ICcardsetKey(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	unsigned char keya_[16][6]={{0xff, 0xff, 0xff, 0xff, 0xff, 0xff},{0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
							    {0xff, 0xff, 0xff, 0xff, 0xff, 0xff},{0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
							    {0xff, 0xff, 0xff, 0xff, 0xff, 0xff},{0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
							    {0xff, 0xff, 0xff, 0xff, 0xff, 0xff},{0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
							    {0xff, 0xff, 0xff, 0xff, 0xff, 0xff},{0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
							    {0xff, 0xff, 0xff, 0xff, 0xff, 0xff},{0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
							    {0xff, 0xff, 0xff, 0xff, 0xff, 0xff},{0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
							    {0xff, 0xff, 0xff, 0xff, 0xff, 0xff},{0xff, 0xff, 0xff, 0xff, 0xff, 0xff}}; 
	unsigned char keyb_[16][6]={{0xff, 0xff, 0xff, 0xff, 0xff, 0xff},{0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
							    {0xff, 0xff, 0xff, 0xff, 0xff, 0xff},{0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
							    {0xff, 0xff, 0xff, 0xff, 0xff, 0xff},{0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
							    {0xff, 0xff, 0xff, 0xff, 0xff, 0xff},{0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
							    {0xff, 0xff, 0xff, 0xff, 0xff, 0xff},{0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
							    {0xff, 0xff, 0xff, 0xff, 0xff, 0xff},{0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
							    {0xff, 0xff, 0xff, 0xff, 0xff, 0xff},{0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
							    {0xff, 0xff, 0xff, 0xff, 0xff, 0xff},{0xff, 0xff, 0xff, 0xff, 0xff, 0xff}}; 
	string info = "";
	//info = hd_ICcard_setKey((unsigned char**)keya_, (unsigned char**)keyb_);
#if defined(__linux__)
#else
	info = GBKToUTF8(info);
#endif
	auto result = v8::String::NewFromUtf8(isolate, info.c_str(), v8::NewStringType::kNormal, (int)info.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}*/
static void ICcarddisable(const FunctionCallbackInfo<Value>& args) 
{
	lg.LogInfo(1, "ICcarddisable =>");
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	if (args.Length() < 1) {
		isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "参数数量错误", NewStringType::kNormal).ToLocalChecked()));
		return;
	}

	if (!args[0]->IsNumber()) {
		isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "参数错误", NewStringType::kNormal).ToLocalChecked()));
		return;
	}
	int blockNum = args[0].As<Number>()->Value();

	Json::Value root;
	Json::FastWriter write;	
	std::string json_out = "";
	
	map<string, boost::shared_ptr<ThreadData>>::iterator it = g_map_DevThreadData.find("icCard");
	if(it != g_map_DevThreadData.end())
	{
		boost::shared_ptr<ThreadData> td = it->second;
		td->param->enable = false;
		if(td->param->subDev == 1){
			kt_IRPD_Disable(0); 
		}
		else if(td->param->subDev == 0){
			kt_IRFM_Disable(0); 
		}
		td->param->blocknum = blockNum;
		root["result_code"] = 0;
		root["result_msg"] = "success";
		root["ICcard_error_code"] = 0;
		root["ICcard_error_msg"] = "IC读卡器禁能成功！";
	}
	else
	{
		root["result_code"] = -39;
		root["result_msg"] = "IC读卡器初始化失败";
		root["ICcard_error_code"] = -2;
		root["ICcard_error_msg"] = "IC读卡器通讯失败";		
	}

	json_out = write.write(root);
	char displaybuf[1024];
	sprintf(displaybuf, "ICcarddisable result:%s", json_out.c_str());
	lg.LogInfo(1, displaybuf);

#if defined(__linux__)
#else
	json_out = GBKToUTF8(json_out);
#endif

	auto result = v8::String::NewFromUtf8(isolate, json_out.c_str(), v8::NewStringType::kNormal, (int)json_out.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}

static void TrafficEnable(const FunctionCallbackInfo<Value>& args)
{
	lg.LogInfo(1, "TrafficEnable =>");
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	if (args.Length() < 1) {
		isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "参数数量错误", NewStringType::kNormal).ToLocalChecked()));
		return;
	}

	if (!args[0]->IsNumber()) {
		isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "参数错误", NewStringType::kNormal).ToLocalChecked()));
		return;
	}
	int money = args[0].As<Number>()->Value();

	Json::Value root;
	Json::FastWriter write;	
	std::string json_out = "";
	
	map<string, boost::shared_ptr<ThreadData>>::iterator it = g_map_DevThreadData.find("traffic");
	if(it != g_map_DevThreadData.end())
	{
		boost::shared_ptr<ThreadData> td = it->second;
		td->param->payed = 0;
		td->param->paylog = 1;
		td->param->money = money;
		td->param->enable = true;	
		root["result_code"] = 0;
		root["result_msg"] = "success";
		root["traffic_error_code"] = 0;
		root["traffic_error_msg"] = "交通卡使能成功！";
	}
	else
	{
		root["result_code"] = -39;
		root["result_msg"] = "交通卡初始化失败";
		root["traffic_error_code"] = -2;
		root["traffic_error_msg"] = "交通卡通讯失败";		
	}

	json_out = write.write(root);
	char displaybuf[1024];
	sprintf(displaybuf, "TrafficEnable(%d) result:%s", money, json_out.c_str());
	lg.LogInfo(1, displaybuf);

#if defined(__linux__)
#else
	json_out = GBKToUTF8(json_out);
#endif

	auto result = v8::String::NewFromUtf8(isolate, json_out.c_str(), v8::NewStringType::kNormal, (int)json_out.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}
static void TrafficReadLog(const FunctionCallbackInfo<Value>& args)
{
	lg.LogInfo(1, "TrafficReadLog =>");
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 

	Json::Value root;
	Json::FastWriter write;	
	std::string json_out = "";
	
	map<string, boost::shared_ptr<ThreadData>>::iterator it = g_map_DevThreadData.find("traffic");
	if(it != g_map_DevThreadData.end())
	{
		boost::shared_ptr<ThreadData> td = it->second;
		td->param->paylog = false;	
		td->param->payed = 1;
		td->param->enable = true;	
		root["result_code"] = 0;
		root["result_msg"] = "success";
		root["traffic_error_code"] = 0;
		root["traffic_error_msg"] = "交通卡请求获取交易记录成功！";
	}
	else
	{
		root["result_code"] = -39;
		root["result_msg"] = "交通卡初始化失败";
		root["traffic_error_code"] = -2;
		root["traffic_error_msg"] = "交通卡通讯失败";		
	}

	json_out = write.write(root);
	char displaybuf[1024];
	sprintf(displaybuf, "TrafficReadLog() result:%s", json_out.c_str());
	lg.LogInfo(1, displaybuf);

#if defined(__linux__)
#else
	json_out = GBKToUTF8(json_out);
#endif

	auto result = v8::String::NewFromUtf8(isolate, json_out.c_str(), v8::NewStringType::kNormal, (int)json_out.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
	
	/*lg.LogInfo(1, "TrafficReadLog =>");
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	Json::Value root;
	Json::FastWriter write;	
	std::string json_out = "";
	
	map<string, boost::shared_ptr<ThreadData>>::iterator it = g_map_DevThreadData.find("traffic");
	if(it != g_map_DevThreadData.end())
	{
		boost::shared_ptr<ThreadData> td = it->second;
		if(td->param->enable){
			root["result_code"] = -68;
			root["result_msg"] = "交通卡获取交易记录失败";
			root["traffic_error_code"] = -4;
			root["traffic_error_msg"] = "使能中，请先禁能";	
		}
		else{
			CARDINFOS infos;
			int ret = Itcard_readLog(0x00, infos);
			if(ret == 0){
				root["result_code"] = 0;
				root["result_msg"] = "success";
				root["traffic_error_code"] = 0;
				for(int i=0; i<10; i++){
					root["traffic_error_msg"].append(display2(infos.cards[i], 16));
				}
			}
			else{
				root["result_code"] = -68;
				root["result_msg"] = "交通卡获取交易记录失败";
				root["traffic_error_code"] = -5;
				root["traffic_error_msg"] = "读取交易记录失败";		
			}
		}
	}
	else
	{
		root["result_code"] = -39;
		root["result_msg"] = "交通卡初始化失败";
		root["traffic_error_code"] = -2;
		root["traffic_error_msg"] = "交通卡通讯失败";		
	}

	json_out = write.write(root);
	char displaybuf[1024];
	sprintf(displaybuf, "TrafficDisable result:%s", json_out.c_str());
	lg.LogInfo(1, displaybuf);

#if defined(__linux__)
#else
	json_out = GBKToUTF8(json_out);
#endif

	auto result = v8::String::NewFromUtf8(isolate, json_out.c_str(), v8::NewStringType::kNormal, (int)json_out.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);*/
}
static void TrafficDisable(const FunctionCallbackInfo<Value>& args)
{
	lg.LogInfo(1, "TrafficDisable =>");
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	Json::Value root;
	Json::FastWriter write;	
	std::string json_out = "";
	
	map<string, boost::shared_ptr<ThreadData>>::iterator it = g_map_DevThreadData.find("traffic");
	if(it != g_map_DevThreadData.end())
	{
		boost::shared_ptr<ThreadData> td = it->second;
		td->param->enable = false;
		td->param->payed = 1;
		td->param->paylog = true;
		root["result_code"] = 0;
		root["result_msg"] = "success";
		root["traffic_error_code"] = 0;
		root["traffic_error_msg"] = "交通卡禁能成功！";
	}
	else
	{
		root["result_code"] = -39;
		root["result_msg"] = "交通卡初始化失败";
		root["traffic_error_code"] = -2;
		root["traffic_error_msg"] = "交通卡通讯失败";		
	}

	json_out = write.write(root);
	char displaybuf[1024];
	sprintf(displaybuf, "TrafficDisable result:%s", json_out.c_str());
	lg.LogInfo(1, displaybuf);

#if defined(__linux__)
#else
	json_out = GBKToUTF8(json_out);
#endif

	auto result = v8::String::NewFromUtf8(isolate, json_out.c_str(), v8::NewStringType::kNormal, (int)json_out.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}

static void ICcardenable(const FunctionCallbackInfo<Value>& args) 
{
	lg.LogInfo(1, "ICcardenable =>");
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	if (args.Length() < 1) {
		isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "参数数量错误", NewStringType::kNormal).ToLocalChecked()));
		return;
	}

	if (!args[0]->IsNumber()) {
		isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "参数错误", NewStringType::kNormal).ToLocalChecked()));
		return;
	}
	int blockNum = args[0].As<Number>()->Value();

	Json::Value root;
	Json::FastWriter write;	
	std::string json_out = "";
	
	map<string, boost::shared_ptr<ThreadData>>::iterator it = g_map_DevThreadData.find("icCard");
	if(it != g_map_DevThreadData.end())
	{
		boost::shared_ptr<ThreadData> td = it->second;
		td->param->enable = true;
		if(td->param->subDev == 1){
			kt_IRPD_Enable(0); 
		}
		else if(td->param->subDev == 0){
			kt_IRFM_Enable(0); 
		}
		td->param->blocknum = blockNum;
		root["result_code"] = 0;
		root["result_msg"] = "success";
		root["ICcard_error_code"] = 0;
		root["ICcard_error_msg"] = "IC读卡器使能成功！";
	}
	else
	{
		root["result_code"] = -39;
		root["result_msg"] = "IC读卡器初始化失败";
		root["ICcard_error_code"] = -2;
		root["ICcard_error_msg"] = "IC读卡器通讯失败";		
	}

	json_out = write.write(root);
	char displaybuf[1024];
	sprintf(displaybuf, "ICcardenable result:%s", json_out.c_str());
	lg.LogInfo(1, displaybuf);

#if defined(__linux__)
#else
	json_out = GBKToUTF8(json_out);
#endif

	auto result = v8::String::NewFromUtf8(isolate, json_out.c_str(), v8::NewStringType::kNormal, (int)json_out.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}
/*static void ICcardsearchCard(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	if (args.Length() < 1) {
		isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "参数数量错误", NewStringType::kNormal).ToLocalChecked()));
		return;
	}

	if (!args[0]->IsNumber()) {
		isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "参数错误", NewStringType::kNormal).ToLocalChecked()));
		return;
	}

	int type = args[0].As<Number>()->Value();

	string info = "";
	//info = hd_ICcard_searchCard(type);
#if defined(__linux__)
#else
	info = GBKToUTF8(info);
#endif
	auto result = v8::String::NewFromUtf8(isolate, info.c_str(), v8::NewStringType::kNormal, (int)info.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}*/
/*static void ICcardreadCard(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	string info = "";
	//info = hd_ICcard_readCard();
#if defined(__linux__)
#else
	info = GBKToUTF8(info);
#endif
	auto result = v8::String::NewFromUtf8(isolate, info.c_str(), v8::NewStringType::kNormal, (int)info.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}*/
/*static void ICcardreadBlock(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	if (args.Length() < 1) {
		isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "参数数量错误", NewStringType::kNormal).ToLocalChecked()));
		return;
	}

	if (!args[0]->IsNumber()) {
		isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "参数错误", NewStringType::kNormal).ToLocalChecked()));
		return;
	}

	int blocknum = args[0].As<Number>()->Value();

	string info = "";
	//info = hd_ICcard_readBlock(blocknum);
#if defined(__linux__)
#else
	info = GBKToUTF8(info);
#endif
	auto result = v8::String::NewFromUtf8(isolate, info.c_str(), v8::NewStringType::kNormal, (int)info.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}*/
/*static void ICcardwriteBlock(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	if (args.Length() < 3) {
		isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "参数数量错误", NewStringType::kNormal).ToLocalChecked()));
		return;
	}

	int blocknum = args[0].As<Number>()->Value();
	v8::String::Utf8Value toWriteBuf(isolate, args[1]); 
	stringstream ss;
	ss << *toWriteBuf;
	int size = args[2].As<Number>()->Value();
	
	string info = "";
	//info = hd_ICcard_writeBlock(blocknum, (unsigned char *)ss.str().c_str(), size);
#if defined(__linux__)
#else
	info = GBKToUTF8(info);
#endif
	auto result = v8::String::NewFromUtf8(isolate, info.c_str(), v8::NewStringType::kNormal, (int)info.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}*/
/*static void ICcardinitWallet(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	if (args.Length() < 2) {
		isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "参数数量错误", NewStringType::kNormal).ToLocalChecked()));
		return;
	}

	int blocknum = args[0].As<Number>()->Value();
	int money = args[1].As<Number>()->Value();
	
	string info = "";
	//info = hd_ICcard_initWallet(blocknum, money);
#if defined(__linux__)
#else
	info = GBKToUTF8(info);
#endif
	auto result = v8::String::NewFromUtf8(isolate, info.c_str(), v8::NewStringType::kNormal, (int)info.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}*/
/*static void ICcardreadBalance(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	if (args.Length() < 1) {
		isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "参数数量错误", NewStringType::kNormal).ToLocalChecked()));
		return;
	}

	int blocknum = args[0].As<Number>()->Value();
	
	string info = "";
	//info = hd_ICcard_readBalance(blocknum);
#if defined(__linux__)
#else
	info = GBKToUTF8(info);
#endif
	auto result = v8::String::NewFromUtf8(isolate, info.c_str(), v8::NewStringType::kNormal, (int)info.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}*/
/*static void ICcardrechargeWallet(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	if (args.Length() < 2) {
		isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "参数数量错误", NewStringType::kNormal).ToLocalChecked()));
		return;
	}

	int blocknum = args[0].As<Number>()->Value();
	int money = args[1].As<Number>()->Value();
	
	string info = "";
	//info = hd_ICcard_rechargeWallet(blocknum, money);
#if defined(__linux__)
#else
	info = GBKToUTF8(info);
#endif
	auto result = v8::String::NewFromUtf8(isolate, info.c_str(), v8::NewStringType::kNormal, (int)info.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}*/
/*static void ICcarddeductWallet(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	if (args.Length() < 2) {
		isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "参数数量错误", NewStringType::kNormal).ToLocalChecked()));
		return;
	}

	int blocknum = args[0].As<Number>()->Value();
	int money = args[1].As<Number>()->Value();
	
	string info = "";
	//info = hd_ICcard_deductWallet(blocknum, money);
#if defined(__linux__)
#else
	info = GBKToUTF8(info);
#endif
	auto result = v8::String::NewFromUtf8(isolate, info.c_str(), v8::NewStringType::kNormal, (int)info.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}*/

/*static void TestJson(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	string info = "";
	//info = hd_test_json();
#if defined(__linux__)
#else
	info = GBKToUTF8(info);
#endif
	auto result = v8::String::NewFromUtf8(isolate, info.c_str(), v8::NewStringType::kNormal, (int)info.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}*/
/*static void Cashoutcheck(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	string info = "";
	//info = hd_cash_out_check();
#if defined(__linux__)
#else
	info = GBKToUTF8(info);
#endif
	auto result = v8::String::NewFromUtf8(isolate, info.c_str(), v8::NewStringType::kNormal, (int)info.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}*/
/*static void Cashoutreset(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	string info = "";
	//info = hd_cash_out_reset();
#if defined(__linux__)
#else
	info = GBKToUTF8(info);
#endif
	auto result = v8::String::NewFromUtf8(isolate, info.c_str(), v8::NewStringType::kNormal, (int)info.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}*/
/*static void Cashoutpayout(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	if (args.Length() < 1) {
		isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "参数数量错误", NewStringType::kNormal).ToLocalChecked()));
		return;
	}

	int money = args[0].As<Number>()->Value();
	
	string info = "";	
	//info = hd_cash_out_payout(money);
#if defined(__linux__)
#else
	info = GBKToUTF8(info);
#endif
	auto result = v8::String::NewFromUtf8(isolate, info.c_str(), v8::NewStringType::kNormal, (int)info.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}*/
/*static void Ecardconsume(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	if (args.Length() < 11) {
		isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "参数数量错误", NewStringType::kNormal).ToLocalChecked()));
		return;
	}

	int mny = args[0].As<Number>()->Value();
	int i_left = args[1].As<Number>()->Value();
	int i_top = args[2].As<Number>()->Value();
	int i_width = args[3].As<Number>()->Value();
	int i_height = args[4].As<Number>()->Value();
	int i_alpha = args[5].As<Number>()->Value();
	int i_r = args[6].As<Number>()->Value();
	int i_g = args[7].As<Number>()->Value();
	int i_b = args[8].As<Number>()->Value();
	int overtimes = args[9].As<Number>()->Value();
	v8::String::Utf8Value i_reserve1(isolate, args[10]); 
	stringstream ss;
	ss << *i_reserve1;

	string info = "";	
	//info = hd_ecard_consume(mny,i_left,i_top,i_width,i_height,i_alpha,i_r,i_g,i_b,overtimes,(char*)ss.str().c_str());
#if defined(__linux__)
#else
	info = GBKToUTF8(info);
#endif
	auto result = v8::String::NewFromUtf8(isolate, info.c_str(), v8::NewStringType::kNormal, (int)info.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}*/
/*static void Ecardsignin(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	string info = "";	
	//info = hd_ecard_sign_in();
#if defined(__linux__)
#else
	info = GBKToUTF8(info);
#endif
	auto result = v8::String::NewFromUtf8(isolate, info.c_str(), v8::NewStringType::kNormal, (int)info.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}*/
/*static void Ecardsettleaccount(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	string info = "";	
	//info = hd_ecard_settle_account();
#if defined(__linux__)
#else
	info = GBKToUTF8(info);
#endif
	auto result = v8::String::NewFromUtf8(isolate, info.c_str(), v8::NewStringType::kNormal, (int)info.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}*/




  
void Init(Local<Object> exports, Local<Object> module) {  
  NODE_SET_METHOD(exports, "add", Add);
  NODE_SET_METHOD(exports, "runCallback", RunCallback);
  NODE_SET_METHOD(exports, "runCallback2", RunCallback2);
  NODE_SET_METHOD(exports, "hdhardwareinit", HardWareInit);
  NODE_SET_METHOD(exports, "hdhardwareend", HardWareEnd);
  
  /*NODE_SET_METHOD(exports, "hdcoininenable", CoinInEnable);
  NODE_SET_METHOD(exports, "hdcoinindisable", CoinInDisable);
  NODE_SET_METHOD(exports, "hdcoinoutcheck", CoinOutCheck);
  NODE_SET_METHOD(exports, "hdcoinoutreset", CoinOutReset);
  NODE_SET_METHOD(exports, "hdcoinoutpayout", CoinOutPayout);
  NODE_SET_METHOD(exports, "hdcoinoutpayoutbynum", CoinOutPayoutByNum);
  NODE_SET_METHOD(exports, "hdcoinoutpayoutbynum3", CoinOutPayoutByNum3);
  NODE_SET_METHOD(exports, "hdcoinoutclear", CoinOutClear);

  NODE_SET_METHOD(exports, "hdgoodsout", GoodsOut);
  NODE_SET_METHOD(exports, "hdgoodstest", GoodsTest);*/

  NODE_SET_METHOD(exports, "hdprinterecho", PrinterEcho);
  NODE_SET_METHOD(exports, "hdprintercut", PrinterCut);
  /*NODE_SET_METHOD(exports, "hdprinterecho12", PrinterEcho12);
  NODE_SET_METHOD(exports, "hdprinterecho2", PrinterEcho2);
  NODE_SET_METHOD(exports, "hdprinterecho22", PrinterEcho22);*/
  NODE_SET_METHOD(exports, "hdprintercheckstatus", PrinterCheckStatus);
  //NODE_SET_METHOD(exports, "hdprinterfastpic", PrinterFastPic);

  NODE_SET_METHOD(exports, "hdcashinenable", Cashinenable);
  NODE_SET_METHOD(exports, "hdcashindisable", Cashindisable);
  NODE_SET_METHOD(exports, "hdcashincheck", Cashincheck);  
  NODE_SET_METHOD(exports, "hdcashinsetnotesaccept", Cashinsetnotesaccept);
  NODE_SET_METHOD(exports, "hdcashinreset", Cashinreset);
  NODE_SET_METHOD(exports, "hdcashingetDatasetVersion", CashingetDatasetVersion);
  NODE_SET_METHOD(exports, "hdcashingetFirmwareVersion", CashingetFirmwareVersion);
  NODE_SET_METHOD(exports, "hdcashinupgradeFirmwareProgram", CashinupgradeFirmwareProgram);
  
  NODE_SET_METHOD(exports, "hdsensorenable", Sensorenable);
  NODE_SET_METHOD(exports, "hdsensordisable", Sensordisable);
  NODE_SET_METHOD(exports, "hdsensorgetstatus", Sensorgetstatus);
  
  NODE_SET_METHOD(exports, "hdTrafficEnable", TrafficEnable);
  NODE_SET_METHOD(exports, "hdTrafficDisable", TrafficDisable);  
  NODE_SET_METHOD(exports, "hdTrafficReadLog", TrafficReadLog);  
  
  NODE_SET_METHOD(exports, "hdlocksendlock", LockSendLock);

  /*NODE_SET_METHOD(exports, "hdunionconsume", Unionconsume);
  NODE_SET_METHOD(exports, "hdunionsettle", Unionsettle);
  NODE_SET_METHOD(exports, "hdunionsignin", Unionsignin);
  NODE_SET_METHOD(exports, "hdunioncancel", Unioncancel);
  
  NODE_SET_METHOD(exports, "hdICcardsetKey", ICcardsetKey);*/
  NODE_SET_METHOD(exports, "hdICcardenable", ICcardenable);
  NODE_SET_METHOD(exports, "hdICcarddisable", ICcarddisable);
  /*NODE_SET_METHOD(exports, "hdICcardsearchCard", ICcardsearchCard);
  NODE_SET_METHOD(exports, "hdICcardreadCard", ICcardreadCard);
  NODE_SET_METHOD(exports, "hdICcardreadBlock", ICcardreadBlock);
  NODE_SET_METHOD(exports, "hdICcardwriteBlock", ICcardwriteBlock);
  NODE_SET_METHOD(exports, "hdICcardinitWallet", ICcardinitWallet);
  NODE_SET_METHOD(exports, "hdICcardreadBalance", ICcardreadBalance);
  NODE_SET_METHOD(exports, "hdICcardrechargeWallet", ICcardrechargeWallet);
  NODE_SET_METHOD(exports, "hdICcarddeductWallet", ICcarddeductWallet);

  NODE_SET_METHOD(exports, "hdtestjson", TestJson);

  NODE_SET_METHOD(exports, "hdcashoutcheck", Cashoutcheck);
  NODE_SET_METHOD(exports, "hdcashoutreset", Cashoutreset);
  NODE_SET_METHOD(exports, "hdcashoutpayout", Cashoutpayout);

  NODE_SET_METHOD(exports, "hdecardconsume", Ecardconsume);
  NODE_SET_METHOD(exports, "hdecardsignin", Ecardsignin);
  NODE_SET_METHOD(exports, "hdecardsettleaccount", Ecardsettleaccount);*/
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Init)