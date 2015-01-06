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
				'source/internal/casemapping.c',
				'source/internal/casemapping.h',
				'source/internal/codepoint.c',
				'source/internal/codepoint.h',
				'source/internal/composition.c',
				'source/internal/composition.h',
				'source/internal/database.c',
				'source/internal/database.h',
				'source/internal/normalization.c',
				'source/internal/normalization.h',
				'source/internal/seeking.c',
				'source/internal/seeking.h',
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
			'target_name': 'tests-rewind',
			'type': 'executable',
			'dependencies': [
				'dependencies.gyp:gtest',
				'utf8rewind',
			],
			'defines': [
				'GTEST_HAS_PTHREAD=0',
			],
			'sources': [
				'source/tests/helpers-casemapping.cpp',
				'source/tests/helpers-casemapping.hpp',
				'source/tests/helpers-normalization.cpp',
				'source/tests/helpers-normalization.hpp',
				'source/tests/helpers-strings.cpp',
				'source/tests/helpers-strings.hpp',
				'source/tests/integration-casemapping.cpp',
				'source/tests/integration-conformance.cpp',
				'source/tests/integration-internal-composition.cpp',
				'source/tests/integration-normalization.cpp',
				'source/tests/integration-quickbrown-casemapping.cpp',
				'source/tests/integration-quickbrown.cpp',
				'source/tests/performance-casemapping.cpp',
				'source/tests/suite-database-querycomposition.cpp',
				'source/tests/suite-database-querydecomposition.cpp',
				'source/tests/suite-database-queryproperty.cpp',
				'source/tests/suite-decodeutf16.cpp',
				'source/tests/suite-decodeutf32.cpp',
				'source/tests/suite-encodeutf16.cpp',
				'source/tests/suite-encodeutf32.cpp',
				'source/tests/suite-len.cpp',
				'source/tests/suite-seek.cpp',
				'source/tests/suite-seek-current.cpp',
				'source/tests/suite-seek-end.cpp',
				'source/tests/suite-seek-forward.cpp',
				'source/tests/suite-seek-set.cpp',
				'source/tests/suite-tolower.cpp',
				'source/tests/suite-toupper.cpp',
				'source/tests/suite-toutf8.cpp',
				'source/tests/suite-towide.cpp',
				'source/tests/suite-transform-compose.cpp',
				'source/tests/suite-transform-decompose.cpp',
				'source/tests/tests-base.hpp',
				'source/tests/tests-main.cpp',
			],
			'conditions': [
				['OS!="win"', {
					'product_dir': 'output/<(platform_name)/<(architecture_name)/<(CONFIGURATION_NAME)',
				}],
				['OS=="win"', {
					'defines': [
						'GTEST_HAS_TR1_TUPLE=0'
					],
				}],
			],
		},
	],
}