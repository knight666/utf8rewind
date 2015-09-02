import os.path
import re
import sys

class Processor:
	def __init__(self):
		self.sectionName = ''
		self.state_map = {
			'section': self.ProcessSection,
			'comments': self.ProcessComments,
			'test': self.ProcessTest
		}
		self.state = 'section'
	
	def Parse(self, filepath):
		with open(filepath, 'rb') as f:
			self.state = 'section'
			
			bytes_read = bytearray()
			while True:
				current = f.read(1)
				if not current:
					break
				
				if current == b'\n':
					line = str(bytes_read, encoding='utf-8')
					self.state = self.state_map[self.state](line)
					
					bytes_read = bytearray()
				else:
					bytes_read.append(ord(current))

	def ProcessSection(self, line):
		match = re.match('#\t(.+)', line)
		if not match:
			exit(1)
		
		self.sectionName = match.group(1)
		print(self.sectionName)
		return 'comments'

	def ProcessComments(self, line):
		if len(line) > 0 and not re.match('#.*', line):
			return 'test'
		
		return 'comments'

	def ProcessTest(self, line):
		if len(line) == 0:
			return 'section'
		
		print(line.encode('utf-8'))
		
		return 'test'

if __name__ == '__main__':
	current_directory = os.path.dirname(os.path.realpath(sys.argv[0]))
	processor = Processor()
	processor.Parse(current_directory + '/../../testdata/big-list-of-naughty-strings-master/blns.txt')