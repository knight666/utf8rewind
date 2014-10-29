class Header:
	def __init__(self, filename):
		self.file = open(filename, 'wb+')
		self.tab_length = 0
	
	def close(self):
		self.file.close()
	
	def indent(self):
		self.tab_length += 1
	
	def outdent(self):
		self.tab_length -= 1
	
	def writeLine(self, line):
		for i in range(0, self.tab_length):
			self.file.write('\t')
		self.file.write(line)
		self.newLine()
	
	def newLine(self):
		self.file.write('\r\n')