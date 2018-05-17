import sys
if sys.version_info < (3, 0):
	sys.stdout.write("Bootstrap requires Python 3.x.\n")
	sys.exit(1)

if __name__ == '__main__':
	print('Strapping boots...')

	exit(0)