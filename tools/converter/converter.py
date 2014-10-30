import argparse
import datetime
import math
import re
import os
import sys
import libs.header
import libs.unicode
import libs.utf8

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
		self.lineNumber = 0
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
	
	def toHeaderString(self):
		result = "{ "
		result += hex(self.codepoint)
		result += ", "
		
		if self.offsetC <> 0:
			result += hex(self.offsetC)
		else:
			result += "0"
		result += ", "
		
		if self.offsetD <> 0:
			result += hex(self.offsetD)
		else:
			result += "0"
		result += ", "
		
		if self.offsetKC <> 0:
			result += hex(self.offsetKC)
		else:
			result += "0"
		result += ", "
		
		if self.offsetKD <> 0:
			result += hex(self.offsetKD)
		else:
			result += "0"
		
		result += " }, "
		result += "/* " + str(self.lineNumber) + " */"
		
		return result

class Normalization(libs.unicode.UnicodeVisitor):
	def __init__(self):
		self.verbose = False
		self.blob = ""
		self.pageSize = 32767
		self.total = 0
		self.offset = 0
		self.sectionRead = False
		self.ignoreHangul = False
		self.entries = []
		self.hashed = dict()
	
	def visitDocument(self, document):
		if self.ignoreHangul:
			print "Ignoring Hangul codepoints."
		return True
	
	def visitSection(self, entry):
		print "section " + entry.identifier
		self.sectionRead = entry.identifier == "Part1"
		return True
	
	def visitEntry(self, entry):
		if not self.sectionRead or (self.ignoreHangul and "HANGUL" in entry.comment):
			return True
		
		composition = CompositionEntry()
		composition.lineNumber = entry.lineNumber
		
		composition.stringCodepoint = self.matchToString(entry.matches[0])
		composition.codepoint = int(entry.matches[0][0], 16)
		
		if self.verbose:
			print "codepoint " + hex(composition.codepoint)
		
		composition.stringC = self.matchToString(entry.matches[1])
		if composition.stringC == composition.stringCodepoint:
			composition.offsetC = 0
		else:
			if self.verbose:
				print "inputC " + str(entry.matches[1])
			composition.offsetC = self.addTranslation(composition.stringC)
		
		composition.stringD = self.matchToString(entry.matches[2])
		if composition.stringD == composition.stringCodepoint:
			composition.offsetD = 0
		else:
			if self.verbose:
				print "inputD " + str(entry.matches[2])
			composition.offsetD = self.addTranslation(composition.stringD)
		
		composition.stringKC = self.matchToString(entry.matches[3])
		if composition.stringKC == composition.stringCodepoint:
			composition.offsetKC = 0
		else:
			if self.verbose:
				print "inputKC " + str(entry.matches[3])
			composition.offsetKC = self.addTranslation(composition.stringKC)
		
		composition.stringKD = self.matchToString(entry.matches[4])
		if composition.stringKD == composition.stringCodepoint:
			composition.offsetKD = 0
		else:
			if self.verbose:
				print "inputKD " + str(entry.matches[4])
			composition.offsetKD = self.addTranslation(composition.stringKD)
		
		#if self.verbose:
		#	print composition
		
		self.entries.append(composition)
		
		return True
	
	def matchToString(self, match):
		result = ""
		
		if match == None:
			return result
		
		for group in match:
			if group <> None:
				codepoint = int(group, 16)
				converted = libs.utf8.codepointToUtf8(codepoint)
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
			
			if self.verbose:
				print "hashing " + translation + " offset " + str(self.offset)
			
			self.hashed[translation] = result
			self.offset += offset
			self.blob += translation
		else:
			result = self.hashed[translation]
		
		if self.verbose:
			print "translated " + translation + " offset " + str(result)
		
		self.total += 1
		
		return result
	
	def writeSource(self, filepath):
		# comment header
		
		command_line = sys.argv[0]
		arguments = sys.argv[1:]
		for a in arguments:
			command_line += " " + a
		
		d = datetime.datetime.now()
		
		header = libs.header.Header(filepath)
		header.writeLine("/*")
		header.indent()
		header.writeLine("DO NOT MODIFY, AUTO-GENERATED")
		header.newLine()
		header.writeLine("Generated on:")
		header.indent()
		header.writeLine(d.strftime("%Y-%m-%dT%H:%M:%S"))
		header.outdent()
		header.newLine()
		header.writeLine("Command line:")
		header.indent()
		header.writeLine(command_line)
		header.outdent()
		header.outdent()
		header.writeLine("*/")
		header.newLine()
		
		# includes
		
		header.writeLine("#include \"normalization.h\"")
		header.newLine()
		
		# composition data
		
		"""
		header.writeLine("static const size_t CompositionDataCount = " + str(len(self.entries)) + ";")
		header.writeLine("static const CompositionEntry CompositionData[" + str(len(self.entries)) + "] = {")
		header.indent()
		for e in self.entries:
			header.writeLine(e.toHeaderString()) 
		header.outdent()
		header.writeLine("};")
		header.newLine()
		"""
		
		# decomposition data
		
		blob_sliced = self.blob
		
		pages = int(math.floor(float(self.offset) / self.pageSize) + 1)
		blob_size = self.offset
		
		header.writeLine("const size_t DecompositionDataPageCount = " + str(pages) + ";")
		header.writeLine("const char* DecompositionData[" + str(pages) + "] = {")
		header.indent()
		
		blob_count = len(re.findall('\\\\x?[^\\\\]+', self.blob))
		print "characterTotal " + str(blob_count)
		
		for p in range(0, pages):
			current_page_size = min(blob_size, self.pageSize)
			print "currentPageSize " + str(current_page_size)
			
			read = current_page_size
			written = 0
			
			first_line = True
			
			while (1):
				character_find_count = min(read, 25)
				
				header.writeIndentation()
				
				character_find = "(\\\\x?[^\\\\]+){" + str(character_find_count) + "}"
				
				character_matches = re.match(character_find, blob_sliced)
				if character_matches:
					header.write("\"")
					if first_line:
						header.write("\\0")
						first_line = False
					header.write(character_matches.group(0))
					header.write("\"")
					
					written += character_find_count
					blob_sliced = blob_sliced[character_matches.end():]
				else:
					print "failed to find " + character_find
					break
				
				read -= character_find_count
				if read <= 0:
					header.write(",")
					header.newLine()
					break
				
				header.newLine()
			
			blob_size -= current_page_size
		
		header.outdent()
		header.writeLine("};")
		
		"""
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
		"""
		
		header.close()
		
		print "entries " + str(self.total) + " hashed " + str(len(self.hashed))

