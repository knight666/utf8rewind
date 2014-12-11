import datetime
import os
import sys
import unicodedata
import libs.header
import libs.unicode
import libs.utf8

def codepointToUnicode(codepoint):
	return "U+%04X" % codepoint

class IntegrationSuite:
	def __init__(self, db):
		self.db = db
		self.checkValid = False
	
	def open(self, filepath):
		command_line = sys.argv[0]
		arguments = sys.argv[1:]
		for a in arguments:
			command_line += " " + a
		
		d = datetime.datetime.now()
		
		script_path = os.path.dirname(os.path.realpath(sys.argv[0]))
		
		self.header = libs.header.Header(script_path + filepath)
		self.header.writeLine("/*")
		self.header.indent()
		self.header.writeLine("DO NOT MODIFY, AUTO-GENERATED")
		self.header.newLine()
		self.header.writeLine("Generated on:")
		self.header.indent()
		self.header.writeLine(d.strftime("%Y-%m-%dT%H:%M:%S"))
		self.header.outdent()
		self.header.newLine()
		self.header.writeLine("Command line:")
		self.header.indent()
		self.header.writeLine(command_line)
		self.header.outdent()
		self.header.outdent()
		self.header.writeLine("*/")
		self.header.newLine()
		self.header.writeLine("#include \"tests-base.hpp\"")
		self.header.newLine()
		self.header.write("#include \"helpers-casemapping.hpp\"")
	
	def close(self):
		self.header.close()
	
	def writeTest(self, codepointRange, name):
		print "Writing tests " + codepointToUnicode(codepointRange[0]) + " - " + codepointToUnicode(codepointRange[len(codepointRange) - 1]) + " \"" + name + "\""
		
		valid_categories = [
			"GeneralCategory_NonspacingMark",
			"GeneralCategory_LetterNumber",
			"GeneralCategory_TitlecaseLetter",
			"GeneralCategory_UppercaseLetter",
			"GeneralCategory_OtherSymbol",
			"GeneralCategory_LowercaseLetter"
		]
		
		self.header.newLine()
		
		self.header.newLine()
		self.header.writeLine("TEST(CaseMapping, Uppercase" + name + ")")
		self.header.writeLine("{")
		self.header.indent()
		
		for i in codepointRange:
			if i not in self.db.records:
				continue
			
			r = self.db.records[i]
			
			if self.checkValid and r.generalCategory not in valid_categories:
				continue
			
			converted_codepoint = "0x%08x" % r.codepoint
			
			if r.uppercase:
				converted_uppercase = ""
				for u in r.uppercase:
					converted_uppercase += libs.utf8.codepointToUtf8(u)
			else:
				converted_uppercase = libs.utf8.codepointToUtf8(r.codepoint)
			
			self.header.writeLine("CHECK_UTF8_UPPERCASE(" + converted_codepoint + ", \"" + r.name + "\", \"" + converted_uppercase + "\");")
		
		self.header.outdent()
		self.header.writeLine("}")
		
		self.header.newLine()
		
		self.header.writeLine("TEST(CaseMapping, Lowercase" + name + ")")
		self.header.writeLine("{")
		self.header.indent()
		
		for i in codepointRange:
			if i not in self.db.records:
				continue
			
			r = self.db.records[i]
			
			if self.checkValid and r.generalCategory not in valid_categories:
				continue
			
			converted_codepoint = "0x%08x" % r.codepoint
			
			if r.lowercase:
				converted_lowercase = ""
				for u in r.lowercase:
					converted_lowercase += libs.utf8.codepointToUtf8(u)
			else:
				converted_lowercase = libs.utf8.codepointToUtf8(r.codepoint)
			
			self.header.writeLine("CHECK_UTF8_LOWERCASE(" + converted_codepoint + ", \"" + r.name + "\", \"" + converted_lowercase + "\");")
		
		self.header.outdent()
		self.header.write("}")

