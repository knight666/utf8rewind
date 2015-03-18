#include "tests-base.hpp"

#include "utf8rewind.h"

#include "helpers-strings.hpp"

TEST(Utf32ToUtf8, Character)
{
	unicode_t c = 'U';
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1, utf32toutf8(&c, sizeof(c), b, s, &errors));
	EXPECT_UTF8EQ("U", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
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