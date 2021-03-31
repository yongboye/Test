{
  "targets": 
  [
    {
      "target_name": "sdkaddon",
      'conditions': 
	  [
        [ 
			'OS=="win"',  # win mac linux 
			{
				"cflags!": [ "-fno-exceptions" ],
				"cflags_cc!": [ "-fno-exceptions" ],
				"sources": [ "sdkaddon.cc" ],
				"include_dirs": [ "<!@(node -p \"require('node-addon-api').include\")", 'sdk2_1/sdk2_1' ],
				'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
				'link_settings': 
				{
					'conditions': 
					[
						[
							'target_arch=="ia32"', 
							{
								'libraries': ['<(PRODUCT_DIR)/../../sdk2_1/Debug/sdk2_1.lib', ],
								"copies": 
								[
									{
										"destination": "build/Release",
										"files": [ "sdk2_1/Debug/sdk2_1.dll" ]
									}
								]
							},
							{
								'libraries': [ '<(PRODUCT_DIR)/../../sdk2_1/x64/Debug/sdk2_1.lib', ],
								"copies": 
								[
									{
										"destination": "build/Release",
										"files": [ "sdk2_1/x64/Debug/sdk2_1.dll" ]
									}
								]
							}
						]
					],
				},
			}
		]
      ],
    }
  ]
}
