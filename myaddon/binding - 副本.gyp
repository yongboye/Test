{
  "targets": [ 
    {
      "target_name": "sdkaddon",
      "sources": [
			"sdkaddon.cc",
			"tools.cpp",
			"ticketsensor_generation.cpp",
			"cashin_nv9.cpp",
			"cash_in_v77e.cpp"			
      ],
      "include_dirs": [
        	"<!@(node -p \"require('node-addon-api').include\")", 
			'/home/pi/nodejs/golang/myaddon/sdk2_1/sdk2_1',
			'/home/pi/nodejs/golang/glog/glog',				
			'/home/pi/nodejs/golang/jsoncpp-src-0.5.0/include',
			'/home/pi/nodejs/golang/libTicketSensor/libticketsensor/libticketsensor',	
			'/home/pi/nodejs/golang/libRPDCardV1withoutThread/libRPDCardV1',
			'/home/pi/nodejs/golang/libRFMCardV3withoutThread/libRFMCardV3',
			'/home/pi/nodejs/golang/generationsEpsonwithoutThread',
			'/home/pi/nodejs/golang/libNV9withoutThread/libNV9',
			'/home/pi/nodejs/golang/ictcashinwithoutThread/libICTcashin'
      ],
	  "library_dirs": [
			'/home/pi/libs'
	  ],
      'libraries': [
			'-lsdk2_1 -lmygLog -lJson -lTicketSensor -lRPDCardV1 -lRFMCardV3 -lgenerationsEpson -lNV9 -lICTcashin -ldl'
	  ],
      "dependencies": [
        "<!(node -p \"require('node-addon-api').gyp\")"
      ],
	"copies": [
			{
				"destination": "build/Release",
				"files": [
					"/home/pi/libs/libsdk2_1.so",	
					"/home/pi/libs/libglog.so.0",
					"/home/pi/libs/libglog.so.0.0.0",
					"/home/pi/libs/libmygLog.so",				
					"/home/pi/libs/libJson.so",
					"/home/pi/libs/libTicketSensor.so",
					"/home/pi/libs/libRPDCardV1.so",
					"/home/pi/libs/libRFMCardV3.so",
					"/home/pi/libs/libgenerationsEpson.so",
					"/home/pi/libs/libNV9.so",
					"/home/pi/libs/libICTcashin.so",
					"/home/pi/libs/libICTcoinin1.so",
					"/home/pi/libs/libboost_chrono.so.1.67.0",
					"/home/pi/libs/libboost_date_time.so.1.67.0",
					"/home/pi/libs/libboost_filesystem.so.1.67.0",
					"/home/pi/libs/libboost_locale.so.1.67.0",
					"/home/pi/libs/libboost_log_setup.so.1.67.0",
					"/home/pi/libs/libboost_log.so.1.67.0",
					"/home/pi/libs/libboost_program_options.so.1.67.0",
					"/home/pi/libs/libcares.so.2",
					"/home/pi/libs/libnode.so.64",
					"/home/pi/libs/libuv.so.1"
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
