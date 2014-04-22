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
								'CompileAs': 1, # Compile as C
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
			'library_dirs': [
				'dependencies/gtest-1.7.0-rc1/lib/windows/x86/<(CONFIGURATION_NAME)',
				'<(CONFIGURATION_NAME)/lib',
			],
			'libraries': [
				'gtest.lib',
				'utf8rewind.lib',
			],
			'sources': [
				'source/suite-charlen.cpp',
				'source/suite-charvalid.cpp',
				'source/suite-convertucs2.cpp',
				'source/suite-decode.cpp',
				'source/suite-encode.cpp',
				'source/suite-len.cpp',
				'source/suite-seek.cpp',
				'source/suite-toutf8.cpp',
				'source/suite-towc.cpp',
				'source/tests-base.hpp',
				'source/tests-main.cpp',
			],
		},
	],
}