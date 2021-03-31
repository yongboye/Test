{
  "targets": 
  [ 
    { # 项目列表1
      "target_name": "sdkaddon", # 项目名称
      "sources": [ # 源文件
        "sdkaddon.cc" 
      ],
      "include_dirs": [ # 包含文件的查找目录，类似于C/C++命令行编译中的-I或/I选项
        "<!@(node -p \"require('node-addon-api').include\")", 
		'sdk2_1/sdk2_1'
      ],
	  "library_dirs": [ # 依赖库目录
		'sdk2_1/x64/Debug/'
	  ],
      'libraries': [ # 依赖库文件
		'<(PRODUCT_DIR)/../../sdk2_1/x64/Debug/sdk2_1.lib', 
	  ],
	  "copies": [ # 文件拷贝
			{
				"destination": "build/Release",
				"files": [ "sdk2_1/x64/Debug/sdk2_1.dll" ]
			}
	  ],
      "dependencies": [ # 依赖target
        "<!(node -p \"require('node-addon-api').gyp\")"
      ],
      "cflags!": ["-fno-exceptions"], # 用于 C 编译器的选项
      "cflags_cc!": ["-fno-exceptions"], # 用于 C++ 编译器的选项
      "defines": ["NAPI_CPP_EXCEPTIONS"], # 预处理宏，类似于C/C++命令行编译中的-D或/D选项
      "xcode_settings": { # win vs, mac xcode, linux makefile
        "GCC_ENABLE_CPP_EXCEPTIONS": "YES"
      }
    }
  ]
}
