import re

group_search = re.compile(' ?([0-9A-Za-z]+) ?([0-9A-Za-z]+)? ?([0-9A-Za-z]+)? ?([0-9A-Za-z]+)?;')

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

class CaseMapping:
	def __init__(self):
		self.codepoint = 0
		self.lowercase = []
		self.titlecase = []
		self.uppercase = []
		self.name = ""

if __name__ == '__main__':
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