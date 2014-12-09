import datetime
import os
import sys
import unicodedata
import libs.header
import libs.unicode
import libs.utf8

class IntegrationSuite:
	def __init__(self, db):
		self.db = db
	
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
	
	def close(self):
		self.header.close()
	
	def writeTest(self, codepointRange, name):
		self.header.writeLine("TEST(AllCaseMapping, " + name + ")")
		self.header.writeLine("{")
		self.header.indent()
		
		for i in codepointRange:
			r = self.db.records[i]
			
			converted_codepoint = "0x%08x" % r.codepoint
			
			if r.uppercase:
				converted_uppercase = ""
				for u in r.uppercase:
					converted_uppercase += libs.utf8.codepointToUtf8(u)
			else:
				converted_uppercase = libs.utf8.codepointToUtf8(r.codepoint)
			
			if r.lowercase:
				converted_lowercase = ""
				for u in r.lowercase:
					converted_lowercase += libs.utf8.codepointToUtf8(u)
			else:
				converted_lowercase = libs.utf8.codepointToUtf8(r.codepoint)
			
			self.header.writeLine("EXPECT_UTF8_CASEMAPPING(" + converted_codepoint + ", \"" + converted_uppercase + "\", \"" + converted_lowercase + "\"); // " + r.name)
		
		self.header.outdent()
		self.header.writeLine("}")

if __name__ == '__main__':
	db = unicodedata.Database()
	db.loadFromFiles(None)
	
	suite = IntegrationSuite(db)
	suite.open('/../../source/tests/integration-uppercase.cpp')
	suite.writeTest(range(0, 0x7F), "BasicLatin")
	suite.close()		