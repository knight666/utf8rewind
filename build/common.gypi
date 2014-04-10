{
	'target_defaults': {
		'default_configuration': 'Debug',
		'configurations': {
			'Common': {
				'abstract': 1,
				'msvs_configuration_attributes': {
					'OutputDirectory': '$(SolutionDir)$(ConfigurationName)',
					'IntermediateDirectory': '$(OutDir)\\obj\\$(ProjectName)',
					'CharacterSet': '1', # unicode
				},
			},
			'Debug_Base': {
				'abstract': 1,
				'msvs_settings': {
					'VCCLCompilerTool': {
						'Optimization': 0, # /Od
						'PreprocessorDefinitions': [ 'DEBUG', '_DEBUG' ],
						'BasicRuntimeChecks': 3,
						'RuntimeLibrary': 1, # /MTd (static debug)
					},
					'VCLinkerTool': {
						'LinkIncremental': 2,
					},
					'VCResourceCompilerTool': {
						'PreprocessorDefinitions': [ 'DEBUG', '_DEBUG' ],
					},
				},
			},
			'Release_Base': {
				'abstract': 1,
				'msvs_settings': {
					'VCCLCompilerTool': {
						'Optimization': 2, # /Os
						'PreprocessorDefinitions': [ 'NDEBUG' ],
						'RuntimeLibrary': 0, # /MT (static release)
					},
					'VCLinkerTool': {
						'LinkIncremental': 1,
					},
					'VCResourceCompilerTool': {
						'PreprocessorDefinitions': [ 'NDEBUG' ],
					},
				},
			},
			'Debug': {
				'inherit_from': ['Common', 'Debug_Base' ],
			},
			'Release': {
				'inherit_from': ['Common', 'Release_Base' ],
			},
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
}