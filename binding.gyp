{
	"targets": [
		{
			"target_name": "regex2",
			"sources": [ "regex2.cc" ],
			"include_dirs" : [
				"<!(node -e \"require('nan')\")"
			],
			"conditions": [
				[ 'OS!="win"', {
					"cflags+": [ "-std=c++11" ],
					"cflags_c+": [ "-std=c++11" ],
					"cflags_cc+": [ "-std=c++11" ],
				}],
				[ 'OS=="mac"', {
					"xcode_settings": {
						"OTHER_CPLUSPLUSFLAGS" : [ "-std=c++11", "-stdlib=libc++" ],
						"OTHER_LDFLAGS": [ "-stdlib=libc++" ],
						"MACOSX_DEPLOYMENT_TARGET": "10.7",
						'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
					},
				}],
			],
			'cflags!': [ '-fno-exceptions' ],
			'cflags_cc!': [ '-fno-exceptions' ]
		}
	],
}
