import argparse
import fileinput
import re
import sys
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

class CompositionEntry:
	def __init__(self):
		self.stringCodepoint = ""
		self.codepoint = 0
		self.stringC = ""
		self.stringD = ""
		self.stringKC = ""
		self.stringKD = ""
		self.offsetC = 0
		self.offsetD = 0
		self.offsetKC = 0
		self.offsetKD = 0
		
		
	def __str__(self):
		return "{ codepoint: " + hex(self.codepoint) + ", " + "offsetC: " + str(self.offsetC) + ", " + "offsetD: " + str(self.offsetD) + ", " + "offsetKC: " + str(self.offsetKC) + ", " +  "offsetKD: " + str(self.offsetKD) + " }"

class BinaryBlob(libs.unicode.UnicodeVisitor):
	def __init__(self):
		self.blob = "\\0"
		self.total = 0
		self.offset = 1
		self.sectionRead = False
		self.entries = []
		self.hashed = dict()
	
	def visitSection(self, entry):
		print "section " + entry.identifier
		self.sectionRead = entry.identifier == "Part1"
	
	def visitEntry(self, entry):
		if not self.sectionRead:
			return
		
		composition = CompositionEntry()
		
		composition.stringCodepoint = self.matchToString(entry.matches[0])
		composition.codepoint = int(entry.matches[0][0], 16)
		
		composition.stringC = self.matchToString(entry.matches[1])
		if composition.stringC == composition.stringCodepoint:
			composition.offsetC = 0
		else:
			composition.offsetC = self.addTranslation(composition.stringC)
		
		composition.stringD = self.matchToString(entry.matches[2])
		if composition.stringD == composition.stringCodepoint:
			composition.offsetD = 0
		else:
			composition.offsetD = self.addTranslation(composition.stringD)
		
		composition.stringKC = self.matchToString(entry.matches[3])
		if composition.stringKC == composition.stringCodepoint:
			composition.offsetKC = 0
		else:
			composition.offsetKC = self.addTranslation(composition.stringKC)
		
		composition.stringKD = self.matchToString(entry.matches[4])
		if composition.stringKD == composition.stringCodepoint:
			composition.offsetKD = 0
		else:
			composition.offsetKD = self.addTranslation(composition.stringKD)
		
		self.entries.append(composition)
	
	def matchToString(self, match):
		result = ""
		
		if match == None:
			return result
		
		for group in match:
			if group <> None:
				codepoint = int(group, 16)
				converted = codepointToUtf8(codepoint)
				result += converted
		result += "\\0"
		
		return result
	
	def addTranslation(self, translation):
		result = 0
		
		if translation not in self.hashed:
			result = self.offset
			
			character_matches = re.findall('\\\\x?[^\\\\]+', translation)
			if character_matches:
				offset = len(character_matches)
			else:
				offset = 0
			
			#print "hashing " + translation + " offset " + str(self.offset)
			
			self.hashed[translation] = result
			self.offset += offset
			self.blob += translation
		else:
			result = self.hashed[translation]
		
		#print "translated " + translation + " offset " + str(result)
		
		self.total += 1
		
		return result
	
	def write(self):
		command_line = sys.argv[0]
		arguments = sys.argv[1:]
		for a in arguments:
			command_line += " " + a
			
		header = Header('output/normalization.h')
		header.writeLine("/*")
		header.indent()
		header.writeLine("DO NOT MODIFY, AUTO-GENERATED")
		header.newLine()
		header.writeLine("Command line:")
		header.writeLine(command_line)
		header.outdent()
		header.writeLine("*/")
		header.newLine()
		header.writeLine("const char* DecompositionData = ")
		header.indent()
		
		blob_sliced = self.blob
		while (1):
			character_matches = re.match('(\\\\x?[^\\\\]+){25}', blob_sliced)
			if character_matches:
				header.writeLine("\"" + character_matches.group(0) + "\"")
				blob_sliced = blob_sliced[character_matches.end():]
			else:
				header.writeLine("\"" + blob_sliced + "\";")
				break
		
		header.close()
		
		print "entries " + str(self.total) + " hashed " + str(len(self.hashed))

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