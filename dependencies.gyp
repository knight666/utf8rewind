{
	'includes': [
		'build/common.gypi',
	],
	'targets': [
		{
			'target_name': 'gtest',
			'type': 'static_library',
			'variables': {
				'project_dir': 'dependencies/gtest-svn-head',
			},
			'defines': [
				'GTEST_HAS_PTHREAD=0',
			],
			'include_dirs': [
				'<(project_dir)',
				'<(project_dir)/include',
			],
			'sources': [
				'<(project_dir)/src/gtest-all.cc',
			],
			'direct_dependent_settings': {
				'include_dirs': [
					'<(project_dir)/include',
				],
			},
			'conditions': [
				['OS!="win"', {
					'product_dir': 'output/<(platform_name)/<(architecture_name)/<(CONFIGURATION_NAME)',
				}],
				['OS=="win"', {
					'defines': [
						'GTEST_HAS_TR1_TUPLE=0',
					],
				}],
			],
		},
	],
}