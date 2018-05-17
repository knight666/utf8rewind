import sys
if sys.version_info < (3, 0):
	sys.stdout.write("Bootstrap requires Python 3.x.\n")
	sys.exit(1)

import build.indent_print as indent_print

if __name__ == '__main__':
	print('Strapping boots...')

	indent_print.push()
	print('blah')
	indent_print.pop()

	print('more')
	with indent_print.guard():
		print('stuff')
	print('and things')

	exit(0)