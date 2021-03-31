{
  "targets": [ 
    {
      "target_name": "sdkaddon",
      "sources": [
        "sdkaddon.cc",
	"tools.cpp",
	"cashin_nv9.cpp",
	"cash_in_v77e.cpp"
      ],
      "include_dirs": [
        	"<!@(node -p \"require('node-addon-api').include\")", 
			'/home/yeyongbo/nodejs/golang/myaddon/sdk2_1/sdk2_1',
			'/home/yeyongbo/nodejs/golang/generationsEpsonwithoutThread',
			'/home/yeyongbo/nodejs/golang/ictcashinwithoutThread/libICTcashin',		
            '/home/yeyongbo/nodejs/golang/libICTcoinin1withoutThread/libICTcoinin1',
			'/home/yeyongbo/nodejs/golang/libNV9withoutThread/libNV9',	
			'/home/yeyongbo/nodejs/golang/libRPDCardV1withoutThread/libRPDCardV1',	
			'/usr/include/jsoncpp',			
			'/usr/local/include'
      ],
	  "library_dirs": [
		'/home/yeyongbo/libs',
		'/usr/lib/x86_64-linux-gnu',
		'/usr/local/lib'
	  ],
      'libraries': [
		'-lsdk2_1 -lgenerationsEpson -lICTcashin -lICTcoinin1 -lNV9 -lRPDCardV1 -ljsoncpp -lboost_log_setup -lboost_locale -lboost_log -lboost_thread -lboost_system -lboost_filesystem -lboost_regex -lboost_chrono -lboost_date_time -lboost_program_options'
	  ],
      "dependencies": [
        "<!(node -p \"require('node-addon-api').gyp\")"
      ],
	"copies": [
			{
				"destination": "build/Release",
				"files": [ 		"/home/yeyongbo/libs/libsdk2_1.so",
							"/home/yeyongbo/libs/libgenerationsEpson.so",
							"/home/yeyongbo/libs/libICTcashin.so",
							"/home/yeyongbo/libs/libICTcoinin1.so",
							"/home/yeyongbo/libs/libNV9.so",
							"/home/yeyongbo/libs/libRPDCardV1.so",
							"/usr/lib/x86_64-linux-gnu/libjsoncpp.so",
							"/usr/local/lib/libboost_log.so",
							"/usr/local/lib/libboost_log_setup.so",
							"/usr/local/lib/libboost_thread.so",
							"/usr/local/lib/libboost_thread.so.1.69.0",
							"/usr/local/lib/libboost_system.so",
							"/usr/local/lib/libboost_filesystem.so",
							"/usr/local/lib/libboost_regex.so",
							"/usr/local/lib/libboost_chrono.so",
							"/usr/local/lib/libboost_date_time.so",
							"/usr/local/lib/libboost_locale.so",
							"/usr/local/lib/libboost_program_options.so"
					 ]
			}
	  ],
      "cflags!": ["-fno-exceptions"],
      "cflags_cc!": ["-fno-exceptions"],
      "defines": ["NAPI_CPP_EXCEPTIONS"],
      "xcode_settings": {
        "GCC_ENABLE_CPP_EXCEPTIONS": "YES"
      }
    }
  ]
}
