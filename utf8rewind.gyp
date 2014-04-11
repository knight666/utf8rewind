{
	'includes': [
		'build/common.gypi',
	],
	'targets': [
		{
			'target_name': 'utf8rewind',
			'type': 'static_library',
			'include_dirs': [
				'include/utf8rewind',
			],
			'sources': [
				'include/utf8rewind/utf8rewind.h',
				'source/utf8rewind.c',
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
		{
			'target_name': 'tests',
			'type': 'executable',
			'dependencies': [
				'utf8rewind',
			],
			'include_dirs': [
				'dependencies/gtest-1.7.0-rc1/include',
				'include/utf8rewind',
			],
			'libraries': [
				'dependencies/gtest-1.7.0-rc1/lib/windows/x86/Debug/gtest.lib',
				'Debug/lib/utf8rewind.lib',
			],
			'sources': [
				'source/suite-charvalid.cpp',
				'source/suite-decode.cpp',
				'source/suite-encode.cpp',
				'source/tests-base.hpp',
				'source/tests-main.cpp',
			]
		},
	],
}