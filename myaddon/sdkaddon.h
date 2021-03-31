#pragma once
#include <iostream>
using namespace std;
#include <map>
#include <boost/shared_ptr.hpp>
#include <uv.h>
#include <v8.h>
using namespace v8;

/*using v8::Exception;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::NewStringType;
using v8::Number;
using v8::Object;
using v8::String;
using v8::Value;
using v8::Context;
using v8::Function;
using v8::Null;*/

typedef struct DevParams{
	DevParams(){
		errStr.clear();
		deviceName.clear();
		subDev = -1;
		exit = false;
		initFlag = false;
		enable = false;
		resultjs.clear();
		status = 0;	

		m_serial = 1;
	}
	
	//local common
	Isolate* isolate;	
	//Persistent<Function, CopyablePersistentTraits<Function>> onSucc{};
	//Persistent<Function, CopyablePersistentTraits<Function>> onError{};
	string errStr;
    Persistent<Function, CopyablePersistentTraits<Function>> oncallback{};
	string deviceName;
	int subDev; //m_cash_in_type、m_iCcard_type
	uv_async_t* callbackSendReq; //v8规定子线程里不能直接调用js的回调函数，需要通过libuv接口通知主线程进行回调，子线程（通知）主线程用 DEVPARAMS data=param
	bool exit;
	bool initFlag;
	bool enable;
	string resultjs;
	int status; 
	
	//iccard
	int blocknum;
	//traffic
	int money;
	int payed;
	bool paylog;
	unsigned int m_serial;
}DEVPARAMS;

struct ThreadData
{	
	//global common
	string inijs;
	string logpath;
	string serverdatetime;

	string deviceName;
	DevParams* param; //（通知）事件
	uv_work_t* req; //多线程，子线程函数参数用 DevParams data=param
};

//extern ThreadData* threadData;
extern map<string, boost::shared_ptr<ThreadData>> g_map_DevThreadData;

