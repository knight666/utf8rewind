import argparse
import re
import libs.unicode

MAX_LEGAL_UNICODE = 0x10FFFF
REPLACEMENT_CHARACTER = 0xFFFD

def codepointToUtf8(codepoint):
	encoded_length = 0
	result = ""
	
	if codepoint < 0x80:
		encoded_length = 1
	elif codepoint < 0x800:
		encoded_length = 2
	elif codepoint < 0x10000:
		encoded_length = 3
	elif codepoint <= MAX_LEGAL_UNICODE:
		encoded_length = 4
	else:
		codepoint = REPLACEMENT_CHARACTER
		encoded_length = 3
	
	if encoded_length == 1:
		result = '\\x' + format(codepoint, '02x')
	elif encoded_length == 2:
		result += '\\x' + format((codepoint >>   6)         | 0xC0, '02x')
		result += '\\x' + format((codepoint         & 0x3F) | 0x80, '02x')
	elif encoded_length == 3:
		result += '\\x' + format((codepoint  >> 12)         | 0xE0, '02x')
		result += '\\x' + format(((codepoint >>  6) & 0x3F) | 0x80, '02x')
		result += '\\x' + format((codepoint         & 0x3F) | 0x80, '02x')
	elif encoded_length == 4:
		result += '\\x' + format((codepoint  >> 18)         | 0xF0, '02x')
		result += '\\x' + format((codepoint  >> 12)         | 0x80, '02x')
		result += '\\x' + format(((codepoint >>  6) & 0x3F) | 0x80, '02x')
		result += '\\x' + format((codepoint         & 0x3F) | 0x80, '02x')
	
	return result

class Header:
	def __init__(self, filename):
		self.file = open(filename, 'w+')
	
	def close(self):
		close(self.file)

class Printer(libs.unicode.UnicodeVisitor):
	def visitDocument(self, document):
		print document.filename
	
	def visitSection(self, section):
		print " - " + section.title
	
	def visitEntry(self, entry):
		result = "{ "
		count = 0
		for group in entry.matches[0].groups():
			if count <> 0:
				result += ", "
			if group <> None:
				result += "0x" + group
			else:
				result += "0"
			count += 1
		result += " }"
		print " - - " + result

class BinaryBlob(libs.unicode.UnicodeVisitor):
	def __init__(self):
		self.blob = "\\0"
		self.total = 0
		self.offset = 1
		self.hashed = dict()
	
	def visitEntry(self, entry):
		for match in entry.matches:
			if match <> None:
				for group in match.groups():
					if group <> None:
						codepoint = int(group, 16)
						converted = codepointToUtf8(codepoint) + "\\0"
						if converted not in self.hashed:
							character_matches = re.findall('\\\\x?[^\\\\]+', converted)
							if character_matches:
								offset = len(character_matches)
							else:
								offset = 0
							
							self.hashed[converted] = self.offset
							self.offset += offset
							self.blob += converted
						else:
							offset = self.hashed[converted]
						print "codepoint " + hex(codepoint) + " offset " + str(self.hashed[converted])
						self.total += 1
	
	def write(self):
		print "total " + str(self.total)
		print "hashed " + str(len(self.hashed))
		print self.blob

if __name__ == '__main__':
	parser = argparse.ArgumentParser(description='Converts Unicode data files.')
	parser.add_argument(
		'--limiter',
		type=int,
		help='limit the amount of entries'
	)
	args = parser.parse_args()

	normalization = libs.unicode.UnicodeDocument()
	normalization.limiter = args.limiter
	normalization.parse('data/NormalizationTest.txt')
	
	printer = BinaryBlob()
	normalization.accept(printer)
	printer.write()