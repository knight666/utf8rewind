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
		self.generalCategoryCombined = 0
		self.canonicalCombiningClass = 0
		self.bidiClass = ""
		self.decompositionType = ""
		self.decompositionCodepoints = []
		self.decomposedNFD = []
		self.decomposedNFKD = []
		self.compositionPairs = dict()
		self.compositionExcluded = False
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
		self.block = None
	
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
		
		mapping = [
			{
				"name": "Cased_Letter",
				"check": [ "Lu", "Ll", "Lt" ],
				"value": 0x03,
			},
			{
				"name": "Letter",
				"check": [ "Lu", "Ll", "Lt", "Lm", "Lo" ],
				"value": 0x01,
			},
			{
				"name": "Mark",
				"check": [ "Mn", "Mc", "Me" ],
				"value": 0x04,
			},
			{
				"name": "Number",
				"check": [ "Nd", "Nl", "No" ],
				"value": 0x08,
			},
			{
				"name": "Punctuation",
				"check": [ "Pc", "Pd", "Ps", "Pe", "Pi", "Pf", "Po" ],
				"value": 0x10,
			},
			{
				"name": "Symbol",
				"check": [ "Sm", "Sc", "Sk", "So" ],
				"value": 0x20,
			},
			{
				"name": "Separator",
				"check": [ "Zs", "Zl", "Zp" ],
				"value": 0x40,
			},
			{
				"name": "Other",
				"check": [ "Cc", "Cf", "Cs", "Co", "Cn" ],
				"value": 0x80,
			},
		]
		for m in mapping:
			if matches[2][0] in m["check"]:
				self.generalCategoryCombined = m["value"]
				break
		
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
			self.generalCategoryCombined |= 0x02
		
		if matches[13]:
			self.lowercase.append(int(matches[13][0], 16))
			self.generalCategoryCombined |= 0x02
		
		if len(matches) >= 15 and matches[14]:
			self.titlecase.append(int(matches[14][0], 16))
			self.generalCategoryCombined |= 0x02
		
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
		if not self.compositionExcluded and self.decompositionCodepoints and self.decompositionType == "DecompositionType_Canonical":
			c = self.decompositionCodepoints[0]
			if c in self.db.records:
				target = self.db.records[c]
				if len(self.decompositionCodepoints) == 2:
					target.compositionPairs[self.decompositionCodepoints[1]] = self.codepoint
			else:
				print "compose failed, missing " + hex(c) + " in database."
	
	def caseMapping(self):
		if self.uppercase:
			converted = libs.utf8.unicodeToUtf8Hex(self.uppercase)
			self.offsetUppercase = self.db.addTranslation(converted + "\\x00")
		if self.lowercase:
			converted = libs.utf8.unicodeToUtf8Hex(self.lowercase)
			self.offsetLowercase = self.db.addTranslation(converted + "\\x00")
		if self.titlecase:
			converted = libs.utf8.unicodeToUtf8Hex(self.titlecase)
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

class UnicodeBlock:
	def __init__(self, db):
		self.db = db
		self.clear()
	
	def __str__(self):
		return "{ name: \"" + self.name + "\", start: " + hex(self.start) + ", end: " + hex(self.end) + " }"
	
	def clear(self):
		self.start = 0
		self.end = 0
		self.name = ""
	
	def parse(self, matches):
		self.clear()
		
		if not matches[0]:
			return False
		
		matched = re.match('([0-9A-Fa-f]+)\.\.([0-9A-Fa-f]+)', matches[0][0])
		if matched:
			self.start = int(matched.groups(1)[0], 16)
			self.end = int(matched.groups(1)[1], 16)
		
		self.name = matches[1][0]
		for m in matches[1][1:]:
			self.name += " " + m
		
		return True

