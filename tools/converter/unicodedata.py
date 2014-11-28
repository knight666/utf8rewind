import argparse
import datetime
import os
import re
import sys
import libs.blobsplitter
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
		self.decompositionCodepoints = []
		self.decomposedNFD = []
		self.decomposedNFKD = []
		self.compositionPairs = dict()
		self.offsetNFC = 0
		self.offsetNFD = 0
		self.offsetNFKC = 0
		self.offsetNFKD = 0
		self.numericType = "NumericType_None"
		self.numericValue = 0
		self.bidiMirrored = False
		self.uppercase = []
		self.lowercase = []
		self.titlecase = []
		self.offsetUppercase = 0
		self.offsetLowercase = 0
		self.offsetTitlecase = 0
	
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
		
		# case mapping
		
		if matches[12]:
			self.uppercase.append(int(matches[12][0], 16))
		
		if matches[13]:
			self.lowercase.append(int(matches[13][0], 16))
		
		if len(matches) >= 15 and matches[14]:
			self.titlecase.append(int(matches[14][0], 16))
		
		return True
	
	def resolveCodepoint(self, compatibility):
		result = []
		
		if self.decompositionCodepoints and (compatibility or self.decompositionType == "DecompositionType_Canonical"):
			for c in self.decompositionCodepoints:
				if c in self.db.records:
					resolved = self.db.records[c].resolveCodepoint(compatibility)
					if resolved:
						result += resolved
				else:
					print "missing " + hex(c) + " in database"
					result.append(c)
		else:
			result.append(self.codepoint)
		
		return result
	
	def decompose(self):
		self.decomposedNFD = self.resolveCodepoint(False)
		self.decomposedNFKD = self.resolveCodepoint(True)
	
	def compose(self):
		if self.decompositionCodepoints and self.decompositionType == "DecompositionType_Canonical":
			c = self.decompositionCodepoints[0]
			if c in self.db.records:
				target = self.db.records[c]
				if len(self.decompositionCodepoints) == 2:
					target.compositionPairs[self.decompositionCodepoints[1]] = self.codepoint
			else:
				print "compose failed, missing " + hex(c) + " in database."
	
	def caseMapping(self):
		# ignore ASCII
		if self.codepoint < 0x7F:
			return
		
		if self.uppercase:
			converted = ""
			for u in self.uppercase:
				converted += libs.utf8.codepointToUtf8(u)
			self.offsetUppercase = self.db.addTranslation(converted + "\\x00")
		if self.lowercase:
			converted = ""
			for u in self.lowercase:
				converted += libs.utf8.codepointToUtf8(u)
			self.offsetLowercase = self.db.addTranslation(converted + "\\x00")
		if self.titlecase:
			converted = ""
			for u in self.titlecase:
				converted += libs.utf8.codepointToUtf8(u)
			self.offsetTitlecase = self.db.addTranslation(converted + "\\x00")
	
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
		
		return "{ " + hex(self.codepoint) + ", " + self.generalCategory + ", " + str(self.canonicalCombiningClass) + ", " + self.bidiClass + ", " + str(self.offsetNFD) + ", " + str(self.offsetNFKD) + ", " + self.numericType + ", " + str(self.numericValue) + ", " + bidiMirroredString + " },"

