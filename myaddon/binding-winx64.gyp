{
  "targets": [ 
    {
      "target_name": "sdkaddon",
      "sources": [
	    "Json.cpp",
        "sdkaddon.cc",
		"tools.cpp",
		"ticketsensor_generation.cpp",
		"cashin_nv9.cpp",
		"cash_in_v77e.cpp"
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")", 
		'D:/golang/myaddon/sdk2_1/sdk2_1',
		'D:/golang/logger',
		'D:/golang/jsoncpp-src-0.5.0/include',
		'D:/golang/libTicketSensor/libticketsensor/libticketsensor',
		'D:/golang/libRPDCardV1withoutThread/libRPDCardV1',
		'D:/golang/libRFMCardV3withoutThread/libRFMCardV3',
		'D:/golang/generationsEpsonwithoutThread',
		'D:/golang/libNV9withoutThread/libNV9',
		'D:/golang/ictcashinwithoutThread/libICTcashin',
		'E:/192.168.6.44/boost_1_68_0'
      ],
	  "library_dirs": [
		'D:/golang/myaddon/sdk2_1/x64/Release',
		'D:/golang/logger/x64/Release',
		'D:/golang/jsoncpp-src-0.5.0/makefiles/vs71/x64/Release',
		'D:/golang/libTicketSensor/libticketsensor/x64/Release',
		'D:/golang/libRPDCardV1withoutThread/x64/Release',
		'D:/golang/libRFMCardV3withoutThread/x64/Release',
		'D:/golang/generationsEpsonwithoutThread/x64/Release',
		'D:/golang/libNV9withoutThread/x64/Release',
		'D:/golang/ictcashinwithoutThread/libICTcashin/x64/Release',
		'E:/192.168.6.44/boost_1_68_0/build/vc140/lib'
	  ],
      'libraries': [ 
		'D:/golang/myaddon/sdk2_1/x64/Release/sdk2_1.lib',
		'D:/golang/logger/x64/Release/logger.lib',
		'D:/golang/jsoncpp-src-0.5.0/makefiles/vs71/x64/Release/json_vc71_libmt.lib',
		'D:/golang/libTicketSensor/libticketsensor/x64/Release/libticketsensor.lib',
		'D:/golang/libRPDCardV1withoutThread/x64/Release/libRPDCardV1.lib',
		'D:/golang/libRFMCardV3withoutThread/x64/Release/libRFMCardV3.lib',
		'D:/golang/generationsEpsonwithoutThread/x64/Release/generationsEpson.lib',
		'D:/golang/libNV9withoutThread/x64/Release/libNV9.lib',
		'D:/golang/ictcashinwithoutThread/libICTcashin/x64/Release/libICTcashin.lib',
		# 'E:/192.168.6.44/boost_1_68_0/build/vc140/lib/libboost_thread-vc140-mt-s-x64-1_68.lib'
	  ],
	  "copies": [
			{
				"destination": "build/Release",
				"files": [ 	"D:/golang/myaddon/sdk2_1/x64/Release/sdk2_1.dll",
							"D:/golang/libTicketSensor/libticketsensor/x64/Release/libticketsensor.dll",
							"D:/golang/libRPDCardV1withoutThread/x64/Release/libRPDCardV1.dll",
							"D:/golang/libRFMCardV3withoutThread/x64/Release/libRFMCardV3.dll",
							"D:/golang/generationsEpsonwithoutThread/x64/Release/generationsEpson.dll",
							"D:/golang/libNV9withoutThread/x64/Release/libNV9.dll",
							"D:/golang/ictcashinwithoutThread/libICTcashin/x64/Release/libICTcashin.dll"
						 ]
			}
	  ],
      "dependencies": [
        "<!(node -p \"require('node-addon-api').gyp\")"
      ],
      "cflags!": ["-fno-exceptions"],
      "cflags_cc!": ["-fno-exceptions"],
      "defines": ["NAPI_CPP_EXCEPTIONS"],
      "xcode_settings": {
        "GCC_ENABLE_CPP_EXCEPTIONS": "YES"
      },
	  'configurations': {                
                'Release': {
                    'msvs_settings': {
                        'VCCLCompilerTool': {
                            'AdditionalOptions': ['/EHsc'],
							'RuntimeTypeInfo': 'true'
                        }
                    }
                }
            }
    }
  ]
}
