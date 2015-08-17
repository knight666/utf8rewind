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
				'source/internal/base.h',
				'source/internal/casemapping.c',
				'source/internal/casemapping.h',
				'source/internal/codepoint.c',
				'source/internal/codepoint.h',
				'source/internal/composition.c',
				'source/internal/composition.h',
				'source/internal/database.c',
				'source/internal/database.h',
				'source/internal/decomposition.c',
				'source/internal/decomposition.h',
				'source/internal/seeking.c',
				'source/internal/seeking.h',
				'source/internal/streaming.c',
				'source/internal/streaming.h',
				'source/unicodedatabase.c',
				'source/unicodedatabase.h',
				'source/utf8rewind.c',
			],
			'direct_dependent_settings': {
				'include_dirs': [
					'include/utf8rewind',
				],
			},
			'conditions': [
				['OS=="win"', {
					'msvs_settings': {
						'VCCLCompilerTool': {
							'CompileAs': 1, # Compile as C
						},
					},
				}],
				['OS!="win"', {
					'product_dir': 'output/<(platform_name)/<(architecture_name)/<(CONFIGURATION_NAME)',
				}],
			],
		},
		{
			'target_name': 'helpers',
			'type': 'static_library',
			'dependencies': [
				'dependencies.gyp:gtest',
				'utf8rewind',
			],
			'sources': [
				'source/helpers/helpers-base.hpp',
				'source/helpers/helpers-casemapping.cpp',
				'source/helpers/helpers-casemapping.hpp',
				'source/helpers/helpers-errors.cpp',
				'source/helpers/helpers-errors.hpp',
				'source/helpers/helpers-normalization.cpp',
				'source/helpers/helpers-normalization.hpp',
				'source/helpers/helpers-seeking.cpp',
				'source/helpers/helpers-seeking.hpp',
				'source/helpers/helpers-streams.cpp',
				'source/helpers/helpers-streams.hpp',
				'source/helpers/helpers-strings.cpp',
				'source/helpers/helpers-strings.hpp',
			],
			'conditions': [
				['OS!="win"', {
					'product_dir': 'output/<(platform_name)/<(architecture_name)/<(CONFIGURATION_NAME)',
				}],
			],
		},
		{
			'target_name': 'tests-rewind',
			'type': 'executable',
			'dependencies': [
				'dependencies.gyp:gtest',
				'utf8rewind',
				'helpers',
			],
			'sources': [
				'source/tests/integration-casemapping.cpp',
				'source/tests/integration-conformance.cpp',
				'source/tests/integration-isnormalized.cpp',
				'source/tests/integration-normalization.cpp',
				'source/tests/integration-quickbrown-casemapping.cpp',
				'source/tests/integration-quickbrown.cpp',
				'source/tests/suite-casemapping-execute.cpp',
				'source/tests/suite-codepoint-decoded-length.cpp',
				'source/tests/suite-codepoint-encoded-length.cpp',
				'source/tests/suite-codepoint-read.cpp',
				'source/tests/suite-codepoint-write.cpp',
				'source/tests/suite-compose-execute.cpp',
				'source/tests/suite-database-querycomposition.cpp',
				'source/tests/suite-database-querydecomposition.cpp',
				'source/tests/suite-database-queryproperty.cpp',
				'source/tests/suite-decompose-execute.cpp',
				'source/tests/suite-stream-read.cpp',
				'source/tests/suite-stream-reorder.cpp',
				'source/tests/suite-stream-write.cpp',
				'source/tests/suite-utf16-toutf8.cpp',
				'source/tests/suite-utf32-toutf8.cpp',
				'source/tests/suite-utf8-isnormalized-compose.cpp',
				'source/tests/suite-utf8-isnormalized-decompose.cpp',
				'source/tests/suite-utf8-isnormalized.cpp',
				'source/tests/suite-utf8-len.cpp',
				'source/tests/suite-utf8-normalize-compose.cpp',
				'source/tests/suite-utf8-normalize-decompose.cpp',
				'source/tests/suite-utf8-normalize.cpp',
				'source/tests/suite-utf8-seek-current.cpp',
				'source/tests/suite-utf8-seek-current-backwards.cpp',
				'source/tests/suite-utf8-seek-current-forward.cpp',
				'source/tests/suite-utf8-seek-end.cpp',
				'source/tests/suite-utf8-seek-set.cpp',
				'source/tests/suite-utf8-seek.cpp',
				'source/tests/suite-utf8-tolower.cpp',
				'source/tests/suite-utf8-totitle.cpp',
				'source/tests/suite-utf8-toupper.cpp',
				'source/tests/suite-utf8-toutf16.cpp',
				'source/tests/suite-utf8-toutf32.cpp',
				'source/tests/suite-utf8-towide.cpp',
				'source/tests/suite-wide-toutf8.cpp',
				'source/tests/tests-base.hpp',
				'source/tests/tests-main.cpp',
			],
			'conditions': [
				['OS!="win"', {
					'product_dir': 'output/<(platform_name)/<(architecture_name)/<(CONFIGURATION_NAME)',
				}],
			],
		},
		{
			'target_name': 'documentation',
			'type': 'none',
			'rules': [
				{
					'rule_name': 'Doxyfile',
					'msvs_cygwin_shell': 0,
					'msvs_quote_cmd': 0,
					'extension': 'doxyfile',
					'outputs': [
						'./documentation/html/index.html',
					],
					'action': [
						'doxygen',
						'utf8rewind.doxyfile',
					],
				},
			],
			'sources': [
				'source/documentation/examples.hpp',
				'source/documentation/overview.hpp',
				'utf8rewind.doxyfile'
			],
		},
	],
}