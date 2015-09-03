import datetime
import os.path
import re
import sys
import libs.header

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

class Test:
	def __init__(self, line, bytes, offset):
		self.line = line
		self.bytes = bytes
		self.offset = offset

	def Render(self, header):
		wrote_hex = False
		converted = ''
		for c in (self.line[pos:pos + 2] for pos in range(0, len(self.line), 2)):
			codepoint = (c[1] << 8) | c[0]
			result, wrote_hex = codepointToUtf16(codepoint, wrote_hex)
			converted += result
		header.writeLine('EXPECT_STREQ(L"' + converted + '", helpers::wide(ReadSection(' + str(self.offset) + ', ' + str(len(self.bytes)) + ')).c_str());')

class Section:
	def __init__(self, name):
		self.name = re.sub('[^A-Za-z0-9_]', '', name)
		self.tests = []
	
	def Render(self, header):
		print('Writing tests for "' + self.name + '"...')
		
		header.newLine()
		header.writeLine("TEST_F(NaughtyStrings, " + self.name + ")")
		header.writeLine("{")
		header.indent()
		
		for t in self.tests:
			t.Render(header)
		
		header.outdent()
		header.writeLine("}")

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
			offset = 0

			while True:
				current = f.read(1)
				offset += 1

				if not current:
					break
				
				if current == b'\n':
					line = str(bytes_read, encoding='utf-8')
					self.state = self.state_map[self.state](line, bytes_read, offset)
					
					bytes_read = bytearray()
				else:
					bytes_read.append(ord(current))
	
	def Render(self, header):
		command_line = sys.argv[0]
		arguments = sys.argv[1:]
		for a in arguments:
			command_line += ' ' + a
		
		header.writeLine('/*')
		header.indent()
		header.writeLine('DO NOT MODIFY, AUTO-GENERATED')
		header.newLine()
		header.writeLine('Generated on:')
		header.indent()
		header.writeLine(datetime.datetime.now().strftime('%Y-%m-%dT%H:%M:%S'))
		header.outdent()
		header.newLine()
		header.writeLine('Command line:')
		header.indent()
		header.writeLine(command_line)
		header.outdent()
		header.outdent()
		header.writeLine('*/')
		header.newLine()
		header.writeLine('#include "tests-base.hpp"')
		header.newLine()
		header.writeLine('#include "../helpers/helpers-strings.hpp"')
		header.newLine()
		header.writeLine('#define NAUGHTY_STRINGS_LENGTH 10370')
		header.newLine()
		header.writeLine('class NaughtyStrings')
		header.writeLine('	: public ::testing::Test')
		header.writeLine('{')
		header.newLine()
		header.writeLine('protected:')
		header.newLine()
		header.writeLine('	void SetUp()')
		header.writeLine('	{')
		header.writeLine('		file.open("testdata/big-list-of-naughty-strings-master/blns.txt", std::ios_base::in);')
		header.writeLine('		ASSERT_TRUE(file.is_open());')
		header.writeLine('	}')
		header.newLine()
		header.writeLine('	void TearDown()')
		header.writeLine('	{')
		header.writeLine('		file.close();')
		header.writeLine('	}')
		header.newLine()
		header.writeLine('	std::string ReadSection(size_t position, size_t length)')
		header.writeLine('	{')
		header.writeLine('		std::string result;')
		header.newLine()
		header.writeLine('		file.seekg(position, std::ios::beg);')
		header.writeLine('		if (file.eof())')
		header.writeLine('		{')
		header.writeLine('			return result;')
		header.writeLine('		}')
		header.newLine()
		header.writeLine('		result.resize(length + 1);')
		header.writeLine('		file.read(&result[0], length);')
		header.newLine()
		header.writeLine('		return result;')
		header.writeLine('	}')
		header.newLine()
		header.writeLine('	std::fstream file;')
		header.newLine()
		header.writeLine('};')
		
		for s in self.sections:
			s.Render(header)

	def ProcessSection(self, line, bytes, offset):
		match = re.match('#[\t ]+(.+)', line)
		if not match:
			return 'exit'
		
		self.current = Section(match.group(1))
		self.sections.append(self.current)
		
		return 'comments'

	def ProcessComments(self, line, bytes, offset):
		if len(line) > 0 and not re.match('#.*', line):
			return self.ProcessTest(line, bytes, offset)
		
		return 'comments'

	def ProcessTest(self, line, bytes, offset):
		if len(line) == 0:
			return 'section'
		
		test = Test(line.encode('utf-16le'), bytes, offset)
		self.current.tests.append(test)
		
		return 'test'
	
	def ProcessExit(self, line, bytes, offset):
		print('Error parsing file.')
		exit(1)

if __name__ == '__main__':
	current_directory = os.path.dirname(os.path.realpath(sys.argv[0]))
	processor = Processor()
	processor.Parse(current_directory + '/../../testdata/big-list-of-naughty-strings-master/blns.txt')
	
	header = libs.header.Header(current_directory + '/integration-naughty-strings.cpp')
	processor.Render(header)