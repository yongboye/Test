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
		'<(PRODUCT_DIR)/../../sdk2_1/sdk2_1',
		'D:/commit/basedOnTheComponent/HWLIB/KT_NJGYPCC_1.0.0/golang/jsoncpp-src-0.5.0/include',
		'D:/commit/basedOnTheComponent/HWLIB/KT_NJGYPCC_1.0.0/golang/libRPDCardV1withoutThread/libRPDCardV1',
		'D:/commit/basedOnTheComponent/HWLIB/KT_NJGYPCC_1.0.0/golang/libRFMCardV3withoutThread/libRFMCardV3',
		'D:/commit/basedOnTheComponent/HWLIB/KT_NJGYPCC_1.0.0/golang/generationsEpsonwithoutThread',
		'D:/commit/basedOnTheComponent/HWLIB/KT_NJGYPCC_1.0.0/golang/libNV9withoutThread/libNV9',
		'D:/commit/basedOnTheComponent/HWLIB/KT_NJGYPCC_1.0.0/golang/ictcashinwithoutThread/libICTcashin',
		'E:/192.168.6.44/boost_1_68_0'
      ],
	  "library_dirs": [
		'<(PRODUCT_DIR)/../../sdk2_1/Release/',
		'D:/commit/basedOnTheComponent/HWLIB/KT_NJGYPCC_1.0.0/golang/jsoncpp-src-0.5.0/build/vs71/release/lib_json',
		'D:/commit/basedOnTheComponent/HWLIB/KT_NJGYPCC_1.0.0/golang/libRPDCardV1withoutThread/Release',
		'D:/commit/basedOnTheComponent/HWLIB/KT_NJGYPCC_1.0.0/golang/libRFMCardV3withoutThread/Release',
		'D:/commit/basedOnTheComponent/HWLIB/KT_NJGYPCC_1.0.0/golang/generationsEpsonwithoutThread/Release',
		'D:/commit/basedOnTheComponent/HWLIB/KT_NJGYPCC_1.0.0/golang/libNV9withoutThread/Release',
		'D:/commit/basedOnTheComponent/HWLIB/KT_NJGYPCC_1.0.0/golang/ictcashinwithoutThread/libICTcashin/Release',		
		'E:/192.168.6.44/boost_1_68_0/build/vc141/lib'
	  ],
      'libraries': [ 
		'<(PRODUCT_DIR)/../../sdk2_1/Release/sdk2_1.lib',
		'D:/commit/basedOnTheComponent/HWLIB/KT_NJGYPCC_1.0.0/golang/jsoncpp-src-0.5.0/build/vs71/release/lib_json/json_vc71_libmt.lib',
		'D:/commit/basedOnTheComponent/HWLIB/KT_NJGYPCC_1.0.0/golang/libRPDCardV1withoutThread/Release/libRPDCardV1.lib',
		'D:/commit/basedOnTheComponent/HWLIB/KT_NJGYPCC_1.0.0/golang/libRFMCardV3withoutThread/Release/libRFMCardV3.lib',
		'D:/commit/basedOnTheComponent/HWLIB/KT_NJGYPCC_1.0.0/golang/generationsEpsonwithoutThread/Release/generationsEpson.lib',
		'D:/commit/basedOnTheComponent/HWLIB/KT_NJGYPCC_1.0.0/golang/libNV9withoutThread/Release/libNV9.lib',
		'D:/commit/basedOnTheComponent/HWLIB/KT_NJGYPCC_1.0.0/golang/ictcashinwithoutThread/libICTcashin/Release/libICTcashin.lib',		
		'E:/192.168.6.44/boost_1_68_0/build/vc141/lib/boost_log-vc141-mt-x32-1_68.lib',
		'E:/192.168.6.44/boost_1_68_0/build/vc141/lib/boost_log_setup-vc141-mt-x32-1_68.lib',
		'E:/192.168.6.44/boost_1_68_0/build/vc141/lib/boost_thread-vc141-mt-x32-1_68.lib',
		'E:/192.168.6.44/boost_1_68_0/build/vc141/lib/boost_system-vc141-mt-x32-1_68.lib',
		'E:/192.168.6.44/boost_1_68_0/build/vc141/lib/boost_filesystem-vc141-mt-x32-1_68.lib',
		'E:/192.168.6.44/boost_1_68_0/build/vc141/lib/boost_chrono-vc141-mt-x32-1_68.lib',
		'E:/192.168.6.44/boost_1_68_0/build/vc141/lib/boost_date_time-vc141-mt-x32-1_68.lib',
		'E:/192.168.6.44/boost_1_68_0/build/vc141/lib/boost_locale-vc141-mt-x32-1_68.lib',
		'E:/192.168.6.44/boost_1_68_0/build/vc141/lib/boost_program_options-vc141-mt-x32-1_68.lib'
	  ],
	  "copies": [
			{
				"destination": "build/Release",
				"files": [ 	"<(PRODUCT_DIR)/../../sdk2_1/Release/sdk2_1.dll",
							"D:/commit/basedOnTheComponent/HWLIB/KT_NJGYPCC_1.0.0/golang/libRPDCardV1withoutThread/Release/libRPDCardV1.dll",
							"D:/commit/basedOnTheComponent/HWLIB/KT_NJGYPCC_1.0.0/golang/libRFMCardV3withoutThread/Release/libRFMCardV3.dll",
							"D:/commit/basedOnTheComponent/HWLIB/KT_NJGYPCC_1.0.0/golang/generationsEpsonwithoutThread/Release/generationsEpson.dll",
							"D:/commit/basedOnTheComponent/HWLIB/KT_NJGYPCC_1.0.0/golang/libNV9withoutThread/Release/libNV9.dll",
							"D:/commit/basedOnTheComponent/HWLIB/KT_NJGYPCC_1.0.0/golang/ictcashinwithoutThread/libICTcashin/Release/libICTcashin.dll",
							"E:/192.168.6.44/boost_1_68_0/build/vc141/lib/boost_log-vc141-mt-x32-1_68.dll",
							"E:/192.168.6.44/boost_1_68_0/build/vc141/lib/boost_log_setup-vc141-mt-x32-1_68.dll",
							"E:/192.168.6.44/boost_1_68_0/build/vc141/lib/boost_thread-vc141-mt-x32-1_68.dll",
							"E:/192.168.6.44/boost_1_68_0/build/vc141/lib/boost_system-vc141-mt-x32-1_68.dll",
							"E:/192.168.6.44/boost_1_68_0/build/vc141/lib/boost_filesystem-vc141-mt-x32-1_68.dll",
							"E:/192.168.6.44/boost_1_68_0/build/vc141/lib/boost_chrono-vc141-mt-x32-1_68.dll",
							"E:/192.168.6.44/boost_1_68_0/build/vc141/lib/boost_date_time-vc141-mt-x32-1_68.dll",
							"E:/192.168.6.44/boost_1_68_0/build/vc141/lib/boost_locale-vc141-mt-x32-1_68.dll",
							"E:/192.168.6.44/boost_1_68_0/build/vc141/lib/boost_program_options-vc141-mt-x32-1_68.dll"
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
      }
    }
  ]
}
