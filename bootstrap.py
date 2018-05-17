import sys
if sys.version_info < (3, 0):
	sys.stdout.write("Bootstrap requires Python 3.x.\n")
	sys.exit(1)

import argparse
import os.path
import re
import subprocess
import build.indent_print as indent_print
import tools.converter.libs.utf8 as utf8

ProgramOptions = None

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

def UnicodeToHexadecimal(text):
	codepoints = []

	match = re.findall('([0-9A-Fa-f]+)', text)
	if match:
		for m in match:
			codepoints.append(int(m, 16))
	else:
		print('Input is not hexadecimal.')
		exit(1)
	
	input = 'U+' + format(codepoints[0], '04X')
	for c in codepoints[1:]:
		input += ' U+' + format(c, '04X')

	print(input + ' "' + utf8.unicodeToUtf8(codepoints) + '"')

if __name__ == '__main__':
	parser = argparse.ArgumentParser(description='Bootstraps project and dependencies')
	parser.add_argument(
		'--uni2hex',
		dest = 'uni2hex',
		nargs = '?',
		help = 'convert Unicode code points to hexadecimal UTF-8 encoded string'
	)
	ProgramOptions = parser.parse_args()

	if ProgramOptions.uni2hex:
		UnicodeToHexadecimal(ProgramOptions.uni2hex)
		exit(0)

	with indent_print.guard():
		GenerateSolution()

	exit(0)