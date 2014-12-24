import re
import sys

MAX_LEGAL_UNICODE = 0x10FFFF
REPLACEMENT_CHARACTER = 0xFFFD

def codepointToUtf8(codepoint, wroteHex = False):
	encoded_length = 0
	result = ""
	
	if codepoint < 0x80:
		encoded_length = 1
	elif codepoint < 0x800:
		encoded_length = 2
	elif codepoint < 0x10000:
		encoded_length = 3
	elif codepoint <= MAX_LEGAL_UNICODE:
		encoded_length = 4
	else:
		codepoint = REPLACEMENT_CHARACTER
		encoded_length = 3
	
	if encoded_length == 1:
		conversion = {
			0x00: "",
			0x07: "\\a",
			0x08: "\\b",
			0x09: "\\t",
			0x0A: "\\n",
			0x0B: "\\v",
			0x0C: "\\f",
			0x0D: "\\r",
			0x22: "\\\"", # must be escaped
		}
		if codepoint in conversion:
			result += conversion[codepoint]
			
			return result,False
		elif codepoint < 0x20:
			result += '\\x' + format(codepoint, '02X')
		else:
			isHex = (codepoint >= 0x41 and codepoint <= 0x46) or (codepoint >= 0x61 and codepoint <= 0x76) or (codepoint >= 0x30 and codepoint <= 0x39)
			
			if wroteHex and isHex:
				result += "\" \""
			result += "%c" % codepoint
			
			return result,False
	elif encoded_length == 2:
		result += '\\x' + format((codepoint >>   6)         | 0xC0, '02X')
		result += '\\x' + format((codepoint         & 0x3F) | 0x80, '02X')
	elif encoded_length == 3:
		result += '\\x' + format((codepoint  >> 12)         | 0xE0, '02X')
		result += '\\x' + format(((codepoint >>  6) & 0x3F) | 0x80, '02X')
		result += '\\x' + format((codepoint         & 0x3F) | 0x80, '02X')
	elif encoded_length == 4:
		result += '\\x' + format((codepoint  >> 18)         | 0xF0, '02X')
		result += '\\x' + format(((codepoint >> 12) & 0x3F) | 0x80, '02X')
		result += '\\x' + format(((codepoint >>  6) & 0x3F) | 0x80, '02X')
		result += '\\x' + format((codepoint         & 0x3F) | 0x80, '02X')
	
	return result,True

def unicodeToUtf8(unicode):
	result = ""
	wroteHex = False
	for u in unicode:
		converted,wroteHex = codepointToUtf8(u, wroteHex)
		result += converted
	return result

if __name__ == '__main__':
	codepoint = int(sys.argv[1], 16)
	print hex(codepoint)
	print codepointToUtf8(codepoint)
