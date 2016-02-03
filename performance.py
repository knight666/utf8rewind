import argparse
import os.path
import re
import subprocess

if __name__ == '__main__':
	parser = argparse.ArgumentParser(description='Runs combinations of performance tests.')
	parser.add_argument(
		'--config',
		dest = 'config',
		default = '',
		help = 'Override configuration to <Platform>_<Configuration>, i.e. x64_Debug.'
	)
	parser.add_argument(
		'--casefolding',
		dest = 'casefolding',
		action = 'store_true',
		help = 'Compare casefolding performance against lowercasing.'
	)
	args = parser.parse_args()

	path = None

	if os.path.exists('output/windows'):
		path = 'output/windows'
	elif os.path.exists('output/linux'):
		path = 'output/linux'

	if not path:
		print('Failed to find executable path.')
		exit(-1)

	if len(args.config) > 0:
		matches = re.match('(\w+)_(\w+)', args.config)
		path = path + '/' + matches.group(1) + '/' + matches.group(2)
	else:
		if os.path.exists(path + '/x64'):
			path += '/x64'
		elif os.path.exists(path + '/Win32'):
			path += '/Win32'
		else:
			print('Failed to find configuration path at ' + path + '.')
			exit(-1)

		if os.path.exists(path + '/Release'):
			path += '/Release'
		elif os.path.exists(path + '/Debug'):
			path += '/Debug'
		else:
			print('Failed to find release configuration path at ' + path + '.')
			exit(-1)

	executable_path = path + '/performance-rewind.exe'

	if not os.path.exists(executable_path):
		print('Failed to find path at ' + path + '.')
		exit(-1)

	print('Running executable at \"' + executable_path + '\".')

	process = subprocess.Popen(
		'"' + executable_path + '"',
		shell=True
	)

	output, errors = process.communicate()
	errcode = process.returncode