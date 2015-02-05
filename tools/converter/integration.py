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
		print "Executing case mapping tests..."
		
		valid_blocks = []
		
		print "Checking for valid blocks..."
		
		for b in db.blocks:
			for u in range(b.start, b.end + 1):
				if u in db.records:
					r = db.records[u]
					if r.uppercase or r.lowercase or r.titlecase:
						valid_blocks.append(b)
						break
		
		print "Writing case mapping tests..."
		
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
		self.header.writeLine("TEST(CaseMapping, " + name + ")")
		self.header.writeLine("{")
		self.header.indent()
		
		for r in records:
			converted_codepoint = "0x%08X" % r.codepoint
			
			if r.uppercase:
				converted_uppercase = libs.utf8.unicodeToUtf8(r.uppercase)
			else:
				converted_uppercase = libs.utf8.codepointToUtf8(r.codepoint)[0]
				
			if r.lowercase:
				converted_lowercase = libs.utf8.unicodeToUtf8(r.lowercase)
			else:
				converted_lowercase = libs.utf8.codepointToUtf8(r.codepoint)[0]
			
			if r.titlecase:
				converted_titlecase = libs.utf8.unicodeToUtf8(r.titlecase)
			else:
				converted_titlecase = libs.utf8.codepointToUtf8(r.codepoint)[0]
			
			self.header.writeLine("CHECK_UTF8_CASEMAPPING(" + converted_codepoint + ", \"" + converted_uppercase + "\", \"" + converted_lowercase + "\", \"" + converted_titlecase + "\", \"" + r.name + "\");")
		
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
	def __init__(self, identifier, title):
		self.identifier = identifier
		self.title = title
		self.entries = []

class NormalizationIntegrationSuite(IntegrationSuite):
	def __init__(self, db):
		self.db = db
		self.current = None
		self.sections = []
		self.blockGroups = dict()
		self.exclusions = CompositionExclusionIntegrationSuite(db)
	
	def execute(self):
		print "Executing normalization tests..."
		
		script_path = os.path.dirname(os.path.realpath(sys.argv[0]))
		
		document_normalization = libs.unicode.UnicodeDocument()
		document_normalization.parse(script_path + '/data/NormalizationTest.txt')
		document_normalization.accept(self)
		
		self.exclusions.execute()
		
		print "Writing normalization tests..."
		
		self.open('/../../source/tests/integration-normalization.cpp')
		
		self.header.writeLine("#include \"helpers-normalization.hpp\"")
		self.header.write("#include \"helpers-strings.hpp\"")
		
		section_mapping = {
			'Part0': self.writeSpecificCasesSection,
			'Part1': self.writeBlockGroupsSection,
			'Part2': self.writeDefaultSection,
			'Part3': self.writeDefaultSection,
		}
		for s in self.sections:
			print s.title + " (" + s.identifier + "):"
			section_mapping[s.identifier](s)
		
		self.writeNormalizationTest(self.exclusions.entries, "Composition exclusions", 100)
		
		self.close()
	
	def writeDefaultSection(self, section):
		if len(section.entries) == 0:
			return
		
		self.writeNormalizationTest(section.entries, section.title, 100)
	
	def writeSpecificCasesSection(self, section):
		if len(section.entries) == 0:
			return
		
		normalization = []
		ordering = []
		
		for e in section.entries:
			if e.codepoint == 0:
				ordering.append(e)
			else:
				normalization.append(e)
		
		self.writeNormalizationTest(normalization, section.title + " Codepoints")
		self.writeNormalizationTest(ordering, section.title + " Ordering")
	
	def writeBlockGroupsSection(self, section):
		groups = dict()
		
		for u in section.entries:
			e = self.db.records[u.codepoint]
			if e.block.name in groups:
				group = groups[e.block.name]
			else:
				group = NormalizationGroup(e.block)
				groups[e.block.name] = group
			group.entries.append(u)
		
		block_groups = sorted(groups.iteritems(), key = lambda item: item[1].block.start)
		
		for g in block_groups:
			if g[1].block.start == 0xAC00 and g[1].block.end == 0xD7AF:
				# ignore hangul syllables
				continue
			
			self.writeNormalizationTest(g[1].entries, "Characters " + g[0])
	
	def writeNormalizationTest(self, entries, title, limit = 2000):
		if len(entries) > limit:
			for i in xrange(0, len(entries), limit):
				chunk = entries[i:i + limit]
				self.writeNormalizationTest(chunk, title + " Part" + str((i / limit) + 1), limit)
			return
		
		title = re.sub('[^\w ]', '', title.title()).replace(' ', '')
		
		print "Writing tests \"" + title + "\""
		
		self.header.newLine()
		
		self.header.newLine()
		self.header.writeLine("TEST(Normalization, " + title + ")")
		self.header.writeLine("{")
		self.header.indent()
		
		for e in entries:
			self.header.writeIndentation()
			
			if e.codepoint == 0:
				self.header.write("CHECK_NORMALIZE_SEQUENCE")
				self.header.write("(\"" + e.source + "\", \"" + e.nfd + "\", \"" + e.nfc + "\", \"" + e.nfkd + "\", \"" + e.nfkc + "\");")
			else:
				self.header.write("CHECK_NORMALIZE_CODEPOINT")
				self.header.write("(0x" + format(e.codepoint, '08X') + ", \"" + e.nfd + "\", \"" + e.nfc + "\", \"" + e.nfkd + "\", \"" + e.nfkc + "\", \"" + self.db.records[e.codepoint].name + "\");")
			
			self.header.newLine()
		
		self.header.outdent()
		self.header.write("}")
	
	def visitDocument(self, document):
		print "Parsing normalization tests..."
		return True
	
	def visitSection(self, section):
		self.current = NormalizationSection(section.identifier, section.title)
		self.sections.append(self.current)
		
		return True
	
	def visitEntry(self, entry):
		normalization = NormalizationEntry()
		normalization.parse(entry)
		self.current.entries.append(normalization)
		
		return True

class CompositionExclusionIntegrationSuite(IntegrationSuite):
	def __init__(self, db):
		self.db = db
		self.entries = []
	
	def execute(self):
		print "Executing composition exclusion tests..."
		
		script_path = os.path.dirname(os.path.realpath(sys.argv[0]))
		
		document_exclusions = libs.unicode.UnicodeDocument()
		document_exclusions.parse(script_path + '/data/CompositionExclusions.txt')
		document_exclusions.accept(self)
	
	def visitDocument(self, document):
		return True
	
	def visitSection(self, section):
		return True
	
	def visitEntry(self, entry):
		codepoint = int(entry.matches[0][0], 16)
		if codepoint in self.db.records:
			record = db.records[codepoint]
			
			entry = NormalizationEntry()
			entry.codepoint = record.codepoint
			entry.source = libs.utf8.codepointToUtf8Hex(record.codepoint)
			entry.nfd = libs.utf8.unicodeToUtf8(record.decomposedNFD)
			entry.nfc = entry.nfd
			entry.nfkd = libs.utf8.unicodeToUtf8(record.decomposedNFD)
			entry.nfkc = entry.nfkd
			
			self.entries.append(entry)
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