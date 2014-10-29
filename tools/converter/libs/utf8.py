MAX_LEGAL_UNICODE = 0x10FFFF
REPLACEMENT_CHARACTER = 0xFFFD

def codepointToUtf8(codepoint):
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
		result = '\\x' + format(codepoint, '02x')
	elif encoded_length == 2:
		result += '\\x' + format((codepoint >>   6)         | 0xC0, '02x')
		result += '\\x' + format((codepoint         & 0x3F) | 0x80, '02x')
	elif encoded_length == 3:
		result += '\\x' + format((codepoint  >> 12)         | 0xE0, '02x')
		result += '\\x' + format(((codepoint >>  6) & 0x3F) | 0x80, '02x')
		result += '\\x' + format((codepoint         & 0x3F) | 0x80, '02x')
	elif encoded_length == 4:
		result += '\\x' + format((codepoint  >> 18)         | 0xF0, '02x')
		result += '\\x' + format((codepoint  >> 12)         | 0x80, '02x')
		result += '\\x' + format(((codepoint >>  6) & 0x3F) | 0x80, '02x')
		result += '\\x' + format((codepoint         & 0x3F) | 0x80, '02x')
	
	return result