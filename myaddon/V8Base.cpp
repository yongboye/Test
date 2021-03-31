#include "V8Base.h"
#include <fstream>
#include <boost/filesystem.hpp>
#include<sstream>
#include <string.h>


static void Load(const v8::FunctionCallbackInfo<v8::Value>& args);

//Args* args = (Args*) data;
//Isolate* isolate = args->isolate;
//"info" - logInfo
/*static void logInfo(const v8::FunctionCallbackInfo<v8::Value>& args)
{	
	void *data = v8::External::Cast( *args.Data() )->Value(); //V8Base*
	if (data == 0) {
		return;
	}

	v8::Isolate* isolate = args.GetIsolate(); //v8虚拟机实例
	v8::HandleScope handleScope(isolate); //内存管理

	std::stringstream ss;
	for (int i = 0; i < args.Length(); i++) {
		v8::String::Utf8Value buf(isolate, args[i]);
		ss << *buf << " ";
	}

	//string lotCode = ((V8Base*)(data))->getLotCode();
	//LogInfo("javascript:" + lotCode , ss.str());
}*/

//"error" - logError
/*static void logError(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	void *data = v8::External::Cast(*args.Data())->Value(); //V8Base*
	if (data == 0) {
		return;
	}

	v8::Isolate* isolate = args.GetIsolate();
	v8::HandleScope handleScope(isolate);

	std::stringstream ss;
	for (int i = 0; i < args.Length(); i++) {
		v8::String::Utf8Value buf(isolate, args[i]);
		ss << *buf << " ";
	}

	//string lotCode = ((V8Base*)(data))->getLotCode();
	//LogError("javascript:" + lotCode, ss.str());
}*/



/*static v8::Local<v8::Context> createContext(v8::Isolate* isolate, map<string, V8Function> *globalFuncs, map<string, map<string, V8Function> > *classFuncs, void *data=0)
{	//通过FunctionTemplate绑定函数Load（带参data）、然后将FunctionTemplate注册到ObjectTemplate，最后返回上下文
	v8::Handle<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);
	
	//默认支持 load 函数 
	global->Set(isolate, "load", v8::FunctionTemplate::New(isolate, Load, v8::External::New(isolate, data)));	
	
	//设置全局函数接口
	if (globalFuncs) {
		for (auto &item : *globalFuncs) {
			global->Set(isolate, item.first.c_str(), v8::FunctionTemplate::New(isolate, item.second.f, v8::External::New(isolate, item.second.data)));
		}
	}	
	//设置对象函数接口 ==
	if (classFuncs) {
		for (auto &item : *classFuncs) {
			v8::Local<v8::ObjectTemplate> tpl = v8::ObjectTemplate::New(isolate);
			for (auto &subItem : item.second) {
				tpl->Set(isolate, subItem.first.c_str(), v8::FunctionTemplate::New(isolate, subItem.second.f, v8::External::New(isolate, subItem.second.data)));
			}
			
			global->Set(isolate, item.first.c_str(), tpl);
		}
	}	
	//设置公共日志接口
	if (data) {
		v8::Local<v8::ObjectTemplate> tpl = v8::ObjectTemplate::New(isolate);
		tpl->Set(isolate, "info", v8::FunctionTemplate::New(isolate, logInfo, v8::External::New(isolate, data)));
		tpl->Set(isolate, "error", v8::FunctionTemplate::New(isolate, logError, v8::External::New(isolate, data)));
		
		global->Set(isolate, "syslog", tpl);
	}
	
	//默认支持 exp 对象
	global->Set(isolate, "exp", v8::ObjectTemplate::New(isolate));
	
	return v8::Context::New(isolate, NULL, global);
}*/

// Extracts a C string from a V8 Utf8Value.
static const char* ToCString(const v8::String::Utf8Value& value) {
	return *value ? *value : "<string conversion failed>";
}

static void ReportException(v8::Isolate* isolate, v8::TryCatch* try_catch) {
	v8::HandleScope handle_scope(isolate);
	
	v8::String::Utf8Value exception(isolate, try_catch->Exception());
	const char* exception_string = ToCString(exception);
	
	v8::Local<v8::Message> message = try_catch->Message();
	if (message.IsEmpty()) {
		// V8 didn't provide any extra information about this error; just
		// print the exception.
		//LogError("javascript", exception_string);
	} else {
		// Print (filename):(line number): (exception message).
		v8::String::Utf8Value filename(isolate, message->GetScriptOrigin().ResourceName());
		v8::Local<v8::Context> context(isolate->GetCurrentContext());
		const char* filename_string = ToCString(filename); //*
		int linenum = message->GetLineNumber(context).FromJust();
		//LogError("javascript", filename_string << ":" << linenum << ": " << exception_string);
		
		// Print line of source code.
		v8::String::Utf8Value sourceline(isolate, message->GetSourceLine(context).ToLocalChecked());
		const char* sourceline_string = ToCString(sourceline);
		//LogError("javascript", sourceline_string);

		v8::Local<v8::Value> stack_trace_string;
		if (try_catch->StackTrace(context).ToLocal(&stack_trace_string) &&
			stack_trace_string->IsString() && v8::Local<v8::String>::Cast(stack_trace_string)->Length() > 0) 
		{
			v8::String::Utf8Value stack_trace(isolate, stack_trace_string);
			const char* stack_trace_string = ToCString(stack_trace);
			//LogError("javascript", stack_trace_string);
		}
	}
}

