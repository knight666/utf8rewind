import re

class UnicodeEntry:
	def __init__(self):
		self.lineNumber = 0
		self.matches = []
		self.comment = ""
	
	def accept(self, visitor):
		return visitor.visitEntry(self)

class UnicodeSection:
	def __init__(self):
		self.identifier = ""
		self.title = ""
		self.entries = []
	
	def accept(self, visitor, limit):
		if not visitor.visitSection(self):
			return False
		for e in self.entries:
			result = e.accept(visitor)
			if not result:
				return False
			elif limit:
				limit -= 1
				if limit == 0:
					return True
		return True

class UnicodeDocument:
	def __init__(self):
		self.lineLimit = None
		self.entryLimit = None
		self.filename = ""
		self.sections = []
	
	def parse(self, filename):
		self.filename = filename
		
		section_search = re.compile('@(\w+) ?# ?(.+)')
		comment_search = re.compile('.*# (.+)')
		
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
				if not self.lineLimit:
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
					entry.lineNumber = line_count
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
					
					if self.lineLimit <> None:
						self.lineLimit -= 1
						if self.lineLimit == 0:
							break
		
		return True
	
	def accept(self, visitor):
		if not visitor.visitDocument(self):
			return False
		for s in self.sections:
			if not s.accept(visitor, self.entryLimit):
				return False
		return True

class UnicodeVisitor:
	def visitDocument(self, document):
		return True
	
	def visitSection(self, section):
		return True
	
	def visitEntry(self, entry):
		return True