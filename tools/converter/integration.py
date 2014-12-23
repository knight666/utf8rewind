import argparse
import datetime
import os
import re
import sys
import unicodedata
import libs.header
import libs.unicode
import libs.utf8

def codepointToUnicode(codepoint):
	return "U+%04X" % codepoint

class IntegrationSuite:
	def __init__(self, db):
		self.db = db
	
	def open(self, filepath):
		command_line = sys.argv[0]
		arguments = sys.argv[1:]
		for a in arguments:
			command_line += " " + a
		
		d = datetime.datetime.now()
		
		script_path = os.path.dirname(os.path.realpath(sys.argv[0]))
		
		self.header = libs.header.Header(script_path + filepath)
		self.header.writeLine("/*")
		self.header.indent()
		self.header.writeLine("DO NOT MODIFY, AUTO-GENERATED")
		self.header.newLine()
		self.header.writeLine("Generated on:")
		self.header.indent()
		self.header.writeLine(d.strftime("%Y-%m-%dT%H:%M:%S"))
		self.header.outdent()
		self.header.newLine()
		self.header.writeLine("Command line:")
		self.header.indent()
		self.header.writeLine(command_line)
		self.header.outdent()
		self.header.outdent()
		self.header.writeLine("*/")
		self.header.newLine()
		self.header.writeLine("#include \"tests-base.hpp\"")
		self.header.newLine()
		self.header.write("#include \"helpers-casemapping.hpp\"")
	
	def close(self):
		self.header.close()
	
	def writeTest(self, codepointRange, name):
		name = re.sub('[ \-]', '', name)
		
		if len(codepointRange) > 4000:
			for i in xrange(0, len(codepointRange), 4000):
				chunk = codepointRange[i:i + 4000]
				self.writeTest(chunk, name + "Part" + str((i / 4000) + 1))
			return
		
		records = []
		
		for i in codepointRange:
			if i not in self.db.records:
				continue
			
			records.append(self.db.records[i])
		
		if len(records) == 0:
			return
		
		print "Writing tests " + codepointToUnicode(codepointRange[0]) + " - " + codepointToUnicode(codepointRange[len(codepointRange) - 1]) + " \"" + name + "\""
		
		self.header.newLine()
		
		self.header.newLine()
		self.header.writeLine("TEST(CaseMapping, Uppercase" + name + ")")
		self.header.writeLine("{")
		self.header.indent()
		
		for r in records:
			converted_codepoint = "0x%08x" % r.codepoint
			
			if r.uppercase:
				converted_uppercase = ""
				for u in r.uppercase:
					converted_uppercase += libs.utf8.codepointToUtf8(u)
			else:
				converted_uppercase = libs.utf8.codepointToUtf8(r.codepoint)
			
			self.header.writeLine("CHECK_UTF8_UPPERCASE(" + converted_codepoint + ", \"" + r.name + "\", \"" + converted_uppercase + "\");")
		
		self.header.outdent()
		self.header.writeLine("}")
		
		self.header.newLine()
		
		self.header.writeLine("TEST(CaseMapping, Lowercase" + name + ")")
		self.header.writeLine("{")
		self.header.indent()
		
		for r in records:
			converted_codepoint = "0x%08x" % r.codepoint
			
			if r.lowercase:
				converted_lowercase = ""
				for u in r.lowercase:
					converted_lowercase += libs.utf8.codepointToUtf8(u)
			else:
				converted_lowercase = libs.utf8.codepointToUtf8(r.codepoint)
			
			self.header.writeLine("CHECK_UTF8_LOWERCASE(" + converted_codepoint + ", \"" + r.name + "\", \"" + converted_lowercase + "\");")
		
		self.header.outdent()
		self.header.write("}")

if __name__ == '__main__':
	parser = argparse.ArgumentParser(description='Parse Unicode codepoint database and write integration tests.')
	parser.add_argument(
		'-v', '--verbove',
		dest = 'verbose',
		action = 'store_true',
		help = 'verbose output'
	)
	parser.add_argument(
		'--casemapping',
		dest = 'casemapping',
		action = 'store_true',
		help = 'write case mapping tests'
	)
	parser.add_argument(
		'--normalization',
		dest = 'normalization',
		action = 'store_true',
		help = 'write normalization tests'
	)
	args = parser.parse_args()
	
	if not args.casemapping and not args.normalization:
		all = True
	else:
		all = False
	
	db = unicodedata.Database()
	db.loadFromFiles(None)
	
	if all or args.casemapping:
		suite = IntegrationSuite(db)
		suite.open('/../../source/tests/integration-casemapping.cpp')
		
		valid_blocks = []
		
		print "Checking for valid blocks..."
		
		for b in db.blocks:
			for u in range(b.start, b.end + 1):
				if u in db.records:
					r = db.records[u]
					if r.uppercase or r.lowercase:
						valid_blocks.append(b)
						break
		
		print "Writing tests..."
		
		for b in valid_blocks:
			suite.writeTest(range(b.start, b.end + 1), b.name)
		
		suite.close()
	