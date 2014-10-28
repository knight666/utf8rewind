import re

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
		self.limiter = None
		self.filename = ""
		self.sections = []
	
	def parse(self, filename):
		self.filename = filename
		
		section_search = re.compile('@(\w+) ?# ?(.+)')
		comment_search = re.compile(' *# (.+)')
		
		section_current = UnicodeSection()
		self.sections = []
		self.sections.append(section_current)
		
		sections_found = 0
		
		line_count = 0
		
		with open(filename, 'r') as f:
			lines = f.readlines()
			lines_total = float(len(lines))
			for line in lines:
				line_count += 1
				if not self.limiter:
					if (line_count % 250) == 0:
						print format(float(line_count) / lines_total * 100.0, ".2f") + "%"
					
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
					
					comment_start = stripped.find('#')
					entry_sliced = stripped[:comment_start]
					
					entry = UnicodeEntry()
					while (1):
						section_end = entry_sliced.find(';')
						sliced = entry_sliced[:section_end]
						matches = re.findall('([0-9A-Za-z]+)', sliced)
						if not matches:
							break
						entry.matches.append(matches)
						entry_sliced = entry_sliced[section_end + 1:]
					
					# comment
					
					comment_match = re.match(comment_search, stripped)
					if comment_match:
						entry.comment = comment_match.group(1)
					
					section_current.entries.append(entry)
					
					if self.limiter <> None:
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