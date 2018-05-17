import sys

__original_write = sys.stdout.write

Level = 0

def _print(text):
	if text != '\n':
		indent = ''
		for i in range(0, Level):
			indent = indent + '\t'
		text = indent + text

	__original_write(text)

sys.stdout.write = _print

def push():
	global Level
	Level = Level + 1

def pop():
	global Level
	if Level > 0:
		Level = Level - 1

class guard:
	def __enter__(self):
		push()

	def __exit__(self, exc_type, exc_val, exc_tb):
		pop()