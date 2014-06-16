#pragma once

// STL

#include <fstream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <vector>

// Google Test

#include <gtest/gtest.h>

// utf8rewind

#include "utf8rewind.h"

// Helpers

#if (UTF8_BYTE_ORDER == UTF8_BYTE_ORDER_LITTLE_ENDIAN)

UTF8_INLINE ucs2_t LittleEndianUcs2(ucs2_t input) { return input; }
UTF8_INLINE ucs2_t BigEndianUcs2(ucs2_t input)
{
	return (
		(input >> 8) | 
		(input << 8)
	);
}

UTF8_INLINE unicode_t LittleEndianUnicode(unicode_t input) { return input; }
UTF8_INLINE unicode_t BigEndianUnicode(unicode_t input)
{
	return (
		(input << 24) |
		(input >> 24) |
		((input >> 8) & 0x0000FF00) |
		((input << 8) & 0x00FF0000)
	);
}

#elif (UTF8_BYTE_ORDER == UTF8_BYTE_ORDER_BIG_ENDIAN)

UTF8_INLINE ucs2_t LittleEndianUcs2(ucs2_t input)
{
	return (
		(input >> 8) | 
		(input << 8)
	);
}
UTF8_INLINE ucs2_t BigEndianUcs2(uint16_t input) { return input; }

UTF8_INLINE unicode_t LittleEndianUnicode(unicode_t input)
{
	return (
		(input << 24) |
		(input >> 24) |
		((input >> 8) & 0x0000FF00) |
		((input << 8) & 0x00FF0000)
	);
}
UTF8_INLINE unicode_t BigEndianUnicode(unicode_t input) { return input; }

#endif