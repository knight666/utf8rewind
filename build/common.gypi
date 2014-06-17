{
	'variables': {
		'conditions': [
			['OS=="win"', {
				'platform_name%': 'windows',
				'architecture_name%': '$(PlatformName)',
			}],
			['OS=="mac"', {
				'platform_name%': 'macosx',
				'architecture_name%': 'x86', # TODO: Determine architecture per target
			}],
			['OS=="linux"', {
				'platform_name%': 'linux',
				'architecture_name%': 'x86', # TODO: Determine architecture per target
			}],
		]
	},
	'target_defaults': {
		'default_configuration': 'Debug',
		'configurations': {
			'Common': {
				'abstract': 1,
				'msvs_configuration_attributes': {
					'OutputDirectory': '$(SolutionDir)output\\<(platform_name)\\<(architecture_name)\\$(ConfigurationName)',
					'IntermediateDirectory': '$(SolutionDir)intermediate\\$(ProjectName)\\<(architecture_name)\\$(ConfigurationName)',
					'CharacterSet': '1', # unicode
				},
				'msvs_settings': {
					'VCLibrarianTool': {
						'OutputFile': '$(OutDir)$(TargetName)$(TargetExt)',
					},
					'VCLinkerTool': {
						'OutputFile': '$(OutDir)$(TargetName)$(TargetExt)',
					},
				},
			},
			'Platform_x86': {
				'abstract': 1,
				'msvs_configuration_platform': 'Win32',
				'msvs_settings': {
					'VCLinkerTool': {
						'TargetMachine': '1', # MachineX86
					},
				},
			},
			'Platform_x64': {
				'abstract': 1,
				'msvs_configuration_platform': 'x64',
				'msvs_settings': {
					'VCLinkerTool': {
						'TargetMachine': '17', # MachineX64
					}
				}
			},
			'Debug_Base': {
				'abstract': 1,
				'msvs_settings': {
					'VCCLCompilerTool': {
						'Optimization': 0, # /Od
						'PreprocessorDefinitions': [ 'DEBUG', '_DEBUG' ],
						'BasicRuntimeChecks': 3,
						'RuntimeLibrary': 3, # /MDd (dynamic debug)
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
						'RuntimeLibrary': 2, # /MD (dynamic release)
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
				'inherit_from': ['Common', 'Platform_x86', 'Debug_Base' ],
			},
			'Release': {
				'inherit_from': ['Common', 'Platform_x86', 'Release_Base' ],
			},
			'Debug_x64': {
				'inherit_from': ['Common', 'Platform_x64', 'Debug_Base' ],
			},
			'Release_x64': {
				'inherit_from': ['Common', 'Platform_x64', 'Release_Base' ],
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