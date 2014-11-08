import argparse
import datetime
import os
import re
import sys
import libs.header
import libs.unicode
import libs.utf8

class UnicodeMapping:
	def __init__(self, db):
		self.db = db
		self.clear()
	
	def __str__(self):
		return "{ codepoint: " + hex(self.codepoint) + ", name: \"" + self.name + "\" generalCategory: \"" + self.generalCategory + "\", canonicalCombiningClass: " + str(self.canonicalCombiningClass) + ", bidiClass: \"" + self.bidiClass + "\", decompositionType: \"" + self.decompositionType + "\", decomposition: \"" + self.decomposedToString() + "\", numericType: \"" + self.numericType + "\", numericValue: " + str(self.numericValue) + ", bidiMirrored: " + str(self.bidiMirrored) + " }"
	
	def clear(self):
		self.codepoint = 0
		self.name = ""
		self.generalCategory = ""
		self.canonicalCombiningClass = 0
		self.bidiClass = ""
		self.decompositionType = ""
		self.decompositionTranslated = ""
		self.decompositionCodepoints = []
		self.numericType = "NumericType_None"
		self.numericValue = 0
		self.bidiMirrored = False
	
	def decomposedToString(self):
		decomposedCodepoints = ""
		if self.decompositionCodepoints:
			decomposedCodepoints = hex(self.decompositionCodepoints[0])
			for c in self.decompositionCodepoints[1:]:
				decomposedCodepoints += " " + hex(c)
		return decomposedCodepoints
	
	def parse(self, matches):
		self.clear()
		
		if not matches[0]:
			return False
		
		# codepoint
		
		self.codepoint = int(matches[0][0], 16)
		
		# name
		
		self.name = matches[1][0]
		for w in matches[1][1:]:
			self.name += " " + w
		
		# general category
		
		generalCategoryMapping = {
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
		try:
			self.generalCategory = generalCategoryMapping[matches[2][0]]
		except:
			raise KeyError("Failed to find general category mapping for value \"" + matches[2][0] + "\"")
		
		# canonical combining class
		
		self.canonicalCombiningClass = int(matches[3][0])
		
		# bidi class
		
		bidiClassMapping = {
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
			"ON": "BidiClass_OtherNeutral",
			"LRI": "BidiClass_LeftToRightIsolate",
			"RLI": "BidiClass_RightToLeftIsolate",
			"FSI": "BidiClass_FirstStrongIsolate",
			"PDI": "BidiClass_PopDirectionalIsolate",
		}
		try:
			self.bidiClass = bidiClassMapping[matches[4][0]]
		except:
			raise KeyError("Failed to find bidi class mapping for value \"" + matches[4][0] + "\"")
		
		# decomposition mapping
		
		if not matches[5]:
			self.decompositionType = "DecompositionType_Canonical"
			self.decompositionMapping = 0
		else:
			decompositionTypeMapping = {
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
			if matches[5][0] in decompositionTypeMapping:
				self.decompositionType = decompositionTypeMapping[matches[5][0]]
				matches[5] = matches[5][1:]
			else:
				self.decompositionType = "DecompositionType_Canonical"
			for c in matches[5]:
				self.decompositionCodepoints.append(int(c, 16))
			self.decompositionTranslated = db.matchToString(matches[5])
			self.decompositionMapping = db.addTranslation(self.decompositionTranslated)
		
		# numerical value
		
		if not matches[6] and not matches[7] and not matches[8]:
			self.numericType = "NumericType_None"
			self.numericValue = 0
		elif matches[8]:
			if matches[7]:
				if matches[6]:
					self.numericType = "NumericType_Decimal"
				else:
					self.numericType = "NumericType_Digit"
				self.numericValue = int(matches[8][0])
			else:
				self.numericType = "NumericType_Numeric"
				value_found = re.match('([0-9]+)/([0-9]+)', matches[8][0])
				if value_found:
					self.numericValue = float(value_found.group(1)) / float(value_found.group(2))
		
		# bidi mirrored
		
		if matches[9][0] == "Y":
			self.bidiMirrored = True
		else:
			self.bidiMirrored = False
		
		return True
	
	def resolveCodepoint(self, compatibility):
		result = []
		
		if self.decompositionCodepoints and (compatibility or self.decompositionType == "DecompositionType_Canonical"):
			for c in self.decompositionCodepoints:
				if c in self.db.records:
					resolved = self.db.records[c].resolveCodepoint(compatibility)
					if resolved:
						result.append(resolved)
				else:
					result.append(c)
		else:
			result.append(self.codepoint)
		
		return result
	
	def decompose(self):
		self.decomposedNFD = self.resolveCodepoint(False)
		self.decomposedNFKD = self.resolveCodepoint(True)
		
		print "codepoint " + hex(self.codepoint)
		print "NFD: " + self.codepointsToString(self.decomposedNFD)
		print "NFKD: " + self.codepointsToString(self.decomposedNFKD)
	
	def codepointsToString(self, values):
		result = ""
		
		if values:
			result = hex(values[0])
			for v in values[1:]:
				result += " " + hex(v)
		
		return result
	
	def toSource(self):
		if self.bidiMirrored:
			bidiMirroredString = "1"
		else:
			bidiMirroredString = "0"
		
		return "{ " + hex(self.codepoint) + ", " + self.generalCategory + ", " + str(self.canonicalCombiningClass) + ", " + self.bidiClass + ", 0, 0, 0, 0, " + self.numericType + ", " + str(self.numericValue) + ", " + bidiMirroredString + " }"

class Database(libs.unicode.UnicodeVisitor):
	def __init__(self):
		self.verbose = False
		self.blob = ""
		self.total = 0
		self.offset = 1
		self.hashed = dict()
		self.records = dict()
	
	def visitDocument(self, document):
		print "Parsing document to codepoint database..."
		return True
	
	def visitEntry(self, entry):
		if not entry.matches[0]:
			return False
		
		u = UnicodeMapping(self)
		try:
			if not u.parse(entry.matches):
				return False
		except KeyError as e:
			print "Failed to parse entry - error: \"" + e + "\" line: " + str(entry.lineNumber)
			for m in entry.matches:
				print m
			return False
		
		self.records[u.codepoint] = u
		
		return True
	
	def resolve(self):
		for u in self.records:
			r = self.records[u]
			r.decompose()
	
	def resolveCodepoint(self, codepoint, compatibility):
		found = self.records[codepoint]
		if not found:
			return ""
		
		if found.decompositionType == "DecompositionType_Canonical":
			type = "Canonical"
		elif found.decompositionCodepoints:
			type = "Compatibility"
		else:
			type = "None"
		
		print found.name + " " + hex(found.codepoint) + " " + type + " \"" + found.decomposedToString() + "\""
		
		if found.decompositionCodepoints and (compatibility or found.decompositionType == "DecompositionType_Canonical"):
			result = ""
			for c in found.decompositionCodepoints:
				result += self.resolveCodepoint(c, compatibility) + " "
			return result
		else:
			return hex(codepoint)
	
	def executeQuery(self, query):
		if query == "":
			return
		
		queryCodepoint = int(query, 16)
		found = self.records[queryCodepoint]
		if found:
			print found
			print "Canonical:"
			print self.resolveCodepoint(queryCodepoint, False)
			print "Compatibility:"
			print self.resolveCodepoint(queryCodepoint, True)

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
	
	def writeSource(self, filepath):
		print "Writing database to " + filepath + "..."
		
		command_line = sys.argv[0]
		arguments = sys.argv[1:]
		for a in arguments:
			command_line += " " + a
		
		d = datetime.datetime.now()
		
		# comment header
		
		header = libs.header.Header(filepath)
		header.writeLine("/*")
		header.indent()
		header.writeLine("DO NOT MODIFY, AUTO-GENERATED")
		header.newLine()
		header.writeLine("Generated on:")
		header.indent()
		header.writeLine(d.strftime("%Y-%m-%dT%H:%M:%S"))
		header.outdent()
		header.newLine()
		header.writeLine("Command line:")
		header.indent()
		header.writeLine(command_line)
		header.outdent()
		header.outdent()
		header.writeLine("*/")
		header.newLine()
		
		# records
		
		header.writeLine("const size_t UnicodeRecordCount = " + str(len(self.records)) + ";")
		header.writeLine("const UnicodeRecord UnicodeRecordData[" + str(len(self.records)) + "] = {")
		
		header.indent()
		for u in self.records:
			r = self.records[u]
			header.writeLine(r.toSource())
		header.outdent()
		
		header.writeLine("};")
		header.writeLine("const UnicodeRecord* UnicodeRecordDataPtr = UnicodeRecordData;")
		header.newLine()

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
	parser.add_argument(
		'--query', '-q',
		dest = 'query',
		default = "",
		help = 'query a codepoint from the database'
	)
	args = parser.parse_args()
	
	script_path = os.path.dirname(os.path.realpath(sys.argv[0]))

	document = libs.unicode.UnicodeDocument()
	document.lineLimit = args.lineLimit
	document.entryLimit = args.entryLimit
	document.entrySkip = args.entrySkip
	document.parse(script_path + '/data/UnicodeData.txt')
	
	db = Database()
	document.accept(db)
	
	#db.resolve()
	db.executeQuery(args.query)
	db.writeSource(script_path + '/../../source/unicodedatabase.c')