class QuickCheckRecord:
	def __init__(self, db):
		self.start = 0
		self.count = 0
		self.end = 0
		self.value = 0

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
		self.blocks = []
		self.qcGeneralCategory = []
		self.qcCanonicalCombiningClass = []
		self.qcNFCRecords = []
		self.qcNFDRecords = []
		self.qcNFKCRecords = []
		self.qcNFKDRecords = []
	
	def loadFromFiles(self, arguments):
		script_path = os.path.dirname(os.path.realpath(sys.argv[0]))
		
		document_database = libs.unicode.UnicodeDocument()
		if arguments:
			self.pageSize = args.pageSize
			document_database.lineLimit = arguments.lineLimit
			document_database.entryLimit = arguments.entryLimit
			document_database.entrySkip = arguments.entrySkip
		document_database.parse(script_path + '/data/UnicodeData.txt')
		document_database.accept(self)
		
		# blocks
		
		blocks = Blocks(self)
		document_blocks = libs.unicode.UnicodeDocument()
		document_blocks.parse(script_path + '/data/Blocks.txt')
		document_blocks.accept(blocks)
		
		self.resolveBlocks()
		
		# missing codepoints
		
		self.resolveMissing()
		
		# derived normalization properties
		
		normalization = Normalization(self)
		document_normalization = libs.unicode.UnicodeDocument()
		document_normalization.parse(script_path + '/data/DerivedNormalizationProps.txt')
		document_normalization.accept(normalization)
		
		self.resolveQuickCheck()
		
		# decomposition
		
		self.resolveDecomposition()
		self.resolveComposition()
		
		# case mapping
		
		document_special_casing = libs.unicode.UnicodeDocument()
		document_special_casing.parse(script_path + '/data/SpecialCasing.txt')
		
		special_casing = SpecialCasing(self)
		document_special_casing.accept(special_casing)
		
		self.resolveCaseMapping()
		
		# properties
		
		self.resolveProperties()
	
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
	
	def getBlockByCodepoint(self, codepoint):
		for b in self.blocks:
			if codepoint >= b.start and codepoint <= b.end:
				return b
		return None
	
	def getBlockByName(self, name):
		for b in self.blocks:
			if b.name == name:
				return b
		return None
	
	def resolveMissing(self):
		print "Adding missing codepoints to database..."
		
		missing = [
			self.getBlockByName("General Punctuation"), # 2000..206F
			self.getBlockByName("CJK Unified Ideographs Extension A"), # 3400..4DBF
			self.getBlockByName("CJK Unified Ideographs"), # 4E00..9FFF
			self.getBlockByName("Hangul Syllables"), # AC00..D7AF
			self.getBlockByName("Specials"), # FFF0..FFFF
			self.getBlockByName("CJK Unified Ideographs Extension B"), # 20000..2A6DF
			self.getBlockByName("CJK Unified Ideographs Extension C"), # 2A700..2B73F
			self.getBlockByName("CJK Unified Ideographs Extension D"), # 2B740..2B81F
			self.getBlockByName("Tags"), # E0000..E007F
			self.getBlockByName("Variation Selectors Supplement"), # E0100..E01EF
			self.getBlockByName("<reserved-E0080>..<reserved-E00FF>"), # E0080..E00FF
			self.getBlockByName("<reserved-E01F0>..<reserved-E0FFF>"), # E01F0..E0FFF
		]
		
		for b in missing:
			for c in range(b.start, b.end + 1):
				if c not in self.records:
					u = UnicodeMapping(self)
					u.codepoint = c
					u.block = b
					self.recordsOrdered.append(u)
					self.records[u.codepoint] = u
		
		self.recordsOrdered = sorted(self.recordsOrdered, key=lambda record: record.codepoint)
	
	def resolveBlocks(self):
		print "Resolving blocks for entries..."
		
		block_index = 0
		block_current = self.blocks[block_index]
		
		for r in self.recordsOrdered:
			if r.codepoint > block_current.end:
				block_index += 1
				block_current = self.blocks[block_index]
			r.block = block_current
		
		# missing from blocks data file
		
		block_reserved1 = UnicodeBlock(self)
		block_reserved1.start = 0xE0080
		block_reserved1.end = 0xE00FF
		block_reserved1.name = "<reserved-E0080>..<reserved-E00FF>"
		self.blocks.append(block_reserved1)
		
		block_reserved2 = UnicodeBlock(self)
		block_reserved2.start = 0xE01F0
		block_reserved2.end = 0xE0FFF
		block_reserved2.name = "<reserved-E01F0>..<reserved-E0FFF>"
		self.blocks.append(block_reserved2)
	
	def resolveQuickCheck(self):
		print "Resolving quick check entries..."
		
		# NFC
		
		self.qcNFCRecords = sorted(self.qcNFCRecords, key=lambda record: record.start)
		nfc_length = len(self.qcNFCRecords)
		nfc_last = self.qcNFCRecords[nfc_length - 1]
		nfc_last.end = nfc_last.start + nfc_last.count
		
		for i in range(0, nfc_length - 1):
			current = self.qcNFCRecords[i]
			self.qcNFCRecords[i].end = self.qcNFCRecords[i + 1].start - 1
		
		# NFD
		
		self.qcNFDRecords = sorted(self.qcNFDRecords, key=lambda record: record.start)
		nfd_length = len(self.qcNFDRecords)
		nfd_last = self.qcNFDRecords[nfd_length - 1]
		nfd_last.end = nfd_last.start + nfd_last.count
		
		for i in range(0, nfd_length - 1):
			current = self.qcNFDRecords[i]
			self.qcNFDRecords[i].end = self.qcNFDRecords[i + 1].start - 1
		
		# NFKC
		
		self.qcNFKCRecords = sorted(self.qcNFKCRecords, key=lambda record: record.start)
		nfkc_length = len(self.qcNFKCRecords)
		nfkc_last = self.qcNFKCRecords[nfkc_length - 1]
		nfkc_last.end = nfkc_last.start + nfkc_last.count
		
		for i in range(0, nfkc_length - 1):
			current = self.qcNFKCRecords[i]
			self.qcNFKCRecords[i].end = self.qcNFKCRecords[i + 1].start - 1
		
		# NFKD
		
		self.qcNFKDRecords = sorted(self.qcNFKDRecords, key=lambda record: record.start)
		nfkd_length = len(self.qcNFKDRecords)
		nfkd_last = self.qcNFKDRecords[nfkd_length - 1]
		nfkd_last.end = nfkd_last.start + nfkd_last.count
		
		for i in range(0, nfkd_length - 1):
			current = self.qcNFKDRecords[i]
			self.qcNFKDRecords[i].end = self.qcNFKDRecords[i + 1].start - 1
		
	def resolveDecomposition(self):
		print "Resolving decomposition..."
		
		for r in self.recordsOrdered:
			r.decompose()
			
			convertedCodepoint = libs.utf8.codepointToUtf8Hex(r.codepoint)
			
			r.offsetNFD = 0
			r.offsetNFKD = 0
			
			if r.decomposedNFD:
				convertedNFD = libs.utf8.unicodeToUtf8Hex(r.decomposedNFD)
				if convertedNFD <> convertedCodepoint:
					r.offsetNFD = self.addTranslation(convertedNFD + "\\x00")
			
			if r.decomposedNFKD:
				convertedNFKD = libs.utf8.unicodeToUtf8Hex(r.decomposedNFKD)
				if convertedNFKD <> convertedCodepoint:
					r.offsetNFKD = self.addTranslation(convertedNFKD + "\\x00")
	
	def resolveComposition(self):
		print "Resolving composition..."
		
		for r in self.recordsOrdered:
			r.compose()
	
	def resolveCaseMapping(self):
		print "Resolving case mappings..."
		
		for r in self.recordsOrdered:
			r.caseMapping()
	
	def resolveProperties(self):
		print "Resolving codepoint properties..."
		
		group_category = None
		group_ccc = None
		
		for r in self.recordsOrdered:
			if r.generalCategoryCombined:
				if not group_category or r.codepoint <> (group_category.start + group_category.count + 1) or r.generalCategoryCombined <> group_category.value:
					if group_category:
						group_category.end = r.codepoint - 1
					group_category = QuickCheckRecord(self)
					group_category.start = r.codepoint
					group_category.end = r.codepoint
					group_category.value = r.generalCategoryCombined
					self.qcGeneralCategory.append(group_category)
				else:
					group_category.count += 1
			
			if r.canonicalCombiningClass:
				if not group_ccc or r.codepoint <> (group_ccc.start + group_ccc.count + 1) or r.canonicalCombiningClass <> group_ccc.value:
					if group_ccc:
						group_ccc.end = r.codepoint - 1
					group_ccc = QuickCheckRecord(self)
					group_ccc.start = r.codepoint
					group_ccc.end = r.codepoint
					group_ccc.value = r.canonicalCombiningClass
					self.qcCanonicalCombiningClass.append(group_ccc)
				else:
					group_ccc.count += 1
		
		if group_category:
			group_category.end = group_category.start + group_category.count
		
		if group_ccc:
			group_ccc.end = group_ccc.start + group_ccc.count
	
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
		if match == None:
			return ""
		
		codepoints = []
		
		for group in match:
			if group <> None:
				codepoints.append(int(group, 16))
		
		result = libs.utf8.unicodeToUtf8Hex(codepoints)
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
	
	def writeCompositionRecords(self, header):
		composed = []
		
		for r in self.recordsOrdered:
			if r.compositionPairs:
				for p in r.compositionPairs.items():
					key = (r.codepoint << 32) + p[0]
					if key in composed:
						print "collision " + hex(key)
					else:
						pair = {
							"key": key,
							"value": p[1]
						}
						composed.append(pair)
		
		composed_ordered = sorted(composed, key=lambda item: item["key"])
		
		header.writeLine("const size_t UnicodeCompositionRecordCount = " + str(len(composed_ordered)) + ";")
		header.writeLine("const CompositionRecord UnicodeCompositionRecord[" + str(len(composed_ordered)) + "] = {")
		header.indent()
		
		count = 0
		
		for c in composed_ordered:
			if (count % 4) == 0:
				header.writeIndentation()
			
			header.write("{ " + hex(c["key"]) + ", " + hex(c["value"]) + " },")
			
			count += 1
			if count <> len(composed_ordered):
				if (count % 4) == 0:
					header.newLine()
				else:
					header.write(" ")
		
		header.newLine()
		header.outdent()
		header.writeLine("};")
		header.writeLine("const CompositionRecord* UnicodeCompositionRecordPtr = UnicodeCompositionRecord;")
		
		header.newLine()
	
	def writeQuickCheck(self, header, records, name):
		header.writeLine("const size_t UnicodeQuickCheck" + name + "RecordCount = " + str(len(records)) + ";")
		header.writeLine("const QuickCheckRecord UnicodeQuickCheck" + name + "Record[" + str(len(records)) + "] = {")
		header.indent()
		
		count = 0
		
		for r in records:
			if (count % 4) == 0:
				header.writeIndentation()
			
			value = (r.value << 24) | r.count
			header.write("{ " + hex(r.start) + ", " + hex(r.end) + ", " + hex(value) + " },")
			
			count += 1
			if count <> len(records):
				if (count % 4) == 0:
					header.newLine()
				else:
					header.write(" ")
		
		header.newLine()
		header.outdent()
		header.writeLine("};")
		header.writeLine("const QuickCheckRecord* UnicodeQuickCheck" + name + "RecordPtr = UnicodeQuickCheck" + name + "Record;")
		
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
		header.copyrightNotice()
		header.outdent()
		header.writeLine("*/")
		
		header.newLine()
		
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
		
		header.writeLine("#include \"unicodedatabase.h\"")
		
		header.newLine()
		
		# quick check records
		
		self.writeQuickCheck(header, self.qcGeneralCategory, "GeneralCategory")
		self.writeQuickCheck(header, self.qcCanonicalCombiningClass, "CanonicalCombiningClass")
		self.writeQuickCheck(header, self.qcNFCRecords, "NFC")
		self.writeQuickCheck(header, self.qcNFDRecords, "NFD")
		self.writeQuickCheck(header, self.qcNFKCRecords, "NFKC")
		self.writeQuickCheck(header, self.qcNFKDRecords, "NFKD")
		
		# decomposition records
		
		self.writeDecompositionRecords(header, nfd_records, "NFD", "offsetNFD")
		self.writeDecompositionRecords(header, nfkd_records, "NFKD", "offsetNFKD")
		
		# composition records
		
		self.writeCompositionRecords(header)
		
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
				output.write("%08X" % r.codepoint + "; ")
				
				if r.uppercase:
					output.write("%08X" % r.uppercase[0])
					for u in r.uppercase[1:]:
						output.write(" %08X" % u)
					output.write("; ")
				else:
					output.write("%08X" % r.codepoint + "; ")
				
				if r.lowercase:
					output.write("%08X" % r.lowercase[0])
					for u in r.lowercase[1:]:
						output.write(" %08X" % u)
					output.write("; ")
				else:
					output.write("%08X" % r.codepoint + "; ")
				
				if r.titlecase:
					output.write("%08X" % r.titlecase[0])
					for u in r.titlecase[1:]:
						output.write(" %08X" % u)
					output.write("; ")
				else:
					output.write("%08X" % r.codepoint + "; ")
				
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

