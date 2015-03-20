#include "tests-base.hpp"

#include "utf8rewind.h"

#include "helpers-strings.hpp"

TEST(Utf32ToUtf8, BasicLatinSingle)
{
	unicode_t i[] = { 'U' };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors;

	EXPECT_EQ(1, utf32toutf8(i, is, o, os, &errors));
	EXPECT_UTF8EQ("U", o);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, BasicLatinSingleFirst)
{
	unicode_t i[] = { 0x00 };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors;

	EXPECT_EQ(1, utf32toutf8(i, is, o, os, &errors));
	EXPECT_UTF8EQ("", o);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, BasicLatinSingleLast)
{
	unicode_t i[] = { 0x7F };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors;

	EXPECT_EQ(1, utf32toutf8(i, is, o, os, &errors));
	EXPECT_UTF8EQ("\x7F", o);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, BasicLatinSingleAmountOfBytes)
{
	unicode_t i[] = { '%' };
	size_t is = sizeof(i);
	int32_t errors;

	EXPECT_EQ(1, utf32toutf8(i, is, nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, BasicLatinSingleNotEnoughSpace)
{
	unicode_t i[] = { '#' };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 0;
	int32_t errors;

	EXPECT_EQ(0, utf32toutf8(i, is, o, os, &errors));
	EXPECT_UTF8EQ("", o);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf32ToUtf8, BasicLatinMultiple)
{
	unicode_t i[] = { 'W', 'o', 'r', 'k', '!' };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors;

	EXPECT_EQ(5, utf32toutf8(i, is, o, os, &errors));
	EXPECT_UTF8EQ("Work!", o);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, BasicLatinMultipleAmountOfBytes)
{
	unicode_t i[] = { 'F', 'r', 'o', 'g' };
	size_t is = sizeof(i);
	int32_t errors;

	EXPECT_EQ(4, utf32toutf8(i, is, nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, BasicLatinMultipleNotEnoughSpace)
{
	unicode_t i[] = { 'W', 'e', 'a', 't', 'h', 'e', 'r' };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 5;
	int32_t errors;

	EXPECT_EQ(5, utf32toutf8(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf32ToUtf8, TwoBytesSingle)
{
	// GREEK CAPITAL LETTER OMEGA

	unicode_t i[] = { 0x03A9 };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors;

	EXPECT_EQ(2, utf32toutf8(i, is, o, os, &errors));
	EXPECT_UTF8EQ("\xCE\xA9", o);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, TwoBytesSingleFirst)
{
	unicode_t i[] = { 0x0080 };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors;

	EXPECT_EQ(2, utf32toutf8(i, is, o, os, &errors));
	EXPECT_UTF8EQ("\xC2\x80", o);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, TwoBytesSingleLast)
{
	unicode_t i[] = { 0x07FF };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors;

	EXPECT_EQ(2, utf32toutf8(i, is, o, os, &errors));
	EXPECT_UTF8EQ("\xDF\xBF", o);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, TwoBytesSingleAmountOfBytes)
{
	// LATIN CAPITAL LETTER D WITH HOOK

	unicode_t i[] = { 0x018A };
	size_t is = sizeof(i);
	int32_t errors;

	EXPECT_EQ(2, utf32toutf8(i, is, nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, TwoBytesSingleNotEnoughSpace)
{
	// CYRILLIC SMALL LETTER STRAIGHT U WITH STROKE

	unicode_t i[] = { 0x04B1 };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 1;
	int32_t errors;

	EXPECT_EQ(0, utf32toutf8(i, is, o, os, &errors));
	EXPECT_UTF8EQ("", o);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf32ToUtf8, TwoBytesMultiple)
{
	// LATIN SMALL LETTER U WITH TILDE
	// LATIN CAPITAL LETTER O WITH BREVE
	// LATIN CAPITAL LETTER F WITH HOOK
	// LATIN CAPITAL LETTER ENG

	unicode_t i[] = { 0x0169, 0x014E, 0x0191, 0x014A };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors;

	EXPECT_EQ(8, utf32toutf8(i, is, o, os, &errors));
	EXPECT_UTF8EQ("\xC5\xA9\xC5\x8E\xC6\x91\xC5\x8A", o);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, TwoBytesMultipleAmountOfBytes)
{
	// CYRILLIC SMALL LETTER KA
	// LATIN SMALL LETTER Z WITH RETROFLEX HOOK
	// GREEK CAPITAL LETTER RHO

	unicode_t i[] = { 0x043A, 0x0290, 0x03A1 };
	size_t is = sizeof(i);
	int32_t errors;

	EXPECT_EQ(6, utf32toutf8(i, is, nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, TwoBytesMultipleNotEnoughSpace)
{
	// SMALL TILDE
	// LATIN SMALL LETTER OPEN E
	// LATIN LETTER SMALL CAPITAL I
	// LATIN CAPITAL LETTER H WITH CARON

	unicode_t i[] = { 0x02DC, 0x025B, 0x026A, 0x021E };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 5;
	int32_t errors;

	EXPECT_EQ(4, utf32toutf8(i, is, o, os, &errors));
	EXPECT_UTF8EQ("\xCB\x9C\xC9\x9B", o);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf32ToUtf8, ThreeBytesSingle)
{
	// IDEOGRAPHIC TELEGRAPH SYMBOL FOR DAY ONE

	unicode_t i[] = { 0x33E0 };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors;

	EXPECT_EQ(3, utf32toutf8(i, is, o, os, &errors));
	EXPECT_UTF8EQ("\xE3\x8F\xA0", o);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, ThreeBytesSingleFirst)
{
	// SAMARITAN LETTER ALAF

	unicode_t i[] = { 0x0800 };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors;

	EXPECT_EQ(3, utf32toutf8(i, is, o, os, &errors));
	EXPECT_UTF8EQ("\xE0\xA0\x80", o);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, ThreeBytesSingleLast)
{
	unicode_t i[] = { 0xFFFF };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors;

	EXPECT_EQ(3, utf32toutf8(i, is, o, os, &errors));
	EXPECT_UTF8EQ("\xEF\xBF\xBF", o);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, ThreeBytesSingleAmountOfBytes)
{
	// TIBETAN SIGN RDEL NAG RDEL DKAR

	unicode_t i[] = { 0x0FCE };
	size_t is = sizeof(i);
	int32_t errors;

	EXPECT_EQ(3, utf32toutf8(i, is, nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, ThreeBytesSingleNotEnoughSpace)
{
	// WHITE SQUARE WITH CENTRE VERTICAL LINE

	unicode_t i[] = { 0x2385 };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 2;
	int32_t errors;

	EXPECT_EQ(0, utf32toutf8(i, is, o, os, &errors));
	EXPECT_UTF8EQ("", o);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf32ToUtf8, ThreeBytesMultiple)
{
	// DINGBAT NEGATIVE CIRCLED DIGIT ONE
	// DINGBAT NEGATIVE CIRCLED DIGIT THREE
	// BLACK HEART SUIT
	// DINGBAT NEGATIVE CIRCLED DIGIT EIGHT

	unicode_t i[] = { 0x2776, 0x2778, 0x2665, 0x277D };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors;

	EXPECT_EQ(12, utf32toutf8(i, is, o, os, &errors));
	EXPECT_UTF8EQ("\xE2\x9D\xB6\xE2\x9D\xB8\xE2\x99\xA5\xE2\x9D\xBD", o);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, ThreeBytesMultipleAmountOfBytes)
{
	// EIGHTH NOTE
	// EIGHTH NOTE
	// BEAMED SIXTEENTH NOTES
	// BEAMED SIXTEENTH NOTES
	// BEAMED SIXTEENTH NOTES
	// BEAMED SIXTEENTH NOTES

	unicode_t i[] = { 0x266A, 0x266A, 0x266C, 0x266C, 0x266C, 0x266C };
	size_t is = sizeof(i);
	int32_t errors;

	EXPECT_EQ(18, utf32toutf8(i, is, nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, ThreeBytesMultipleNotEnoughSpace)
{
	// INVERTED PENTAGRAM
	// UNIVERSAL RECYCLING SYMBOL
	// STAFF OF AESCULAPIUS
	// DIE FACE-6

	unicode_t i[] = { 0x26E7, 0x2672, 0x2695, 0x2685 };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 11;
	int32_t errors;

	EXPECT_EQ(9, utf32toutf8(i, is, o, os, &errors));
	EXPECT_UTF8EQ("\xE2\x9B\xA7\xE2\x99\xB2\xE2\x9A\x95", o);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf32ToUtf8, FourBytesSingle)
{
	// MATHEMATICAL BOLD SMALL K

	unicode_t i[] = { 0x1D424 };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors;

	EXPECT_EQ(4, utf32toutf8(i, is, o, os, &errors));
	EXPECT_UTF8EQ("\xF0\x9D\x90\xA4", o);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, FourBytesSingleFirst)
{
	// 10000;LINEAR B SYLLABLE B008 A

	unicode_t i[] = { 0x10000 };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors;

	EXPECT_EQ(4, utf32toutf8(i, is, o, os, &errors));
	EXPECT_UTF8EQ("\xF0\x90\x80\x80", o);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, FourByteSingleLast)
{
	unicode_t i[] = { 0x10FFFF };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors;

	EXPECT_EQ(4, utf32toutf8(i, is, o, os, &errors));
	EXPECT_UTF8EQ("\xF4\x8F\xBF\xBF", o);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, FourByteSingleAmountOfBytes)
{
	// CUNEIFORM SIGN AB TIMES GAL

	unicode_t i[] = { 0x1200D };
	size_t is = sizeof(i);
	int32_t errors;

	EXPECT_EQ(4, utf32toutf8(i, is, nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, FourByteSingleNotEnoughSpace)
{
	// EGYPTIAN HIEROGLYPH X008A

	unicode_t i[] = { 0x133DA };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 3;
	int32_t errors;

	EXPECT_EQ(0, utf32toutf8(i, is, o, os, &errors));
	EXPECT_UTF8EQ("", o);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf32ToUtf8, FourBytesMultiple)
{
	// SQUARED CL
	// SQUARED SOS
	// SQUARED VS

	unicode_t i[] = { 0x1F191, 0x1F198, 0x1F19A };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors;

	EXPECT_EQ(12, utf32toutf8(i, is, o, os, &errors));
	EXPECT_UTF8EQ("\xF0\x9F\x86\x91\xF0\x9F\x86\x98\xF0\x9F\x86\x9A", o);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, FourBytesMultipleAmountOfBytes)
{
	// LAST QUARTER MOON SYMBOL
	// CLOUD WITH RAIN
	// SLICE OF PIZZA
	// SOFT ICE CREAM

	unicode_t i[] = { 0x1F317, 0x1F327, 0x1F355, 0x1F366 };
	size_t is = sizeof(i);
	int32_t errors;

	EXPECT_EQ(16, utf32toutf8(i, is, nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, FourBytesMultipleNotEnoughSpace)
{
	// NEGATIVE CIRCLED LATIN CAPITAL LETTER F
	// SQUARED MV
	// CINEMA
	// CHERRIES

	unicode_t i[] = { 0x1F155, 0x1F14B, 0x1F3A6, 0x1F352 };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 9;
	int32_t errors;

	EXPECT_EQ(8, utf32toutf8(i, is, o, os, &errors));
	EXPECT_UTF8EQ("\xF0\x9F\x85\x95\xF0\x9F\x85\x8B", o);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf32ToUtf8, AboveLegalUnicodeSingle)
{
	unicode_t i[] = { 0x110001 };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors;

	EXPECT_EQ(3, utf32toutf8(i, is, o, os, &errors));
	EXPECT_UTF8EQ("\xEF\xBF\xBD", o);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, AboveLegalUnicodeSingleAmountOfBytes)
{
	unicode_t i[] = { 0x199128 };
	size_t is = sizeof(i);
	int32_t errors;

	EXPECT_EQ(3, utf32toutf8(i, is, nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, AboveLegalUnicodeSingleNotEnoughSpace)
{
	unicode_t i[] = { 0x1D2D3D4D };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 1;
	int32_t errors;

	EXPECT_EQ(0, utf32toutf8(i, is, o, os, &errors));
	EXPECT_UTF8EQ("", o);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf32ToUtf8, AboveLegalUnicodeMultiple)
{
	unicode_t i[] = { 0x0ADA108A, 0xBADBADBA, 0xDEADBEA7 };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors;

	EXPECT_EQ(9, utf32toutf8(i, is, o, os, &errors));
	EXPECT_UTF8EQ("\xEF\xBF\xBD\xEF\xBF\xBD\xEF\xBF\xBD", o);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, AboveLegalUnicodeMultipleAmountOfBytes)
{
	unicode_t i[] = { 0x55518281, 0x10000000, 0xAEDBCAED };
	size_t is = sizeof(i);
	int32_t errors;

	EXPECT_EQ(9, utf32toutf8(i, is, nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, AboveLegalUnicodeMultipleNotEnoughSpace)
{
	unicode_t i[] = { 0x25AE1DD1, 0xAB18ECD9 };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 0;
	int32_t errors;

	EXPECT_EQ(0, utf32toutf8(i, is, o, os, &errors));
	EXPECT_UTF8EQ("", o);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf32ToUtf8, SurrogatePairSingle)
{
	unicode_t i[] = { 0xD834, 0xDE45 };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors;

	EXPECT_EQ(4, utf32toutf8(i, is, o, os, &errors));
	EXPECT_UTF8EQ("\xF0\x9D\x89\x85", o);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, SurrogatePairSingleFirst)
{
	unicode_t i[] = { 0xD800, 0xDC00 };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors;

	EXPECT_EQ(4, utf32toutf8(i, is, o, os, &errors));
	EXPECT_UTF8EQ("\xF0\x90\x80\x80", o);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, SurrogatePairSingleLast)
{
	unicode_t i[] = { 0xDBFF, 0xDFFF };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors;

	EXPECT_EQ(4, utf32toutf8(i, is, o, os, &errors));
	EXPECT_UTF8EQ("\xF4\x8F\xBF\xBF", o);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, SurrogatePairSingleAmountOfBytes)
{
	unicode_t i[] = { 0xD9AD, 0xDCAD };
	size_t is = sizeof(i);
	int32_t errors;

	EXPECT_EQ(4, utf32toutf8(i, is, nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, SurrogatePairSingleUnmatchedHigh)
{
	unicode_t i[] = { 0xD820, 0x0017 };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors;

	EXPECT_EQ(4, utf32toutf8(i, is, o, os, &errors));
	EXPECT_UTF8EQ("\xEF\xBF\xBD\x17", o);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, SurrogatePairSingleUnmatchedLow)
{
	unicode_t i[] = { 0xDD1E, 0xD834 };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors;

	EXPECT_EQ(6, utf32toutf8(i, is, o, os, &errors));
	EXPECT_UTF8EQ("\xEF\xBF\xBD\xEF\xBF\xBD", o);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, SurrogatePairSingleMissingLow)
{
	unicode_t i[] = { 0xDAA2 };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors;

	EXPECT_EQ(3, utf32toutf8(i, is, o, os, &errors));
	EXPECT_UTF8EQ("\xEF\xBF\xBD", o);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, SurrogatePairSingleMissingHigh)
{
	unicode_t i[] = { 0xDD1A };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors;

	EXPECT_EQ(3, utf32toutf8(i, is, o, os, &errors));
	EXPECT_UTF8EQ("\xEF\xBF\xBD", o);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, SurrogatePairSingleNotEnoughSpace)
{
	unicode_t i[] = { 0xDBBA, 0xDDED };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 2;
	int32_t errors;

	EXPECT_EQ(0, utf32toutf8(i, is, o, os, &errors));
	EXPECT_UTF8EQ("", o);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf32ToUtf8, SurrogatePairMultiple)
{
	unicode_t i[] = { 0xD83D, 0xDE12, 0xD83D, 0xDE22, 0xD83D, 0xDE24 };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors;

	EXPECT_EQ(12, utf32toutf8(i, is, o, os, &errors));
	EXPECT_UTF8EQ("\xF0\x9F\x98\x92\xF0\x9F\x98\xA2\xF0\x9F\x98\xA4", o);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, SurrogatePairMultipleAmountOfBytes)
{
	unicode_t i[] = { 0xD9AD, 0xDDA7, 0xD8FA, 0xDF18 };
	size_t is = sizeof(i);
	int32_t errors;

	EXPECT_EQ(8, utf32toutf8(i, is, nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, SurrogatePairMultipleUnmatchedPair)
{
	unicode_t i[] = { 0x0D87, 0xDE12, 0xD83D, 0x0988, 0xD999 };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors;

	EXPECT_EQ(15, utf32toutf8(i, is, o, os, &errors));
	EXPECT_UTF8EQ("\xE0\xB6\x87\xEF\xBF\xBD\xEF\xBF\xBD\xE0\xA6\x88\xEF\xBF\xBD", o);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, SurrogatePairMultipleNotEnoughSpace)
{
	unicode_t i[] = { 0xD9AD, 0xDDA7, 0xD8FA, 0xDF18 };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 5;
	int32_t errors;

	EXPECT_EQ(4, utf32toutf8(i, is, o, os, &errors));
	EXPECT_UTF8EQ("\xF1\xBB\x96\xA7", o);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf32ToUtf8, String)
{
	// LATIN CAPITAL LETTER S
	// CIRCLED WZ
	// LATIN CAPITAL LETTER H
	// PHAGS-PA LETTER KA
	// NKO LETTER PA

	unicode_t i[] = { 0x0053, 0x1F12E, 0x0048, 0xA840, 0x07D4 };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors;

	EXPECT_EQ(11, utf32toutf8(i, is, o, os, &errors));
	EXPECT_UTF8EQ("S\xF0\x9F\x84\xAEH\xEA\xA1\x80\xDF\x94", o);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, StringEndsInMiddle)
{
	// DIGIT ONE FULL STOP
	// LEFT SQUARE BRACKET
	// LATIN SMALL LETTER R
	// NULL
	// LATIN SMALL LETTER N

	unicode_t i[] = { 0x2488, 0x005B, 0x0072, 0, 0x006E };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors;

	EXPECT_EQ(7, utf32toutf8(i, is, o, os, &errors));
	EXPECT_MEMEQ("\xE2\x92\x88[r\x00" "n", o, 7);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, ErrorsIsReset)
{
	unicode_t i[] = { 0x1B001 };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 1288;

	EXPECT_EQ(4, utf32toutf8(i, is, o, os, &errors));
	EXPECT_UTF8EQ("\xF0\x9B\x80\x81", o);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, InvalidData)
{
	int32_t errors = 0;

	EXPECT_EQ(0, utf32toutf8(nullptr, 1, nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(Utf32ToUtf8, OverlappingParametersFits)
{
	int32_t errors = 0;

	uint8_t data[128] = { 0 };
	data[0 * sizeof(unicode_t)] = 'B';
	data[1 * sizeof(unicode_t)] = 'a';
	data[2 * sizeof(unicode_t)] = 'n';
	data[3 * sizeof(unicode_t)] = 'd';

	const unicode_t* i = (const unicode_t*)data;
	size_t is = 16;
	char* o = (char*)(data + 16);
	size_t os = 16;

	EXPECT_EQ(4, utf32toutf8(i, is, o, os, &errors));
	EXPECT_MEMEQ("B\0\0\0a\0\0\0n\0\0\0d\0\0\0Band", (const char*)data, 16);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, OverlappingParametersStartsEqual)
{
	int32_t errors = 0;

	uint8_t data[128] = { 0 };

	const unicode_t* i = (const unicode_t*)data;
	size_t is = 15;
	char* o = (char*)data;
	size_t os = 33;

	EXPECT_EQ(0, utf32toutf8(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST(Utf32ToUtf8, OverlappingParametersEndsEqual)
{
	int32_t errors = 0;

	uint8_t data[128] = { 0 };

	const unicode_t* i = (const unicode_t*)(data + 12);
	size_t is = 28;
	char* o = (char*)(data + 8);
	size_t os = 32;

	EXPECT_EQ(0, utf32toutf8(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST(Utf32ToUtf8, OverlappingParametersInputStartsInTarget)
{
	int32_t errors = 0;

	uint8_t data[128] = { 0 };

	const unicode_t* i = (const unicode_t*)(data + 17);
	size_t is = 11;
	char* o = (char*)(data + 4);
	size_t os = 26;

	EXPECT_EQ(0, utf32toutf8(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST(Utf32ToUtf8, OverlappingParametersInputEndsInTarget)
{
	int32_t errors = 0;

	uint8_t data[128] = { 0 };

	const unicode_t* i = (const unicode_t*)(data + 29);
	size_t is = 19;
	char* o = (char*)(data + 33);
	size_t os = 33;

	EXPECT_EQ(0, utf32toutf8(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST(Utf32ToUtf8, OverlappingParametersInputInsideTarget)
{
	int32_t errors = 0;

	uint8_t data[128] = { 0 };

	const unicode_t* i = (const unicode_t*)(data + 50);
	size_t is = 8;
	char* o = (char*)(data + 48);
	size_t os = 22;

	EXPECT_EQ(0, utf32toutf8(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST(Utf32ToUtf8, OverlappingParametersTargetStartsInInput)
{
	int32_t errors = 0;

	uint8_t data[128] = { 0 };

	const unicode_t* i = (const unicode_t*)(data + 25);
	size_t is = 25;
	char* o = (char*)(data + 34);
	size_t os = 49;

	EXPECT_EQ(0, utf32toutf8(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST(Utf32ToUtf8, OverlappingParametersTargetEndsInInput)
{
	int32_t errors = 0;

	uint8_t data[128] = { 0 };

	const unicode_t* i = (const unicode_t*)(data + 51);
	size_t is = 19;
	char* o = (char*)(data + 34);
	size_t os = 25;

	EXPECT_EQ(0, utf32toutf8(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST(Utf32ToUtf8, OverlappingParametersTargetInsideInput)
{
	int32_t errors = 0;

	uint8_t data[128] = { 0 };

	const unicode_t* i = (const unicode_t*)(data + 2);
	size_t is = 20;
	char* o = (char*)(data + 4);
	size_t os = 16;

	EXPECT_EQ(0, utf32toutf8(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}