#include <v8.h>
#include <libplatform/libplatform.h>
#include <stdio.h>

class V8Init {
public:
	V8Init(const char *program) {
		/*v8::V8::InitializeICUDefaultLocation(program);
		v8::V8::InitializeExternalStartupData(program);
		platform = v8::platform::NewDefaultPlatform(); //
		v8::V8::InitializePlatform(platform.get());
		v8::V8::Initialize();*/
	}
	~V8Init() {
		/*v8::V8::Dispose();
		v8::V8::ShutdownPlatform();*/
	}

private:
	//std::unique_ptr<v8::Platform> platform;
};

static V8Init *init;

void v8init(const char *program)
{
	if (init == 0) {
		init = new V8Init(program);
	}
}