class Blocks(libs.unicode.UnicodeVisitor):
	def __init__(self, db):
		self.db = db
	
	def visitDocument(self, document):
		print "Parsing block mappings..."
		return True
	
	def visitEntry(self, entry):
		if not entry.matches[0]:
			return False
		
		block = UnicodeBlock(self.db)
		if not block.parse(entry.matches):
			return False
		
		self.db.blocks.append(block)
		
		return True

class Normalization(libs.unicode.UnicodeVisitor):
	def __init__(self, db):
		self.db = db
	
	def parseEntry(self, start, count, matches):
		property = matches[1][0]
		
		def full_composition_exclusion(property):
			for i in range(start, start + count + 1):
				if i in self.db.records:
					record = self.db.records[i]
					record.compositionExcluded = True
				else:
					print "missing " + hex(start) + " in database (\"" + self.db.getBlockByCodepoint(start).name + "\")"
		
		def quick_check(property):
			nf_member = {
				"NFD_QC": "qcNFDRecords",
				"NFC_QC": "qcNFCRecords",
				"NFKD_QC": "qcNFKDRecords",
				"NFKC_QC": "qcNFKCRecords",
			}
			nf_value = {
				"N": 2,
				"M": 1,
				"Y": 0,
			}
			
			qc = QuickCheckRecord(self.db)
			qc.start = start
			qc.count = count
			qc.value = nf_value[matches[2][0]]
			
			self.db.__dict__[nf_member[property]].append(qc)
		
		def case_fold(property):
			pass
		
		def changes_when_nfkc_casefolded(property):
			pass
		
		property_values = {
			"Full_Composition_Exclusion": full_composition_exclusion,
			"NFD_QC": quick_check,
			"NFC_QC": quick_check,
			"NFKD_QC": quick_check,
			"NFKC_QC": quick_check,
			"NFKC_CF": case_fold,
			"Changes_When_NFKC_Casefolded": changes_when_nfkc_casefolded,
		}
		if property in property_values:
			property_values[property](property)
		
	
	def visitDocument(self, document):
		print "Parsing derived normalization properties..."
		return True
	
	def visitEntry(self, entry):
		if not entry.matches[0]:
			return False
		
		match = re.match('([0-9A-Fa-f]+)\.?\.?([0-9A-Fa-f]+)?', entry.matches[0][0])
		if match:
			start = int(match.group(1), 16)
			if match.group(2):
				count = int(match.group(2), 16) - start
			else:
				count = 0
			
			self.parseEntry(start, count, entry.matches)
		
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
	db.loadFromFiles(args)
	
	db.executeQuery(args.query)
	
	db.writeSource(script_path + '/../../source/unicodedatabase.c')
	db.writeCaseMapping(script_path + '/../../testdata/CaseMapping.txt')