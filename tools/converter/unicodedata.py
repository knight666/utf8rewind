import argparse
import os
import sys
import libs.header
import libs.unicode
import libs.utf8

class UnicodeMapping:
	def __init__(self):
		self.codepoint = 0
		self.generalCategory = ""
	
	def setGeneralCategory(self, value):
		mapping = {
			"Lu": "GeneralCategory_UppercaseLetter",
			"Ll": "GeneralCategory_LowercaseLetter",
			"Lt": "GeneralCategory_TitlecaseLetter",
			"Lm": "GeneralCategory_ModifierLetter",
			"Lo": "GeneralCategory_OtherLetter",
			"Mn": "GeneralCategory_NonspacingMark",
			"Mc": "GeneralCategory_SpacingMark",
			"Me": "GeneralCategory_EnclosingMark",
			"Nd": "GeneralCategory_DecimalNumber",
			"Nl": "GeneralCategory_LetterNumber",
			"No": "GeneralCategory_OtherNumber",
			"Pc": "GeneralCategory_ConnectorPunctuation",
			"Pd": "GeneralCategory_DashPunctuation",
			"Ps": "GeneralCategory_OpenPunctuation",
			"Pe": "GeneralCategory_ClosePunctuation",
			"Pi": "GeneralCategory_InitialPunctuation",
			"Pf": "GeneralCategory_FinalPunctuation",
			"Po": "GeneralCategory_OtherPunctuation",
			"Sm": "GeneralCategory_MathSymbol",
			"Sc": "GeneralCategory_CurrencySymbol",
			"Sk": "GeneralCategory_ModifierSymbol",
			"So": "GeneralCategory_OtherSymbol",
			"Zs": "GeneralCategory_SpaceSeparator",
			"Zl": "GeneralCategory_LineSeparator",
			"Zp": "GeneralCategory_ParagraphSeparator",
			"Cc": "GeneralCategory_Control",
			"Cf": "GeneralCategory_Format",
			"Cs": "GeneralCategory_Surrogate",
			"Co": "GeneralCategory_PrivateUse",
			"Cn": "GeneralCategory_Unassigned"
		}
		self.generalCategory = mapping[value]
	
	def __str__(self):
		return "{ codepoint: " + hex(self.codepoint) + ", generalCategory: " + self.generalCategory + " }"

class Database(libs.unicode.UnicodeVisitor):
	def __init__(self):
		pass
	
	def visitEntry(self, entry):
		if not entry.matches[0]:
			return False
		
		u = UnicodeMapping()
		u.codepoint = int(entry.matches[0][0], 16)
		u.setGeneralCategory(entry.matches[2][0])
		for e in entry.matches:
			print e
		
		print u
		
		return True

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
	args = parser.parse_args()
	
	script_path = os.path.dirname(os.path.realpath(sys.argv[0]))

	document = libs.unicode.UnicodeDocument()
	document.lineLimit = args.lineLimit
	document.entryLimit = args.entryLimit
	document.parse(script_path + '/data/UnicodeData.txt')
	
	db = Database()
	document.accept(db)