if __name__ == '__main__':
	db = unicodedata.Database()
	db.loadFromFiles(None)
	
	suite = IntegrationSuite(db)
	suite.open('/../../source/tests/integration-casemapping.cpp')
	suite.writeTest(range(0x00, 0x80), "Basic Latin")
	suite.writeTest(range(0x80, 0x100), "Latin-1 Supplement")
	suite.writeTest(range(0x100, 0x180), "Latin Extended-A")
	suite.writeTest(range(0x180, 0x250), "Latin Extended-B")
	suite.writeTest(range(0x250, 0x2B0), "IPA Extensions")
	suite.writeTest(range(0x300, 0x370), "Combining Diacritical Marks")
	suite.writeTest(range(0x370, 0x400), "Greek And Coptic")
	suite.writeTest(range(0x400, 0x500), "Cyrillic")
	suite.writeTest(range(0x500, 0x530), "CyrillicSupplement")
	suite.writeTest(range(0x530, 0x590), "Armenian")
	suite.writeTest(range(0x590, 0x600), "Hebrew")
	suite.writeTest(range(0x600, 0x700), "Arabic")
	suite.writeTest(range(0x700, 0x750), "Syriac")
	suite.writeTest(range(0x750, 0x780), "Arabic Supplement")
	suite.writeTest(range(0x780, 0x7C0), "Thaana")
	suite.writeTest(range(0x7C0, 0x800), "NKo")
	suite.writeTest(range(0x800, 0x840), "Samaritan")
	suite.writeTest(range(0x840, 0x860), "Mandaic")
	suite.writeTest(range(0x8A0, 0x900), "Arabic Extended-A")
	suite.writeTest(range(0x900, 0x980), "Devanagari")
	suite.writeTest(range(0x980, 0xA00), "Bengali")
	suite.writeTest(range(0xA00, 0xA7F), "Gurmukhi")
	suite.writeTest(range(0xA80, 0xB00), "Gujarati")
	suite.writeTest(range(0xB00, 0xB80), "Oriya")
	suite.writeTest(range(0xB80, 0xC00), "Tamil")
	suite.writeTest(range(0xC00, 0xC80), "Telugu")
	suite.writeTest(range(0xC80, 0xD00), "Kannada")
	suite.writeTest(range(0xD00, 0xD80), "Malayalam")
	suite.writeTest(range(0xD80, 0xE00), "Sinhala")
	suite.writeTest(range(0xE00, 0xE80), "Thai")
	suite.writeTest(range(0xE80, 0xF00), "Lao")
	suite.writeTest(range(0xF00, 0x1000), "Tibetan")
	suite.writeTest(range(0x1000, 0x10A0), "Myanmar")
	suite.writeTest(range(0x10A0, 0x1100), "Georgian")
	suite.writeTest(range(0x1100, 0x1200), "Hangul Jamo")
	suite.writeTest(range(0x1200, 0x1380), "Ethiopic")
	suite.writeTest(range(0x1380, 0x13A0), "Ethiopic Supplement")
	suite.writeTest(range(0x13A0, 0x1400), "Cherokee")
	suite.writeTest(range(0x1400, 0x1680), "Unified Canadian Aboriginal Syllabics")
	suite.writeTest(range(0x1680, 0x16A0), "Ogham")
	suite.writeTest(range(0x16A0, 0x1700), "Runic")
	suite.writeTest(range(0x1700, 0x1720), "Tagalog")
	suite.writeTest(range(0x1720, 0x1740), "Hanunoo")
	suite.writeTest(range(0x1740, 0x1760), "Buhid")
	suite.writeTest(range(0x1760, 0x1780), "Tagbanwa")
	suite.writeTest(range(0x1780, 0x1800), "Khmer")
	suite.writeTest(range(0x1800, 0x18B0), "Mongolian")
	suite.writeTest(range(0x18B0, 0x1900), "Unified Canadian Aboriginal Syllabics Extended")
	suite.writeTest(range(0x1900, 0x1950), "Limbu")
	suite.writeTest(range(0x1950, 0x1980), "Tai Le")
	suite.writeTest(range(0x1980, 0x19E0), "New Tai Le")
	suite.writeTest(range(0x19E0, 0x1A00), "Khmer Symbols")
	suite.writeTest(range(0x1A00, 0x1A20), "Buginese")
	suite.writeTest(range(0x1A20, 0x1AB0), "Tai Tham")
	suite.writeTest(range(0x1B00, 0x1B80), "Balinese")
	suite.writeTest(range(0x1B80, 0x1BC0), "Sundanese")
	suite.writeTest(range(0x1BC0, 0x1C00), "Batak")
	suite.writeTest(range(0x1C00, 0x1C50), "Lepcha")
	suite.writeTest(range(0x1C50, 0x1C80), "Ol Chiki")
	suite.writeTest(range(0x1CC0, 0x1CD0), "Sundanese Supplement")
	suite.writeTest(range(0x1CD0, 0x1D00), "Vedic Extensions")
	suite.writeTest(range(0x1D00, 0x1D80), "Phonetic Extensions")
	suite.writeTest(range(0x1D80, 0x1DC0), "Phonetic Extensions Supplement")
	suite.writeTest(range(0x1DC0, 0x1E00), "Combining Diacritical Marks Supplement")
	suite.writeTest(range(0x1E00, 0x1F00), "Latin Extended Additional")
	suite.writeTest(range(0x1F00, 0x2000), "Greek Extended")
	suite.writeTest(range(0x2000, 0x2070), "General Punctuation")
	suite.writeTest(range(0x2070, 0x20A0), "Superscripts and Subscripts")
	suite.writeTest(range(0x20A0, 0x20D0), "Currency Symbols")
	suite.writeTest(range(0x20D0, 0x2100), "Combining Diacritical Marks for Symbols")
	suite.writeTest(range(0x2100, 0x2150), "Letterlike Symbols")
	suite.writeTest(range(0x2150, 0x2190), "Number Forms")
	suite.writeTest(range(0x2190, 0x2200), "Arrows")
	suite.writeTest(range(0x2200, 0x2300), "Mathematical Operators")
	suite.writeTest(range(0x2300, 0x2400), "Miscellaneous Technical")
	suite.writeTest(range(0x2400, 0x2440), "Control Pictures")
	suite.writeTest(range(0x2440, 0x2460), "Optical Character Recognition")
	suite.writeTest(range(0x2460, 0x2500), "Enclosed Alphanumerics")
	suite.writeTest(range(0x2500, 0x2580), "Box Drawing")
	suite.writeTest(range(0x2580, 0x25A0), "Block Elements")
	suite.writeTest(range(0x25A0, 0x2600), "Geometric Shapes")
	suite.writeTest(range(0x2600, 0x2700), "Miscellaneous Symbols")
	suite.writeTest(range(0x2700, 0x27C0), "Dingbats")
	suite.writeTest(range(0x27C0, 0x27F0), "Miscellaneous Mathematical Symbols-A")
	suite.writeTest(range(0x27F0, 0x2800), "Supplemental Arrows-A")
	suite.writeTest(range(0x2800, 0x2900), "Braille Patterns")
	suite.writeTest(range(0x2900, 0x2980), "Supplemental Arrows-B")
	suite.writeTest(range(0x2980, 0x2A00), "Miscellaneous Mathematical Symbols-B")
	suite.writeTest(range(0x2A00, 0x2B00), "Supplemental Mathematical Operators")
	suite.writeTest(range(0x2B00, 0x2C00), "Miscellaneous Symbols And Arrows")
	suite.writeTest(range(0x2C00, 0x2C60), "Glagolitic")
	suite.writeTest(range(0x2C60, 0x2C80), "Latin Extended-C")
	suite.writeTest(range(0x2C80, 0x2D00), "Coptic")
	suite.writeTest(range(0x2D00, 0x2D30), "Georgian Supplement")
	suite.writeTest(range(0x2D30, 0x2D80), "Tifinagh")
	suite.writeTest(range(0x2D80, 0x2DE0), "Ethiopic Extended")
	suite.writeTest(range(0x2DE0, 0x2E00), "Cyrillic Extended-A")
	suite.writeTest(range(0x2E00, 0x2E80), "Supplemental Punctuation")
	suite.writeTest(range(0x2E80, 0x2F00), "CJK Radicals Supplement")
	suite.writeTest(range(0x2F00, 0x2FF0), "Kangxi Radicals")
	suite.writeTest(range(0x2FF0, 0x3000), "Ideographic Description Characters")
	suite.writeTest(range(0x3000, 0x3040), "CJK Symbols And Punctuation")
	suite.writeTest(range(0x3040, 0x30A0), "Hiragana")
	suite.writeTest(range(0x30A0, 0x3100), "Katakana")
	suite.writeTest(range(0x3100, 0x3130), "Bopomofo")
	suite.writeTest(range(0x3130, 0x3190), "Hangul Compatibility Jamo")
	suite.writeTest(range(0x3190, 0x31A0), "Kanbun")
	suite.writeTest(range(0x31A0, 0x31C0), "Bopomofo Extended")
	suite.writeTest(range(0x31C0, 0x31F0), "CJK Strokes")
	suite.writeTest(range(0x31F0, 0x3200), "Katakana Phonetic Extensions")
	suite.writeTest(range(0x3200, 0x3300), "Enclosed CJK Letters and Months")
	suite.writeTest(range(0x3300, 0x3400), "CJK Compatibility")
	suite.writeTest(range(0x3400, 0x4DC0), "CJK Unified Ideographs Extension-A")
	suite.writeTest(range(0x4DC0, 0x4E00), "Yijing Hexagram Symbols")
	suite.writeTest(range(0x4E00, 0xA000), "CJK Unified Ideographs")
	suite.writeTest(range(0xA000, 0xA490), "Yi Syllables")
	suite.writeTest(range(0xA490, 0xA4D0), "Yi Radicals")
	suite.writeTest(range(0xA4D0, 0xA500), "Lisu")
	suite.writeTest(range(0xA500, 0xA640), "Vai")
	suite.writeTest(range(0xA640, 0xA6A0), "Cyrillic Extended-B")
	suite.writeTest(range(0xA6A0, 0xA700), "Bamum")
	suite.writeTest(range(0xA700, 0xA720), "Modifier Tone Letters")
	suite.writeTest(range(0xA720, 0xA800), "Latin Extended-D")
	suite.writeTest(range(0xA800, 0xA830), "Syloti Nagri")
	suite.writeTest(range(0xA830, 0xA840), "Common Indic Number Forms")
	suite.writeTest(range(0xA840, 0xA880), "Phags-pa")
	suite.writeTest(range(0xA880, 0xA8E0), "Saurashtra")
	suite.writeTest(range(0xA8E0, 0xA900), "Devanagari Extended")
	suite.writeTest(range(0xA900, 0xA930), "Kayah Li")
	suite.writeTest(range(0xFB00, 0xFB50), "Alphabetic Presentation Forms")
	suite.writeTest(range(0xFF00, 0xFFF0), "Halfwidth And Fullwidth Forms")
	suite.writeTest(range(0x10400, 0x10450), "Deseret")
	suite.writeTest(range(0x118A0, 0x11900), "Warang Citi")
	suite.close()