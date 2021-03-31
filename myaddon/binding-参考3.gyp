{
  "targets": 
  [  
    { 
      "target_name": "sdkaddon",
      "sources": [ 
        "sdkaddon.cc"
      ],
      "include_dirs": [ 
        "<!@(node -p \"require('node-addon-api').include\")", 
		'sdk2_1/sdk2_1'
      ],
	  "library_dirs": [ 
		'sdk2_1/Debug/'
	  ],
      "libraries": [ 
        '-lssl3 -lcrypto -lsdk2_2',
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
