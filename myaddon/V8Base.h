#pragma once
#include <v8.h>
using namespace v8;

#include <string>

#include <map>
#include <memory>
using std::map;
using std::string;

struct V8Function {
	v8::FunctionCallback f;
	void *data;
};

class V8Base
{
struct V8Params {
	V8Params() {
		create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
	}
	~V8Params() {
		delete create_params.array_buffer_allocator;
	}
	
	v8::Isolate::CreateParams create_params; //public构造时赋值
};

class IsolateWraper {
public:
	IsolateWraper(){}
	IsolateWraper(v8::Isolate* isolate) : isolate(isolate) {
	}
	~IsolateWraper() {
		isolate->Dispose();
	}
	
	v8::Isolate* isolate; //public构造时赋值
};

public:
	V8Base(v8::Isolate* isolate, Local<Context> context2, map<string, V8Function> *globalFuncs, map<string, map<string, V8Function> > *classFuncs);
	~V8Base();


	int load(const string &jsFile);
	v8::Local<v8::Value> execute(const string &script);
	
	
	v8::Isolate *getIsolate() {
		return isolateWraper.isolate;
	}
	map<string, V8Function> *getGlobalFuncs() {
		return globalFuncs.get();
	}
	map<string, map<string, V8Function> >*getClassFuncs() {
		return classFuncs.get();
	}
	const string &getPath() {
		return path;
	}

private:
	string path; //路径
	string mainJsPath; //路径文件

	std::unique_ptr<map<string, V8Function> > globalFuncs;
	std::unique_ptr <map<string, map<string, V8Function> > > classFuncs;

	V8Params v8Params;
	IsolateWraper isolateWraper;
	v8::Isolate::Scope isolate_scope;
	v8::HandleScope handle_scope;
	v8::Local<v8::Context> context;
	//v8::Context::Scope context_scope;
};

