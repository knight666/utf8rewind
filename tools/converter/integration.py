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
	suite.writeTest(range(0x00, 0x80), "BasicLatin")
	suite.writeTest(range(0x80, 0x100), "Latin1Supplement")
	suite.writeTest(range(0x100, 0x180), "LatinExtendedA")
	suite.writeTest(range(0x180, 0x250), "LatinExtendedB")
	suite.writeTest(range(0x250, 0x2B0), "IPAExtensions")
	suite.writeTest(range(0x300, 0x370), "CombiningDiacriticalMarks")
	suite.writeTest(range(0x370, 0x400), "GreekAndCoptic")
	suite.writeTest(range(0x400, 0x500), "Cyrillic")
	suite.writeTest(range(0x500, 0x530), "CyrillicSupplement")
	suite.writeTest(range(0x530, 0x590), "Armenian")
	suite.writeTest(range(0x10A0, 0x1100), "Georgian")
	suite.writeTest(range(0x1D00, 0x1D80), "PhoneticExtensions")
	suite.writeTest(range(0x1E00, 0x1F00), "LatinExtendedAdditional")
	suite.writeTest(range(0x1F00, 0x2000), "GreekExtended")
	suite.writeTest(range(0x2100, 0x2150), "LetterlikeSymbols")
	suite.writeTest(range(0x2150, 0x2190), "NumberForms")
	suite.writeTest(range(0x2460, 0x2500), "EnclosedAlphanumerics")
	suite.writeTest(range(0x2C00, 0x2C60), "Glagolitic")
	suite.writeTest(range(0x2C60, 0x2C80), "LatinExtendedC")
	suite.writeTest(range(0x2C80, 0x2D00), "Coptic")
	suite.writeTest(range(0x2D00, 0x2D30), "GeorgianSupplement")
	suite.writeTest(range(0xA640, 0xA6A0), "CyrillicExtendedB")
	suite.writeTest(range(0xA720, 0xA800), "LatinExtendedD")
	suite.writeTest(range(0xFB00, 0xFB50), "AlphabeticPresentationForms")
	suite.writeTest(range(0xFF00, 0xFFF0), "HalfwidthAndFullwidthForms")
	suite.writeTest(range(0x10400, 0x10450), "Deseret")
	suite.writeTest(range(0x118A0, 0x11900), "WarangCiti")
	suite.close()