class Database(libs.unicode.UnicodeVisitor):
	def __init__(self):
		self.verbose = False
		self.blob = ""
		self.pageSize = 32767
		self.total = 0
		self.offset = 1
		self.hashed = dict()
		self.recordsOrdered = []
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
		
		self.recordsOrdered.append(u)
		self.records[u.codepoint] = u
		
		return True
	
	def resolveMissing(self):
		print "Adding missing codepoints to database..."
		
		missing = []
		missing += range(0x3401, 0x4DB4) # CJK Ideograph Extension A
		missing += range(0x4E01, 0x9FCB) # CJK Ideograph
		missing += range(0xAC01, 0xD7A2) # Hangul Syllable
		missing += range(0x20001, 0x2A6D5) # CJK Ideograph Extension B
		missing += range(0x2A701, 0x2B733) # CJK Ideograph Extension C
		missing += range(0x2B73F, 0x2B81C) # CJK Ideograph Extension D
		
		for c in missing:
			u = UnicodeMapping(self)
			u.codepoint = c
			self.recordsOrdered.append(u)
			self.records[u.codepoint] = u
	
	def resolveDecomposition(self):
		print "Resolving decomposition..."
		
		for r in self.recordsOrdered:
			r.decompose()
			
			convertedCodepoint = libs.utf8.codepointToUtf8(r.codepoint)
			
			r.offsetNFD = 0
			r.offsetNFKD = 0
			
			if r.decomposedNFD:
				convertedNFD = ""
				for d in r.decomposedNFD:
					convertedNFD += libs.utf8.codepointToUtf8(d)
				if convertedNFD <> convertedCodepoint:
					r.offsetNFD = self.addTranslation(convertedNFD + "\\x00")
			
			if r.decomposedNFKD:
				convertedNFKD = ""
				for d in r.decomposedNFKD:
					convertedNFKD += libs.utf8.codepointToUtf8(d)
				if convertedNFKD <> convertedCodepoint:
					r.offsetNFKD = self.addTranslation(convertedNFKD + "\\x00")
	
	def resolveComposition(self):
		print "Resolving composition..."
		
		for r in self.recordsOrdered:
			r.compose()
		
		composed = []
		
		for r in self.recordsOrdered:
			if r.compositionPairs:
				for p in r.compositionPairs.items():
					key = r.codepoint + p[0]
					if key in composed:
						print "collision " + hex(key)
					else:
						pair = {
							"key": key,
							"value": p[1]
						}
						composed.append(pair)
		
		print "composed", str(len(composed))
	
	def resolveCaseMapping(self):
		print "Resolving case mappings..."
		
		for r in self.recordsOrdered:
			r.caseMapping()
	
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
	
	def composePair(self, left, right):
		if left not in self.records:
			return None
		
		leftCodepoint = self.records[left]
		if len(leftCodepoint.compositionPairs) == 0:
			return None
		
		if not right in leftCodepoint.compositionPairs:
			return None
		
		return leftCodepoint.compositionPairs[right]
	
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
	
	def writeDecompositionRecords(self, header, records, name, field):
		header.writeLine("const size_t Unicode" + name + "RecordCount = " + str(len(records)) + ";")
		header.writeLine("const DecompositionRecord Unicode" + name + "Record[" + str(len(records)) + "] = {")
		header.indent()
		
		count = 0
		
		for r in records:
			if (count % 4) == 0:
				header.writeIndentation()
			
			header.write("{ " + hex(r.codepoint) + ", " + hex(r.__dict__[field]) + " },")
			
			count += 1
			if count <> len(records):
				if (count % 4) == 0:
					header.newLine()
				else:
					header.write(" ")
		
		header.newLine()
		header.outdent()
		header.writeLine("};")
		header.writeLine("const DecompositionRecord* Unicode" + name + "RecordPtr = Unicode" + name + "Record;")
		
		header.newLine()
	
	def writeSource(self, filepath):
		print "Writing database to " + filepath + "..."
		
		command_line = sys.argv[0]
		arguments = sys.argv[1:]
		for a in arguments:
			command_line += " " + a
		
		d = datetime.datetime.now()
		
		nfd_records = []
		nfkd_records = []
		uppercase_records = []
		lowercase_records = []
		titlecase_records = []
		
		for r in self.recordsOrdered:
			if r.offsetNFD <> 0:
				nfd_records.append(r)
			
			if r.offsetNFKD <> 0:
				nfkd_records.append(r)
			
			if r.offsetUppercase <> 0:
				uppercase_records.append(r)
			
			if r.offsetLowercase <> 0:
				lowercase_records.append(r)
			
			if r.offsetTitlecase <> 0:
				titlecase_records.append(r)
		
		sliced = libs.blobsplitter.BlobSplitter()
		sliced.split(self.blob, self.offset)
		
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
		
		# includes
		
		header.writeLine("#include \"normalization.h\"")
		header.newLine()
		
		# decomposition records
		
		self.writeDecompositionRecords(header, nfd_records, "NFD", "offsetNFD")
		self.writeDecompositionRecords(header, nfkd_records, "NFKD", "offsetNFKD")
		
		# case mapping records
		
		self.writeDecompositionRecords(header, uppercase_records, "Uppercase", "offsetUppercase")
		self.writeDecompositionRecords(header, lowercase_records, "Lowercase", "offsetLowercase")
		self.writeDecompositionRecords(header, titlecase_records, "Titlecase", "offsetTitlecase")
		
		# decomposition data
		
		header.writeLine("const char* DecompositionData = ")
		header.indent()
		
		for p in sliced.pages:
			p.start()
			while not p.atEnd:
				p.nextLine()
				header.writeIndentation()
				header.write(p.line)
				header.newLine()
		
		header.outdent()
		header.writeLine(";")
		header.write("const size_t DecompositionDataLength = " + str(self.offset) + ";")
	
	def writeCaseMapping(self, filepath):
		print "Writing case mapping to " + filepath + "..."
		
		command_line = sys.argv[0]
		arguments = sys.argv[1:]
		for a in arguments:
			command_line += " " + a
		
		d = datetime.datetime.now()
		
		output = libs.header.Header(filepath)
		
		# comment header
		
		output.writeLine("# DO NOT MODIFY, AUTO-GENERATED")
		output.writeLine("#")
		output.writeLine("# Generated on:")
		output.writeLine("# " + d.strftime("%Y-%m-%dT%H:%M:%S"))
		output.writeLine("#")
		output.writeLine("# Command line:")
		output.writeLine("# " + command_line)
		output.writeLine("#")
		output.writeLine("# Data format:")
		output.writeLine("#")
		output.writeLine("# Codepoint")
		output.writeLine("# Uppercase")
		output.writeLine("# Lowercase")
		output.writeLine("# Titlecase")
		output.newLine()
		
		# data
		
		for r in self.recordsOrdered:
			if r.uppercase or r.lowercase or r.titlecase:
				output.write("%08x" % r.codepoint + "; ")
				
				if r.uppercase:
					output.write("%08x" % r.uppercase[0])
					for u in r.uppercase[1:]:
						output.write(" %08x" % u)
					output.write("; ")
				else:
					output.write("%08x" % r.codepoint + "; ")
				
				if r.lowercase:
					output.write("%08x" % r.lowercase[0])
					for u in r.lowercase[1:]:
						output.write(" %08x" % u)
					output.write("; ")
				else:
					output.write("%08x" % r.codepoint + "; ")
				
				if r.titlecase:
					output.write("%08x" % r.titlecase[0])
					for u in r.titlecase[1:]:
						output.write(" %08x" % u)
					output.write("; ")
				else:
					output.write("%08x" % r.codepoint + "; ")
				
				output.write("# " + r.name)
				output.newLine()

