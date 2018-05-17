import sys
if sys.version_info < (3, 0):
	sys.stdout.write("Bootstrap requires Python 3.x.\n")
	sys.exit(1)

# indented print

__original_write = sys.stdout.write

IndentationLevel = 0

def IndentationPrint(text):
	if text != '\n':
		indent = ''
		for i in range(0, IndentationLevel):
			indent = indent + '\t'
		text = indent + text

	__original_write(text)

sys.stdout.write = IndentationPrint

def IndentationPush():
	global IndentationLevel
	IndentationLevel = IndentationLevel + 1

def IndentationPop():
	global IndentationLevel
	if IndentationLevel > 0:
		IndentationLevel = IndentationLevel - 1

class IndentationGuard:
	def __enter__(self):
		IndentationPush()

	def __exit__(self, exc_type, exc_val, exc_tb):
		IndentationPop()

if __name__ == '__main__':
	print('Strapping boots...')

	IndentationPush()
	print('blah')
	IndentationPop()

	print('more')
	with IndentationGuard():
		print('stuff')
	print('and things')

	exit(0)