if __name__ == '__main__':
	parser = argparse.ArgumentParser(description='Converts Unicode data files.')
	parser.add_argument(
		'--verbove', '-v',
		dest = 'verbose',
		action = 'store_true',
		help = 'verbose output'
	)
	parser.add_argument(
		'--line-limit', '-l',
		dest = 'lineLimit',
		type = int,
		help = 'limit the amount of lines read'
	)
	parser.add_argument(
		'--entry-limit', '-e',
		dest = 'entryLimit',
		type = int,
		help = 'limit the amount of entries parsed'
	)
	parser.add_argument(
		'--no-hangul',
		dest = 'hangul',
		default = True,
		action = 'store_false',
		help = 'remove Hangul codepoints from output'
	)
	parser.add_argument(
		'--page-size', '-p',
		dest = 'pageSize',
		default = 32767,
		type = int,
		help = 'maximum page size for written strings'
	)
	args = parser.parse_args()
	
	script_path = os.path.dirname(os.path.realpath(sys.argv[0]))

	document = libs.unicode.UnicodeDocument()
	document.lineLimit = args.lineLimit
	document.entryLimit = args.entryLimit
	document.parse(script_path + '/data/NormalizationTest.txt')
	
	printer = Normalization()
	printer.verbose = args.verbose
	printer.ignoreHangul = not args.hangul
	printer.pageSize = args.pageSize
	document.accept(printer)
	printer.writeSource(script_path + '/../../source/normalizationdata.c')