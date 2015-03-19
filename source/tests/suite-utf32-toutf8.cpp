﻿#include "tests-base.hpp"

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

TEST(Utf32ToUtf8, BasicLatinMultipleEndsInMiddle)
{
	unicode_t i[] = { 'S', 't', 'a', 'n', 0x00, 'd', 's' };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors;

	EXPECT_EQ(7, utf32toutf8(i, is, o, os, &errors));
	EXPECT_MEMEQ("Stan\x00" "ds", o, 7);
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


TEST(Utf32ToUtf8, BufferTooSmall)
{
	unicode_t c = 0x00001F61; // ὡ
	const size_t s = 2;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf32toutf8(&c, sizeof(c), b, s, &errors));
	EXPECT_UTF8EQ("", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf32ToUtf8, ZeroLength)
{
	unicode_t c = 0x8712;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf32toutf8(&c, sizeof(c), b, 0, &errors));
	EXPECT_UTF8EQ("", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf32ToUtf8, ZeroBuffer)
{
	unicode_t c = 'K';
	const size_t s = 256;
	int32_t errors = 0;

	EXPECT_EQ(1, utf32toutf8(&c, sizeof(c), nullptr, s, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, String)
{
	unicode_t c[] = {
		'S',
		0x1F12E,
		'H',
		0xA840, // ꡀ
		0x07D4  // ߔ
	};
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(11, utf32toutf8(c, sizeof(c), b, s, &errors));
	EXPECT_UTF8EQ("S\xF0\x9F\x84\xAEH\xEA\xA1\x80\xDF\x94", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, StringEndsInMiddle)
{
	unicode_t c[] = {
		0x2488, // ⒈
		'[',
		'r',
		0,
		'n'
	};
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(5, utf32toutf8(c, sizeof(c), b, s, &errors));
	EXPECT_UTF8EQ("\xE2\x92\x88[r", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, StringEmpty)
{
	unicode_t c[] = {
		0
	};
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf32toutf8(c, sizeof(c), b, s, &errors));
	EXPECT_UTF8EQ("", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, StringBufferTooSmall)
{
	unicode_t c[] = {
		'F',
		'o',
		'o',
		0x3DB1 // 㶱
	};
	const size_t s = 4;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf32toutf8(c, sizeof(c), b, s, &errors));
	EXPECT_UTF8EQ("Foo", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Utf32ToUtf8, Ascii)
{
	unicode_t c = ')';
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1, utf32toutf8(&c, sizeof(c), b, s, &errors));
	EXPECT_UTF8EQ(")", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, AsciiFirst)
{
	unicode_t c = 0;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf32toutf8(&c, sizeof(c), b, s, &errors));
	EXPECT_UTF8EQ("", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, AsciiLast)
{
	unicode_t c = 0x7F;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1, utf32toutf8(&c, sizeof(c), b, s, &errors));
	EXPECT_UTF8EQ("\x7F", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, AsciiString)
{
	unicode_t c[] = { 'B', 'o', 'm', 'b' };
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf32toutf8(c, sizeof(c), b, s, &errors));
	EXPECT_UTF8EQ("Bomb", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, TwoBytes)
{
	unicode_t c = 0x000003A9; // Ω
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf32toutf8(&c, sizeof(c), b, s, &errors));
	EXPECT_UTF8EQ("\xCE\xA9", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, TwoBytesFirst)
{
	unicode_t c = 0x00000080; // Ω
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf32toutf8(&c, sizeof(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC2\x80", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, TwoBytesLast)
{
	unicode_t c = 0x000007FF;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf32toutf8(&c, sizeof(c), b, s, &errors));
	EXPECT_UTF8EQ("\xDF\xBF", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, TwoBytesString)
{
	unicode_t c[] = {
		0x00000169, // ũ
		0x0000014E, // Ŏ
		0x00000191, // Ƒ
		0x0000014A  // Ŋ
	};
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(8, utf32toutf8(c, sizeof(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC5\xA9\xC5\x8E\xC6\x91\xC5\x8A", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, ThreeBytes)
{
	unicode_t c = 0x00003DB1; // 㶱
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf32toutf8(&c, sizeof(c), b, s, &errors));
	EXPECT_UTF8EQ("\xE3\xB6\xB1", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, ThreeBytesFirst)
{
	unicode_t c = 0x00000800;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf32toutf8(&c, sizeof(c), b, s, &errors));
	EXPECT_UTF8EQ("\xE0\xA0\x80", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, ThreeBytesLast)
{
	unicode_t c = 0x0000FFFF;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf32toutf8(&c, sizeof(c), b, s, &errors));
	EXPECT_UTF8EQ("\xEF\xBF\xBF", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, ThreeBytesString)
{
	unicode_t c[] = {
		0x00002776, // ❶
		0x00002778, // ❸
		0x00002665, // ♥
		0x0000277D  // ❽
	};
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(12, utf32toutf8(c, sizeof(c), b, s, &errors));
	EXPECT_UTF8EQ("\xE2\x9D\xB6\xE2\x9D\xB8\xE2\x99\xA5\xE2\x9D\xBD", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, FourBytes)
{
	unicode_t c = 0x0001D424; // 𝐤
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf32toutf8(&c, sizeof(c), b, s, &errors));
	EXPECT_UTF8EQ("\xF0\x9D\x90\xA4", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, FourBytesFirst)
{
	unicode_t c = 0x00010000;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf32toutf8(&c, sizeof(c), b, s, &errors));
	EXPECT_UTF8EQ("\xF0\x90\x80\x80", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, FourBytesLast)
{
	unicode_t c = 0x0010FFFF;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf32toutf8(&c, sizeof(c), b, s, &errors));
	EXPECT_UTF8EQ("\xF4\x8F\xBF\xBF", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, FourBytesLength)
{
	unicode_t c = 0x1200D;
	int32_t errors = 0;

	EXPECT_EQ(4, utf32toutf8(&c, sizeof(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, FourBytesString)
{
	unicode_t c[] = {
		0x1F191, // 🆑
		0x1F198, // 🆘
		0x1F19A  // 🆚
	};
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(12, utf32toutf8(c, sizeof(c), b, s, &errors));
	EXPECT_UTF8EQ("\xF0\x9F\x86\x91\xF0\x9F\x86\x98\xF0\x9F\x86\x9A", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, AboveLegalUnicode)
{
	unicode_t c = 0x110001;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf32toutf8(&c, sizeof(c), b, s, &errors));
	EXPECT_UTF8EQ("\xEF\xBF\xBD", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, SurrogatePair)
{
	unicode_t c[] = {
		0xD834, 0xDE45
	};
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf32toutf8(c, sizeof(c), b, s, &errors));
	EXPECT_UTF8EQ("\xF0\x9D\x89\x85", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, SurrogatePairFirst)
{
	unicode_t c[] = {
		0xD800, 0xDC00
	};
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf32toutf8(c, sizeof(c), b, s, &errors));
	EXPECT_UTF8EQ("\xF0\x90\x80\x80", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, SurrogatePairLast)
{
	unicode_t c[] = {
		0xDBFF, 0xDFFF
	};
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf32toutf8(c, sizeof(c), b, s, &errors));
	EXPECT_UTF8EQ("\xF4\x8F\xBF\xBF", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, SurrogatePairString)
{
	unicode_t c[] = {
		0xD83D, 0xDE12,
		0xD83D, 0xDE22,
		0xD83D, 0xDE24,
	};
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(12, utf32toutf8(c, sizeof(c), b, s, &errors));
	EXPECT_UTF8EQ("\xF0\x9F\x98\x92\xF0\x9F\x98\xA2\xF0\x9F\x98\xA4", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, SurrogatePairUnmatchedLow)
{
	unicode_t c[] = {
		0xD820, 0x17
	};
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf32toutf8(c, sizeof(c), b, s, &errors));
	EXPECT_UTF8EQ("", b);
	EXPECT_ERROREQ(UTF8_ERR_UNMATCHED_LOW_SURROGATE_PAIR, errors);
}

TEST(Utf32ToUtf8, SurrogatePairUnmatchedHigh)
{
	unicode_t c[] = {
		0xDD1E, 0xD834
	};
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf32toutf8(c, sizeof(c), b, s, &errors));
	EXPECT_UTF8EQ("", b);
	EXPECT_ERROREQ(UTF8_ERR_UNMATCHED_HIGH_SURROGATE_PAIR, errors);
}

TEST(Utf32ToUtf8, SurrogatePairStringUnmatchedPair)
{
	unicode_t c[] = {
		0xD83D, 0xDE12,
		0xD83D, 0x988,
		0xD834, 0xDE24,
	};
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf32toutf8(c, sizeof(c), b, s, &errors));
	EXPECT_UTF8EQ("\xF0\x9F\x98\x92", b);
	EXPECT_ERROREQ(UTF8_ERR_UNMATCHED_LOW_SURROGATE_PAIR, errors);
}

TEST(Utf32ToUtf8, AmountOfBytes)
{
	unicode_t c = 0x00000656;
	int32_t errors = 0;

	EXPECT_EQ(2, utf32toutf8(&c, sizeof(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, AmountOfBytesSurrogatePair)
{
	unicode_t c[] = {
		0x0000D967, 0x0000DDDD
	};
	int32_t errors = 0;

	EXPECT_EQ(4, utf32toutf8(c, sizeof(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, AmountOfBytesAboveLegalUnicode)
{
	unicode_t c = 0x199128;
	int32_t errors = 0;

	EXPECT_EQ(3, utf32toutf8(&c, sizeof(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf32ToUtf8, AmountOfBytesNoData)
{
	int32_t errors = 0;

	EXPECT_EQ(0, utf32toutf8(nullptr, 1, nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(Utf32ToUtf8, ErrorsIsReset)
{
	unicode_t c = 0x1788A;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf32toutf8(&c, sizeof(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("\xF0\x97\xA2\x8A", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
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