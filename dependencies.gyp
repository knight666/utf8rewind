{
	'includes': [
		'build/common.gypi',
	],
	'targets': [
		{
			'target_name': 'gtest',
			'type': 'static_library',
			'variables': {
				'project_dir': 'dependencies/gtest-1.7.0-rc1',
			},
			'defines': [
				'GTEST_HAS_PTHREAD=0'
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
		},
	],
}