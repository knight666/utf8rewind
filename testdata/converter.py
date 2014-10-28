import re

section_search = re.compile('@(\w+) ?# ?(.+)')
group_search = re.compile(' ?([0-9A-Za-z]+) ?([0-9A-Za-z]+)? ?([0-9A-Za-z]+)? ?([0-9A-Za-z]+)? ?([0-9A-Za-z]+)? ?([0-9A-Za-z]+)? ?([0-9A-Za-z]+)? ?([0-9A-Za-z]+)?;')

def WriteGroup(file, matches, last):
	file.write('{ ')
	count = 0
	if matches:
		for group in matches.groups():
			if group <> None:
				file.write('0x' + group)
			else:
				file.write('0')
			if count <> 3:
				file.write(',')
			count += 1
			file.write(' ')
	else:
		file.write('0, 0, 0, 0 ')
	
	if last:
		converted.write('} ')
	else:
		converted.write('}, ')

class Header:
	def __init__(self, filename):
		self.file = open(filename, 'w+')
	
	def close(self):
		close(self.file)

class UnicodeEntry:
	def __init__(self):
		self.matches = []
		self.comment = ""
	
	def accept(self, visitor):
		visitor.visitEntry(self)

class UnicodeSection:
	def __init__(self):
		self.identifier = ""
		self.title = ""
		self.entries = []
	
	def accept(self, visitor):
		visitor.visitSection(self)
		for e in self.entries:
			e.accept(visitor)

class UnicodeDocument:
	def __init__(self):
		self.limiter = -1
		self.filename = ""
		self.sections = []
	
	def parse(self, filename):
		self.filename = filename
		
		section_current = UnicodeSection()
		self.sections = []
		self.sections.append(section_current)
		
		sections_found = 0
		
		with open(filename, 'r') as f:
			for line in f:
				if len(line) > 0 and not line.startswith('\n') and not line.startswith('#'):
					stripped = line.rstrip('\n')
					
					# section
					
					section_match = re.match(section_search, stripped)
					if section_match:
						if sections_found > 0:
							section_current = UnicodeSection()
							self.sections.append(section_current)
						
						section_current.identifier = section_match.group(1)
						section_current.title = section_match.group(2)
						sections_found += 1
						
						continue
					
					# entry
					
					entry = UnicodeEntry()
					while (1):
						matches = re.match(group_search, stripped)
						if not matches:
							break
						entry.matches.append(matches)
						stripped = stripped[matches.end():]
					
					# comment
					
					comment_match = re.match(' *# (.+)', stripped)
					if comment_match:
						entry.comment = comment_match.group(1)
					
					section_current.entries.append(entry)
					
					if self.limiter > -1:
						self.limiter -= 1
						if self.limiter == 0:
							break
		
		return True
	
	def accept(self, visitor):
		visitor.visitDocument(self)
		for s in self.sections:
			s.accept(visitor)

class UnicodeVisitor:
	def visitDocument(self, document):
		pass
	
	def visitSection(self, section):
		pass
	
	def visitEntry(self, entry):
		pass

class Printer(UnicodeVisitor):
	def visitDocument(self, document):
		print document.filename
	
	def visitSection(self, section):
		print " - " + section.title
	
	def visitEntry(self, entry):
		print " - - " + entry.matches[0].group(1)

if __name__ == '__main__':
	normalization = UnicodeDocument()
	normalization.limiter = 100
	normalization.parse('NormalizationTest.txt')
	
	printer = Printer()
	normalization.accept(printer)
	
	exit(0)
	
	with open('converted.h', 'w+') as converted:
		with open('SpecialCasing.txt', 'r') as f:
			for line in f:
				if len(line) > 0 and not line.startswith('\n') and not line.startswith('#'):
					stripped = line.rstrip('\n')
					
					# codepoint
					
					codepoint_matches = re.match(group_search, stripped)
					if not codepoint_matches:
						break
					stripped = stripped[codepoint_matches.end():]
					
					# lowercase
					
					lowercase_matches = re.match(group_search, stripped)
					if lowercase_matches:
						stripped = stripped[lowercase_matches.end():]
					else:
						stripped = stripped[stripped.find(';'):]
					
					# titlecase
					
					titlecase_matches = re.match(group_search, stripped)
					if titlecase_matches:
						stripped = stripped[titlecase_matches.end():]
					else:
						stripped = stripped[stripped.find(';'):]
					
					# uppercase
					
					uppercase_matches = re.match(group_search, stripped)
					if uppercase_matches:
						stripped = stripped[uppercase_matches.end():]
					else:
						stripped = stripped[stripped.find(';'):]
					
					# conditional
					
					conditional_match = re.match('(.+);', stripped)
					if conditional_match:
						stripped = stripped[conditional_match.end():]
					
					# comment
					
					comment_matches = re.match(' *# ([\w ]+)', stripped)
					
					# write
					
					if not conditional_match:
						converted.write('{ ')
						converted.write('0x' + codepoint_matches.group(1) + ', ')
						
						WriteGroup(converted, lowercase_matches, False)
						WriteGroup(converted, titlecase_matches, False)
						WriteGroup(converted, uppercase_matches, True)
						
						converted.write('}, ')
						converted.write('/* ' + comment_matches.group(1) + ' */\n')