// Reads a file into a v8 string.
v8::MaybeLocal<v8::String> ReadFile(v8::Isolate* isolate, const char* name) {
	FILE* file = fopen(name, "rb");
	if (file == NULL) return v8::MaybeLocal<v8::String>();

	fseek(file, 0, SEEK_END);
	size_t size = ftell(file);
	rewind(file);

	char* chars = new char[size + 1];
	chars[size] = '\0';
	for (size_t i = 0; i < size;) {
		i += fread(&chars[i], 1, size - i, file);
		if (ferror(file)) {
			fclose(file);
			return v8::MaybeLocal<v8::String>();
		}
	}
	fclose(file);
	v8::MaybeLocal<v8::String> result = v8::String::NewFromUtf8(
		isolate, chars, v8::NewStringType::kNormal, static_cast<int>(size));
	delete[] chars;
	return result;
}

//执行js脚本
static v8::Local<v8::Value> ExecuteString(
											v8::Isolate* isolate, 
											v8::Local<v8::String> source, 
											v8::Local<v8::Value> name, 
											bool report_exceptions ) 
{
	v8::EscapableHandleScope handleScope(isolate);
	v8::TryCatch try_catch(isolate);
	
	v8::Local<v8::Context> context(isolate->GetCurrentContext());
	v8::ScriptOrigin origin(name);
	v8::Local<v8::Script> script;
	if (!v8::Script::Compile(context, source, &origin).ToLocal(&script)) {
		// Print errors that happened during compilation.
		if (report_exceptions)
			ReportException(isolate, &try_catch);
		return v8::Undefined(isolate);
	} else {
		v8::Local<v8::Value> result;
		if (!script->Run(context).ToLocal(&result)) {
			if (try_catch.HasCaught()) {
				v8::Local<v8::Value> exception = try_catch.Exception();
				v8::String::Utf8Value exception_str(isolate, exception);
				//LogError(0, "V8 execute error:" << *exception_str);
				//return v8::Undefined(isolate);
			}
			// Print errors that happened during execution.
			if (report_exceptions)
				ReportException(isolate, &try_catch);
			return v8::Undefined(isolate);
		} else {
			if (try_catch.HasCaught()) {
				v8::Local<v8::Value> exception = try_catch.Exception();
				v8::String::Utf8Value exception_str(isolate, exception);
				//LogError(0, "V8 execute error:" << *exception_str);
				return v8::Undefined(isolate);
			}
			if (!result->IsUndefined()) {
				return handleScope.Escape(result);
			}
		}
	}
	
	return v8::Undefined(isolate);
}




//加载其他模块js,load('other.js')
//"load" - Load
static void Load(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	v8::HandleScope handleScope(isolate);
	
	if (args.Length() != 1) {
		return;
	}
	v8::String::Utf8Value file(isolate, args[0]);
	string fileName = *file;
	V8Base *v8Base = (V8Base*)v8::External::Cast(*args.Data())->Value();
	if (v8Base != 0 && strlen(*file)>0 && (*file)[0] != '/') {
		fileName = v8Base->getPath() + "/" + (*file);
	}

	v8::Local<v8::String> source;
	if (!ReadFile(isolate, fileName.c_str()).ToLocal(&source)) {
		//LogError("javascript", "Error loading file:" << fileName);
		return;
	}
	v8::Local<v8::String> name = v8::String::NewFromUtf8(isolate, fileName.c_str(), v8::NewStringType::kNormal, (int)fileName.size()).ToLocalChecked();
	auto result = ExecuteString(isolate, source, name, true); //v8::Local<v8::Value>
	
	args.GetReturnValue().Set(result);
}
















//////////////////////////////////////////////////////////////////////////////////////////////////
V8Base::V8Base(v8::Isolate* isolate, Local<Context> context2, map<string, V8Function> *globalFuncs, map<string, map<string, V8Function> > *classFuncs) :
	//isolateWraper(v8::Isolate::New(v8Params.create_params))
	isolateWraper(isolate)
	, isolate_scope(isolateWraper.isolate), handle_scope(isolateWraper.isolate), context(Context::New(isolateWraper.isolate)) 
	//, context_scope(context)
{	
	
	if (globalFuncs) {
		this->globalFuncs.reset(new map<string, V8Function>(*globalFuncs));
	}
	if (classFuncs) {
		this->classFuncs.reset(new map<string, map<string, V8Function> >(*classFuncs));
	}
}

V8Base::~V8Base()
{
	
}

int V8Base::load(const string &jsFile)
{
	std::ifstream f(jsFile.c_str(), std::ios::in);
	if (f.is_open() == false) {
		//LogError(lotCode, "V8Base::load file error:" << jsFile);
		return -1;
	}
	std::stringstream ss;
	ss << f.rdbuf();
	f.close();

	boost::filesystem::path pf(jsFile);
	path = pf.parent_path().string();

	v8::HandleScope handleScope(isolateWraper.isolate);
	v8::Local<v8::String> source = v8::String::NewFromUtf8(isolateWraper.isolate, ss.str().c_str(), v8::NewStringType::kNormal).ToLocalChecked();
	v8::Local<v8::String> name = v8::String::NewFromUtf8(isolateWraper.isolate, jsFile.c_str(), v8::NewStringType::kNormal, (int)jsFile.size()).ToLocalChecked();
	ExecuteString(isolateWraper.isolate, source, name, true);
	
	mainJsPath = jsFile;
	return 0;
}

v8::Local<v8::Value> V8Base::execute(const string &script)
{
	v8::EscapableHandleScope handleScope(isolateWraper.isolate);
	
	v8::Local<v8::String> source 	= v8::String::NewFromUtf8(isolateWraper.isolate, script.c_str(), v8::NewStringType::kNormal).ToLocalChecked();
	v8::Local<v8::String> name 		= v8::String::NewFromUtf8(isolateWraper.isolate, mainJsPath.c_str(), v8::NewStringType::kNormal).ToLocalChecked();
	auto result = ExecuteString(isolateWraper.isolate, source, name, true);
	
	return handleScope.Escape(result);
}