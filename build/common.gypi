{
	'variables': {
		'conditions': [
			['OS=="win"', {
				'platform_name%': 'windows',
				'architecture_name%': '$(PlatformName)',
			}],
			['OS=="mac"', {
				'platform_name%': 'macosx',
				'architecture_name%': '$(TOOLSET)',
			}],
			['OS=="linux"', {
				'platform_name%': 'linux',
				'architecture_name%': '$(TOOLSET)',
			}],
		]
	},
	'target_defaults': {
		'default_configuration': 'Debug',
		'configurations': {
			'Common': {
				'abstract': 1,
				'conditions': [
					['OS=="win"', {
						'msvs_configuration_attributes': {
							'OutputDirectory': '$(SolutionDir)output\\<(platform_name)\\<(architecture_name)\\$(ConfigurationName)',
							'IntermediateDirectory': '$(SolutionDir)intermediate\\$(ProjectName)\\<(architecture_name)\\$(ConfigurationName)',
							'CharacterSet': '1', # unicode
						},
					}],
					['OS=="linux"', {
						'cflags': [ '-g', '-Wall', '-Wextra' ],
						'cflags_cc': [ '-std=c++11' ],
					}],
				],
			},
			'Platform_x86': {
				'abstract': 1,
				'conditions': [
					['OS=="win"', {
						'msvs_configuration_platform': 'Win32',
						'msvs_settings': {
							'VCLinkerTool': {
								'TargetMachine': '1', # MachineX86
							},
						},
					}],
					['OS=="linux"', {
						'cflags': [ '-m32' ],
					}],
				]
			},
			'Platform_x64': {
				'abstract': 1,
				'conditions': [
					['OS=="win"', {
						'msvs_configuration_platform': 'x64',
						'msvs_settings': {
							'VCLinkerTool': {
								'TargetMachine': '17', # MachineX64
							}
						},
					}],
					['OS=="linux"', {
						'cflags': [ '-m64' ],
					}],
				],
			},
			'Debug_Base': {
				'abstract': 1,
				'conditions': [
					['OS=="win"', {
						'msvs_settings': {
							'VCCLCompilerTool': {
								'PreprocessorDefinitions': [ 'DEBUG', '_DEBUG' ],
								'Optimization': 0, # /Od
								'BasicRuntimeChecks': 3, # /RTC1
								'RuntimeLibrary': 3, # /MDd (dynamic debug)
							},
							'VCLinkerTool': {
								'LinkIncremental': 2, # /INCREMENTAL
							},
						},
					}],
					['OS=="linux"', {
						'cflags': [ '-DDEBUG', '-D_DEBUG' ],
					}],
				],
			},
			'Release_Base': {
				'abstract': 1,
				'conditions': [
					['OS=="win"', {
						'msvs_settings': {
							'VCCLCompilerTool': {
								'PreprocessorDefinitions': [ 'NDEBUG' ],
								'RuntimeLibrary': 2, # /MD (dynamic release)
								'Optimization': 3, # /Ox
								'InlineFunctionExpansion': 2, # /Ob2
								'EnableIntrinsicFunctions': 'true', # /Oi
								'FavorSizeOrSpeed': 1, # /Ot
								'EnableEnhancedInstructionSet': 2, # /arch:SSE2
								'FloatingPointModel': 2, # /fp:fast
							},
							'VCLinkerTool': {
								'LinkIncremental': 2, # /INCREMENTAL
							},
						},
					}],
					['OS=="linux"', {
						'cflags': [ '-DNDEBUG' ],
					}]
				],
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
		'conditions': [
			['OS == "win"', {
				'defines': [ 'WIN32' ],
			}]
		],
	},
}