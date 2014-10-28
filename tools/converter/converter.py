import argparse
import libs.unicode

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
	
	printer = Printer()
	normalization.accept(printer)