{
	'target_defaults': {
		'default_configuration': 'Debug',
		'configurations': {
			'Debug': {
				'defines': [ 'DEBUG', '_DEBUG' ],
				'msvs_settings': {
					'VCCLCompilerTool': {
						'RuntimeLibrary': 1, # static debug
					},
				},
			},
			'Release': {
				'defines': [ 'NDEBUG' ],
				'msvs_settings': {
					'VCCLCompilerTool': {
						'RuntimeLibrary': 0, # static release
					},
				},
			}
		},
		'msvs_settings': {
			'VCCLCompilerTool': {
			},
			'VCLibrarianTool': {
			},
			'VCLinkerTool': {
				'GenerateDebugInformation': 'true',
			},
		},
		'conditions': [
			['OS == "win"', {
				'defines': [ 'WIN32' ],
			}]
		],
	},
	'targets': [
		{
			'target_name': 'utf8rewind',
			'type': 'static_library',
			'include_dirs': [
				'include/utf8rewind'
			],
			'sources': [
				'include/utf8rewind/utf8rewind.h',
				'source/utf8rewind.c'
			],
			'conditions': [
				['OS=="win"', {
						'msvs_settings': {
							'VCCLCompilerTool': {
								'CompileAs': 1,
							},
						},
					}
				]
			],
		},
	],
}