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
	
	def write(self, line):
		self.file.write(line)
	
	def newLine(self):
		self.file.write('\r\n')
	
	def writeIndentation(self):
		for i in range(0, self.tab_length):
			self.file.write('\t')
	
	def writeLine(self, line):
		self.writeIndentation()
		self.write(line)
		self.newLine()