{
	'includes': [
		'build/common.gypi',
	],
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