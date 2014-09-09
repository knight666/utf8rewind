﻿#include "tests-base.hpp"

#include "utf8rewind.h"

TEST(EncodeUtf32, Character)
{
	unicode_t c = 'U';
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1, utf32toutf8(&c, sizeof(c), b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("U", b);
}

TEST(EncodeUtf32, BufferTooSmall)
{
	unicode_t c = 0x00001F61; // ὡ
	const size_t s = 2;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf32toutf8(&c, sizeof(c), b, s, &errors));
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
	EXPECT_STREQ("", b);
}

TEST(EncodeUtf32, ZeroLength)
{
	unicode_t c = 0x8712;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf32toutf8(&c, sizeof(c), b, 0, &errors));
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
	EXPECT_STREQ("", b);
}

TEST(EncodeUtf32, ZeroBuffer)
{
	unicode_t c = 'K';
	const size_t s = 256;
	int32_t errors = 0;

	EXPECT_EQ(1, utf32toutf8(&c, sizeof(c), nullptr, s, &errors));
	EXPECT_EQ(0, errors);
}

TEST(EncodeUtf32, String)
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
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("S\xF0\x9F\x84\xAEH\xEA\xA1\x80\xDF\x94", b);
}

TEST(EncodeUtf32, StringEndsInMiddle)
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
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xE2\x92\x88[r", b);
}

TEST(EncodeUtf32, StringEmpty)
{
	unicode_t c[] = {
		0
	};
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf32toutf8(c, sizeof(c), b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("", b);
}

TEST(EncodeUtf32, StringBufferTooSmall)
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
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
	EXPECT_STREQ("Foo", b);
}

TEST(EncodeUtf32, Ascii)
{
	unicode_t c = ')';
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1, utf32toutf8(&c, sizeof(c), b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ(")", b);
}

TEST(EncodeUtf32, AsciiFirst)
{
	unicode_t c = 0;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf32toutf8(&c, sizeof(c), b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("", b);
}

TEST(EncodeUtf32, AsciiLast)
{
	unicode_t c = 0x7F;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1, utf32toutf8(&c, sizeof(c), b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\x7F", b);
}

TEST(EncodeUtf32, AsciiString)
{
	unicode_t c[] = { 'B', 'o', 'm', 'b' };
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf32toutf8(c, sizeof(c), b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("Bomb", b);
}

TEST(EncodeUtf32, TwoBytes)
{
	unicode_t c = 0x000003A9; // Ω
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf32toutf8(&c, sizeof(c), b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xCE\xA9", b);
}

TEST(EncodeUtf32, TwoBytesFirst)
{
	unicode_t c = 0x00000080; // Ω
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf32toutf8(&c, sizeof(c), b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xC2\x80", b);
}

TEST(EncodeUtf32, TwoBytesLast)
{
	unicode_t c = 0x000007FF;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf32toutf8(&c, sizeof(c), b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xDF\xBF", b);
}

TEST(EncodeUtf32, TwoBytesString)
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
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xC5\xA9\xC5\x8E\xC6\x91\xC5\x8A", b);
}

TEST(EncodeUtf32, ThreeBytes)
{
	unicode_t c = 0x00003DB1; // 㶱
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf32toutf8(&c, sizeof(c), b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xE3\xB6\xB1", b);
}

TEST(EncodeUtf32, ThreeBytesFirst)
{
	unicode_t c = 0x00000800;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf32toutf8(&c, sizeof(c), b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xE0\xA0\x80", b);
}

TEST(EncodeUtf32, ThreeBytesLast)
{
	unicode_t c = 0x0000FFFF;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf32toutf8(&c, sizeof(c), b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xEF\xBF\xBF", b);
}

TEST(EncodeUtf32, ThreeBytesString)
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
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xE2\x9D\xB6\xE2\x9D\xB8\xE2\x99\xA5\xE2\x9D\xBD", b);
}

TEST(EncodeUtf32, FourBytes)
{
	unicode_t c = 0x0001D424; // 𝐤
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf32toutf8(&c, sizeof(c), b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xF0\x9D\x90\xA4", b);
}

TEST(EncodeUtf32, FourBytesFirst)
{
	unicode_t c = 0x00010000;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf32toutf8(&c, sizeof(c), b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xF0\x90\x80\x80", b);
}

TEST(EncodeUtf32, FourBytesLast)
{
	unicode_t c = 0x0010FFFF;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf32toutf8(&c, sizeof(c), b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xF4\x8F\xBF\xBF", b);
}

TEST(EncodeUtf32, FourBytesLength)
{
	unicode_t c = 0x1200D;
	int32_t errors = 0;

	EXPECT_EQ(4, utf32toutf8(&c, sizeof(c), nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(EncodeUtf32, FourBytesString)
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
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xF0\x9F\x86\x91\xF0\x9F\x86\x98\xF0\x9F\x86\x9A", b);
}

TEST(EncodeUtf32, AboveLegalUnicode)
{
	unicode_t c = 0x110001;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf32toutf8(&c, sizeof(c), b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xEF\xBF\xBD", b);
}

TEST(EncodeUtf32, SurrogatePair)
{
	unicode_t c[] = {
		0xD834, 0xDE45
	};
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf32toutf8(c, sizeof(c), b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xF0\x9D\x89\x85", b);
}

TEST(EncodeUtf32, SurrogatePairFirst)
{
	unicode_t c[] = {
		0xD800, 0xDC00
	};
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf32toutf8(c, sizeof(c), b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xF0\x90\x80\x80", b);
}

TEST(EncodeUtf32, SurrogatePairLast)
{
	unicode_t c[] = {
		0xDBFF, 0xDFFF
	};
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf32toutf8(c, sizeof(c), b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xF4\x8F\xBF\xBF", b);
}

TEST(EncodeUtf32, SurrogatePairUnmatchedLow)
{
	unicode_t c[] = {
		0xD820, 0x17
	};
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf32toutf8(c, sizeof(c), b, s, &errors));
	EXPECT_EQ(UTF8_ERR_UNMATCHED_LOW_SURROGATE_PAIR, errors);
	EXPECT_STREQ("", b);
}

TEST(EncodeUtf32, SurrogatePairUnmatchedHigh)
{
	unicode_t c[] = {
		0xDD1E, 0xD834
	};
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf32toutf8(c, sizeof(c), b, s, &errors));
	EXPECT_EQ(UTF8_ERR_UNMATCHED_HIGH_SURROGATE_PAIR, errors);
	EXPECT_STREQ("", b);
}

TEST(EncodeUtf32, SurrogatePairStringUnmatchedPair)
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
	EXPECT_EQ(UTF8_ERR_UNMATCHED_LOW_SURROGATE_PAIR, errors);
	EXPECT_STREQ("\xF0\x9F\x98\x92", b);
}

TEST(EncodeUtf32, AmountOfBytes)
{
	unicode_t c = 0x00000656;
	int32_t errors = 0;

	EXPECT_EQ(2, utf32toutf8(&c, sizeof(c), nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(EncodeUtf32, AmountOfBytesAboveLegalUnicode)
{
	unicode_t c = 0x199128;
	int32_t errors = 0;

	EXPECT_EQ(3, utf32toutf8(&c, sizeof(c), nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(EncodeUtf32, AmountOfBytesNoData)
{
	int32_t errors = 0;

	EXPECT_EQ(0, utf32toutf8(nullptr, 1, nullptr, 0, &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}