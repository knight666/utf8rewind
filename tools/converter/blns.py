import os.path
import re
import sys

class Section:
	def __init__(self, name):
		self.name = re.sub('[^A-Za-z0-9_]', '', name)
		self.tests = []
	
	def Render(self, header):
		print(self.name)
		for t in self.tests:
			print(str(t, encoding='utf-8').encode('utf-8'))

class Processor:
	def __init__(self):
		self.current = None
		self.sections = []
		self.state_map = {
			'section': self.ProcessSection,
			'comments': self.ProcessComments,
			'test': self.ProcessTest,
			'exit': self.ProcessExit
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
					self.state = self.state_map[self.state](line, bytes_read)
					
					bytes_read = bytearray()
				else:
					bytes_read.append(ord(current))
	
	def Render(self, header):
		for s in self.sections:
			s.Render(None)

	def ProcessSection(self, line, bytes):
		match = re.match('#[\t ]+(.+)', line)
		if not match:
			print(line)
			return 'exit'
		
		self.current = Section(match.group(1))
		self.sections.append(self.current)
		
		return 'comments'

	def ProcessComments(self, line, bytes):
		if len(line) > 0 and not re.match('#.*', line):
			return self.ProcessTest(line, bytes)
		
		return 'comments'

	def ProcessTest(self, line, bytes):
		if len(line) == 0:
			return 'section'
		
		self.current.tests.append(bytes)
		
		return 'test'
	
	def ProcessExit(self, line, bytes):
		print('Error parsing file.')
		exit(1)

if __name__ == '__main__':
	current_directory = os.path.dirname(os.path.realpath(sys.argv[0]))
	processor = Processor()
	processor.Parse(current_directory + '/../../testdata/big-list-of-naughty-strings-master/blns.txt')
	processor.Render(None)