#include "tests-base.hpp"

#include "utf8rewind.h"

#include "helpers-strings.hpp"

TEST(Utf8ToUtf32, OneByteSingle)
{
	const char* i = "_";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ(0x005F, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf32, OneByteSingleFirst)
{
	const char* i = "\0";
	size_t is = 1;
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ(0x0000, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf32, OneByteSingleLast)
{
	const char* i = "\x7F";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ(0x007F, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf32, OneByteSingleAmountOfBytes)
{
	const char* i = "L";
	size_t is = strlen(i);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf32, OneByteSingleNotEnoughSpaceOneByte)
{
	const char* i = "o";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = sizeof(unicode_t) - 1;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ(0x0000, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf32, OneByteSingleNotEnoughSpaceTwoBytes)
{
	const char* i = "~";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = sizeof(unicode_t) - 2;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ(0x0000, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf32, OneByteSingleNotEnoughSpaceThreeBytes)
{
	const char* i = "[";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = sizeof(unicode_t) - 3;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ(0x0000, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf32, OneByteSingleNotEnoughSpaceFourBytes)
{
	const char* i = "<";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = sizeof(unicode_t) - 4;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ(0x0000, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf32, OneByteSingleInvalidContinuationByteFirst)
{
	const char* i = "\x80";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ(0xFFFD, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf32, OneByteSingleInvalidContinuationByteLast)
{
	const char* i = "\xBF";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ(0xFFFD, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf32, OneByteSingleInvalidContinuationByteAmountOfBytes)
{
	const char* i = "\x8A";
	size_t is = strlen(i);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf32, OneByteSingleInvalidContinuationByteNotEnoughSpaceOneByte)
{
	const char* i = "\x9A";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = sizeof(unicode_t) - 1;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ(0x0000, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf32, OneByteSingleInvalidContinuationByteNotEnoughSpaceTwoBytes)
{
	const char* i = "\xAF";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = sizeof(unicode_t) - 2;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ(0x0000, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf32, OneByteSingleInvalidContinuationByteNotEnoughSpaceThreeBytes)
{
	const char* i = "\xB2";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = sizeof(unicode_t) - 3;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ(0x0000, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf32, OneByteSingleInvalidContinuationByteNotEnoughSpaceFourBytes)
{
	const char* i = "\x8E";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = sizeof(unicode_t) - 4;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ(0x0000, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf32, OneByteSingleIllegalByteFirst)
{
	const char* i = "\xFE";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ(0xFFFD, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf32, OneByteSingleIllegalByteLast)
{
	const char* i = "\xFE";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ(0xFFFD, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf32, OneByteSingleIllegalByteAmountOfBytes)
{
	const char* i = "\xFE";
	size_t is = strlen(i);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf32, OneByteSingleIllegalByteNotEnoughSpaceOneByte)
{
	const char* i = "\xFF";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = sizeof(unicode_t) - 1;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ(0x0000, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf32, OneByteSingleIllegalByteNotEnoughSpaceTwoBytes)
{
	const char* i = "\xFE";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = sizeof(unicode_t) - 2;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ(0x0000, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf32, OneByteSingleIllegalByteNotEnoughSpaceThreeBytes)
{
	const char* i = "\xFE";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = sizeof(unicode_t) - 3;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ(0x0000, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf32, OneByteSingleIllegalByteNotEnoughSpaceFourBytes)
{
	const char* i = "\xFF";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = sizeof(unicode_t) - 4;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ(0x0000, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf32, OneByteMultiple)
{
	const char* i = "Boat";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(16, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ('B', o[0]);
	EXPECT_CPEQ('o', o[1]);
	EXPECT_CPEQ('a', o[2]);
	EXPECT_CPEQ('t', o[3]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf32, OneByteMultipleInvalidContinuationByteCombined)
{
	const char* i =
		"\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8A\x8B\x8C\x8D\x8E\x8F" \
		"\x90\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9A\x9B\x9C\x9D\x9E\x9F" \
		"\xA0\xA1\xA2\xA3\xA4\xA5\xA6\xA7\xA8\xA9\xAA\xAB\xAC\xAD\xAE\xAF" \
		"\xB0\xB1\xB2\xB3\xB4\xB5\xB6\xB7\xB8\xB9\xBA\xBB\xBC\xBD\xBE\xBF";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(256, utf8toutf32(i, is, o, os, &errors));
	for (size_t i = 0; i < 64; ++i)
	{
		EXPECT_CPEQ(0x0000FFFD, o[i]);
	}
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf32, OneByteMultipleInvalidContinuationByteAndValid)
{
	const char* i = "\x84" "7";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(8, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ(0xFFFD, o[0]);
	EXPECT_CPEQ('7', o[1]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf32, OneByteMultipleValidAndInvalidContinuationByte)
{
	const char* i = "uh" "\xB4";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(12, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ('u', o[0]);
	EXPECT_CPEQ('h', o[1]);
	EXPECT_CPEQ(0xFFFD, o[2]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf32, OneByteMultipleIllegalAndValid)
{
	const char* i = "\xFFn";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(8, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ(0xFFFD, o[0]);
	EXPECT_CPEQ('n', o[1]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf32, OneByteMultipleValidAndIllegal)
{
	const char* i = "Ban\xFE";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(16, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ('B', o[0]);
	EXPECT_CPEQ('a', o[1]);
	EXPECT_CPEQ('n', o[2]);
	EXPECT_CPEQ(0xFFFD, o[3]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf32, OneByteMultipleAmountOfBytes)
{
	const char* i = "Macaron";
	size_t is = strlen(i);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(28, utf8toutf32(i, is, nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf32, OneByteMultipleNotEnoughSpaceOneByte)
{
	const char* i = "Waffle";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = (6 * sizeof(unicode_t)) - 1;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(20, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ('W', o[0]);
	EXPECT_CPEQ('a', o[1]);
	EXPECT_CPEQ('f', o[2]);
	EXPECT_CPEQ('f', o[3]);
	EXPECT_CPEQ('l', o[4]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf32, OneByteMultipleNotEnoughSpaceTwoBytes)
{
	const char* i = "Pine";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = (4 * sizeof(unicode_t)) - 2;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(12, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ('P', o[0]);
	EXPECT_CPEQ('i', o[1]);
	EXPECT_CPEQ('n', o[2]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf32, OneByteMultipleNotEnoughSpaceThreeBytes)
{
	const char* i = "[12.0]";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = (6 * sizeof(unicode_t)) - 3;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(20, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ('[', o[0]);
	EXPECT_CPEQ('1', o[1]);
	EXPECT_CPEQ('2', o[2]);
	EXPECT_CPEQ('.', o[3]);
	EXPECT_CPEQ('0', o[4]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf32, OneByteMultipleNotEnoughSpaceFourBytes)
{
	const char* i = "Match";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = (5 * sizeof(unicode_t)) - 4;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(16, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ('M', o[0]);
	EXPECT_CPEQ('a', o[1]);
	EXPECT_CPEQ('t', o[2]);
	EXPECT_CPEQ('c', o[3]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf32, TwoBytesSingle)
{
	// CENT SIGN

	const char* i = "\xC2\xA2";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ(0x00A2, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf32, TwoBytesSingleFirst)
{
	const char* i = "\xC2\x80";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ(0x0080, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf32, TwoBytesSingleLast)
{
	const char* i = "\xDF\xBF";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ(0x07FF, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf32, TwoBytesSingleInvalidContinuationFirstByteLower)
{
	const char* i = "\xDD\x13";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(8, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ(0xFFFD, o[0]);
	EXPECT_CPEQ(0x0013, o[1]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf32, TwoBytesSingleInvalidContinuationFirstByteUpper)
{
	const char* i = "\xD7\xC3";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(8, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ(0xFFFD, o[0]);
	EXPECT_CPEQ(0xFFFD, o[1]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf32, TwoBytesSingleAmountOfBytes)
{
	const char* i = "\xC3\x9F";
	size_t is = strlen(i);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf32, TwoBytesSingleNotEnoughSpaceOneByte)
{
	// LATIN SMALL LETTER N WITH TILDE

	const char* i = "\xC3\xB1";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = sizeof(unicode_t) - 1;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ(0x0000, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf32, TwoBytesSingleNotEnoughSpaceTwoBytes)
{
	// LATIN CAPITAL LETTER ESH

	const char* i = "\xC6\xA9";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = sizeof(unicode_t) - 2;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ(0x0000, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf32, TwoBytesSingleNotEnoughSpaceThreeBytes)
{
	// MODIFIER LETTER MIDDLE GRAVE ACCENT

	const char* i = "\xCB\xB4";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = sizeof(unicode_t) - 3;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ(0x0000, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf32, TwoBytesSingleNotEnoughSpaceFourBytes)
{
	// LATIN SMALL LETTER Y WITH DIAERESIS

	const char* i = "\xC3\xBF";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = sizeof(unicode_t) - 4;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ(0x0000, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf32, TwoBytesSingleMissingOneByte)
{
	const char* i = "\xC4";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ(0xFFFD, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf32, TwoBytesSingleMissingAmountOfBytes)
{
	const char* i = "\xCA";
	size_t is = strlen(i);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf32, TwoBytesSingleMissingNotEnoughSpaceOneByte)
{
	const char* i = "\xC2";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = sizeof(unicode_t) - 1;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ(0x0000, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf32, TwoBytesSingleMissingNotEnoughSpaceTwoBytes)
{
	const char* i = "\xD2";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = sizeof(unicode_t) - 2;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ(0x0000, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf32, TwoBytesSingleMissingNotEnoughSpaceThreeBytes)
{
	const char* i = "\xC7";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = sizeof(unicode_t) - 3;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ(0x0000, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf32, TwoBytesSingleMissingNotEnoughSpaceFourBytes)
{
	const char* i = "\xCF";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = sizeof(unicode_t) - 4;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ(0x0000, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf32, TwoBytesSingleOverlong)
{
	const char* i = "\xC0\xAF";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ(0xFFFD, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf32, TwoBytesSingleOverlongFirst)
{
	const char* i = "\xC0\x80";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ(0xFFFD, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf32, TwoBytesSingleOverlongLast)
{
	const char* i = "\xC1\xBF";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ(0xFFFD, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf32, TwoBytesSingleOverlongAmountOfBytes)
{
	const char* i = "\xC1\xBB";
	size_t is = strlen(i);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf32, TwoBytesSingleOverlongNotEnoughSpaceOneByte)
{
	const char* i = "\xC0\x88";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = sizeof(unicode_t) - 1;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ(0x0000, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf32, TwoBytesSingleOverlongNotEnoughSpaceTwoBytes)
{
	const char* i = "\xC0\xA9";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = sizeof(unicode_t) - 2;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ(0x0000, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf32, TwoBytesSingleOverlongNotEnoughSpaceThreeBytes)
{
	const char* i = "\xC1\xB0";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = sizeof(unicode_t) - 3;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ(0x0000, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf32, TwoBytesSingleOverlongNotEnoughSpaceFourBytes)
{
	const char* i = "\xC0\xB2";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = sizeof(unicode_t) - 4;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ(0x0000, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf32, TwoBytesMultiple)
{
	// ARABIC LETTER HAH WITH SMALL ARABIC LETTER TAH BELOW
	// THAANA LETTER FAAFU
	// NKO DIGIT ZERO

	const char* i = "\xDD\xAE\xDE\x8A\xDF\x80";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(12, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ(0x076E, o[0]);
	EXPECT_CPEQ(0x078A, o[1]);
	EXPECT_CPEQ(0x07C0, o[2]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf32, TwoBytesMultipleOverlong)
{
	const char* i = "\xC0\x87\xC0\xA2\xC1\xAA";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(12, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ(0xFFFD, o[0]);
	EXPECT_CPEQ(0xFFFD, o[1]);
	EXPECT_CPEQ(0xFFFD, o[2]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf32, TwoBytesMultipleAmountOfBytes)
{
	// <control>
	// <control>
	// DIAERESIS

	const char* i = "\xC2\x91\xC2\x9A\xC2\xA8";
	size_t is = strlen(i);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(12, utf8toutf32(i, is, nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf32, TwoBytesMultipleNotEnoughSpaceOneByte)
{
	// LATIN SMALL LETTER G WITH CARON
	// ARABIC LETTER HEH WITH YEH ABOVE
	// ARABIC LETTER AIN WITH TWO DOTS VERTICALLY ABOVE
	// LATIN CAPITAL LETTER N WITH LEFT HOOK

	const char* i = "\xC7\xA7\xDB\x80\xDD\x9F\xC6\x9D";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = (4 * sizeof(unicode_t)) - 1;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(12, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ(0x01E7, o[0]);
	EXPECT_CPEQ(0x06C0, o[1]);
	EXPECT_CPEQ(0x075F, o[2]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf32, TwoBytesMultipleNotEnoughSpaceTwoBytes)
{
	// ARABIC LETTER GAF WITH RING
	// NKO LETTER E
	// SYRIAC LETTER HETH

	const char* i = "\xDA\xB0\xDF\x8D\xDC\x9A";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = (3 * sizeof(unicode_t)) - 2;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(8, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ(0x06B0, o[0]);
	EXPECT_CPEQ(0x07CD, o[1]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf32, TwoBytesMultipleNotEnoughSpaceThreeBytes)
{
	// CYRILLIC SMALL LETTER ZHE WITH BREVE
	// GREEK SMALL LETTER UPSILON WITH TONOS
	// CYRILLIC CAPITAL LETTER IZHITSA WITH DOUBLE GRAVE ACCENT
	// COMBINING CYRILLIC PALATALIZATION

	const char* i = "\xD3\x82\xCF\x8D\xD1\xB6\xD2\x84";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = (4 * sizeof(unicode_t)) - 3;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(12, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ(0x04C2, o[0]);
	EXPECT_CPEQ(0x03CD, o[1]);
	EXPECT_CPEQ(0x0476, o[2]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf32, TwoBytesMultipleNotEnoughSpaceFourBytes)
{
	// LATIN SMALL LETTER E WITH CARON
	// LATIN LETTER SMALL CAPITAL G
	// LATIN SMALL LETTER OPEN E

	const char* i = "\xC4\x9B\xC9\xA2\xC9\x9B";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = (3 * sizeof(unicode_t)) - 4;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(8, utf8toutf32(i, is, o, os, &errors));
	EXPECT_CPEQ(0x011B, o[0]);
	EXPECT_CPEQ(0x0262, o[1]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}



TEST(Utf8ToUtf32, ThreeBytes)
{
	const char* i = "\xE1\x8C\x8A";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000130A, o[0]);
}

TEST(Utf8ToUtf32, ThreeBytesFirst)
{
	const char* i = "\xE0\xA0\x80";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x00000800, o[0]);
}

TEST(Utf8ToUtf32, ThreeBytesLast)
{
	const char* i = "\xEF\xBF\xBF";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFF, o[0]);
}

TEST(Utf8ToUtf32, ThreeBytesString)
{
	const char* i = "\xE3\x81\x8A\xE3\x81\x8D\xE3\x81\x99\xE3\x81\x88";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(16, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000304A, o[0]);
	EXPECT_EQ(0x0000304D, o[1]);
	EXPECT_EQ(0x00003059, o[2]);
	EXPECT_EQ(0x00003048, o[3]);
}

TEST(Utf8ToUtf32, ThreeBytesOverlong)
{
	const char* i = "\xE0\x80\xAF";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, ThreeBytesOverlongNull)
{
	const char* i = "\xE0\x80\x80";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, ThreeBytesOverlongMaximum)
{
	const char* i = "\xE0\x9F\xBF";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, ThreeBytesLonelyStartFirst)
{
	const char* i = "\xE0 ";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(8, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(' ', o[1]);
}

TEST(Utf8ToUtf32, ThreeBytesLonelyStartLast)
{
	const char* i = "\xEF ";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(8, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(' ', o[1]);
}

TEST(Utf8ToUtf32, ThreeBytesLonelyStartCombined)
{
	const char* i =
		"\xE0 \xE1 \xE2 \xE3 \xE4 \xE5 \xE6 \xE7 \xE8 \xE9 \xEA \xEB \xEC \xED \xEE \xEF ";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(128, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);

	for (size_t i = 0; i < 16; i += 2)
	{
		EXPECT_EQ(0x0000FFFD, o[i]);
		EXPECT_EQ(' ', o[i + 1]);
	}
}

TEST(Utf8ToUtf32, ThreeBytesNotEnoughData)
{
	const char* i = "\xEF\xBF";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, FourBytes)
{
	const char* i = "\xF0\x9F\x98\x8E";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0001F60E, o[0]);
}

TEST(Utf8ToUtf32, FourBytesFirst)
{
	const char* i = "\xF0\x90\x80\x80";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x00010000, o[0]);
}

TEST(Utf8ToUtf32, FourBytesLast)
{
	const char* i = "\xF4\x80\x83\xBF";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x001000FF, o[0]);
}

TEST(Utf8ToUtf32, FourBytesString)
{
	const char* i = "\xF0\x90\xB0\x97\xF0\x90\xB0\x8C\xF0\x90\xA8\x95";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(12, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x00010C17, o[0]);
	EXPECT_EQ(0x00010C0C, o[1]);
	EXPECT_EQ(0x00010A15, o[2]);
}

TEST(Utf8ToUtf32, FourBytesOverlong)
{
	const char* i = "\xF0\x80\x80\xAF";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, FourBytesOverlongNull)
{
	const char* i = "\xF0\x80\x80\x80";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, FourBytesOverlongFirst)
{
	const char* i = "\xF4\x90\x80\x80";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, FourBytesOverlongLast)
{
	const char* i = "\xF7\xBF\xBF\xBF";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, FourBytesOverlongMaximum)
{
	const char* i = "\xF0\x8F\xBF\xBF";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, FourBytesLonelyStartFirst)
{
	const char* i = "\xF0 ";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(8, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(' ', o[1]);
}

TEST(Utf8ToUtf32, FourBytesLonelyStartLast)
{
	const char* i = "\xF7 ";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(8, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(' ', o[1]);
}

TEST(Utf8ToUtf32, FourBytesLonelyStartCombined)
{
	const char* i =
		"\xF0 \xF1 \xF2 \xF3 \xF4 \xF5 \xF6 \xF7 ";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(64, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);

	for (size_t i = 0; i < 8; i += 2)
	{
		EXPECT_EQ(0x0000FFFD, o[i]);
		EXPECT_EQ(' ', o[i + 1]);
	}
}

TEST(Utf8ToUtf32, FourBytesNotEnoughData)
{
	const char* i = "\xF0\x90\x80";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, FiveBytesOverlong)
{
	const char* i = "\xF8\x80\x80\x80\xAF";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, FiveBytesOverlongFirst)
{
	const char* i = "\xF8\x88\x80\x80\x80";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, FiveBytesOverlongLast)
{
	const char* i = "\xFB\xBF\xBF\xBF\xBF";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, FiveBytesOverlongNull)
{
	const char* i = "\xF8\x80\x80\x80\x80";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, FiveBytesOverlongMaximum)
{
	const char* i = "\xF8\x87\xBF\xBF\xBF";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, FiveBytesLonelyStartFirst)
{
	const char* i = "\xF8 ";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(8, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(' ', o[1]);
}

TEST(Utf8ToUtf32, FiveBytesLonelyStartLast)
{
	const char* i = "\xFB ";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(8, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(' ', o[1]);
}

TEST(Utf8ToUtf32, FiveBytesLonelyStartCombined)
{
	const char* i =
		"\xF8 \xF9 \xFA \xFB ";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(32, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);

	for (size_t i = 0; i < 4; i += 2)
	{
		EXPECT_EQ(0x0000FFFD, o[i]);
		EXPECT_EQ(' ', o[i + 1]);
	}
}

TEST(Utf8ToUtf32, FiveBytesNotEnoughData)
{
	const char* i = "\xF8\x80\x80\x80";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, SixBytesOverlong)
{
	const char* i = "\xFC\x80\x80\x80\x80\xAF";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, SixBytesOverlongFirst)
{
	const char* i = "\xFC\x84\x80\x80\x80\x80";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, SixBytesOverlongLast)
{
	const char* i = "\xFD\xBF\xBF\xBF\xBF\xBF";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, SixBytesOverlongNull)
{
	const char* i = "\xFC\x80\x80\x80\x80\x80";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, SixBytesOverlongMaximum)
{
	const char* i = "\xFC\x83\xBF\xBF\xBF\xBF";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, SixBytesLonelyStartFirst)
{
	const char* i = "\xFC ";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(8, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(' ', o[1]);
}

TEST(Utf8ToUtf32, SixBytesLonelyStartLast)
{
	const char* i = "\xFD ";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(8, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(' ', o[1]);
}

TEST(Utf8ToUtf32, SixBytesLonelyStartCombined)
{
	const char* i =
		"\xFC \xFD ";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(16, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);

	for (size_t i = 0; i < 2; i += 2)
	{
		EXPECT_EQ(0x0000FFFD, o[i]);
		EXPECT_EQ(' ', o[i + 1]);
	}
}

TEST(Utf8ToUtf32, SixBytesNotEnoughData)
{
	const char* i = "\xFC\x80\x80\x80\x80";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, SixBytesNotEnoughSpace)
{
	const char* i = "Der\xFD\xBF\xBF\xBF\xBF\xBFp";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 4 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(16, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
	EXPECT_EQ('D', o[0]);
	EXPECT_EQ('e', o[1]);
	EXPECT_EQ('r', o[2]);
	EXPECT_EQ(0x0000FFFD, o[3]);
}

TEST(Utf8ToUtf32, SurrogatePair)
{
	const char* i = "\xED\xAD\x80\xED\xBE\x80";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(8, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(0x0000FFFD, o[1]);
}

TEST(Utf8ToUtf32, SurrogatePairCombined)
{
	const char* i = "\
\xED\xA0\x80\xED\xB0\x80\
\xED\xA0\x80\xED\xBF\xBF\
\xED\xAD\xBF\xED\xB0\x80\
\xED\xAD\xBF\xED\xBF\xBF\
\xED\xAE\x80\xED\xB0\x80\
\xED\xAE\x80\xED\xBF\xBF\
\xED\xAF\xBF\xED\xB0\x80\
\xED\xAF\xBF\xED\xBF\xBF";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(64, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(0x0000FFFD, o[1]);
	EXPECT_EQ(0x0000FFFD, o[2]);
	EXPECT_EQ(0x0000FFFD, o[3]);
	EXPECT_EQ(0x0000FFFD, o[4]);
	EXPECT_EQ(0x0000FFFD, o[5]);
	EXPECT_EQ(0x0000FFFD, o[6]);
	EXPECT_EQ(0x0000FFFD, o[7]);
	EXPECT_EQ(0x0000FFFD, o[8]);
	EXPECT_EQ(0x0000FFFD, o[9]);
	EXPECT_EQ(0x0000FFFD, o[10]);
	EXPECT_EQ(0x0000FFFD, o[11]);
	EXPECT_EQ(0x0000FFFD, o[12]);
	EXPECT_EQ(0x0000FFFD, o[13]);
	EXPECT_EQ(0x0000FFFD, o[14]);
	EXPECT_EQ(0x0000FFFD, o[15]);
}

TEST(Utf8ToUtf32, SurrogatePairHigh)
{
	const char* i = "\xED\xAD\xBF";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, SurrogatePairHighStart)
{
	const char* i = "\xED\xA0\x80";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, SurrogatePairHighEnd)
{
	const char* i = "\xED\xAF\xBF";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, SurrogatePairLow)
{
	const char* i = "\xED\xBE\x80";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, SurrogatePairLowStart)
{
	const char* i = "\xED\xB0\x80";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, SurrogatePairLowEnd)
{
	const char* i = "\xED\xBF\xBF";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, SurrogatePairOverlongHighStart)
{
	const char* i = "\xF0\x8D\xA0\x80";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, SurrogatePairOverlongHighEnd)
{
	const char* i = "\xF0\x8D\xAF\xBF";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, SurrogatePairOverlongLowStart)
{
	const char* i = "\xF0\x8D\xAD\xBF";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, SurrogatePairOverlongLowEnd)
{
	const char* i = "\xF0\x8D\xBF\xBF";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, SurrogatePairNotEnoughData)
{
	const char* i = "\xED\xBF";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, Character)
{
	const char* i = "\xF0\x9F\x98\xA4";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0001F624, o[0]);
}

TEST(Utf8ToUtf32, String)
{
	const char* i = "\xE0\xA4\x9C\xE0\xA4\xA1\xE0\xA4\xA4";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(12, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000091C, o[0]);
	EXPECT_EQ(0x00000921, o[1]);
	EXPECT_EQ(0x00000924, o[2]);
}

TEST(Utf8ToUtf32, StringEndsInMiddle)
{
	const char* i = "How un\0for";
	size_t is = 10;
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(40, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ('H', o[0]);
	EXPECT_EQ('o', o[1]);
	EXPECT_EQ('w', o[2]);
	EXPECT_EQ(' ', o[3]);
	EXPECT_EQ('u', o[4]);
	EXPECT_EQ('n', o[5]);
	EXPECT_EQ(0, o[6]);
	EXPECT_EQ('f', o[7]);
	EXPECT_EQ('o', o[8]);
	EXPECT_EQ('r', o[9]);
}

TEST(Utf8ToUtf32, StringDataSizeUnder)
{
	const char* i = "Tree";
	size_t is = 3;
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(12, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ('T', o[0]);
	EXPECT_EQ('r', o[1]);
	EXPECT_EQ('e', o[2]);
}

TEST(Utf8ToUtf32, StringDataSizeOver)
{
	const char* i = "Bark";
	size_t is = 8;
	unicode_t o[256] = { 0 };
	size_t os = 255 * sizeof(unicode_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(32, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ('B', o[0]);
	EXPECT_EQ('a', o[1]);
	EXPECT_EQ('r', o[2]);
	EXPECT_EQ('k', o[3]);
	EXPECT_EQ(0, o[4]);
}

TEST(Utf8ToUtf32, AmountOfBytes)
{
	const char* i = "\xF0\x90\xB0\xAC";
	size_t is = strlen(i);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf32, AmountOfBytesString)
{
	const char* i = "Pchn\xC4\x85\xC4\x87 w t\xC4\x99";
	size_t is = strlen(i);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(44, utf8toutf32(i, is, nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf32, AmountOfBytesNotEnoughData)
{
	const char* i = "\xC8";
	size_t is = strlen(i);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf32, AmountOfBytesOverlong)
{
	const char* i = "\xFC\x84\x80\x80\x80\x80";
	size_t is = strlen(i);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf32(i, is, nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf32, AmountOfBytesNoData)
{
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8toutf32(nullptr, 1, nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(Utf8ToUtf32, ErrorsIsReset)
{
	const char* i = "\xE0\xA2\xA8";
	size_t is = strlen(i);
	unicode_t o[256] = { 0 };
	const size_t os = 255 * sizeof(unicode_t);
	int32_t errors = 8711;

	EXPECT_EQ(4, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x000008A8, o[0]);
}

TEST(Utf8ToUtf32, OverlappingParametersFits)
{
	int32_t errors = UTF8_ERR_NONE;

	char data[128] = { 0 };
	strcpy(data, "silence");

	const char* i = data;
	size_t is = 7;
	unicode_t* o = (unicode_t*)(data + 7);
	size_t os = 28;

	EXPECT_EQ(28, utf8toutf32(i, is, o, os, &errors));
	EXPECT_MEMEQ("silences\0\0\0i\0\0\0l\0\0\0e\0\0\0n\0\0\0c\0\0\0e\0\0\0", data, 33);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf32, OverlappingParametersStartsEqual)
{
	int32_t errors = UTF8_ERR_NONE;

	char data[128] = { 0 };

	const char* i = data + 16;
	size_t is = 24;
	unicode_t* o = (unicode_t*)(data + 16);
	size_t os = 18;

	EXPECT_EQ(0, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST(Utf8ToUtf32, OverlappingParametersEndsEqual)
{
	int32_t errors = UTF8_ERR_NONE;

	char data[128] = { 0 };

	const char* i = data + 40;
	size_t is = 20;
	unicode_t* o = (unicode_t*)(data + 19);
	size_t os = 41;

	EXPECT_EQ(0, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST(Utf8ToUtf32, OverlappingParametersInputStartsInTarget)
{
	int32_t errors = UTF8_ERR_NONE;

	char data[128] = { 0 };

	const char* i = data + 17;
	size_t is = 51;
	unicode_t* o = (unicode_t*)(data + 2);
	size_t os = 27;

	EXPECT_EQ(0, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST(Utf8ToUtf32, OverlappingParametersInputEndsInTarget)
{
	int32_t errors = UTF8_ERR_NONE;

	char data[128] = { 0 };

	const char* i = data + 21;
	size_t is = 45;
	unicode_t* o = (unicode_t*)(data + 2);
	size_t os = 100;

	EXPECT_EQ(0, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST(Utf8ToUtf32, OverlappingParametersInputInsideTarget)
{
	int32_t errors = UTF8_ERR_NONE;

	char data[128] = { 0 };

	const char* i = data + 50;
	size_t is = 11;
	unicode_t* o = (unicode_t*)(data + 41);
	size_t os = 38;

	EXPECT_EQ(0, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST(Utf8ToUtf32, OverlappingParametersTargetStartsInInput)
{
	int32_t errors = UTF8_ERR_NONE;

	char data[128] = { 0 };

	const char* i = data + 14;
	size_t is = 34;
	unicode_t* o = (unicode_t*)(data + 21);
	size_t os = 34;

	EXPECT_EQ(0, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST(Utf8ToUtf32, OverlappingParametersTargetEndsInInput)
{
	int32_t errors = UTF8_ERR_NONE;

	char data[128] = { 0 };

	const char* i = data + 60;
	size_t is = 31;
	unicode_t* o = (unicode_t*)(data + 14);
	size_t os = 71;

	EXPECT_EQ(0, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST(Utf8ToUtf32, OverlappingParametersTargetInsideInput)
{
	int32_t errors = UTF8_ERR_NONE;

	char data[128] = { 0 };

	const char* i = data + 2;
	size_t is = 100;
	unicode_t* o = (unicode_t*)(data + 41);
	size_t os = 12;

	EXPECT_EQ(0, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}