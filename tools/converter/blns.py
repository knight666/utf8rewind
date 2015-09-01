import os.path
import re
import sys

section_name = ''

def ProcessSection(line):
	match = re.match('#\t(.+)', line)
	if not match:
		exit(1)
	
	section_name = match.group(1)
	print(section_name)
	return 'comments'

def ProcessComments(line):
	if len(line) > 0 and not re.match('#.*', line):
		return 'test'
	
	return 'comments'

def ProcessTest(line):
	if len(line) == 0:
		return 'section'
	
	print(line.encode('utf-8'))
	
	return 'test'

if __name__ == '__main__':
	with open(os.path.dirname(os.path.realpath(sys.argv[0])) + '/data/blns.txt', 'rb') as f:
		state_map = {
			'section': ProcessSection,
			'comments': ProcessComments,
			'test': ProcessTest
		}
		state = 'section'
		
		bytes_read = bytearray()
		while True:
			current = f.read(1)
			if not current:
				break
			
			if current == b'\n':
				line = str(bytes_read, encoding="utf-8")
				state = state_map[state](line)
				
				bytes_read = bytearray()
			else:
				bytes_read.append(ord(current))