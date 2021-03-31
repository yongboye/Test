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
		# 'D:/golang/libTicketSensor/libticketsensor/libticketsensor',
		# 'D:/golang/libEleCtlLock',
		# 'D:/golang/libRPDCardV1withoutThread/libRPDCardV1',
		# 'D:/golang/libRFMCardV3withoutThread/libRFMCardV3',
		# 'D:/golang/libSHtransportationcard',
		# 'D:/golang/generationsEpsonwithoutThread',
		# 'D:/golang/libNV9withoutThread/libNV9',
		# 'D:/golang/ictcashinwithoutThread/libICTcashin',
		'E:/192.168.6.44/boost_1_68_0'
      ],
	  "library_dirs": [
		'D:/golang/myaddon/sdk2_1/Release',
		'D:/golang/logger/Release',
		'D:/golang/jsoncpp-src-0.5.0/build/vs71/release/lib_json',
		# 'D:/golang/libTicketSensor/libticketsensor/Release',
		# 'D:/golang/libEleCtlLock/Release',
		# 'D:/golang/libRPDCardV1withoutThread/Release',
		# 'D:/golang/libRFMCardV3withoutThread/Release',
		# 'D:/golang/libSHtransportationcard/Release',
		# 'D:/golang/generationsEpsonwithoutThread/Release',
		# 'D:/golang/libNV9withoutThread/Release',
		# 'D:/golang/ictcashinwithoutThread/libICTcashin/Release',
		'E:/192.168.6.44/boost_1_68_0/build/vc140/lib'
	  ],
      'libraries': [ 
		'D:/golang/myaddon/sdk2_1/Release/sdk2_1.lib',
		'D:/golang/logger/Release/logger.lib',
		'D:/golang/jsoncpp-src-0.5.0/build/vs71/release/lib_json/json_vc71_libmt.lib',
		# 'D:/golang/libTicketSensor/libticketsensor/Release/libticketsensor.lib',
		# 'D:/golang/libEleCtlLock/Release/libEleCtlLock.lib',
		# 'D:/golang/libRPDCardV1withoutThread/Release/libRPDCardV1.lib',
		# 'D:/golang/libRFMCardV3withoutThread/Release/libRFMCardV3.lib',
		# 'D:/golang/libSHtransportationcard/Release/libSHtransportationcard.lib',
		# 'D:/golang/generationsEpsonwithoutThread/Release/generationsEpson.lib',
		# 'D:/golang/libNV9withoutThread/Release/libNV9.lib',
		# 'D:/golang/ictcashinwithoutThread/libICTcashin/Release/libICTcashin.lib',
		# 'E:/192.168.6.44/boost_1_68_0/build/vc140/lib/libboost_thread-vc140-mt-s-x32-1_68.lib'
	  ],
	  "copies": [
			{
				"destination": "build/Release",
				"files": [ 	"D:/golang/myaddon/sdk2_1/Release/sdk2_1.dll",
							"D:/golang/libTicketSensor/libticketsensor/Release/libticketsensor.dll",
							"D:/golang/libEleCtlLock/Release/libEleCtlLock.dll",
							"D:/golang/libRPDCardV1withoutThread/Release/libRPDCardV1.dll",
							"D:/golang/libRFMCardV3withoutThread/Release/libRFMCardV3.dll",
							'D:/golang/libSHtransportationcard/Release/libSHtransportationcard.dll',
							"D:/golang/generationsEpsonwithoutThread/Release/generationsEpson.dll",
							"D:/golang/libNV9withoutThread/Release/libNV9.dll",
							"D:/golang/ictcashinwithoutThread/libICTcashin/Release/libICTcashin.dll",
							"D:/golang/ThirdPartyLibraries/MasterCom.dll",
							"D:/golang/ThirdPartyLibraries/MasterRD.dll"
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
