#include <node.h>
using namespace node;
using namespace v8;

#include "sdk2_1.h"
#include<sstream>
#include <iostream>
using namespace std;

namespace demo {

	using v8::Exception;
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
	using v8::Null;

	void Add(const FunctionCallbackInfo<Value>& args) {
	  Isolate* isolate = args.GetIsolate();

	  if (args.Length() < 2) {
		isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "参数数量错误", NewStringType::kNormal).ToLocalChecked()));
		return;
	  }

	  if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
		isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "参数错误", NewStringType::kNormal).ToLocalChecked()));
		return;
	  }

	  //Local<Number> -> double
	  double value = args[0].As<Number>()->Value() + args[1].As<Number>()->Value(); //Number
	  //double -> Local<Number>
	  Local<Number> num = Number::New(isolate, value);

	  char str[100] = {"hello"};
	  sdk::Test(str); //call dll/so

	  args.GetReturnValue().Set(num);
	}

	void RunCallback(const FunctionCallbackInfo<Value>& args) {
	  Isolate* isolate = args.GetIsolate();
	  Local<Context> context = isolate->GetCurrentContext();  
	  Local<Function> cb = Local<Function>::Cast(args[0]); //Function 坑，离开区域会爆
	  
	  const unsigned argc = 1;
	  //string -> Local<Value>
	  Local<Value> argv[argc] = {
		  String::NewFromUtf8(isolate, "hello world!!!", NewStringType::kNormal).ToLocalChecked() 
	  };
	  cb->Call(context, Null(isolate), argc, argv).ToLocalChecked();
	}
	

	void RunCallback2(const FunctionCallbackInfo<Value>& args) {
	  Isolate* isolate = args.GetIsolate();
	  Local<Context> context = isolate->GetCurrentContext();  
	  Local<Function> cb = Local<Function>::Cast(args[0]); //Function 坑，离开区域会爆
	  
	  //Local<Value> -> string
	  v8::String::Utf8Value buf(isolate, args[1]); //String
	  stringstream ss;
	  ss << *buf;
	  cout << ss.str()<<endl;
			  
	  const unsigned argc = 1;
	  //string -> Local<Value>
	  Local<Value> argv[argc] = {
		  String::NewFromUtf8(isolate, "hello yyb!!!", NewStringType::kNormal).ToLocalChecked() 
	  };
	  cb->Call(context, Null(isolate), argc, argv).ToLocalChecked();
	  
	  char str[100] = {"hello"};
	  //sdk::Test2(str, &cb); //call dll/so

	}

	void Init(Local<Object> exports, Local<Object> module) {
	  //NODE_SET_METHOD(module, "exports", RunCallback);
	  
	  NODE_SET_METHOD(exports, "add", Add);
	  NODE_SET_METHOD(exports, "runCallback", RunCallback);
	  NODE_SET_METHOD(exports, "runCallback2", RunCallback2);
	}

	NODE_MODULE(NODE_GYP_MODULE_NAME, Init)

}