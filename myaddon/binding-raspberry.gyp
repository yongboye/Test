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
			'/home/pi/nodejs/golang/myaddon/sdk2_1/sdk2_1',	
			'/home/pi/nodejs/golang/jsoncpp-src-0.5.0/include',	
			'/home/pi/nodejs/golang/libRPDCardV1withoutThread/libRPDCardV1',
			'/home/pi/nodejs/golang/libRFMCardV3withoutThread/libRFMCardV3',
			'/home/pi/nodejs/golang/generationsEpsonwithoutThread',
			'/home/pi/nodejs/golang/libNV9withoutThread/libNV9',
			'/home/pi/nodejs/golang/ictcashinwithoutThread/libICTcashin'
      ],
	  "library_dirs": [
			'/usr/lib'
	  ],
      'libraries': [
			'-lsdk2_1 -lJson -lRPDCardV1 -lRFMCardV3 -lgenerationsEpson -lNV9 -lICTcashin -ldl'
	  ],
      "dependencies": [
        "<!(node -p \"require('node-addon-api').gyp\")"
      ],
	"copies": [
			{
				"destination": "build/Release",
				"files": [
					"/home/pi/libs/libsdk2_1.so",
					"/home/pi/libs/libJson.so",
					"/home/pi/libs/libRPDCardV1.so",
					"/home/pi/libs/libRFMCardV3.so",
					"/home/pi/libs/libgenerationsEpson.so",
					"/home/pi/libs/libNV9.so",
					"/home/pi/libs/libICTcashin.so"
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
