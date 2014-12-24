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
	
	def close(self):
		self.header.close()
	
	def execute(self):
		pass

class CaseMappingIntegrationSuite(IntegrationSuite):
	def execute(self):
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
		
		self.open('/../../source/tests/integration-casemapping.cpp')
		
		self.header.write("#include \"helpers-casemapping.hpp\"")
		
		for b in valid_blocks:
			self.writeTest(range(b.start, b.end + 1), b.name)
		
		self.close()
	
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

class NormalizationEntry:
	def __init__(self):
		self.codepoint = 0
		self.source = ""
		self.nfc = ""
		self.nfd = ""
		self.nfkc = ""
		self.nfkd = ""
	
	def __str__(self):
		return "{ codepoint " + hex(self.codepoint) + " source " + self.source + " nfc " + self.nfc + " nfd " + self.nfd + " nfkc " + self.nfkc + " nfkd " + self.nfkd + " }";
	
	def parse(self, entry):
		if len(entry.matches[0]) == 1:
			self.codepoint = int(entry.matches[0][0], 16)
		self.source = self.matchToString(entry.matches[0])
		self.nfc = self.matchToString(entry.matches[1])
		self.nfd = self.matchToString(entry.matches[2])
		self.nfkc = self.matchToString(entry.matches[3])
		self.nfkd = self.matchToString(entry.matches[4])
	
	def matchToString(self, match):
		codepoints = []
		
		for m in match:
			codepoints.append(int(m, 16))
		
		return libs.utf8.unicodeToUtf8(codepoints)

class NormalizationGroup:
	def __init__(self, block):
		self.block = block
		self.entries = []

class NormalizationSection:
	def __init__(self, title):
		self.title = title
		self.entries = []

class NormalizationIntegrationSuite(IntegrationSuite):
	def __init__(self, db):
		self.db = db
		self.current = None
		self.sections = []
		self.blockGroups = dict()
	
	def execute(self):
		script_path = os.path.dirname(os.path.realpath(sys.argv[0]))
		
		document_normalization = libs.unicode.UnicodeDocument()
		document_normalization.parse(script_path + '/data/NormalizationTest.txt')
		document_normalization.accept(self)
		
		groups = dict()
		
		for u in self.sections[1].entries:
			e = self.db.records[u.codepoint]
			if e.block.name in groups:
				group = groups[e.block.name]
			else:
				group = NormalizationGroup(e.block)
				groups[e.block.name] = group
			group.entries.append(u)
		
		self.blockGroups = sorted(groups.iteritems(), key=lambda item: item[1].block.start)
		
		self.open('/../../source/tests/integration-normalization.cpp')
		
		self.header.writeLine("#include \"helpers-normalization.hpp\"")
		self.header.write("#include \"helpers-strings.hpp\"")
		
		# block group tests
		
		print self.sections[1].title + ":"
		
		for g in self.blockGroups:
			self.writeTest(g[1].entries, g[0], False)
			self.writeTest(g[1].entries, g[0], True)
		
		# others
		
		for s in self.sections:
			if s <> self.sections[1]:
				self.writeSection(s)
		
		self.close()
	
	def writeSection(self, section):
		if len(section.entries) == 0:
			return
		
		self.writeTest(section.entries, section.title, False)
		self.writeTest(section.entries, section.title, True)
	
	def writeTest(self, entries, title, compatibility):
		compiler_limit = 2000
		if len(entries) > compiler_limit:
			for i in xrange(0, len(entries), compiler_limit):
				chunk = entries[i:i + compiler_limit]
				self.writeTest(chunk, title + " Part" + str((i / compiler_limit) + 1), compatibility)
			return
		
		title = re.sub('[^\w ]', '', title.title()).replace(' ', '')
		
		if compatibility:
			title = "Compatibility" + title
		else:
			title = "Regular" + title
		
		print "Writing tests \"" + title + "\""
		
		self.header.newLine()
		
		self.header.newLine()
		self.header.writeLine("TEST(Normalization, " + title + ")")
		self.header.writeLine("{")
		self.header.indent()
		
		for e in entries:
			self.header.writeIndentation()
			if compatibility:
				self.header.write("CHECK_NORMALIZE_COMPATIBILITY")
			else:
				self.header.write("CHECK_NORMALIZE")
			self.header.write("(0x" + format(e.codepoint, '08X') + ", \"" + e.nfd + "\", \"" + e.nfc + "\", \"" + self.db.records[e.codepoint].name + "\");")
			self.header.newLine()
		
		self.header.outdent()
		self.header.write("}")
	
	def visitDocument(self, document):
		print "Parsing normalization tests..."
		return True
	
	def visitSection(self, section):
		title = re.sub('[^\w ]', '', section.title).title()
		title = title.replace(' ', '')
		
		self.current = NormalizationSection(section.title)
		self.sections.append(self.current)
		
		return True
	
	def visitEntry(self, entry):
		normalization = NormalizationEntry()
		normalization.parse(entry)
		if normalization.codepoint <> 0:
			self.current.entries.append(normalization)
		
		return True

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
		suite = CaseMappingIntegrationSuite(db)
		suite.execute()
	
	if all or args.normalization:
		suite = NormalizationIntegrationSuite(db)
		suite.execute()