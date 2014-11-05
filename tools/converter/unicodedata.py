import argparse
import os
import re
import sys
import libs.header
import libs.unicode
import libs.utf8

class UnicodeMapping:
	def __init__(self, db):
		self.db = db
		self.codepoint = 0
		self.generalCategory = ""
		self.canonicalCombiningClass = ""
		self.bidiClass = ""
		self.decompositionType = ""
		self.decompositionTranslated = ""
	
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
	
	def setCanonicalCombiningClass(self, value):
		mapping = {
			"0": "CanonicalCombiningClass_NotReordered",
			"1": "CanonicalCombiningClass_Overlay",
			"7": "CanonicalCombiningClass_Nukta",
			"8": "CanonicalCombiningClass_KanaVoicing",
			"9": "CanonicalCombiningClass_Virama",
			"10": "CanonicalCombiningClass_FixedPositionStart",
			"199": "CanonicalCombiningClass_FixedPositionEnd",
			"200": "CanonicalCombiningClass_AttachedBelowLeft",
			"202": "CanonicalCombiningClass_AttachedBelow",
			"204": "CanonicalCombiningClass_AttachedTopRight",
			"208": "CanonicalCombiningClass_AttachedLeft",
			"210": "CanonicalCombiningClass_AttachedRight",
			"212": "CanonicalCombiningClass_AttachedTopLeft",
			"214": "CanonicalCombiningClass_AttachedAbove",
			"216": "CanonicalCombiningClass_AttachedAboveRight",
			"218": "CanonicalCombiningClass_BelowLeft",
			"220": "CanonicalCombiningClass_Below",
			"222": "CanonicalCombiningClass_BelowRight",
			"224": "CanonicalCombiningClass_Left",
			"226": "CanonicalCombiningClass_Right",
			"228": "CanonicalCombiningClass_AboveLeft",
			"230": "CanonicalCombiningClass_Above",
			"232": "CanonicalCombiningClass_AboveRight",
			"233": "CanonicalCombiningClass_DoubleBelow",
			"234": "CanonicalCombiningClass_DoubleAbove",
			"240": "CanonicalCombiningClass_IotaSubscript"
		}
		self.canonicalCombiningClass = mapping[value]
	
	def setBidiClass(self, value):
		mapping = {
			"L": "BidiClass_LeftToRight",
			"LRE": "BidiClass_LeftToRightEmbedding",
			"LRO": "BidiClass_LeftToRightOverride",
			"R": "BidiClass_RightToLeft",
			"AL": "BidiClass_ArabicLetter",
			"RLE": "BidiClass_RightToLeftEmbedding",
			"RLO": "BidiClass_RightToLeftOverride",
			"PDF": "BidiClass_PopDirectionalFormat",
			"EN": "BidiClass_EuropeanNumber",
			"ES": "BidiClass_EuropeanSeparator",
			"ET": "BidiClass_EuropeanTerminator",
			"AN": "BidiClass_ArabicNumber",
			"CS": "BidiClass_CommonSeparator",
			"NSM": "BidiClass_NonspacingMark",
			"BN": "BidiClass_BoundaryNeutral",
			"B": "BidiClass_ParagraphSeparator",
			"S": "BidiClass_SegmentSeparator",
			"WS": "BidiClass_WhiteSpace",
			"ON": "BidiClass_OtherNeutral"
		}
		self.bidiClass = mapping[value]
	
	def setDecompositionMapping(self, value):
		if not value:
			self.decompositionType = "DecompositionType_Canonical"
			self.decompositionMapping = 0
		else:
			mapping = {
				"<font>": "DecompositionType_Font",
				"<noBreak>": "DecompositionType_NoBreak",
				"<initial>": "DecompositionType_InitialArabic",
				"<medial>": "DecompositionType_MedialArabic",
				"<final>": "DecompositionType_FinalArabic",
				"<isolated>": "DecompositionType_IsolatedArabic",
				"<circle>": "DecompositionType_Circle",
				"<super>": "DecompositionType_Superscript",
				"<sub>": "DecompositionType_Subscript",
				"<vertical>": "DecompositionType_Vertical",
				"<wide>": "DecompositionType_Wide",
				"<narrow>": "DecompositionType_Narrow",
				"<small>": "DecompositionType_Small",
				"<square>": "DecompositionType_SquaredCJK",
				"<fraction>": "DecompositionType_Fraction",
				"<compat>": "DecompositionType_Unspecified"
			}
			if value[0] in mapping:
				self.decompositionType = mapping[value[0]]
				value = value[1:]
			else:
				self.decompositionType = "DecompositionType_Canonical"
			self.decompositionTranslated = db.matchToString(value)
			self.decompositionMapping = db.addTranslation(self.decompositionTranslated)
	
	def __str__(self):
		return "{ codepoint: " + hex(self.codepoint) + ", generalCategory: " + self.generalCategory + ", canonicalCombiningClass: " + self.canonicalCombiningClass+ ", bidiClass: " + self.bidiClass + ", decompositionType: " + self.decompositionType + ", decompositionTranslated: " + self.decompositionTranslated + " }"

class Database(libs.unicode.UnicodeVisitor):
	def __init__(self):
		self.verbose = False
		self.blob = ""
		self.total = 0
		self.offset = 1
		self.hashed = dict()
	
	def visitEntry(self, entry):
		if not entry.matches[0]:
			return False
		
		u = UnicodeMapping(self)
		u.codepoint = int(entry.matches[0][0], 16)
		u.setGeneralCategory(entry.matches[2][0])
		u.setCanonicalCombiningClass(entry.matches[3][0])
		u.setBidiClass(entry.matches[4][0])
		u.setDecompositionMapping(entry.matches[5])
		
		if u.decompositionTranslated <> "":
			for e in entry.matches:
				print e
			print u
		
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
		result += "\\x00"
		
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
			print "translated", translation, "offset", result
		
		self.total += 1
		
		return result

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
		'--entry-skip', '-s',
		dest = 'entrySkip',
		default = 0,
		type = int,
		help = 'start offset for entries'
	)
	args = parser.parse_args()
	
	script_path = os.path.dirname(os.path.realpath(sys.argv[0]))

	document = libs.unicode.UnicodeDocument()
	document.lineLimit = args.lineLimit
	document.entryLimit = args.entryLimit
	document.entrySkip = args.entrySkip
	document.parse(script_path + '/data/UnicodeData.txt')
	
	db = Database()
	db.entrySkip = args.entrySkip
	document.accept(db)