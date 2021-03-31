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
//#include <gLog.h> //2020-10-21
//#include "Logger.h"
//#include <json/json.h>
#include "Json.h"
#include <libRPDCard.h>
#include <libRFMCard.h>
#include <libGenerationsEpson.h> 
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

//#include "ICTcoinin1.h"
//#include <boost/thread.hpp>
//#include <boost/date_time/posix_time/posix_time.hpp>


//////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////

int g_epson_type=1;
bool g_epson_init=false;
map<string, boost::shared_ptr<ThreadData>> g_map_DevThreadData; //init new\end del
Icash_in *g_cashin = NULL;
bool cashin_InT = false;
Isensor  *g_sensor = NULL;
bool sensor_InT = false;
bool iccard_InT = false;
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
	
	//Json::Value root;
	//Json::FastWriter writer;  
	//root["method"] = method;
	//root["stusData"] = stusData;
	//data->resultjs = writer.write(root);
	Json root;
	root.put("method", method);
	root.put("stusData", stusData);
	data->resultjs = root.toString();
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
	//Json::Value root;
	//Json::FastWriter writer;  
	Json root;
	cout<<"cash_in_event_code="<<cash_in_event_code<<endl;
	//root["eventcode"] = cash_in_event_code;
	//root["eventcodeDescription"] = event_code_description;
	//root["money"] = money;
	//root["currency"] = str.c_str(); //2020-10-20
	//data->resultjs = writer.write(root);
	root.put("eventcode", cash_in_event_code);
	root.put("eventcodeDescription", event_code_description);
	root.put("money", money);
	root.put("currency", str.c_str());
	data->resultjs = root.toString();

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
		//LOGINFO"callGoThread in thread deviceName:icCard");
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
					bRet = IRPD_Request(1, buf);
					//cout<<"IRPD_Request end"<<endl;
					if (bRet){		
						memset(buf, 0x00, sizeof(buf));
						bRet = IRPD_Anticoll(buf);
						if(bRet){
							nRet = IRPD_Select(buf);
							if (nRet >= 0){
								bRet = IRPD_VerifyPassWord(1, devParam->blocknum);
								if (bRet){
									string o_data = IRPD_ReadBlock(devParam->blocknum);
									if (o_data != ""){	
										IRPD_Beep(10);
										#if defined(__linux__)
										sleep(1); 
										#else
										Sleep(1000);
										#endif

										//Json::Value root;
										//Json::FastWriter writer;  
										//root["cardno"] = o_data;
										//devParam->resultjs = writer.write(root);
										Json root;
										root.put("cardno", o_data);
										devParam->resultjs = root.toString();
										uv_async_send(devParam->callbackSendReq); 									
									}
								}
							}
						}
					}
				}
				else if(devParam->subDev == 0)
				{	//rfm
					//cout<<"icCard enable"<<endl;
					bool bRet; char bufdata[33] = {0};
					bRet = IRFM_operateAntenna(true);
					if(bRet){
						string res = IRFM_searchCard(0);
						if(res != "")
						{
							memset(buf, 0x00, sizeof(buf));
							memcpy(buf, res.data(), res.size());
							string serial = RFM_preventConflict();
							if(serial != "")
							{
								string str=RFM_selectCard(serial);
								if(str != "")
								{
									bRet = RFM_verifyPassWord(1, devParam->blocknum);
									if(bRet)
									{
										string o_data=RFM_readBlock(devParam->blocknum);
										if(o_data != "")
										{
											memset(bufdata, 0x00, sizeof(bufdata)); 
											memcpy(bufdata, o_data.c_str(), MINFUNCTION(o_data.length(), 32));
											RFM_beer(2);
											RFM_beer(1);
											//Json::Value root;
											//Json::FastWriter writer;  
											//root["cardno"] = bufdata;
											//devParam->resultjs = writer.write(root);
											Json root;
											root.put("cardno", bufdata);
											devParam->resultjs = root.toString();
											
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
			//LOGINFO"callGoThread icCard while break IRPD_cleanUp");
			IRPD_cleanUp();
		}
		if(devParam->subDev == 0){
			//LOGINFO"callGoThread icCard while break IRFM_cleanUp");
			IRFM_cleanUp();
		}
		
		iccard_InT = false;
		//LOGINFO"callGoThread out thread deviceName:icCard");
	} //icCard
	else if(devParam->deviceName == "sensor")
	{		
		if(devParam->subDev == 0){
			//LOGINFO"callGoThread in thread deviceName:sensor");
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
			//LOGINFO"callGoThread out thread deviceName:sensor");
		}else{
			return;
		}
	}
	else if(devParam->deviceName == "cashin")
	{
		if(devParam->subDev == 1){ //itl-nv9
			//LOGINFO"callGoThread in thread deviceName:itl-nv9");
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
			//LOGINFO"callGoThread out thread deviceName:itl-nv9");
		}else if(devParam->subDev == 2){ //ict-v77e		
			//LOGINFO"callGoThread in thread deviceName:ict-v77e");		
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
			//LOGINFO"callGoThread out thread deviceName:ict-v77e");
		}else{
			return;
		}
	} //cashin
}

void freeGoThreadData(DevParams* data) 
{
	if(data == 0){
		//LOGINFO"freeGoThreadData DevParams is NULL, return");
		return;
	}
	//LOGINFO"freeGoThreadData 1");
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
	//LOGINFO"freeGoThreadData 2");
}

void afterGoTread (uv_work_t* req, int status) {
	if(req == 0){
		//LOGINFO"afterGoTread uv_work_t is NULL, return");
		return;
	}
	//LOGINFO"afterGoTread 1");
    auto* devParam = (DevParams*) req->data;
    char buf[1024] = {0};
    sprintf(buf, "afterGoTread deviceName:%s", devParam->deviceName.c_str());    
    devParam->status = 2;
    HandleScope handle_scope(Isolate::GetCurrent()); //这里是必须的,调用js函数需要一个handle scope
    uv_close((uv_handle_t*) devParam->callbackSendReq, close_cb); //3.删除通知js的事件
    freeGoThreadData(devParam); //释放内存
	//LOGINFO"afterGoTread 2");
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
	//LOGINFO"HardWareEnd =>");
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	//exit thread
	//LOGINFO"HardWareEnd => 1");
	map<string, boost::shared_ptr<ThreadData>>::iterator it = g_map_DevThreadData.find("icCard");
	if(it != g_map_DevThreadData.end())
	{
		//LOGINFO"HardWareEnd => ready to exit icCard thread");
		boost::shared_ptr<ThreadData> td = it->second;
		if(td.get() != NULL)
		{
			if(!td->param->exit) td->param->exit = true;
		}
		//LOGINFO"HardWareEnd => wait for exit icCard thread");
		while(1){
			if(!iccard_InT)break;
			#if defined(__linux__)
			usleep(100); 
			#else
			Sleep(100);
			#endif
		}
		//LOGINFO"HardWareEnd => exited icCard thread.");
		
		g_map_DevThreadData.erase(it);
		
		#if defined(__linux__)
		usleep(500); 
		#else
		Sleep(500);
		#endif
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
	//LOGINFO"HardWareEnd => 2");
	map<string, boost::shared_ptr<ThreadData>>::iterator it1 = g_map_DevThreadData.find("sensor");
	if(it1 != g_map_DevThreadData.end())
	{
		//LOGINFO"HardWareEnd => ready to exit sensor thread");
		boost::shared_ptr<ThreadData> td = it1->second;
		if(td != NULL)
		{
			if(!td->param->exit) td->param->exit = true;
		}
		//LOGINFO"HardWareEnd => wait for exit sensor thread");
		while(1){
			if(!sensor_InT)break;
			#if defined(__linux__)
			usleep(100); 
			#else
			Sleep(100);
			#endif
		}
		//LOGINFO"HardWareEnd => exited sensor thread.");
		
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

	//LOGINFO"HardWareEnd => 3");
	map<string, boost::shared_ptr<ThreadData>>::iterator it2 = g_map_DevThreadData.find("cashin");
	if(it2 != g_map_DevThreadData.end())
	{
		boost::shared_ptr<ThreadData> td = it2->second;
		//LOGINFO"HardWareEnd => ready to exit cashin thread");
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
		//LOGINFO"HardWareEnd => wait for exit cashin thread");
		while(1){
			if(!cashin_InT)break;
			#if defined(__linux__)
			usleep(100); 
			#else
			Sleep(100);
			#endif
		}
		//LOGINFO"HardWareEnd => exited cashin thread.");
		
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
	
	//LOGINFO"HardWareEnd => 4");
	if(g_epson_init){
		//cout<<"lib_printer_end"<<endl;
		lib_printer_end();
	}
	
	//Json::Value root;
	//Json::FastWriter write;	
	Json root;
	std::string json_out = "";
	//root["result_code"] = 0;
	//root["result_msg"] = "success";
	//json_out = write.write(root);
	root.put("result_code", 0);
	root.put("result_msg", "success");
	json_out = root.toString();
	auto result = v8::String::NewFromUtf8(isolate, json_out.c_str(), v8::NewStringType::kNormal, (int)json_out.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
	//LOGINFO"HardWareEnd result:%s", json_out.c_str());
}

void HardWareInit(const FunctionCallbackInfo<Value>& args) 
{
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
  //Json::Value outRoot;
  Json outRoot;
  //Json::Value outData;
  Json outData;
  //Json::Value outdevcoin;
  Json outdevcoin;
  //Json::Value outdevcashin;
  Json outdevcashin;
  //Json::Value outdevsensor;
  Json outdevsensor;
  //Json::Value outdevcashout;
  Json outdevcashout;
  //Json::Value outdevprinter;
  Json outdevprinter;
  //Json::Value outdevunion;
  Json outdevunion;
  //Json::Value outdeviCcard;
  Json outdeviCcard;
  //Json::FastWriter outWriter;  
  string outJson;
  //outRoot["result_code"] = 0;
  //outRoot["result_msg"] = "success";
  outRoot.put("result_code", 0);
  outRoot.put("result_msg", "success");

  //v8::String::Utf8Value execPath(args[0]->ToString());  
  Local<Value> v = args[0]; 
  v8::String::Utf8Value execPath(isolate, v); 
  
  stringstream ss;
  ss << *execPath;
  
  stringstream ss2;
  v8::String::Utf8Value callbackJSFile(isolate, args[1]); 
  ss2 << *callbackJSFile;
  
  v8::String::Utf8Value logpath(isolate, args[3]); 
  stringstream ss4;
  ss4 << *logpath;
  //cout<<"logpath:"<<ss4.str().c_str()<<endl;
  if(!glogInit){
	  glogInit = true;
	  //LOGINIT(ss4.str().c_str(), ""); //2020-10-22
  }
  //LOGINFO"HardWareInit =>");
  //LOGINFO"HardWareInit VER:KT_NJGYPCC_1.0.7");
  
  v8::String::Utf8Value serverdatetime(isolate, args[4]); 
  stringstream ss5;
  ss5 << *serverdatetime;
  //cout<<"serverdatetime:"<<ss5.str().c_str()<<endl;

  //v8插件
  //HardWareApp::start(isolate, context, ss.str().c_str(), "", ss2.str().c_str());
  
  //in json
  v8::String::Utf8Value inijs(isolate, args[2]); 
  //Json::Reader reader;
  //Json::Value  root; //解析
  //Json::Value  jsonVal_coin;
  int m_coin_type=-1;
  int m_national_cny=-1;
  char m_serial_coin_in[32] = {0};
  int m_include_coin_out_half = -1;
  int m_include_coin_out_one = -1;
  int m_box_coin_out_one = -1;
  int m_serial_coin_out_half = -1;
  int m_serial_coin_out_one = -1;
  //Json::Value  jsonVal_cashin;
  int m_cash_in_type = -1;
  char m_serial_cash_in[32] = {0};
  int m_cash_accept = -1;
  int m_cash_model = -1;
  //Json::Value	jsonVal_sensor;
  char m_serial_sensor[32] = {0};
  int sensortype = -1;
  
  //Json::Value  jsonVal_cashout;
  Json jsonVal_cashout;
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
  //Json::Value  jsonVal_printer;
  int m_printer_type = -1;
  char m_serial_printer[32] = {0};
  int m_baudrate_printer = -1;
  //Json::Value  jsonVal_union;
  Json jsonVal_union;
  int m_union_type = -1;
  char m_serial_union[32] = {0};
  int m_union_zhongyu = -1;
  int m_union_BsweepC = -1;
  //Json::Value  jsonVal_iccard;
  static int m_iCcard_type = -1;
  char m_serial_iccard[32] = {0};
  //Json::Value  jsonVal_system;
  string m_authorization_code = "";
  //cout<<"1"<<endl;
  stringstream ss6;
  ss6 << *inijs;
  //LOGINFO"HardWareInit inijs:%s", ss6.str().c_str());
  //if(reader.parse(*inijs, root))
  Json root(*inijs);
  if(1)
  {
	  //cout<<"2"<<endl;
	  //jsonVal_system = root["System"];
	  //m_authorization_code = jsonVal_system["authorization_code"].asString();
	  boost::property_tree::ptree pt = root.getObject("System");
	  Json jsonVal_system(Json::toString(pt));
	  m_authorization_code = jsonVal_system.getString("authorization_code");
	  //coin
	  /*jsonVal_coin = root["CoinDevType"];
	  m_coin_type = atoi(jsonVal_coin["coin_type"].asString().c_str());
	  m_national_cny = atoi(jsonVal_coin["national_cny"].asString().c_str());*/
	  pt = root.getObject("CoinDevType");
	  Json jsonVal_coin(Json::toString(pt));
	  m_coin_type = atoi(jsonVal_coin.getString("coin_type").c_str());
	  m_national_cny = atoi(jsonVal_coin.getString("national_cny").c_str());
	  if(m_coin_type != -1){
	  }else{
		  /*outRoot["result_code"] = -1;
		  outRoot["result_msg"] = "硬件初始化失败！";
		  outdevcoin["dev_name"] = "coin";
		  outdevcoin["init_result"] = false;
		  outdevcoin["init_msg"] = "不包含硬币器模块"; 
		  outData.append(outdevcoin);*/
		  outRoot.put("result_code", -1);
		  outRoot.put("result_msg", "硬件初始化失败！");
		  outdevcoin.put("dev_name", "coin");
		  outdevcoin.put("init_result", false);
		  outdevcoin.put("init_msg", "不包含硬币器模块");
		  outData.putObject("", outdevcoin.getObject());
	  }
	  //printer
	  pt = root.getObject("PrinterDevType");
	  Json jsonVal_printer(Json::toString(pt));
	  m_printer_type = atoi(jsonVal_printer.getString("printer_type").c_str()); //3一代 4二代
	  if(m_printer_type == 3)
		  g_epson_type = 1;
	  else if(m_printer_type == 4)
		  g_epson_type = 2;
	  else if(m_printer_type == 6) //2020-08-12
		  g_epson_type = 3;
      strcpy(m_serial_printer, jsonVal_printer.getString("serial_printer").c_str());
      m_baudrate_printer = atoi(jsonVal_printer.getString("baudrate_printer").c_str());
	  /*jsonVal_printer = root["PrinterDevType"];
	  m_printer_type = atoi(jsonVal_printer["printer_type"].asString().c_str()); //3一代 4二代	 
	  if(m_printer_type == 3)
		  g_epson_type = 1;
	  else if(m_printer_type == 4)
		  g_epson_type = 2;
	  else if(m_printer_type == 6) //2020-08-12
		  g_epson_type = 3;
      strcpy(m_serial_printer, jsonVal_printer["serial_printer"].asString().c_str());
      m_baudrate_printer = atoi(jsonVal_printer["baudrate_printer"].asString().c_str());*/
	  if(m_printer_type != -1){
		  g_epson_init = lib_printer_init(0, m_serial_printer, m_baudrate_printer, ss4.str().c_str());
		  if (!g_epson_init)
		  {
			  /*outRoot["result_code"] = -1;
			  outRoot["result_msg"] = "硬件初始化失败！";
			  outdevprinter["dev_name"] = "printer";
			  outdevprinter["init_result"] = false;
			  outdevprinter["init_msg"] = "打印机初始化失败"; 
			  outData.append(outdevprinter);*/
			  outRoot.put("result_code", -1);
			  outRoot.put("result_msg", "硬件初始化失败！");
			  outdevprinter.put("dev_name", "printer");
			  outdevprinter.put("init_result", false);
			  outdevprinter.put("init_msg", "打印机初始化失败");
			  outData.putObject("", outdevprinter.getObject());
		  }
		  else
		  {
			  /*outdevprinter["dev_name"] = "printer";
			  outdevprinter["init_result"] = true;
			  outdevprinter["init_msg"] = "打印机初始化成功";
			  outData.append(outdevprinter);*/
			  outdevprinter.put("dev_name", "printer");
			  outdevprinter.put("init_result", true);
			  outdevprinter.put("init_msg", "打印机初始化成功");
			  outData.putObject("", outdevprinter.getObject());
		  }
	  }else{
		  /*outRoot["result_code"] = -1;
		  outRoot["result_msg"] = "硬件初始化失败！";
		  outdevprinter["dev_name"] = "printer";
		  outdevprinter["init_result"] = false;
		  outdevprinter["init_msg"] = "不包含打印机模块"; 
		  outData.append(outdevprinter);*/
		  outRoot.put("result_code", -1);
		  outRoot.put("result_msg", "硬件初始化失败！");
		  outdevprinter.put("dev_name", "printer");
		  outdevprinter.put("init_result", false);
		  outdevprinter.put("init_msg", "不包含打印机模块"); 
		  outData.putObject("", outdevprinter.getObject());
	  }
	  //iccard
	  pt = root.getObject("ICcardDevType");
	  Json jsonVal_iccard(Json::toString(pt));
	  m_iCcard_type = atoi(jsonVal_iccard.getString("iCcard_type").c_str());
	  strcpy(m_serial_iccard, jsonVal_iccard.getString("serial_iccard").c_str());
	  /*jsonVal_iccard = root["ICcardDevType"];
	  m_iCcard_type = atoi(jsonVal_iccard["iCcard_type"].asString().c_str());
	  strcpy(m_serial_iccard, jsonVal_iccard["serial_iccard"].asString().c_str());*/	
	  unsigned char *keya_[16]; unsigned char *keyb_[16];	
	  if(m_iCcard_type == 1){
		  if(IRPD_init(0, m_serial_iccard, keya_, keyb_, 0, ss4.str().c_str()) == 0)
		  {
			 /*outdeviCcard["dev_name"] = "icCard";
			 outdeviCcard["init_result"] = true;
			 outdeviCcard["init_msg"] = "IC读卡器初始化成功";
			 outData.append(outdeviCcard);*/
			 outdeviCcard.put("dev_name", "icCard");
			 outdeviCcard.put("init_result", true);
			 outdeviCcard.put("init_msg", "IC读卡器初始化成功");
			 outData.putObject("", outdeviCcard.getObject());
		  }
		  else
		  {
			 /*outRoot["result_code"] = -1;
			 outRoot["result_msg"] = "硬件初始化失败！";
			 outdeviCcard["dev_name"] = "icCard";
			 outdeviCcard["init_result"] = false;
			 outdeviCcard["init_msg"] = "IC读卡器初始化失败"; 
			 outData.append(outdeviCcard);*/
			 //IRPD_cleanUp();
			 outRoot.put("result_code", -1);
			 outRoot.put("result_msg", "硬件初始化失败！");
			 outdeviCcard.put("dev_name", "icCard");
			 outdeviCcard.put("init_result", false);
			 outdeviCcard.put("init_msg", "IC读卡器初始化失败"); 
			 outData.putObject("", outdeviCcard.getObject());
		  }		  
	  }else if(m_iCcard_type == 0){
		if(IRFM_init(0, m_serial_iccard, keya_, keyb_, 0, ss4.str().c_str()) == 0)
		{
			 /*outdeviCcard["dev_name"] = "icCard";
			 outdeviCcard["init_result"] = true;
			 outdeviCcard["init_msg"] = "IC读卡器初始化成功";
			 outData.append(outdeviCcard);*/
			 outdeviCcard.put("dev_name", "icCard");
			 outdeviCcard.put("init_result", true);
			 outdeviCcard.put("init_msg", "IC读卡器初始化成功");
			 outData.putObject("", outdeviCcard.getObject());
		}
		else
		{
			 /*outRoot["result_code"] = -1;
			 outRoot["result_msg"] = "硬件初始化失败！";
			 outdeviCcard["dev_name"] = "icCard";
			 outdeviCcard["init_result"] = false;
			 outdeviCcard["init_msg"] = "IC读卡器初始化失败"; 
			 outData.append(outdeviCcard);*/
			 outRoot.put("result_code", -1);
			 outRoot.put("result_msg", "硬件初始化失败！");
			 outdeviCcard.put("dev_name", "icCard");
			 outdeviCcard.put("init_result", false);
			 outdeviCcard.put("init_msg", "IC读卡器初始化失败"); 
			 outData.putObject("", outdeviCcard.getObject());
			 //IRFM_cleanUp();
		}
	  }else{
		/*outRoot["result_code"] = -1;
	   	outRoot["result_msg"] = "硬件初始化失败！";
	   	outdeviCcard["dev_name"] = "icCard";
	   	outdeviCcard["init_result"] = false;
	   	outdeviCcard["init_msg"] = "不包含读卡器模块"; 
	   	outData.append(outdeviCcard);	*/		
		outRoot.put("result_code", -1);
	   	outRoot.put("result_msg", "硬件初始化失败！");
	   	outdeviCcard.put("dev_name", "icCard");
	   	outdeviCcard.put("init_result", false);
	   	outdeviCcard.put("init_msg", "不包含读卡器模块"); 
	   	outData.putObject("", outdeviCcard.getObject());			
	  }
	  
	  //sensor
	  bool toInitFlag = false;
	  //jsonVal_sensor = root["SensorDevType"];
	  //sensortype = atoi(jsonVal_sensor["SensorDev_Type"].asString().c_str());
	  //cout<<"SensorDev_Type="<<sensortype<<endl;
	  //strcpy(m_serial_sensor, jsonVal_sensor["serial_sensor"].asString().c_str());
	  pt = root.getObject("SensorDevType");
	  Json jsonVal_sensor(Json::toString(pt));
	  sensortype = atoi(jsonVal_sensor.getString("SensorDev_Type").c_str());
	  strcpy(m_serial_sensor, jsonVal_sensor.getString("serial_sensor").c_str());
	  if(sensortype != -1)
	  {
		  //cout<<"3"<<endl;
		  if(sensortype == 0){
			  //g_sensor = new sensor_generation(sensorhandler, m_serial_sensor);
			  //toInitFlag = true;
		  }
		  else{
			  outRoot.put("result_code", -1);
			  outRoot.put("result_msg", "硬件初始化失败！");
			  outdevsensor.put("dev_name", "sensor");
			  outdevsensor.put("init_result", false);
			  outdevsensor.put("init_msg", "未知的传感器"); 
			  outData.putObject("", outdevsensor.getObject());
		  }
		  //cout<<"4"<<endl;
	  }
	  else
	  {
		  outRoot.put("result_code", -1);
		  outRoot.put("result_msg", "硬件初始化失败！");
		  outdevsensor.put("dev_name", "sensor");
		  outdevsensor.put("init_result", false);
		  outdevsensor.put("init_msg", "不包含传感器模块"); 
		  outData.putObject("", outdevsensor.getObject());
	  }
	  //cout<<"5"<<endl;
	  if(toInitFlag)
	  {
		  //cout<<"6"<<endl;
		  if(g_sensor == NULL)
		  {
			  outRoot.put("result_code", -18);
			  outRoot.put("result_msg", "传感器模块初始化失败(对象实例化失败)");
			  outdevsensor.put("dev_name", "sensor");
			  outdevsensor.put("init_result", false);
			  outdevsensor.put("init_msg", "传感器实例化失败"); 
			  outData.putObject("", outdevsensor.getObject());
		  }
		  else
		  {
			  //cout<<"7"<<endl;
			  int sensor_init_res = g_sensor->sensor_init(ss4.str().c_str());
			  //cout<<"8"<<endl;
			  if(sensor_init_res == 0)
			  {
				  //cout<<"9"<<endl;
				  outdevsensor.put("dev_name", "sensor");
				  outdevsensor.put("init_result", true);
				  outdevsensor.put("init_msg", "传感器初始化成功"); 
				  outData.putObject("", outdevsensor.getObject());
			  }
			  else
			  {
				  //cout<<"10"<<endl;
				  outRoot.put("result_code", -1);
				  outRoot.put("result_msg", "硬件初始化失败！");
				  outdevsensor.put("dev_name", "sensor");
				  outdevsensor.put("init_result", false);
				  outdevsensor.put("init_msg", "传感器初始化失败！"); 
				  outData.putObject("", outdevsensor.getObject());
			  }
		  }
	  }
	  //cashin 
	  /*jsonVal_cashin = root["CashInDevType"];
	  m_cash_in_type = atoi(jsonVal_cashin["cash_in_type"].asString().c_str());
	  strcpy(m_serial_cash_in, jsonVal_cashin["serial_cash_in"].asString().c_str());
	  m_cash_accept = atoi(jsonVal_cashin["cash_accept"].asString().c_str());
	  m_cash_model = atoi(jsonVal_cashin["cash_model"].asString().c_str());*/
	  pt = root.getObject("CashInDevType");
	  Json jsonVal_cashin(Json::toString(pt));
	  m_cash_in_type = atoi(jsonVal_cashin.getString("cash_in_type").c_str());
	  strcpy(m_serial_cash_in, jsonVal_cashin.getString("serial_cash_in").c_str());
	  m_cash_accept = atoi(jsonVal_cashin.getString("cash_accept").c_str());
	  m_cash_model = atoi(jsonVal_cashin.getString("cash_model").c_str());
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
			  /*outRoot["result_code"] = -1;
			  outRoot["result_msg"] = "硬件初始化失败！";
			  outdevcashin["dev_name"] = "cashin";
			  outdevcashin["init_result"] = false;
			  outdevcashin["init_msg"] = "未知的纸币接收器"; 
			  outData.append(outdevcashin);*/
			  outRoot.put("result_code", -1);
			  outRoot.put("result_msg", "硬件初始化失败！");
			  outdevcashin.put("dev_name", "cashin");
			  outdevcashin.put("init_result", false);
			  outdevcashin.put("init_msg", "未知的纸币接收器"); 
			  outData.putObject("", outdevcashin.getObject());
			  break;
		  }
	  }else{
		  /*outRoot["result_code"] = -1;
		  outRoot["result_msg"] = "硬件初始化失败！";
		  outdevcashin["dev_name"] = "cashin";
		  outdevcashin["init_result"] = false;
		  outdevcashin["init_msg"] = "不包含纸币接收器模块"; 
		  outData.append(outdevcashin);*/
		  outRoot.put("result_code", -1);
		  outRoot.put("result_msg", "硬件初始化失败！");
		  outdevcashin.put("dev_name", "cashin");
		  outdevcashin.put("init_result", false);
		  outdevcashin.put("init_msg", "不包含纸币接收器模块"); 
		  outData.putObject("", outdevcashin.getObject());
	  }	  
	  if(toInitFlag)
	  {
		  if(g_cashin == NULL)
		  {
			  /*outRoot["result_code"] = -18;
			  outRoot["result_msg"] = "纸币接收模块初始化失败(对象实例化失败)";
			  outdevcashin["dev_name"] = "cashin";
			  outdevcashin["init_result"] = false;
			  outdevcashin["init_msg"] = "纸币接收器实例化失败"; 
			  outData.append(outdevcashin);*/
			  outRoot.put("result_code", -18);
			  outRoot.put("result_msg", "纸币接收模块初始化失败(对象实例化失败)");
			  outdevcashin.put("dev_name", "cashin");
			  outdevcashin.put("init_result", false);
			  outdevcashin.put("init_msg", "纸币接收器实例化失败"); 
			  outData.putObject("", outdevcashin.getObject());
		  }
		  else
		  {
			  int cashin_init_res = 0;
			  if(g_cashin != NULL) cashin_init_res = g_cashin->cash_in_init(ss4.str().c_str());
			  if(cashin_init_res != 0)
			  {
				  /*outdevcashin["dev_name"] = "cashin";
				  outdevcashin["init_result"] = true;
				  outdevcashin["init_msg"] = "纸币接收器初始化成功"; 
				  outData.append(outdevcashin);*/
				  outdevcashin.put("dev_name", "cashin");
				  outdevcashin.put("init_result", true);
				  outdevcashin.put("init_msg", "纸币接收器初始化成功"); 
				  outData.putObject("", outdevcashin.getObject());
			  }
			  else
			  {
				  /*outRoot["result_code"] = -1;
				  outRoot["result_msg"] = "硬件初始化失败！";
				  outdevcashin["dev_name"] = "cashin";
				  outdevcashin["init_result"] = false;
				  outdevcashin["init_msg"] = "纸币接收初始化失败！"; 
				  outData.append(outdevcashin);*/
				  outRoot.put("result_code", -1);
				  outRoot.put("result_msg", "硬件初始化失败！");
				  outdevcashin.put("dev_name", "cashin");
				  outdevcashin.put("init_result", false);
				  outdevcashin.put("init_msg", "纸币接收初始化失败！"); 
				  outData.putObject("", outdevcashin.getObject());
			  }
		  }
	  }
  }
   
  //创建线程
  if(outdevsensor.getString("init_result") == "true")
  {
	  //LOGINFO"HardWareInit 创建传感器线程");
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
	  sensorParam->subDev = sensortype;
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
  if(outdevcashin.getString("init_result") == "true")
  {
	  //LOGINFO"HardWareInit 创建纸币接收器线程");
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
  if(outdeviCcard.getString("init_result") == "true")
  {	  
	  //LOGINFO"HardWareInit 创建读卡器线程");
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
  
  //outRoot["data"] = outData;
  //outJson = outWriter.write(outRoot);
  outRoot.putObject("data", outData.getObject());
  outJson = outRoot.toString();

  //Local<Number> num = Number::New(isolate, ret);
  //args.GetReturnValue().Set(num);
  auto result = v8::String::NewFromUtf8(isolate, outJson.c_str(), v8::NewStringType::kNormal, (int)outJson.size()).ToLocalChecked();
  args.GetReturnValue().Set(result);
  //LOGINFO"HardWareInit result:%s", outJson.c_str());
}

/*static void CoinInEnable(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	string info = "";
	//threadData->enable = true;
	info = "enable succ";//hd_coin_in_enable();
	auto result = v8::String::NewFromUtf8(isolate, info.c_str(), v8::NewStringType::kNormal, (int)info.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}*/
/*static void CoinInDisable(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	string info = "";
	//info = hd_coin_in_disable();
	auto result = v8::String::NewFromUtf8(isolate, info.c_str(), v8::NewStringType::kNormal, (int)info.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}*/
/*static void CoinOutCheck(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	string info = "";
	//info = hd_coin_out_check();
	auto result = v8::String::NewFromUtf8(isolate, info.c_str(), v8::NewStringType::kNormal, (int)info.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}*/
/*static void CoinOutReset(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	string info = "";
	//info = hd_coin_out_reset();
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
	auto result = v8::String::NewFromUtf8(isolate, info.c_str(), v8::NewStringType::kNormal, (int)info.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}*/
/*static void CoinOutClear(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	string info = "";
	//info = hd_coin_out_clear();
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
	auto result = v8::String::NewFromUtf8(isolate, info.c_str(), v8::NewStringType::kNormal, (int)info.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}*/
static void PrinterCheckStatus(const FunctionCallbackInfo<Value>& args) 
{
	//LOGINFO"PrinterCheckStatus =>");
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	//Json::Value root;
	Json root;
	int ret = lib_printer_get_status(); //-1未初始化不能调用接口 -2失败(写串口错误) 0有纸 1纸尽 2纸将尽 3未知状态
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
		//root["result_code"] = 0;
		//root["result_msg"] = "success";
		root.put("result_code", 0);
		root.put("result_msg", "success");
	}
	else
	{
		//root["result_code"] = -54;
		//root["result_msg"] = "fail";	
		root.put("result_code", -54);
		root.put("result_msg", "fail");			
	}
	//root["printer_error_code"] =stu;
	//root["printer_error_msg"] = get_status_str(ret);
	root.put("printer_error_code", stu);
	
	//Json::FastWriter outWriter;  
    string outJson = "";
	//outJson = outWriter.write(root);
	outJson = root.toString();

	auto result = v8::String::NewFromUtf8(isolate, outJson.c_str(), v8::NewStringType::kNormal, (int)outJson.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
	//LOGINFO"PrinterCheckStatus result:%s", outJson.c_str());
}
static void PrinterCut(const FunctionCallbackInfo<Value>& args) 
{
	//LOGINFO"PrinterCut =>");
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	//Json::Value root;
	Json root;
	int ret = lib_printer_cut(1); //1全切 2半切 3进纸半切 4半切
	if(ret == 0)
	{
		//root["result_code"] = 0;
		//root["result_msg"] = "success";
		root.put("result_code", 0);
		root.put("result_msg", "success");
	}
	else
	{
		//root["result_code"] = -54;
		//root["result_msg"] = "fail";		
		root.put("result_code", -54);
		root.put("result_msg", "fail");		
	}
	//root["printer_error_code"] =ret;
	//root["printer_error_msg"] = get_status_str(ret);
	root.put("printer_error_code", ret);
	
	//Json::FastWriter outWriter;  
    string outJson = "";
	//outJson = outWriter.write(root);
	outJson = root.toString();

	auto result = v8::String::NewFromUtf8(isolate, outJson.c_str(), v8::NewStringType::kNormal, (int)outJson.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
	//LOGINFO"PrinterCut result:%s", outJson.c_str());
}
static void PrinterEcho(const FunctionCallbackInfo<Value>& args) 
{
	//LOGINFO"PrinterEcho =>");
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
	//Json::Value root;
	Json root;
	string str2 = "";
	string str = "";
	int ret = 0;
	switch(flag)
	{
		case 1:
			str = "打印文本";
			lib_printer_set_strong(bold1);
			lib_printer_set_align(align);
			lib_printer_set_size(size);
			ret = lib_printer_println((char*)ss.str().c_str());
			if(size != 0)
				lib_printer_set_size(0);
			if(bold1 != 0)
				lib_printer_set_strong(0);

			break;
		case 2:
			str = "打印条码";
			lib_printer_set_align(align);
			ret = lib_printer_printBarcode(g_epson_type,100,2,2,align,(char*)ss.str().c_str());
			break;
		case 3: 
			str = "小票打印机打印二维码";
			lib_printer_set_align(align);
			ret = lib_printer_printQr(g_epson_type, (char*)ss.str().c_str(), size, 2);
			break;
	}
	if(align != 0)
		lib_printer_set_align(0);
	
	if(ret == 0){
		str2 = str += "成功";
		//root["result_code"] = 0;
		//root["result_msg"] = "success";
		root.put("result_code", 0);
		root.put("result_msg", "success");
	}else{
		str2 = str + "失败";
		//root["result_code"] = -15;
		//root["result_msg"] = str2;
		root.put("result_code", -15);
		root.put("result_msg", str2);
	}
	//root["printer_error_code"] =ret;
	//root["printer_error_msg"] = get_status_str(ret);
	root.put("printer_error_code", ret);
	
	//Json::FastWriter outWriter;  
    string outJson = "";
	//outJson = outWriter.write(root);
	outJson = root.toString();

	//outJson = hd_printer_echo(flag, ss.str().c_str(), size, bold1, align);
	auto result = v8::String::NewFromUtf8(isolate, outJson.c_str(), v8::NewStringType::kNormal, (int)outJson.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
	//LOGINFO"PrinterEcho result:%s", outJson.c_str());
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
	auto result = v8::String::NewFromUtf8(isolate, info.c_str(), v8::NewStringType::kNormal, (int)info.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}*/

static void Sensorenable(const FunctionCallbackInfo<Value>& args) 
{
	//LOGINFO"Sensorenable =>");
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	//Json::Value root;
	//Json::FastWriter write;	
	Json root;
	std::string json_out = "";
	
	map<string, boost::shared_ptr<ThreadData>>::iterator it = g_map_DevThreadData.find("sensor");
	if(it != g_map_DevThreadData.end())
	{
		boost::shared_ptr<ThreadData> td = it->second;
		json_out = g_sensor->sensor_enable();
	}
	else
	{
		//root["result_code"] = -7;
		//root["result_msg"] = "传感器初始化失败";
		root.put("result_code", -7);
		root.put("result_msg", "传感器初始化失败");
		//json_out = write.write(root);
		json_out = root.toString();
	}

	auto result = v8::String::NewFromUtf8(isolate, json_out.c_str(), v8::NewStringType::kNormal, (int)json_out.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
	//LOGINFO"Sensorenable result:%s", json_out.c_str());
}
static void Sensordisable(const FunctionCallbackInfo<Value>& args) 
{
	//LOGINFO"Sensordisable =>");
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	//Json::Value root;
	//Json::FastWriter write;	
	Json root;
	std::string json_out = "";
	
	map<string, boost::shared_ptr<ThreadData>>::iterator it = g_map_DevThreadData.find("sensor");
	if(it != g_map_DevThreadData.end())
	{
		boost::shared_ptr<ThreadData> td = it->second;
		json_out = g_sensor->sensor_disable();
	}
	else
	{
		//root["result_code"] = -7;
		//root["result_msg"] = "传感器初始化失败";
		//json_out = write.write(root);
		root.put("result_code", -7);
		root.put("result_msg", "传感器初始化失败");
		json_out = root.toString();
	}

	auto result = v8::String::NewFromUtf8(isolate, json_out.c_str(), v8::NewStringType::kNormal, (int)json_out.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
	//LOGINFO"Sensordisable result:%s", json_out.c_str());
}
static void Sensorgetstatus(const FunctionCallbackInfo<Value>& args) 
{
	//LOGINFO"Sensorgetstatus =>");
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	//Json::Value root;
	//Json::FastWriter write;	
	Json root;
	std::string json_out = "";
	
	map<string, boost::shared_ptr<ThreadData>>::iterator it = g_map_DevThreadData.find("sensor");
	if(it != g_map_DevThreadData.end())
	{
		boost::shared_ptr<ThreadData> td = it->second;
		json_out = g_sensor->sensor_get_Status();
	}
	else
	{
		/*root["result_code"] = -61;
		root["result_msg"] = "票取走传感器获取状态失败";
		root["sensor_error_code"] = -1;
		root["sensor_error_msg"] = "传感器初始化失败";
		json_out = write.write(root);*/
		root.put("result_code", -61);
		root.put("result_msg", "票取走传感器获取状态失败");
		root.put("sensor_error_code", -1);
		root.put("sensor_error_msg", "传感器初始化失败");
		json_out = root.toString();
	}

	auto result = v8::String::NewFromUtf8(isolate, json_out.c_str(), v8::NewStringType::kNormal, (int)json_out.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
	//LOGINFO"Sensorgetstatus result:%s", json_out.c_str());
}
  
static void Cashinenable(const FunctionCallbackInfo<Value>& args) 
{
	//LOGINFO"Cashinenable =>");
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	//Json::Value root;
	//Json::FastWriter write;	
	Json root;
	std::string json_out = "";
	
	map<string, boost::shared_ptr<ThreadData>>::iterator it = g_map_DevThreadData.find("cashin");
	if(it != g_map_DevThreadData.end())
	{
		boost::shared_ptr<ThreadData> td = it->second;
		if(g_cashin != NULL) json_out = g_cashin->cash_in_enable();
	}
	else
	{
		/*root["result_code"] = -7;
		root["result_msg"] = "纸币接收器初始化失败";
		json_out = write.write(root);*/
		root.put("result_code", -7);
		root.put("result_msg", "纸币接收器初始化失败");
		json_out = root.toString();
	}

	auto result = v8::String::NewFromUtf8(isolate, json_out.c_str(), v8::NewStringType::kNormal, (int)json_out.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
	//LOGINFO"Cashinenable result:%s", json_out.c_str());
}
static void Cashincheck(const FunctionCallbackInfo<Value>& args)
{
	//LOGINFO"Cashincheck =>");
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	//Json::Value root;
	//Json::FastWriter write;	
	Json root;
	std::string json_out = "";
	
	map<string, boost::shared_ptr<ThreadData>>::iterator it = g_map_DevThreadData.find("cashin");
	if(it != g_map_DevThreadData.end())
	{
		boost::shared_ptr<ThreadData> td = it->second;
		if(g_cashin != NULL) json_out = g_cashin->cash_in_check();
	}
	else
	{
		/*root["result_code"] = -7;
		root["result_msg"] = "纸币接收器初始化失败";
		json_out = write.write(root);*/
		root.put("result_code", -7);
		root.put("result_msg", "纸币接收器初始化失败");
		json_out = root.toString();
	}

	auto result = v8::String::NewFromUtf8(isolate, json_out.c_str(), v8::NewStringType::kNormal, (int)json_out.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
	//LOGINFO"Cashincheck result:%s", json_out.c_str());
}
static void Cashindisable(const FunctionCallbackInfo<Value>& args) 
{
	//LOGINFO"Cashindisable =>");
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	//Json::Value root;
	//Json::FastWriter write;	
	Json root;
	std::string json_out = "";
	
	map<string, boost::shared_ptr<ThreadData>>::iterator it = g_map_DevThreadData.find("cashin");
	if(it != g_map_DevThreadData.end())
	{
		boost::shared_ptr<ThreadData> td = it->second;
		if(g_cashin != NULL) json_out = g_cashin->cash_in_disable();
	}
	else
	{
		/*root["result_code"] = -7;
		root["result_msg"] = "纸币接收器初始化失败";
		json_out = write.write(root);*/
		root.put("result_code", -7);
		root.put("result_msg", "纸币接收器初始化失败");
		json_out = root.toString();
	}

	auto result = v8::String::NewFromUtf8(isolate, json_out.c_str(), v8::NewStringType::kNormal, (int)json_out.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
	//LOGINFO"Cashindisable result:%s", json_out.c_str());
}
static void Cashinreset(const FunctionCallbackInfo<Value>& args) 
{
	//LOGINFO"Cashinreset =>");
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	//Json::Value root;
	//Json::FastWriter write;	
	Json root;
	std::string json_out = "";
	
	map<string, boost::shared_ptr<ThreadData>>::iterator it = g_map_DevThreadData.find("cashin");
	if(it != g_map_DevThreadData.end())
	{
		boost::shared_ptr<ThreadData> td = it->second;
		if(g_cashin != NULL) json_out = g_cashin->cash_in_reset();
	}
	else
	{
		/*root["result_code"] = -7;
		root["result_msg"] = "纸币接收器初始化失败";
		json_out = write.write(root);*/
		root.put("result_code", -7);
		root.put("result_msg", "纸币接收器初始化失败");
		json_out = root.toString();
	}

	auto result = v8::String::NewFromUtf8(isolate, json_out.c_str(), v8::NewStringType::kNormal, (int)json_out.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
	//LOGINFO"Cashinreset result:%s", json_out.c_str());
}
static void Cashinsetnotesaccept(const FunctionCallbackInfo<Value>& args) 
{
	//LOGINFO"Cashinsetnotesaccept =>");
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	if (args.Length() < 3) {
		isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "参数数量错误", NewStringType::kNormal).ToLocalChecked()));
		return;
	}

	int cash_type = args[0].As<Number>()->Value();
	int accept = args[1].As<Number>()->Value(); 
	int finish = args[2].As<Number>()->Value(); 

	//Json::Value root;
	//Json::FastWriter write;	
	Json root;
	std::string json_out = "";
	
	map<string, boost::shared_ptr<ThreadData>>::iterator it = g_map_DevThreadData.find("cashin");
	if(it != g_map_DevThreadData.end())
	{
		boost::shared_ptr<ThreadData> td = it->second;
		if(g_cashin != NULL) json_out = g_cashin->cash_in_set_notes_accept(cash_type, accept, finish);
	}
	else
	{
		/*root["result_code"] = -7;
		root["result_msg"] = "纸币接收器初始化失败";
		json_out = write.write(root);*/
		root.put("result_code", -7);
		root.put("result_msg", "纸币接收器初始化失败");
		json_out = root.toString();
	}

	auto result = v8::String::NewFromUtf8(isolate, json_out.c_str(), v8::NewStringType::kNormal, (int)json_out.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
	//LOGINFO"Cashinsetnotesaccept result:%s", json_out.c_str());
}
static void CashingetDatasetVersion(const FunctionCallbackInfo<Value>& args) 
{
	//LOGINFO"CashingetDatasetVersion =>");
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	//Json::Value root;
	//Json::FastWriter write;	
	Json root;
	std::string json_out = "";
	
	map<string, boost::shared_ptr<ThreadData>>::iterator it = g_map_DevThreadData.find("cashin");
	if(it != g_map_DevThreadData.end())
	{
		boost::shared_ptr<ThreadData> td = it->second;
		if(g_cashin != NULL) json_out = g_cashin->cash_in_get_Dataset_Version();
	}
	else
	{
		/*root["result_code"] = -7;
		root["result_msg"] = "纸币接收器初始化失败";
		json_out = write.write(root);*/
		root.put("result_code", -7);
		root.put("result_msg", "纸币接收器初始化失败");
		json_out = root.toString();
	}

	auto result = v8::String::NewFromUtf8(isolate, json_out.c_str(), v8::NewStringType::kNormal, (int)json_out.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
	//LOGINFO"CashingetDatasetVersion result:%s", json_out.c_str());
}
static void CashingetFirmwareVersion(const FunctionCallbackInfo<Value>& args) 
{
	//LOGINFO"CashingetFirmwareVersion =>");
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	//Json::Value root;
	//Json::FastWriter write;
	Json root;
	std::string json_out = "";
	
	map<string, boost::shared_ptr<ThreadData>>::iterator it = g_map_DevThreadData.find("cashin");
	if(it != g_map_DevThreadData.end())
	{
		boost::shared_ptr<ThreadData> td = it->second;
		if(g_cashin != NULL) json_out = g_cashin->cash_in_get_Firmware_Version();
	}
	else
	{
		//root["result_code"] = -7;
		//root["result_msg"] = "纸币接收器初始化失败";
		//json_out = write.write(root);
		root.put("result_code", -7);
		root.put("result_msg", "纸币接收器初始化失败");
		json_out = root.toString();
	}

	auto result = v8::String::NewFromUtf8(isolate, json_out.c_str(), v8::NewStringType::kNormal, (int)json_out.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
	//LOGINFO"CashingetFirmwareVersion result:%s", json_out.c_str());
}
static void CashinupgradeFirmwareProgram(const FunctionCallbackInfo<Value>& args) 
{
	//LOGINFO"CashinupgradeFirmwareProgram =>");
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

	//Json::Value root;
	//Json::FastWriter write;	
	Json root;
	std::string json_out = "";
	
	map<string, boost::shared_ptr<ThreadData>>::iterator it = g_map_DevThreadData.find("cashin");
	if(it != g_map_DevThreadData.end())
	{
		boost::shared_ptr<ThreadData> td = it->second;
		if(g_cashin != NULL) json_out = g_cashin->cash_in_upgrade_Firmware_Program(ss1.str().c_str(), ss2.str().c_str());
	}
	else
	{
		/*root["result_code"] = -7;
		root["result_msg"] = "纸币接收器初始化失败";
		json_out = write.write(root);*/
		root.put("result_code", -7);
		root.put("result_msg", "纸币接收器初始化失败");
		json_out = root.toString();
	}

	auto result = v8::String::NewFromUtf8(isolate, json_out.c_str(), v8::NewStringType::kNormal, (int)json_out.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
	//LOGINFO"CashinupgradeFirmwareProgram result:%s", json_out.c_str());
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
	auto result = v8::String::NewFromUtf8(isolate, info.c_str(), v8::NewStringType::kNormal, (int)info.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}*/
/*static void Unionsettle(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	string info = "";
	//info = hd_union_settle();
	auto result = v8::String::NewFromUtf8(isolate, info.c_str(), v8::NewStringType::kNormal, (int)info.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}*/
/*static void Unionsignin(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	string info = "";
	//info = hd_union_signin();
	auto result = v8::String::NewFromUtf8(isolate, info.c_str(), v8::NewStringType::kNormal, (int)info.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}*/
/*static void Unioncancel(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	string info = "";
	//info = hd_union_cancel();
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
	auto result = v8::String::NewFromUtf8(isolate, info.c_str(), v8::NewStringType::kNormal, (int)info.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}*/
static void ICcarddisable(const FunctionCallbackInfo<Value>& args) 
{
	//LOGINFO"ICcarddisable =>");
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

	//Json::Value root;
	//Json::FastWriter write;	
	Json root;
	std::string json_out = "";
	
	map<string, boost::shared_ptr<ThreadData>>::iterator it = g_map_DevThreadData.find("icCard");
	if(it != g_map_DevThreadData.end())
	{
		boost::shared_ptr<ThreadData> td = it->second;
		td->param->enable = false;
		if(td->param->subDev == 1){
			IRPD_Disable(0); 
		}
		else if(td->param->subDev == 0){
			IRFM_Disable(0); 
		}
		td->param->blocknum = blockNum;
		/*root["result_code"] = 0;
		root["result_msg"] = "success";
		root["ICcard_error_code"] = 0;
		root["ICcard_error_msg"] = "RPD IC读卡器禁能成功！";*/
		root.put("result_code", 0);
		root.put("result_msg", "success");
		root.put("ICcard_error_code", 0);
		root.put("ICcard_error_msg", "RPD IC读卡器禁能成功！");
	}
	else
	{
		/*root["result_code"] = -39;
		root["result_msg"] = "RPD IC读卡器初始化失败";
		root["ICcard_error_code"] = -2;
		root["ICcard_error_msg"] = "RPD IC读卡器通讯失败";	*/	
		root.put("result_code", -39);
		root.put("result_msg", "RPD IC读卡器初始化失败");
		root.put("ICcard_error_code", -2);
		root.put("ICcard_error_msg", "RPD IC读卡器通讯失败");
	}

	//json_out = write.write(root);
	json_out = root.toString();

	auto result = v8::String::NewFromUtf8(isolate, json_out.c_str(), v8::NewStringType::kNormal, (int)json_out.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
	//LOGINFO"ICcarddisable result:%s", json_out.c_str());
}
static void ICcardenable(const FunctionCallbackInfo<Value>& args) 
{
	//LOGINFO"ICcardenable =>");
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

	//Json::Value root;
	//Json::FastWriter write;	
	Json root;
	std::string json_out = "";
	
	map<string, boost::shared_ptr<ThreadData>>::iterator it = g_map_DevThreadData.find("icCard");
	if(it != g_map_DevThreadData.end())
	{
		boost::shared_ptr<ThreadData> td = it->second;
		td->param->enable = true;
		if(td->param->subDev == 1){
			IRPD_Enable(0); 
		}
		else if(td->param->subDev == 0){
			IRFM_Enable(0); 
		}
		td->param->blocknum = blockNum;
		/*root["result_code"] = 0;
		root["result_msg"] = "success";
		root["ICcard_error_code"] = 0;
		root["ICcard_error_msg"] = "RPD IC读卡器使能成功！";*/
		root.put("result_code", 0);
		root.put("result_msg", "success");
		root.put("ICcard_error_code", 0);
		root.put("ICcard_error_msg", "RPD IC读卡器使能成功！");
	}
	else
	{
		/*root["result_code"] = -39;
		root["result_msg"] = "RPD IC读卡器初始化失败";
		root["ICcard_error_code"] = -2;
		root["ICcard_error_msg"] = "RPD IC读卡器通讯失败";	*/	
		root.put("result_code", -39);
		root.put("result_msg", "RPD IC读卡器初始化失败");
		root.put("ICcard_error_code", -2);
		root.put("ICcard_error_msg", "RPD IC读卡器通讯失败");
	}

	//json_out = write.write(root);
	json_out = root.toString();

	auto result = v8::String::NewFromUtf8(isolate, json_out.c_str(), v8::NewStringType::kNormal, (int)json_out.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
	//LOGINFO"ICcardenable result:%s", json_out.c_str());
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
	auto result = v8::String::NewFromUtf8(isolate, info.c_str(), v8::NewStringType::kNormal, (int)info.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}*/
/*static void ICcardreadCard(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	string info = "";
	//info = hd_ICcard_readCard();
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
	auto result = v8::String::NewFromUtf8(isolate, info.c_str(), v8::NewStringType::kNormal, (int)info.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}*/

/*static void TestJson(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	string info = "";
	//info = hd_test_json();
	auto result = v8::String::NewFromUtf8(isolate, info.c_str(), v8::NewStringType::kNormal, (int)info.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}*/
/*static void Cashoutcheck(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	string info = "";
	//info = hd_cash_out_check();
	auto result = v8::String::NewFromUtf8(isolate, info.c_str(), v8::NewStringType::kNormal, (int)info.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}*/
/*static void Cashoutreset(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	string info = "";
	//info = hd_cash_out_reset();
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
	auto result = v8::String::NewFromUtf8(isolate, info.c_str(), v8::NewStringType::kNormal, (int)info.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}*/
/*static void Ecardsignin(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	string info = "";	
	//info = hd_ecard_sign_in();
	auto result = v8::String::NewFromUtf8(isolate, info.c_str(), v8::NewStringType::kNormal, (int)info.size()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}*/
/*static void Ecardsettleaccount(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext(); 
	
	string info = "";	
	//info = hd_ecard_settle_account();
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