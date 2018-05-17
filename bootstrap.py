import sys
if sys.version_info < (3, 0):
	sys.stdout.write("Bootstrap requires Python 3.x.\n")
	sys.exit(1)

import os.path
import subprocess
import build.indent_print as indent_print

def RunCommand(args, expected = 0, vcVars = False):
	if vcVars:
		vcvars_path = os.path.abspath(os.getenv('VS140COMNTOOLS') + '..\\..\\VC\\vcvarsall.bat')
		args = [ vcvars_path, 'amd64', '&&' ] + args

	with subprocess.Popen(args, shell = True, stdout = subprocess.PIPE, stderr = subprocess.STDOUT) as proc:
		for line in iter(proc.stdout.readline, b''):
			print('\t' + str(line.rstrip()))
		proc.communicate()
		print('')
		return proc.returncode == expected

def GenerateSolution():
	print('Generating solution with GYP...')

	if not RunCommand(['tools\gyp\gyp', '--depth=.', 'utf8rewind.gyp']):
		return False

	print('SUCCEEDED')

	return True

if __name__ == '__main__':
	with indent_print.guard():
		GenerateSolution()

	exit(0)