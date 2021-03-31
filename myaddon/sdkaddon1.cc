#include <napi.h>

#include "sdk2_1.h"
#include "entry.h"
struct ThreadData
{};


#include <sstream>
#include <string>
#include <iostream>
using namespace std;

#include <cstring>
#include <map>
#include <memory>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

using namespace Napi;

Napi::String Method(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
	std::cout<<"info number:" << info.Length()<<endl;
	cout<<info[0]<<":"<<info[0].As<Napi::String>().Utf8Value()<<endl;
	cout<<info[1]<<":"<<info[1].As<Napi::Number>().Int32Value()<<endl;
	cout<<info[2]<<":"<<endl;	

	char str[100] = {"hello"};
	sdk::Test(str); //call dll/so
    //sdk::Test2(str, info[2]); //call dll/so
	string username = info[0].As<String>().Utf8Value(); 
	strcpy(str, username.c_str());
	
    return Napi::String::New(env, str);
}

Napi::String HardWareInit(const Napi::CallbackInfo &info) 
{
  Napi::Env env = info.Env();
  
  std::string str1 = info[2].As<String>().Utf8Value(); 
  std::string str2 = info[3].As<String>().Utf8Value(); 
  std::string str3 = info[4].As<String>().Utf8Value(); 
  
  string res = "";
  res = hd_hardware_init(NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, NULL, str1.c_str(), NULL, str2.c_str(), NULL, str3.c_str());
  return Napi::String::New(env, res);
}
Napi::String ICcardenable(const Napi::CallbackInfo &info) 
{
	Napi::Env env = info.Env();

	int blockNum = info[0].As<Napi::Number>().Int32Value();

	string res = "";
	res = hd_ICcard_enable(blockNum);
	return Napi::String::New(env, res);
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(Napi::String::New(env, "test"), Napi::Function::New(env, Method));
	exports.Set(Napi::String::New(env, "hdhardwareinit"), Napi::Function::New(env, HardWareInit));
	exports.Set(Napi::String::New(env, "hdICcardenable"), Napi::Function::New(env, ICcardenable));
    return exports;
}
NODE_API_MODULE(sdkaddon, Init) 

//napi_property_descriptor desc = { "hello", 0, HelloMethod, 0, 0, 0, napi_default, 0 }; //napi_property_descriptor 为结构体，作用是描述扩展暴露的 属性/方法 的描述
//napi_define_properties(env, exports, 1, &desc);  // 定义暴露的方法
// 注册扩展，扩展名叫做sdkaddon，Init为扩展的初始化方法
//npm install 编译
//node-gyp rebuild
//node js
