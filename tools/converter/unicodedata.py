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
		self.quickNFC = 0
		self.offsetNFD = 0
		self.quickNFD = 0
		self.offsetNFKC = 0
		self.quickNFKC = 0
		self.offsetNFKD = 0
		self.quickNFKD = 0
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
					print('missing ' + hex(c) + ' in database')
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
				print('compose failed, missing ' + hex(c) + ' in database.')
	
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

class Compression:
	def __init__(self, database):
		self.database = database
		self.chunk_size = 0
		self.uncompressed_size = 0
		self.table_index = []
		self.table_data = []

	def process(self, field, chunkSize):
		print('Compressing property "' + field + '" with a chunk size of ' + str(chunkSize) + '...')

		self.chunk_size = chunkSize
		self.uncompressed_size = 0
		self.table_index = []
		self.table_data = []

		for i in range(0, len(self.database.recordsOrdered), chunkSize):
			chunk = []
			for r in self.database.recordsOrdered[i:i + chunkSize]:
				chunk.append(r.__dict__[field])

			offset = 0
			index = 0
			overlapping = False

			for t in range(len(self.table_data)):
				if self.table_data[t] == chunk[offset]:
					offset += 1
					if offset == len(chunk):
						overlapping = True
						break
				else:
					index = t + 1
					offset = 0

			if not overlapping:
				offset = 0
				index = 0

				if len(self.table_data) > 0:
					for t in range(len(self.table_data)):
						ti = len(self.table_data) - t - 1
						if self.table_data[ti] != chunk[offset]:
							break
						offset += 1
						if offset == len(chunk):
							break
					index = len(self.table_data) - offset

			"""if len(self.table_index) == 27:
				print('chunk ' + str(chunk))
				print('overlapping ' + str(overlapping))
				print('data_size ' + str(len(self.table_data)) + ' index_size ' + str(len(self.table_index)) + ' offset ' + str(offset) + ' index ' + str(index))
				print('chunk_after ' + str(chunk[offset:]))
				print('table_before ' + str(self.table_data))
				exit(0)"""

			self.table_index.append(index)
			self.table_data.extend(chunk[offset:])

			self.uncompressed_size += len(chunk)

		compressed_size = len(self.table_index) + len(self.table_data)
		ratio = (1.0 - (compressed_size / self.uncompressed_size)) * 100.0
		print(field + ': uncompressed ' + str(self.uncompressed_size) + ' compressed ' + str(compressed_size) + ' savings ' + ('%.2f%%' % ratio))

	def render(self, header, name):
		print('Rendering compressed data for "' + name + '"...')

		header.newLine()
		header.writeLine("const size_t " + name + "Index[" + str(len(self.table_index)) + "] = {")
		header.indent()

		count = 0
		for c in self.table_index:
			if (count % self.chunk_size) == 0:
				header.writeIndentation()
			
			header.write('%d,' % c)
			
			count += 1
			if count != len(self.table_index):
				if (count % self.chunk_size) == 0:
					header.newLine()
				else:
					header.write(' ')

		header.newLine()
		header.outdent()
		header.writeLine("};")
		header.writeLine("const size_t* " + name + "IndexPtr = " + name + "Index;")

		header.newLine()

		header.writeLine("const uint8_t " + name + "Data[" + str(len(self.table_data)) + "] = {")
		header.indent()

		count = 0
		for c in self.table_data:
			if (count % self.chunk_size) == 0:
				header.writeIndentation()
			
			header.write('0x%02X,' % c)
			
			count += 1
			if count != len(self.table_data):
				if (count % self.chunk_size) == 0:
					header.newLine()
				else:
					header.write(' ')

		header.newLine()
		header.outdent()
		header.writeLine("};")
		header.write("const uint8_t* " + name + "DataPtr = " + name + "Data;")

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
		print('Parsing document to codepoint database...')
		return True
	
	def visitEntry(self, entry):
		if not entry.matches[0]:
			return False
		
		u = UnicodeMapping(self)
		try:
			if not u.parse(entry.matches):
				return False
		except KeyError as e:
			print('Failed to parse entry - error: "' + e + '" line: ' + str(entry.lineNumber))
			for m in entry.matches:
				print(m)
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
		print('Adding missing codepoints to database...')
		
		missing = [
			self.getBlockByName("Basic Latin"), # 0000..007F
			self.getBlockByName("Latin-1 Supplement"), # 0080..00FF
			self.getBlockByName("Latin Extended-A"), # 0100..017F
			self.getBlockByName("Latin Extended-B"), # 0180..024F
			self.getBlockByName("IPA Extensions"), # 0250..02AF
			self.getBlockByName("Spacing Modifier Letters"), # 02B0..02FF
			self.getBlockByName("Combining Diacritical Marks"), # 0300..036F
			self.getBlockByName("Greek and Coptic"), # 0370..03FF
			self.getBlockByName("Cyrillic"), # 0400..04FF
			self.getBlockByName("Cyrillic Supplement"), # 0500..052F
			self.getBlockByName("Armenian"), # 0530..058F
			self.getBlockByName("Hebrew"), # 0590..05FF
			self.getBlockByName("Arabic"), # 0600..06FF
			self.getBlockByName("Syriac"), # 0700..074F
			self.getBlockByName("Arabic Supplement"), # 0750..077F
			self.getBlockByName("Thaana"), # 0780..07BF
			self.getBlockByName("NKo"), # 07C0..07FF
			self.getBlockByName("Samaritan"), # 0800..083F
			self.getBlockByName("Mandaic"), # 0840..085F
			self.getBlockByName("<reserved-0860>..<reserved-089F>"),
			self.getBlockByName("Arabic Extended-A"), # 08A0..08FF
			self.getBlockByName("Devanagari"), # 0900..097F
			self.getBlockByName("Bengali"), # 0980..09FF
			self.getBlockByName("Gurmukhi"), # 0A00..0A7F
			self.getBlockByName("Gujarati"), # 0A80..0AFF
			self.getBlockByName("Oriya"), # 0B00..0B7F
			self.getBlockByName("Tamil"), # 0B80..0BFF
			self.getBlockByName("Telugu"), # 0C00..0C7F
			self.getBlockByName("Kannada"), # 0C80..0CFF
			self.getBlockByName("Malayalam"), # 0D00..0D7F
			self.getBlockByName("Sinhala"), # 0D80..0DFF
			self.getBlockByName("Thai"), # 0E00..0E7F
			self.getBlockByName("Lao"), # 0E80..0EFF
			self.getBlockByName("Tibetan"), # 0F00..0FFF
			self.getBlockByName("Myanmar"), # 1000..109F
			self.getBlockByName("Georgian"), # 10A0..10FF
			self.getBlockByName("Hangul Jamo"), # 1100..11FF
			self.getBlockByName("Ethiopic"), # 1200..137F
			self.getBlockByName("Ethiopic Supplement"), # 1380..139F
			self.getBlockByName("Cherokee"), # 13A0..13FF
			self.getBlockByName("Unified Canadian Aboriginal Syllabics"), # 1400..167F
			self.getBlockByName("Ogham"), # 1680..169F
			self.getBlockByName("Runic"), # 16A0..16FF
			self.getBlockByName("Tagalog"), # 1700..171F
			self.getBlockByName("Hanunoo"), # 1720..173F
			self.getBlockByName("Buhid"), # 1740..175F
			self.getBlockByName("Tagbanwa"), # 1760..177F
			self.getBlockByName("Khmer"), # 1780..17FF
			self.getBlockByName("Mongolian"), # 1800..18AF
			self.getBlockByName("Unified Canadian Aboriginal Syllabics Extended"), # 18B0..18FF
			self.getBlockByName("Limbu"), # 1900..194F
			self.getBlockByName("Tai Le"), # 1950..197F
			self.getBlockByName("New Tai Lue"), # 1980..19DF
			self.getBlockByName("Khmer Symbols"), # 19E0..19FF
			self.getBlockByName("Buginese"), # 1A00..1A1F
			self.getBlockByName("Tai Tham"), # 1A20..1AAF
			self.getBlockByName("Combining Diacritical Marks Extended"), # 1AB0..1AFF
			self.getBlockByName("Balinese"), # 1B00..1B7F
			self.getBlockByName("Sundanese"), # 1B80..1BBF
			self.getBlockByName("Batak"), # 1BC0..1BFF
			self.getBlockByName("Lepcha"), # 1C00..1C4F
			self.getBlockByName("Ol Chiki"), # 1C50..1C7F
			self.getBlockByName("<reserved-1C80>..<reserved-1CBF>"),
			self.getBlockByName("Sundanese Supplement"), # 1CC0..1CCF
			self.getBlockByName("Vedic Extensions"), # 1CD0..1CFF
			self.getBlockByName("Phonetic Extensions"), # 1D00..1D7F
			self.getBlockByName("Phonetic Extensions Supplement"), # 1D80..1DBF
			self.getBlockByName("Combining Diacritical Marks Supplement"), # 1DC0..1DFF
			self.getBlockByName("Latin Extended Additional"), # 1E00..1EFF
			self.getBlockByName("Greek Extended"), # 1F00..1FFF
			self.getBlockByName("General Punctuation"), # 2000..206F
			self.getBlockByName("Superscripts and Subscripts"), # 2070..209F
			self.getBlockByName("Currency Symbols"), # 20A0..20CF
			self.getBlockByName("Combining Diacritical Marks for Symbols"), # 20D0..20FF
			self.getBlockByName("Letterlike Symbols"), # 2100..214F
			self.getBlockByName("Number Forms"), # 2150..218F
			self.getBlockByName("Arrows"), # 2190..21FF
			self.getBlockByName("Mathematical Operators"), # 2200..22FF
			self.getBlockByName("Miscellaneous Technical"), # 2300..23FF
			self.getBlockByName("Control Pictures"), # 2400..243F
			self.getBlockByName("Optical Character Recognition"), # 2440..245F
			self.getBlockByName("Enclosed Alphanumerics"), # 2460..24FF
			self.getBlockByName("Box Drawing"), # 2500..257F
			self.getBlockByName("Block Elements"), # 2580..259F
			self.getBlockByName("Geometric Shapes"), # 25A0..25FF
			self.getBlockByName("Miscellaneous Symbols"), # 2600..26FF
			self.getBlockByName("Dingbats"), # 2700..27BF
			self.getBlockByName("Miscellaneous Mathematical Symbols-A"), # 27C0..27EF
			self.getBlockByName("Supplemental Arrows-A"), # 27F0..27FF
			self.getBlockByName("Braille Patterns"), # 2800..28FF
			self.getBlockByName("Supplemental Arrows-B"), # 2900..297F
			self.getBlockByName("Miscellaneous Mathematical Symbols-B"), # 2980..29FF
			self.getBlockByName("Supplemental Mathematical Operators"), # 2A00..2AFF
			self.getBlockByName("Miscellaneous Symbols and Arrows"), # 2B00..2BFF
			self.getBlockByName("Glagolitic"), # 2C00..2C5F
			self.getBlockByName("Latin Extended-C"), # 2C60..2C7F
			self.getBlockByName("Coptic"), # 2C80..2CFF
			self.getBlockByName("Georgian Supplement"), # 2D00..2D2F
			self.getBlockByName("Tifinagh"), # 2D30..2D7F
			self.getBlockByName("Ethiopic Extended"), # 2D80..2DDF
			self.getBlockByName("Cyrillic Extended-A"), # 2DE0..2DFF
			self.getBlockByName("Supplemental Punctuation"), # 2E00..2E7F
			self.getBlockByName("CJK Radicals Supplement"), # 2E80..2EFF
			self.getBlockByName("Kangxi Radicals"), # 2F00..2FDF
			self.getBlockByName("<reserved-2FE0>..<reserved-2FEF>"),
			self.getBlockByName("Ideographic Description Characters"), # 2FF0..2FFF
			self.getBlockByName("CJK Symbols and Punctuation"), # 3000..303F
			self.getBlockByName("Hiragana"), # 3040..309F
			self.getBlockByName("Katakana"), # 30A0..30FF
			self.getBlockByName("Bopomofo"), # 3100..312F
			self.getBlockByName("Hangul Compatibility Jamo"), # 3130..318F
			self.getBlockByName("Kanbun"), # 3190..319F
			self.getBlockByName("Bopomofo Extended"), # 31A0..31BF
			self.getBlockByName("CJK Strokes"), # 31C0..31EF
			self.getBlockByName("Katakana Phonetic Extensions"), # 31F0..31FF
			self.getBlockByName("Enclosed CJK Letters and Months"), # 3200..32FF
			self.getBlockByName("CJK Compatibility"), # 3300..33FF
			self.getBlockByName("CJK Unified Ideographs Extension A"), # 3400..4DBF
			self.getBlockByName("Yijing Hexagram Symbols"), # 4DC0..4DFF
			self.getBlockByName("CJK Unified Ideographs"), # 4E00..9FFF
			self.getBlockByName("Yi Syllables"), # A000..A48F
			self.getBlockByName("Yi Radicals"), # A490..A4CF
			self.getBlockByName("Lisu"), # A4D0..A4FF
			self.getBlockByName("Vai"), # A500..A63F
			self.getBlockByName("Cyrillic Extended-B"), # A640..A69F
			self.getBlockByName("Bamum"), # A6A0..A6FF
			self.getBlockByName("Modifier Tone Letters"), # A700..A71F
			self.getBlockByName("Latin Extended-D"), # A720..A7FF
			self.getBlockByName("Syloti Nagri"), # A800..A82F
			self.getBlockByName("Common Indic Number Forms"), # A830..A83F
			self.getBlockByName("Phags-pa"), # A840..A87F
			self.getBlockByName("Saurashtra"), # A880..A8DF
			self.getBlockByName("Devanagari Extended"), # A8E0..A8FF
			self.getBlockByName("Kayah Li"), # A900..A92F
			self.getBlockByName("Rejang"), # A930..A95F
			self.getBlockByName("Hangul Jamo Extended-A"), # A960..A97F
			self.getBlockByName("Javanese"), # A980..A9DF
			self.getBlockByName("Myanmar Extended-B"), # A9E0..A9FF
			self.getBlockByName("Cham"), # AA00..AA5F
			self.getBlockByName("Myanmar Extended-A"), # AA60..AA7F
			self.getBlockByName("Tai Viet"), # AA80..AADF
			self.getBlockByName("Meetei Mayek Extensions"), # AAE0..AAFF
			self.getBlockByName("Ethiopic Extended-A"), # AB00..AB2F
			self.getBlockByName("Latin Extended-E"), # AB30..AB6F
			self.getBlockByName("<reserved-AB70>..<reserved-ABBF>"),
			self.getBlockByName("Meetei Mayek"), # ABC0..ABFF
			self.getBlockByName("Hangul Syllables"), # AC00..D7AF
			self.getBlockByName("Hangul Jamo Extended-B"), # D7B0..D7FF
			self.getBlockByName("High Surrogates"), # D800..DB7F
			self.getBlockByName("High Private Use Surrogates"), # DB80..DBFF
			self.getBlockByName("Low Surrogates"), # DC00..DFFF
			self.getBlockByName("Private Use Area"), # E000..F8FF
			self.getBlockByName("CJK Compatibility Ideographs"), # F900..FAFF
			self.getBlockByName("Alphabetic Presentation Forms"), # FB00..FB4F
			self.getBlockByName("Arabic Presentation Forms-A"), # FB50..FDFF
			self.getBlockByName("Variation Selectors"), # FE00..FE0F
			self.getBlockByName("Vertical Forms"), # FE10..FE1F
			self.getBlockByName("Combining Half Marks"), # FE20..FE2F
			self.getBlockByName("CJK Compatibility Forms"), # FE30..FE4F
			self.getBlockByName("Small Form Variants"), # FE50..FE6F
			self.getBlockByName("Arabic Presentation Forms-B"), # FE70..FEFF
			self.getBlockByName("Halfwidth and Fullwidth Forms"), # FF00..FFEF
			self.getBlockByName("Specials"), # FFF0..FFFF
			self.getBlockByName("Linear B Syllabary"), # 10000..1007F
			self.getBlockByName("Linear B Ideograms"), # 10080..100FF
			self.getBlockByName("Aegean Numbers"), # 10100..1013F
			self.getBlockByName("Ancient Greek Numbers"), # 10140..1018F
			self.getBlockByName("Ancient Symbols"), # 10190..101CF
			self.getBlockByName("Phaistos Disc"), # 101D0..101FF
			self.getBlockByName("Lycian"), # 10280..1029F
			self.getBlockByName("Carian"), # 102A0..102DF
			self.getBlockByName("Coptic Epact Numbers"), # 102E0..102FF
			self.getBlockByName("Old Italic"), # 10300..1032F
			self.getBlockByName("Gothic"), # 10330..1034F
			self.getBlockByName("Old Permic"), # 10350..1037F
			self.getBlockByName("Ugaritic"), # 10380..1039F
			self.getBlockByName("Old Persian"), # 103A0..103DF
			self.getBlockByName("Deseret"), # 10400..1044F
			self.getBlockByName("Shavian"), # 10450..1047F
			self.getBlockByName("Osmanya"), # 10480..104AF
			self.getBlockByName("Elbasan"), # 10500..1052F
			self.getBlockByName("Caucasian Albanian"), # 10530..1056F
			self.getBlockByName("Linear A"), # 10600..1077F
			self.getBlockByName("Cypriot Syllabary"), # 10800..1083F
			self.getBlockByName("Imperial Aramaic"), # 10840..1085F
			self.getBlockByName("Palmyrene"), # 10860..1087F
			self.getBlockByName("Nabataean"), # 10880..108AF
			self.getBlockByName("Phoenician"), # 10900..1091F
			self.getBlockByName("Lydian"), # 10920..1093F
			self.getBlockByName("Meroitic Hieroglyphs"), # 10980..1099F
			self.getBlockByName("Meroitic Cursive"), # 109A0..109FF
			self.getBlockByName("Kharoshthi"), # 10A00..10A5F
			self.getBlockByName("Old South Arabian"), # 10A60..10A7F
			self.getBlockByName("Old North Arabian"), # 10A80..10A9F
			self.getBlockByName("Manichaean"), # 10AC0..10AFF
			self.getBlockByName("Avestan"), # 10B00..10B3F
			self.getBlockByName("Inscriptional Parthian"), # 10B40..10B5F
			self.getBlockByName("Inscriptional Pahlavi"), # 10B60..10B7F
			self.getBlockByName("Psalter Pahlavi"), # 10B80..10BAF
			self.getBlockByName("Old Turkic"), # 10C00..10C4F
			self.getBlockByName("Rumi Numeral Symbols"), # 10E60..10E7F
			self.getBlockByName("Brahmi"), # 11000..1107F
			self.getBlockByName("Kaithi"), # 11080..110CF
			self.getBlockByName("Sora Sompeng"), # 110D0..110FF
			self.getBlockByName("Chakma"), # 11100..1114F
			self.getBlockByName("Mahajani"), # 11150..1117F
			self.getBlockByName("Sharada"), # 11180..111DF
			self.getBlockByName("Sinhala Archaic Numbers"), # 111E0..111FF
			self.getBlockByName("Khojki"), # 11200..1124F
			self.getBlockByName("Khudawadi"), # 112B0..112FF
			self.getBlockByName("Grantha"), # 11300..1137F
			self.getBlockByName("Tirhuta"), # 11480..114DF
			self.getBlockByName("Siddham"), # 11580..115FF
			self.getBlockByName("Modi"), # 11600..1165F
			self.getBlockByName("Takri"), # 11680..116CF
			self.getBlockByName("Warang Citi"), # 118A0..118FF
			self.getBlockByName("Pau Cin Hau"), # 11AC0..11AFF
			self.getBlockByName("Cuneiform"), # 12000..123FF
			self.getBlockByName("Cuneiform Numbers and Punctuation"), # 12400..1247F
			self.getBlockByName("Egyptian Hieroglyphs"), # 13000..1342F
			self.getBlockByName("Bamum Supplement"), # 16800..16A3F
			self.getBlockByName("Mro"), # 16A40..16A6F
			self.getBlockByName("Bassa Vah"), # 16AD0..16AFF
			self.getBlockByName("Pahawh Hmong"), # 16B00..16B8F
			self.getBlockByName("Miao"), # 16F00..16F9F
			self.getBlockByName("Kana Supplement"), # 1B000..1B0FF
			self.getBlockByName("Duployan"), # 1BC00..1BC9F
			self.getBlockByName("Shorthand Format Controls"), # 1BCA0..1BCAF
			self.getBlockByName("Byzantine Musical Symbols"), # 1D000..1D0FF
			self.getBlockByName("Musical Symbols"), # 1D100..1D1FF
			self.getBlockByName("Ancient Greek Musical Notation"), # 1D200..1D24F
			self.getBlockByName("Tai Xuan Jing Symbols"), # 1D300..1D35F
			self.getBlockByName("Counting Rod Numerals"), # 1D360..1D37F
			self.getBlockByName("Mathematical Alphanumeric Symbols"), # 1D400..1D7FF
			self.getBlockByName("Mende Kikakui"), # 1E800..1E8DF
			self.getBlockByName("Arabic Mathematical Alphabetic Symbols"), # 1EE00..1EEFF
			self.getBlockByName("Mahjong Tiles"), # 1F000..1F02F
			self.getBlockByName("Domino Tiles"), # 1F030..1F09F
			self.getBlockByName("Playing Cards"), # 1F0A0..1F0FF
			self.getBlockByName("Enclosed Alphanumeric Supplement"), # 1F100..1F1FF
			self.getBlockByName("Enclosed Ideographic Supplement"), # 1F200..1F2FF
			self.getBlockByName("Miscellaneous Symbols and Pictographs"), # 1F300..1F5FF
			self.getBlockByName("Emoticons"), # 1F600..1F64F
			self.getBlockByName("Ornamental Dingbats"), # 1F650..1F67F
			self.getBlockByName("Transport and Map Symbols"), # 1F680..1F6FF
			self.getBlockByName("Alchemical Symbols"), # 1F700..1F77F
			self.getBlockByName("Geometric Shapes Extended"), # 1F780..1F7FF
			self.getBlockByName("Supplemental Arrows-C"), # 1F800..1F8FF
			self.getBlockByName("CJK Unified Ideographs Extension B"), # 20000..2A6DF
			self.getBlockByName("CJK Unified Ideographs Extension C"), # 2A700..2B73F
			self.getBlockByName("CJK Unified Ideographs Extension D"), # 2B740..2B81F
			self.getBlockByName("CJK Compatibility Ideographs Supplement"), # 2F800..2FA1F
			self.getBlockByName("Tags"), # E0000..E007F
			self.getBlockByName("<reserved-E0080>..<reserved-E00FF>"), # E0080..E00FF
			self.getBlockByName("Variation Selectors Supplement"), # E0100..E01EF
			self.getBlockByName("<reserved-E01F0>..<reserved-E0FFF>"), # E01F0..E0FFF
			self.getBlockByName("Supplementary Private Use Area-A"), # F0000..FFFFF
			self.getBlockByName("Supplementary Private Use Area-B"), # 100000..10FFFF
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
		print('Resolving blocks for entries...')
		
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

		block_reserved3 = UnicodeBlock(self)
		block_reserved3.start = 0x0860
		block_reserved3.end = 0x089F
		block_reserved3.name = "<reserved-0860>..<reserved-089F>"
		self.blocks.append(block_reserved3)

		block_reserved4 = UnicodeBlock(self)
		block_reserved4.start = 0x1C80
		block_reserved4.end = 0x1CBF
		block_reserved4.name = "<reserved-1C80>..<reserved-1CBF>"
		self.blocks.append(block_reserved4)

		block_reserved5 = UnicodeBlock(self)
		block_reserved5.start = 0x2FE0
		block_reserved5.end = 0x2FEF
		block_reserved5.name = "<reserved-2FE0>..<reserved-2FEF>"
		self.blocks.append(block_reserved5)

		block_reserved6 = UnicodeBlock(self)
		block_reserved6.start = 0xAB70
		block_reserved6.end = 0xABBF
		block_reserved6.name = "<reserved-AB70>..<reserved-ABBF>"
		self.blocks.append(block_reserved6)
	
	def resolveQuickCheck(self):
		print('Resolving quick check entries...')
		
		# NFC
		
		self.qcNFCRecords = sorted(self.qcNFCRecords, key=lambda record: record.start)
		nfc_length = len(self.qcNFCRecords)
		nfc_last = self.qcNFCRecords[nfc_length - 1]
		nfc_last.end = nfc_last.start + nfc_last.count
		
		for i in range(0, nfc_length - 1):
			current = self.qcNFCRecords[i]
			self.qcNFCRecords[i].end = self.qcNFCRecords[i + 1].start - 1

		for n in self.qcNFCRecords:
			for r in self.recordsOrdered[n.start:n.start + n.count]:
				r.quickNFC = n.value
		
		# NFD
		
		self.qcNFDRecords = sorted(self.qcNFDRecords, key=lambda record: record.start)
		nfd_length = len(self.qcNFDRecords)
		nfd_last = self.qcNFDRecords[nfd_length - 1]
		nfd_last.end = nfd_last.start + nfd_last.count
		
		for i in range(0, nfd_length - 1):
			current = self.qcNFDRecords[i]
			self.qcNFDRecords[i].end = self.qcNFDRecords[i + 1].start - 1

		for n in self.qcNFDRecords:
			for r in self.recordsOrdered[n.start:n.start + n.count]:
				r.quickNFD = n.value
		
		# NFKC
		
		self.qcNFKCRecords = sorted(self.qcNFKCRecords, key=lambda record: record.start)
		nfkc_length = len(self.qcNFKCRecords)
		nfkc_last = self.qcNFKCRecords[nfkc_length - 1]
		nfkc_last.end = nfkc_last.start + nfkc_last.count
		
		for i in range(0, nfkc_length - 1):
			current = self.qcNFKCRecords[i]
			self.qcNFKCRecords[i].end = self.qcNFKCRecords[i + 1].start - 1

		for n in self.qcNFKCRecords:
			for r in self.recordsOrdered[n.start:n.start + n.count]:
				r.quickNFKC = n.value
		
		# NFKD
		
		self.qcNFKDRecords = sorted(self.qcNFKDRecords, key=lambda record: record.start)
		nfkd_length = len(self.qcNFKDRecords)
		nfkd_last = self.qcNFKDRecords[nfkd_length - 1]
		nfkd_last.end = nfkd_last.start + nfkd_last.count
		
		for i in range(0, nfkd_length - 1):
			current = self.qcNFKDRecords[i]
			self.qcNFKDRecords[i].end = self.qcNFKDRecords[i + 1].start - 1

		for n in self.qcNFKDRecords:
			for r in self.recordsOrdered[n.start:n.start + n.count]:
				r.quickNFKD = n.value
		
	def resolveDecomposition(self):
		print('Resolving decomposition...')
		
		for r in self.recordsOrdered:
			r.decompose()
			
			convertedCodepoint = libs.utf8.codepointToUtf8Hex(r.codepoint)
			
			r.offsetNFD = 0
			r.offsetNFKD = 0
			
			if r.decomposedNFD:
				convertedNFD = libs.utf8.unicodeToUtf8Hex(r.decomposedNFD)
				if convertedNFD != convertedCodepoint:
					r.offsetNFD = self.addTranslation(convertedNFD + "\\x00")
			
			if r.decomposedNFKD:
				convertedNFKD = libs.utf8.unicodeToUtf8Hex(r.decomposedNFKD)
				if convertedNFKD != convertedCodepoint:
					r.offsetNFKD = self.addTranslation(convertedNFKD + "\\x00")
	
	def resolveComposition(self):
		print('Resolving composition...')
		
		for r in self.recordsOrdered:
			r.compose()
	
	def resolveCaseMapping(self):
		print('Resolving case mappings...')
		
		for r in self.recordsOrdered:
			r.caseMapping()
	
	def resolveProperties(self):
		print('Resolving codepoint properties...')
		
		group_category = None
		group_ccc = None
		
		for r in self.recordsOrdered:
			if r.generalCategoryCombined:
				if not group_category or r.codepoint != (group_category.start + group_category.count + 1) or r.generalCategoryCombined != group_category.value:
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
				if not group_ccc or r.codepoint != (group_ccc.start + group_ccc.count + 1) or r.canonicalCombiningClass != group_ccc.value:
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
		
		print(found.name + " " + hex(found.codepoint) + " " + type + " \"" + found.decomposedToString() + "\"")
		
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
			print(found)
			print("Canonical:")
			print(self.resolveCodepoint(queryCodepoint, False))
			print("Compatibility:")
			print(self.resolveCodepoint(queryCodepoint, True))

	def matchToString(self, match):
		if match == None:
			return ""
		
		codepoints = []
		
		for group in match:
			if group != None:
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
				print("hashing " + translation + " offset " + str(self.offset))
			
			self.hashed[translation] = result
			self.offset += offset
			self.blob += translation
		else:
			result = self.hashed[translation]
		
		if self.verbose:
			print("translated", translation, "offset", result)
		
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
			if count != len(records):
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
						print('"collision " + hex(key)')
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
			if count != len(composed_ordered):
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
			if count != len(records):
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
		print('Writing database to "' + os.path.realpath(filepath) + '"...')
		
		nfd_records = []
		nfkd_records = []
		uppercase_records = []
		lowercase_records = []
		titlecase_records = []
		
		for r in self.recordsOrdered:
			if r.offsetNFD != 0:
				nfd_records.append(r)
			
			if r.offsetNFKD != 0:
				nfkd_records.append(r)
			
			if r.offsetUppercase != 0:
				uppercase_records.append(r)
			
			if r.offsetLowercase != 0:
				lowercase_records.append(r)
			
			if r.offsetTitlecase != 0:
				titlecase_records.append(r)
		
		sliced = libs.blobsplitter.BlobSplitter()
		sliced.split(self.blob, self.offset)
		
		# comment header
		
		header = libs.header.Header(os.path.realpath(filepath))
		
		header.writeLine("/*")
		header.indent()
		header.copyrightNotice()
		header.outdent()
		header.writeLine("*/")
		header.newLine()
		
		header.generatedNotice()
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
		print('Writing case mapping to "' + os.path.realpath(filepath) + '"...')
		
		command_line = os.path.relpath(os.path.realpath(sys.argv[0]), os.getcwd())

		for a in sys.argv[1:]:
			command_line += " " + a
		
		d = datetime.datetime.now()
		
		output = libs.header.Header(os.path.realpath(filepath))
		
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

	def writeCompressed(self, filepath):
		print('Compressing code point properties...')

		compress_gc = Compression(db)
		compress_gc.process('generalCategoryCombined', 32)
		
		compress_ccc = Compression(db)
		compress_ccc.process('canonicalCombiningClass', 32)

		compress_nfc = Compression(db)
		compress_nfc.process('quickNFC', 32)

		compress_nfd = Compression(db)
		compress_nfd.process('quickNFD', 32)

		compress_nfkc = Compression(db)
		compress_nfkc.process('quickNFKC', 32)

		compress_nfkd = Compression(db)
		compress_nfkd.process('quickNFKD', 32)

		print('Writing compresed data to "' + os.path.realpath(filepath) + '"...')

		header = libs.header.Header(os.path.realpath(filepath))
		header.generatedNotice()

		header.newLine()
		header.writeLine("#include \"compressedproperties.h\"")

		compress_gc.render(header, 'GeneralCategory')
		header.newLine()
		
		compress_ccc.render(header, 'CanonicalCombiningClass')
		header.newLine()

		compress_nfc.render(header, 'QuickCheckNFC')
		header.newLine()

		compress_nfd.render(header, 'QuickCheckNFD')
		header.newLine()

		compress_nfkc.render(header, 'QuickCheckNFKC')
		header.newLine()

		compress_nfkd.render(header, 'QuickCheckNFKD')

class SpecialCasing(libs.unicode.UnicodeVisitor):
	def __init__(self, db):
		self.db = db
	
	def visitDocument(self, document):
		print("Parsing special case mappings...")
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
		print("Parsing block mappings...")
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
					print("missing " + hex(start) + " in database (\"" + self.db.getBlockByCodepoint(start).name + "\")")
		
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
		print("Parsing derived normalization properties...")
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
	db.writeCompressed(script_path + '/../../source/internal/compressedproperties.c')
	db.writeCaseMapping(script_path + '/../../testdata/CaseMapping.txt')