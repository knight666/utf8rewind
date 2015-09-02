import os.path
import re
import sys

def codepointToUtf16(codepoint, wroteHex = False):
	result = ''
	
	if codepoint <= 0x7F:
		conversion = {
			0x00: "\\0",
			0x07: "\\a",
			0x08: "\\b",
			0x09: "\\t",
			0x0A: "\\n",
			0x0B: "\\v",
			0x0C: "\\f",
			0x0D: "\\r",
			
			# must be escaped
			0x22: "\\\"",
			0x5C: "\\\\",
		}
		if codepoint in conversion:
			result += conversion[codepoint]
			
			return result, False
		elif codepoint < 0x20:
			result += '\\x' + format(codepoint, 'X')
			
			return result, True
		else:
			isHex = (codepoint >= 0x41 and codepoint <= 0x46) or (codepoint >= 0x61 and codepoint <= 0x76) or (codepoint >= 0x30 and codepoint <= 0x39)
			
			if wroteHex and isHex:
				result += "\" \""
			result += "%c" % codepoint
			
			return result, False
	elif codepoint <= 0xFFFF:
		result += '\\x' + format(codepoint, 'X')
		
		return result, True
	else:
		decoded = codepoint - 0x10000
		surrogate_high = 0xD800 + (decoded >> 10)
		surrogate_low = 0xDC00 + (decoded & 0x03FF)
		
		result += '\\x' + format(surrogate_high, '4X')
		result += '\\x' + format(surrogate_low, '4X')
		
		return result, True

class Section:
	def __init__(self, name):
		self.name = re.sub('[^A-Za-z0-9_]', '', name)
		self.tests = []
	
	def Render(self, header):
		print(self.name)
		for t in self.tests:
			wrote_hex = False
			converted = ''
			for c in t:
				result, wrote_hex = codepointToUtf16(c, wrote_hex)
				converted += result
			print('"' + converted + '"')

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