class SpecialCasing(libs.unicode.UnicodeVisitor):
	def __init__(self, db):
		self.db = db
	
	def visitDocument(self, document):
		print "Parsing special case mappings..."
		return True
	
	def visitEntry(self, entry):
		if not entry.matches[0]:
			return False
		
		# ignore entries with special requirements
		if len(entry.matches) == 5:
			return True
		
		codepoint = int(entry.matches[0][0], 16)
		
		r = self.db.records[codepoint]
		
		if entry.matches[1]:
			r.lowercase = []
			for u in entry.matches[1]:
				r.lowercase.append(int(u, 16))
		
		if entry.matches[2]:
			r.titlecase = []
			for u in entry.matches[2]:
				r.titlecase.append(int(u, 16))
		
		if entry.matches[3]:
			r.uppercase = []
			for u in entry.matches[3]:
				r.uppercase.append(int(u, 16))
		
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
	parser.add_argument(
		'--page-size', '-p',
		dest = 'pageSize',
		default = 32767,
		type = int,
		help = 'maximum page size for written strings'
	)
	args = parser.parse_args()
	
	script_path = os.path.dirname(os.path.realpath(sys.argv[0]))
	
	db = Database()
	
	document = libs.unicode.UnicodeDocument()
	document.lineLimit = args.lineLimit
	document.entryLimit = args.entryLimit
	document.entrySkip = args.entrySkip
	document.parse(script_path + '/data/UnicodeData.txt')
	
	db.pageSize = args.pageSize
	document.accept(db)
	db.resolveMissing()
	db.resolveDecomposition()
	db.resolveComposition()
	
	document_special_casing = libs.unicode.UnicodeDocument()
	document_special_casing.parse(script_path + '/data/SpecialCasing.txt')
	
	special_casing = SpecialCasing(db)
	document_special_casing.accept(special_casing)
	
	db.resolveCaseMapping()
	
	db.executeQuery(args.query)
	
	db.writeSource(script_path + '/../../source/unicodedatabase.c')
	db.writeCaseMapping(script_path + '/../../testdata/CaseMapping.txt')