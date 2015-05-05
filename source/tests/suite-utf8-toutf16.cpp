#include "tests-base.hpp"

#include "utf8rewind.h"

#include "helpers-strings.hpp"

TEST(Utf8ToUtf16, OneByteSingle)
{
	const char* i = "`";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = 255 * sizeof(utf16_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ('`', o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, OneByteSingleFirst)
{
	const char* i = "\0";
	size_t is = 1;
	utf16_t o[256] = { 0 };
	size_t os = 255 * sizeof(utf16_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0x0000, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, OneByteSingleLast)
{
	const char* i = "\x7F";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = 255 * sizeof(utf16_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0x007F, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, OneByteSingleAmountOfBytes)
{
	const char* i = "A";
	size_t is = strlen(i);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, is, nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, OneByteSingleNotEnoughSpaceOneByte)
{
	const char* i = "x";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = sizeof(utf16_t) - 1;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0x0000, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf16, OneByteSingleNotEnoughSpaceTwoBytes)
{
	const char* i = "V";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = sizeof(utf16_t) - 2;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0x0000, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf16, OneByteSingleInvalidContinuationByteFirst)
{
	const char* i = "\x80";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = 255 * sizeof(utf16_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0xFFFD, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, OneByteSingleInvalidContinuationByteLast)
{
	const char* i = "\xBF";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = 255 * sizeof(utf16_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0xFFFD, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, OneByteSingleInvalidContinuationByteAmountOfBytes)
{
	const char* i = "\xA2";
	size_t is = strlen(i);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, is, nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, OneByteSingleInvalidContinuationByteNotEnoughSpaceOneByte)
{
	const char* i = "\x9A";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = sizeof(utf16_t) - 1;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0x0000, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf16, OneByteSingleInvalidContinuationByteNotEnoughSpaceTwoBytes)
{
	const char* i = "\xA7";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = sizeof(utf16_t) - 2;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0x0000, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf16, OneByteSingleIllegalByteFirst)
{
	const char* i = "\xFE";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = 255 * sizeof(utf16_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0xFFFD, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, OneByteSingleIllegalByteLast)
{
	const char* i = "\xFF";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = 255 * sizeof(utf16_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0xFFFD, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, OneByteSingleIllegalByteAmountOfBytes)
{
	const char* i = "\xFE";
	size_t is = strlen(i);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, is, nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, OneByteSingleIllegalByteNotEnoughSpaceOneByte)
{
	const char* i = "\xFF";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = sizeof(utf16_t) - 1;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0x0000, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf16, OneByteSingleIllegalByteNotEnoughSpaceTwoBytes)
{
	const char* i = "\xFF";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = sizeof(utf16_t) - 2;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0x0000, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf16, OneByteMultiple)
{
	const char* i = "home";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = 255 * sizeof(utf16_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(8, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ('h', o[0]);
	EXPECT_CPEQ('o', o[1]);
	EXPECT_CPEQ('m', o[2]);
	EXPECT_CPEQ('e', o[3]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, OneByteMultipleInvalidContinuationByteCombined)
{
	const char* i =
		"\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8A\x8B\x8C\x8D\x8E\x8F" \
		"\x90\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9A\x9B\x9C\x9D\x9E\x9F" \
		"\xA0\xA1\xA2\xA3\xA4\xA5\xA6\xA7\xA8\xA9\xAA\xAB\xAC\xAD\xAE\xAF" \
		"\xB0\xB1\xB2\xB3\xB4\xB5\xB6\xB7\xB8\xB9\xBA\xBB\xBC\xBD\xBE\xBF";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = 255 * sizeof(utf16_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(128, utf8toutf16(i, is, o, os, &errors));
	for (size_t i = 0; i < 64; ++i)
	{
		EXPECT_CPEQ(0x0000FFFD, o[i]);
	}
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, OneByteMultipleInvalidContinuationByteAndValid)
{
	const char* i = "\x87" "ab";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = 255 * sizeof(utf16_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(6, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0xFFFD, o[0]);
	EXPECT_CPEQ('a', o[1]);
	EXPECT_CPEQ('b', o[2]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, OneByteMultipleValidAndInvalidContinuationByte)
{
	const char* i = "and" "\xB7";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = 255 * sizeof(utf16_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(8, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ('a', o[0]);
	EXPECT_CPEQ('n', o[1]);
	EXPECT_CPEQ('d', o[2]);
	EXPECT_CPEQ(0xFFFD, o[3]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, OneByteMultipleIllegalAndValid)
{
	const char* i = "\xFE" "yum";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = 255 * sizeof(utf16_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(8, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0xFFFD, o[0]);
	EXPECT_CPEQ('y', o[1]);
	EXPECT_CPEQ('u', o[2]);
	EXPECT_CPEQ('m', o[3]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, OneByteMultipleValidAndIllegal)
{
	const char* i = "soup\xFE";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = 255 * sizeof(utf16_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(10, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ('s', o[0]);
	EXPECT_CPEQ('o', o[1]);
	EXPECT_CPEQ('u', o[2]);
	EXPECT_CPEQ('p', o[3]);
	EXPECT_CPEQ(0xFFFD, o[4]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, OneByteMultipleAmountOfBytes)
{
	const char* i = "denker";
	size_t is = strlen(i);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(12, utf8toutf16(i, is, nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, OneByteMultipleNotEnoughSpaceOneByte)
{
	const char* i = "valid";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = (5 * sizeof(utf16_t)) - 1;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(8, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ('v', o[0]);
	EXPECT_CPEQ('a', o[1]);
	EXPECT_CPEQ('l', o[2]);
	EXPECT_CPEQ('i', o[3]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf16, OneByteMultipleNotEnoughSpaceTwoBytes)
{
	const char* i = "return";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = (6 * sizeof(utf16_t)) - 1;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(10, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ('r', o[0]);
	EXPECT_CPEQ('e', o[1]);
	EXPECT_CPEQ('t', o[2]);
	EXPECT_CPEQ('u', o[3]);
	EXPECT_CPEQ('r', o[4]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf16, TwoBytesSingle)
{
	const char* i = "\xC5\xA9";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = 255 * sizeof(utf16_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0x0169, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, TwoBytesSingleFirst)
{
	const char* i = "\xC2\x80";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = 255 * sizeof(utf16_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0x0080, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, TwoBytesSingleLast)
{
	const char* i = "\xDF\xBF";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = 255 * sizeof(utf16_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0x07FF, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, TwoBytesSingleInvalidContinuationFirstByteLower)
{
	const char* i = "\xC3\x65";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = 255 * sizeof(utf16_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0xFFFD, o[0]);
	EXPECT_CPEQ(0x0065, o[1]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, TwoBytesSingleInvalidContinuationFirstByteUpper)
{
	const char* i = "\xC3\xDD";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = 255 * sizeof(utf16_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0xFFFD, o[0]);
	EXPECT_CPEQ(0xFFFD, o[1]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, TwoBytesSingleAmountOfBytes)
{
	const char* i = "\xC2\x89";
	size_t is = strlen(i);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, is, nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, TwoBytesSingleNotEnoughSpaceOneByte)
{
	const char* i = "\xC3\x97";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = sizeof(utf16_t) - 1;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0x0000, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf16, TwoBytesSingleNotEnoughSpaceTwoBytes)
{
	const char* i = "\xC4\x87";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = sizeof(utf16_t) - 2;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0x0000, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf16, TwoBytesSingleMissingOneByte)
{
	const char* i = "\xD4";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = 255 * sizeof(utf16_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0xFFFD, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, TwoBytesSingleMissingAmountOfBytes)
{
	const char* i = "\xD8";
	size_t is = strlen(i);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, is, nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, TwoBytesSingleMissingNotEnoughSpaceOneByte)
{
	const char* i = "\xD7";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = sizeof(utf16_t) - 1;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0x0000, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf16, TwoBytesSingleMissingNotEnoughSpaceTwoBytes)
{
	const char* i = "\xCF";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = sizeof(utf16_t) - 2;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0x0000, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf16, TwoBytesSingleOverlongOneByteFirst)
{
	// U+0000

	const char* i = "\xC0\x80";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = 255 * sizeof(utf16_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0xFFFD, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, TwoBytesSingleOverlongOneByteLast)
{
	// U+0000

	const char* i = "\xC1\xBF";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = 255 * sizeof(utf16_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0xFFFD, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, TwoBytesSingleOverlongAmountOfBytes)
{
	const char* i = "\xC0\x8A";
	size_t is = strlen(i);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, is, nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, TwoBytesSingleOverlongNotEnoughSpaceOneByte)
{
	const char* i = "\xC0\x97";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = sizeof(utf16_t) - 1;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0x0000, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf16, TwoBytesSingleOverlongNotEnoughSpaceTwoBytes)
{
	const char* i = "\xC0\xDA";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = sizeof(utf16_t) - 2;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0x0000, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf16, TwoBytesMultiple)
{
	const char* i = "\xDD\xAE\xDE\x8A\xDF\x80";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = 255 * sizeof(utf16_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(6, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0x076E, o[0]);
	EXPECT_CPEQ(0x078A, o[1]);
	EXPECT_CPEQ(0x07C0, o[2]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, TwoBytesMultipleOverlong)
{
	const char* i = "\xC0\xAF\xC1\xA6\xC0\xA6";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = 255 * sizeof(utf16_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(6, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0xFFFD, o[0]);
	EXPECT_CPEQ(0xFFFD, o[1]);
	EXPECT_CPEQ(0xFFFD, o[2]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, TwoBytesMultipleAmountOfBytes)
{
	const char* i = "\xC2\xA6\xC3\x98\xC4\x96\xC4\xA1";
	size_t is = strlen(i);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(8, utf8toutf16(i, is, nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, TwoBytesMultipleNotEnoughSpaceOneByte)
{
	const char* i = "\xC3\x97\xC2\xB8\xC2\xAA";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = (3 * sizeof(utf16_t)) - 1;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0x00D7, o[0]);
	EXPECT_CPEQ(0x00B8, o[1]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf16, TwoBytesMultipleNotEnoughSpaceTwoBytes)
{
	const char* i = "\xD2\xA7\xC6\x8D\xD6\xAE\xC5\xBC";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = (4 * sizeof(utf16_t)) - 2;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(6, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0x04A7, o[0]);
	EXPECT_CPEQ(0x018D, o[1]);
	EXPECT_CPEQ(0x05AE, o[2]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf16, ThreeBytesSingle)
{
	const char* i = "\xE1\x8C\x8A";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = 255 * sizeof(utf16_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0x130A, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, ThreeBytesSingleFirst)
{
	const char* i = "\xE0\xA0\x80";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = 255 * sizeof(utf16_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0x0800, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, ThreeBytesSingleLast)
{
	const char* i = "\xEF\xBF\xBF";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = 255 * sizeof(utf16_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0xFFFF, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, ThreeBytesSingleInvalidContinuationFirstByteLower)
{
	const char* i = "\xE4\x2F\x8A";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = 255 * sizeof(utf16_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(6, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0xFFFD, o[0]);
	EXPECT_CPEQ(0x002F, o[1]);
	EXPECT_CPEQ(0xFFFD, o[2]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, ThreeBytesSingleInvalidContinuationFirstByteUpper)
{
	const char* i = "\xE1\xC5\xA3";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = 255 * sizeof(utf16_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0xFFFD, o[0]);
	EXPECT_CPEQ(0x0163, o[1]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, ThreeBytesSingleInvalidContinuationSecondByteLower)
{
	const char* i = "\xE0\x87\x2E";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = 255 * sizeof(utf16_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0xFFFD, o[0]);
	EXPECT_CPEQ(0x002E, o[1]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, ThreeBytesSingleInvalidContinuationSecondByteUpper)
{
	const char* i = "\xE2\xAB\xD4";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = 255 * sizeof(utf16_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0xFFFD, o[0]);
	EXPECT_CPEQ(0xFFFD, o[1]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, ThreeBytesSingleAmountOfBytes)
{
	const char* i = "\xE0\xA0\x91";
	size_t is = strlen(i);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, is, nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, ThreeBytesSingleNotEnoughSpaceOneByte)
{
	const char* i = "\xE0\xA6\xA1";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = sizeof(utf16_t) - 1;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0x0000, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf16, ThreeBytesSingleNotEnoughSpaceTwoBytes)
{
	const char* i = "\xE0\xAA\xBC";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = sizeof(utf16_t) - 2;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0x0000, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf16, ThreeBytesSingleMissingOneByte)
{
	const char* i = "\xE1\x81";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = 255 * sizeof(utf16_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0xFFFD, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, ThreeBytesSingleMissingTwoBytes)
{
	const char* i = "\xE2";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = 255 * sizeof(utf16_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0xFFFD, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, ThreeBytesSingleMissingAmountOfBytes)
{
	const char* i = "\xE1\x87";
	size_t is = strlen(i);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, is, nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, ThreeBytesSingleMissingNotEnoughSpaceOneByte)
{
	const char* i = "\xE1\x9A";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = sizeof(utf16_t) - 1;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0x0000, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf16, ThreeBytesSingleMissingNotEnoughSpaceTwoBytes)
{
	const char* i = "\xE2";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = sizeof(utf16_t) - 2;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0x0000, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf16, ThreeBytesSingleOverlongOneByteFirst)
{
	// U+0000

	const char* i = "\xE0\x80\x80";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = 255 * sizeof(utf16_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0xFFFD, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, ThreeBytesSingleOverlongOneByteLast)
{
	// U+007F

	const char* i = "\xE0\x81\xBF";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = 255 * sizeof(utf16_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0xFFFD, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, ThreeBytesSingleOverlongTwoBytesFirst)
{
	// U+0080

	const char* i = "\xE0\x82\x80";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = 255 * sizeof(utf16_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0xFFFD, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, ThreeBytesSingleOverlongTwoBytesLast)
{
	// U+07FF

	const char* i = "\xE0\x9F\xBF";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = 255 * sizeof(utf16_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0xFFFD, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, ThreeBytesSingleOverlongAmountOfBytes)
{
	const char* i = "\xE0\x8A\xA1";
	size_t is = strlen(i);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, is, nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, ThreeBytesSingleOverlongNotEnoughSpaceOneByte)
{
	const char* i = "\xE1\xA2\x8A";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = sizeof(utf16_t) - 1;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0x0000, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf16, ThreeBytesSingleOverlongNotEnoughSpaceTwoBytes)
{
	const char* i = "\xE0\xB2\xA7";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = sizeof(utf16_t) - 2;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0x0000, o[0]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf16, ThreeBytesMultiple)
{
	const char* i = "\xE3\x81\x8A\xE3\x81\x8D\xE3\x81\x99\xE3\x81\x88";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = 255 * sizeof(utf16_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(8, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0x304A, o[0]);
	EXPECT_CPEQ(0x304D, o[1]);
	EXPECT_CPEQ(0x3059, o[2]);
	EXPECT_CPEQ(0x3048, o[3]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, ThreeBytesMultipleOverlong)
{
	const char* i = "\xE0\x88\xA1\xE0\x81\xA3\xE0\x8A\x9F";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = 255 * sizeof(utf16_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(6, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0xFFFD, o[0]);
	EXPECT_CPEQ(0xFFFD, o[1]);
	EXPECT_CPEQ(0xFFFD, o[2]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, ThreeBytesMultipleAmountOfBytes)
{
	const char* i = "\xE0\xA0\x97\xE0\xA6\xAA\xE0\xA8\xA0";
	size_t is = strlen(i);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(6, utf8toutf16(i, is, nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, ThreeBytesMultipleOverlongNotEnoughSpaceOneByte)
{
	const char* i = "\xE0\x81\xA7\xE0\x91\xB1\xE0\x87\x86";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = (3 * sizeof(utf16_t)) - 1;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0xFFFD, o[0]);
	EXPECT_CPEQ(0xFFFD, o[1]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf16, ThreeBytesMultipleOverlongNotEnoughSpaceTwoBytes)
{
	const char* i = "\xE0\x87\xA1\xE0\x86\xA1\xE0\x91\xE2";
	size_t is = strlen(i);
	utf16_t o[256] = { 0 };
	size_t os = (3 * sizeof(utf16_t)) - 2;
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0xFFFD, o[0]);
	EXPECT_CPEQ(0xFFFD, o[1]);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf8ToUtf16, FourBytes)
{
	const char* i = "\xF0\x9F\x98\x8E";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_CPEQ(0xD83D, o[0]);
	EXPECT_CPEQ(0xDE0E, o[1]);
}

TEST(Utf8ToUtf16, FourBytesFirst)
{
	const char* i = "\xF0\x90\x80\x80";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_CPEQ(0xD800, o[0]);
	EXPECT_CPEQ(0xDC00, o[1]);
}

TEST(Utf8ToUtf16, FourBytesLast)
{
	const char* i = "\xF4\x80\x83\xBF";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_CPEQ(0xDBC0, o[0]);
	EXPECT_CPEQ(0xDCFF, o[1]);
}

TEST(Utf8ToUtf16, FourBytesString)
{
	const char* i = "\xF0\x90\xA1\x8A\xF0\x90\xB0\xAC\xF0\x90\xB0\xB8\xF0\x90\xB0\xA6";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(16, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_CPEQ(0xD802, o[0]);
	EXPECT_CPEQ(0xDC4A, o[1]);
	EXPECT_CPEQ(0xD803, o[2]);
	EXPECT_CPEQ(0xDC2C, o[3]);
	EXPECT_CPEQ(0xD803, o[4]);
	EXPECT_CPEQ(0xDC38, o[5]);
	EXPECT_CPEQ(0xD803, o[6]);
	EXPECT_CPEQ(0xDC26, o[7]);
}

TEST(Utf8ToUtf16, FourBytesOverlong)
{
	const char* i = "\xF0\x80\x80\xAF";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_CPEQ(0xFFFD, o[0]);
}

TEST(Utf8ToUtf16, FourBytesOverlongNull)
{
	const char* i = "\xF0\x80\x80\x80";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_CPEQ(0xFFFD, o[0]);
}

TEST(Utf8ToUtf16, FourBytesOverlongFirst)
{
	const char* i = "\xF4\x90\x80\x80";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_CPEQ(0xFFFD, o[0]);
}

TEST(Utf8ToUtf16, FourBytesOverlongLast)
{
	const char* i = "\xF7\xBF\xBF\xBF";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_CPEQ(0xFFFD, o[0]);
}

TEST(Utf8ToUtf16, FourBytesOverlongMaximum)
{
	const char* i = "\xF0\x8F\xBF\xBF";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_CPEQ(0xFFFD, o[0]);
}

TEST(Utf8ToUtf16, FourBytesLonelyStartFirst)
{
	const char* i = "\xF0 ";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_CPEQ(0xFFFD, o[0]);
	EXPECT_CPEQ(' ', o[1]);
}

TEST(Utf8ToUtf16, FourBytesLonelyStartLast)
{
	const char* i = "\xF7 ";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_CPEQ(0xFFFD, o[0]);
	EXPECT_CPEQ(' ', o[1]);
}

TEST(Utf8ToUtf16, FourBytesLonelyStartCombined)
{
	const char* i =
		"\xF0 \xF1 \xF2 \xF3 \xF4 \xF5 \xF6 \xF7 ";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(32, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);

	for (size_t i = 0; i < 8; i += 2)
	{
		EXPECT_CPEQ(0xFFFD, o[i]);
		EXPECT_EQ(' ', o[i + 1]);
	}
}

TEST(Utf8ToUtf16, FourBytesNotEnoughData)
{
	const char* i = "\xF0\x90\x80";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_CPEQ(0xFFFD, o[0]);
}

TEST(Utf8ToUtf16, FiveBytesOverlong)
{
	const char* i = "\xF8\x80\x80\x80\xAF";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_CPEQ(0xFFFD, o[0]);
}

TEST(Utf8ToUtf16, FiveBytesOverlongFirst)
{
	const char* i = "\xF8\x88\x80\x80\x80";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_CPEQ(0xFFFD, o[0]);
}

TEST(Utf8ToUtf16, FiveBytesOverlongLast)
{
	const char* i = "\xFB\xBF\xBF\xBF\xBF";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_CPEQ(0xFFFD, o[0]);
}

TEST(Utf8ToUtf16, FiveBytesOverlongNull)
{
	const char* i = "\xF8\x80\x80\x80\x80";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_CPEQ(0xFFFD, o[0]);
}

TEST(Utf8ToUtf16, FiveBytesOverlongMaximum)
{
	const char* i = "\xF8\x87\xBF\xBF\xBF";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_CPEQ(0xFFFD, o[0]);
}

TEST(Utf8ToUtf16, FiveBytesLonelyStartFirst)
{
	const char* i = "\xF8 ";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_CPEQ(0xFFFD, o[0]);
	EXPECT_CPEQ(' ', o[1]);
}

TEST(Utf8ToUtf16, FiveBytesLonelyStartLast)
{
	const char* i = "\xFB ";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_CPEQ(0xFFFD, o[0]);
	EXPECT_CPEQ(' ', o[1]);
}

TEST(Utf8ToUtf16, FiveBytesLonelyStartCombined)
{
	const char* i =
		"\xF8 \xF9 \xFA \xFB ";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(16, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);

	for (size_t i = 0; i < 4; i += 2)
	{
		EXPECT_CPEQ(0xFFFD, o[i]);
		EXPECT_EQ(' ', o[i + 1]);
	}
}

TEST(Utf8ToUtf16, FiveBytesNotEnoughData)
{
	const char* i = "\xF8\x80\x80\x80";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_CPEQ(0xFFFD, o[0]);
}

TEST(Utf8ToUtf16, SixBytesOverlong)
{
	const char* i = "\xFC\x80\x80\x80\x80\xAF";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_CPEQ(0xFFFD, o[0]);
}

TEST(Utf8ToUtf16, SixBytesOverlongFirst)
{
	const char* i = "\xFC\x84\x80\x80\x80\x80";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_CPEQ(0xFFFD, o[0]);
}

TEST(Utf8ToUtf16, SixBytesOverlongLast)
{
	const char* i = "\xFD\xBF\xBF\xBF\xBF\xBF";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_CPEQ(0xFFFD, o[0]);
}

TEST(Utf8ToUtf16, SixBytesOverlongNull)
{
	const char* i = "\xFC\x80\x80\x80\x80\x80";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_CPEQ(0xFFFD, o[0]);
}

TEST(Utf8ToUtf16, SixBytesOverlongMaximum)
{
	const char* i = "\xFC\x83\xBF\xBF\xBF\xBF";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_CPEQ(0xFFFD, o[0]);
}

TEST(Utf8ToUtf16, SixBytesLonelyStartFirst)
{
	const char* i = "\xFC ";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_CPEQ(0xFFFD, o[0]);
	EXPECT_CPEQ(' ', o[1]);
}

TEST(Utf8ToUtf16, SixBytesLonelyStartLast)
{
	const char* i = "\xFD ";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_CPEQ(0xFFFD, o[0]);
	EXPECT_CPEQ(' ', o[1]);
}

TEST(Utf8ToUtf16, SixBytesLonelyStartCombined)
{
	const char* i =
		"\xFC \xFD ";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(8, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);

	for (size_t i = 0; i < 2; i += 2)
	{
		EXPECT_CPEQ(0xFFFD, o[i]);
		EXPECT_EQ(' ', o[i + 1]);
	}
}

TEST(Utf8ToUtf16, SixBytesNotEnoughData)
{
	const char* i = "\xFC\x80\x80\x80\x80";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_CPEQ(0xFFFD, o[0]);
}

TEST(Utf8ToUtf16, SixBytesNotEnoughSpace)
{
	const char* i = "Der\xFD\xBF\xBF\xBF\xBF\xBFp";
	const size_t s = 4;
	utf16_t o[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(8, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
	EXPECT_CPEQ('D', o[0]);
	EXPECT_CPEQ('e', o[1]);
	EXPECT_CPEQ('r', o[2]);
	EXPECT_CPEQ(0xFFFD, o[3]);
}

TEST(Utf8ToUtf16, SurrogatePair)
{
	const char* i = "\xED\xAD\x80\xED\xBE\x80";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_CPEQ(0xFFFD, o[0]);
	EXPECT_CPEQ(0xFFFD, o[1]);
}

TEST(Utf8ToUtf16, SurrogatePairCombined)
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
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(32, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_CPEQ(0xFFFD, o[0]);
	EXPECT_CPEQ(0xFFFD, o[1]);
	EXPECT_CPEQ(0xFFFD, o[2]);
	EXPECT_CPEQ(0xFFFD, o[3]);
	EXPECT_CPEQ(0xFFFD, o[4]);
	EXPECT_CPEQ(0xFFFD, o[5]);
	EXPECT_CPEQ(0xFFFD, o[6]);
	EXPECT_CPEQ(0xFFFD, o[7]);
	EXPECT_CPEQ(0xFFFD, o[8]);
	EXPECT_CPEQ(0xFFFD, o[9]);
	EXPECT_EQ(0xFFFD, o[10]);
	EXPECT_EQ(0xFFFD, o[11]);
	EXPECT_EQ(0xFFFD, o[12]);
	EXPECT_EQ(0xFFFD, o[13]);
	EXPECT_EQ(0xFFFD, o[14]);
	EXPECT_EQ(0xFFFD, o[15]);
}

TEST(Utf8ToUtf16, SurrogatePairHigh)
{
	const char* i = "\xED\xAD\xBF";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_CPEQ(0xFFFD, o[0]);
}

TEST(Utf8ToUtf16, SurrogatePairHighStart)
{
	const char* i = "\xED\xA0\x80";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_CPEQ(0xFFFD, o[0]);
}

TEST(Utf8ToUtf16, SurrogatePairHighEnd)
{
	const char* i = "\xED\xAF\xBF";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_CPEQ(0xFFFD, o[0]);
}

TEST(Utf8ToUtf16, SurrogatePairLow)
{
	const char* i = "\xED\xBE\x80";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_CPEQ(0xFFFD, o[0]);
}

TEST(Utf8ToUtf16, SurrogatePairLowStart)
{
	const char* i = "\xED\xB0\x80";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_CPEQ(0xFFFD, o[0]);
}

TEST(Utf8ToUtf16, SurrogatePairLowEnd)
{
	const char* i = "\xED\xBF\xBF";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_CPEQ(0xFFFD, o[0]);
}

TEST(Utf8ToUtf16, SurrogatePairOverlongHighStart)
{
	const char* i = "\xF0\x8D\xA0\x80";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_CPEQ(0xFFFD, o[0]);
}

TEST(Utf8ToUtf16, SurrogatePairOverlongHighEnd)
{
	const char* i = "\xF0\x8D\xAF\xBF";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_CPEQ(0xFFFD, o[0]);
}

TEST(Utf8ToUtf16, SurrogatePairOverlongLowStart)
{
	const char* i = "\xF0\x8D\xAD\xBF";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_CPEQ(0xFFFD, o[0]);
}

TEST(Utf8ToUtf16, SurrogatePairOverlongLowEnd)
{
	const char* i = "\xF0\x8D\xBF\xBF";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_CPEQ(0xFFFD, o[0]);
}

TEST(Utf8ToUtf16, SurrogatePairNotEnoughData)
{
	const char* i = "\xED\xBF";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_CPEQ(0xFFFD, o[0]);
}

TEST(Utf8ToUtf16, AmountOfBytes)
{
	const char* i = "\xE0\xB4\x8B";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, strlen(i), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, AmountOfBytesString)
{
	const char* i = "Pchn\xC4\x85\xC4\x87 w t\xC4\x99";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(22, utf8toutf16(i, strlen(i), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, AmountOfBytesNotEnoughData)
{
	const char* i = "\xF0\x90\xB1";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, strlen(i), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, AmountOfBytesOverlong)
{
	const char* i = "\xFC\x84\x80\x80\x80\x80";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toutf16(i, strlen(i), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, AmountOfBytesNoData)
{
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8toutf16(nullptr, 1, nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(Utf8ToUtf16, StringEndsInMiddle)
{
	const char* i = "\xCE\xBA\xE1\xBD\xB9\x00\xCF\x83\xCE\xBC\xCE\xB5";
	size_t is = 12;
	utf16_t o[256] = { 0 };
	size_t os = 255 * sizeof(utf16_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(12, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0x03BA, o[0]);
	EXPECT_CPEQ(0x1F79, o[1]);
	EXPECT_CPEQ(0, o[2]);
	EXPECT_CPEQ(0x03C3, o[3]);
	EXPECT_CPEQ(0x03BC, o[4]);
	EXPECT_CPEQ(0x03B5, o[5]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, StringDataSizeUnder)
{
	const char* i = "p\xC3\xA5 xylofon";
	size_t is = 4;
	utf16_t o[256] = { 0 };
	size_t os = 255 * sizeof(utf16_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(6, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ('p', o[0]);
	EXPECT_CPEQ(0x00E5, o[1]);
	EXPECT_CPEQ(' ', o[2]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, StringDataSizeOver)
{
	char i[31] = { 0 };
	strcpy(i, "\xCE\x93\xCE\xB1\xCE\xB6");
	size_t is = 18;
	utf16_t o[256] = { 0 };
	size_t os = 255 * sizeof(utf16_t);
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(30, utf8toutf16(i, is, o, os, &errors));
	EXPECT_CPEQ(0x0393, o[0]);
	EXPECT_CPEQ(0x03B1, o[1]);
	EXPECT_CPEQ(0x03B6, o[2]);
	EXPECT_CPEQ(0, o[3]);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, ErrorsIsReset)
{
	const char* i = "\xC2\x89";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = 12334;

	EXPECT_EQ(2, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_CPEQ(0x0089, o[0]);
}

TEST(Utf8ToUtf16, OverlappingParametersFits)
{
	int32_t errors = UTF8_ERR_NONE;

	char data[128] = { 0 };
	strcpy(data, "kg");

	const char* i = data;
	size_t is = 2;
	utf16_t* o = (utf16_t*)(data + 2);
	size_t os = 4;

	EXPECT_EQ(4, utf8toutf16(i, is, o, os, &errors));
	EXPECT_MEMEQ("kgk\0g\0", data, 6);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf16, OverlappingParametersStartsEqual)
{
	int32_t errors = UTF8_ERR_NONE;

	char data[128] = { 0 };

	const char* i = data + 10;
	size_t is = 40;
	utf16_t* o = (utf16_t*)(data + 10);
	size_t os = 4;

	EXPECT_EQ(0, utf8toutf16(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST(Utf8ToUtf16, OverlappingParametersEndsEqual)
{
	int32_t errors = UTF8_ERR_NONE;

	char data[128] = { 0 };

	const char* i = data + 1;
	size_t is = 39;
	utf16_t* o = (utf16_t*)(data + 20);
	size_t os = 20;

	EXPECT_EQ(0, utf8toutf16(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST(Utf8ToUtf16, OverlappingParametersInputStartsInTarget)
{
	int32_t errors = UTF8_ERR_NONE;

	char data[128] = { 0 };

	const char* i = data + 18;
	size_t is = 64;
	utf16_t* o = (utf16_t*)(data + 14);
	size_t os = 46;

	EXPECT_EQ(0, utf8toutf16(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST(Utf8ToUtf16, OverlappingParametersInputEndsInTarget)
{
	int32_t errors = UTF8_ERR_NONE;

	char data[128] = { 0 };

	const char* i = data + 4;
	size_t is = 24;
	utf16_t* o = (utf16_t*)(data + 10);
	size_t os = 28;

	EXPECT_EQ(0, utf8toutf16(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST(Utf8ToUtf16, OverlappingParametersInputInsideTarget)
{
	int32_t errors = UTF8_ERR_NONE;

	char data[128] = { 0 };

	const char* i = data + 28;
	size_t is = 22;
	utf16_t* o = (utf16_t*)(data + 22);
	size_t os = 34;

	EXPECT_EQ(0, utf8toutf16(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST(Utf8ToUtf16, OverlappingParametersTargetStartsInInput)
{
	int32_t errors = UTF8_ERR_NONE;

	char data[128] = { 0 };

	const char* i = data + 19;
	size_t is = 17;
	utf16_t* o = (utf16_t*)(data + 24);
	size_t os = 38;

	EXPECT_EQ(0, utf8toutf16(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST(Utf8ToUtf16, OverlappingParametersTargetEndsInInput)
{
	int32_t errors = UTF8_ERR_NONE;

	char data[128] = { 0 };

	const char* i = data + 44;
	size_t is = 21;
	utf16_t* o = (utf16_t*)(data + 12);
	size_t os = 34;

	EXPECT_EQ(0, utf8toutf16(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST(Utf8ToUtf16, OverlappingParametersTargetInsideInput)
{
	int32_t errors = UTF8_ERR_NONE;

	char data[128] = { 0 };

	const char* i = data + 28;
	size_t is = 28;
	utf16_t* o = (utf16_t*)(data + 32);
	size_t os = 6;

	EXPECT_EQ(0, utf8toutf16(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}