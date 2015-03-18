#include "tests-base.hpp"

#include "utf8rewind.h"

#include "helpers-strings.hpp"

TEST(Utf16ToUtf8, Character)
{
	utf16_t c = 0xDD;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf16toutf8(&c, sizeof(c), b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xC3\x9D", b);
}

TEST(Utf16ToUtf8, BufferTooSmall)
{
	utf16_t c = 0x0922;
	const size_t s = 2;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf16toutf8(&c, sizeof(c), b, s, &errors));
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
	EXPECT_STREQ("", b);
}

TEST(Utf16ToUtf8, ZeroLength)
{
	utf16_t c = 0x1118;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf16toutf8(&c, sizeof(c), b, 0, &errors));
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
	EXPECT_STREQ("", b);
}

TEST(Utf16ToUtf8, ZeroBuffer)
{
	utf16_t c = 0x33DE;
	const size_t s = 256;
	int32_t errors = 0;

	EXPECT_EQ(3, utf16toutf8(&c, sizeof(c), nullptr, s, &errors));
	EXPECT_EQ(0, errors);
}

TEST(Utf16ToUtf8, String)
{
	utf16_t c[] = {
		'Y',
		0x88AD,
		'Q',
		0x0125,
		'#'
	};
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(8, utf16toutf8(c, sizeof(c), b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("Y\xE8\xA2\xADQ\xC4\xA5#", b);
}

TEST(Utf16ToUtf8, StringEndsInMiddle)
{
	utf16_t c[] = {
		0x98AD,
		'K',
		0,
		'B',
		'Y',
		'E'
	};
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf16toutf8(c, sizeof(c), b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xE9\xA2\xADK", b);
}

TEST(Utf16ToUtf8, StringEmpty)
{
	utf16_t c[] = {
		0
	};
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf16toutf8(c, sizeof(c), b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("", b);
}

TEST(Utf16ToUtf8, StringBufferTooSmall)
{
	utf16_t c[] = {
		'D',
		0x0998,
		'g',
		'g'
	};
	const size_t s = 3;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1, utf16toutf8(c, sizeof(c), b, s, &errors));
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
	EXPECT_STREQ("D", b);
}

TEST(Utf16ToUtf8, Ascii)
{
	utf16_t c = 'j';
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1, utf16toutf8(&c, sizeof(c), b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("j", b);
}

TEST(Utf16ToUtf8, AsciiFirst)
{
	utf16_t c = 0;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf16toutf8(&c, sizeof(c), b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("", b);
}

TEST(Utf16ToUtf8, AsciiLast)
{
	utf16_t c = 0x7F;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1, utf16toutf8(&c, sizeof(c), b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\x7F", b);
}

TEST(Utf16ToUtf8, AsciiString)
{
	utf16_t c[] = { 'x', '=', '(', '2', '*', '2', ')' };
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(7, utf16toutf8(c, sizeof(c), b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("x=(2*2)", b);
}

TEST(Utf16ToUtf8, TwoBytes)
{
	utf16_t c = 0x20D;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf16toutf8(&c, sizeof(c), b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xC8\x8D", b);
}

TEST(Utf16ToUtf8, TwoBytesFirst)
{
	utf16_t c = 0x80;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf16toutf8(&c, sizeof(c), b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xC2\x80", b);
}

TEST(Utf16ToUtf8, TwoBytesLast)
{
	utf16_t c = 0x07FF;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf16toutf8(&c, sizeof(c), b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xDF\xBF", b);
}

TEST(Utf16ToUtf8, TwoBytesString)
{
	utf16_t c[] = {
		0x0169, // ũ
		0x014E, // Ŏ
		0x0191, // Ƒ
		0x014A  // Ŋ
	};
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(8, utf16toutf8(c, sizeof(c), b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xC5\xA9\xC5\x8E\xC6\x91\xC5\x8A", b);
}

TEST(Utf16ToUtf8, ThreeBytes)
{
	utf16_t c = 0x78AD;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf16toutf8(&c, sizeof(c), b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xE7\xA2\xAD", b);
}

TEST(Utf16ToUtf8, ThreeBytesFirst)
{
	utf16_t c = 0x800;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf16toutf8(&c, sizeof(c), b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xE0\xA0\x80", b);
}

TEST(Utf16ToUtf8, ThreeBytesLast)
{
	utf16_t c = 0xFFFF;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf16toutf8(&c, sizeof(c), b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xEF\xBF\xBF", b);
}

TEST(Utf16ToUtf8, ThreeBytesString)
{
	utf16_t c[] = {
		0x0922,
		0x093F,
		0x0914,
		0x0B86
	};
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(12, utf16toutf8(c, sizeof(c), b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xE0\xA4\xA2\xE0\xA4\xBF\xE0\xA4\x94\xE0\xAE\x86", b);
}

TEST(Utf16ToUtf8, SurrogatePair)
{
	utf16_t c[] = {
		0xD834, 0xDD1E
	};
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf16toutf8(c, sizeof(c), b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xF0\x9D\x84\x9E", b);
}

TEST(Utf16ToUtf8, SurrogatePairFirst)
{
	utf16_t c[] = {
		0xD800, 0xDC00
	};
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf16toutf8(c, sizeof(c), b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xF0\x90\x80\x80", b);
}

TEST(Utf16ToUtf8, SurrogatePairLast)
{
	utf16_t c[] = {
		0xDBFF, 0xDFFF
	};
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf16toutf8(c, sizeof(c), b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xF4\x8F\xBF\xBF", b);
}

TEST(Utf16ToUtf8, SurrogatePairString)
{
	utf16_t c[] = {
		0xD821, 0xDC7D,
		0xD85E, 0xDF88,
		0xD955, 0xDDED
	};
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(12, utf16toutf8(c, sizeof(c), b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xF0\x98\x91\xBD\xF0\xA7\xAE\x88\xF1\xA5\x97\xAD", b);
}

TEST(Utf16ToUtf8, SurrogatePairUnmatchedLow)
{
	utf16_t c[] = {
		0xD800, 0x1100
	};
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf16toutf8(c, sizeof(c), b, s, &errors));
	EXPECT_EQ(UTF8_ERR_UNMATCHED_LOW_SURROGATE_PAIR, errors);
	EXPECT_STREQ("", b);
}

TEST(Utf16ToUtf8, SurrogatePairUnmatchedHigh)
{
	utf16_t c[] = {
		0xDD1E, 0xD834
	};
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf16toutf8(c, sizeof(c), b, s, &errors));
	EXPECT_EQ(UTF8_ERR_UNMATCHED_HIGH_SURROGATE_PAIR, errors);
	EXPECT_STREQ("", b);
}

TEST(Utf16ToUtf8, SurrogatePairStringUnmatchedPair)
{
	utf16_t c[] = {
		0xD85E, 0xDF88,
		0xD955, 0xDDED,
		0xD821, 0xDD,
		0xD83D, 0xDE12
	};
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(8, utf16toutf8(c, sizeof(c), b, s, &errors));
	EXPECT_EQ(UTF8_ERR_UNMATCHED_LOW_SURROGATE_PAIR, errors);
	EXPECT_STREQ("\xF0\xA7\xAE\x88\xF1\xA5\x97\xAD", b);
}

TEST(Utf16ToUtf8, AmountOfBytes)
{
	utf16_t c = 0x612;
	int32_t errors = 0;

	EXPECT_EQ(2, utf16toutf8(&c, sizeof(c), nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(Utf16ToUtf8, AmountOfBytesSurrogatePair)
{
	utf16_t c[] = {
		0xD967, 0xDDDD
	};
	int32_t errors = 0;

	EXPECT_EQ(4, utf16toutf8(c, sizeof(c), nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(Utf16ToUtf8, AmountOfBytesNoData)
{
	int32_t errors = 0;

	EXPECT_EQ(0, utf16toutf8(nullptr, 1, nullptr, 0, &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(Utf16ToUtf8, ErrorsIsReset)
{
	utf16_t c = 0x1A8D;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 128;

	EXPECT_EQ(3, utf16toutf8(&c, sizeof(c), b, s - 1, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_UTF8EQ("\xE1\xAA\x8D", b);
}

TEST(Utf16ToUtf8, OverlappingParametersFits)
{
	int32_t errors = 0;

	uint8_t data[128] = { 0 };
	data[0] = 'A';
	data[2] = 'n';
	data[4] = 's';

	const utf16_t* i = (const utf16_t*)data;
	size_t is = 6;
	char* o = (char*)(data + 6);
	size_t os = 3;

	EXPECT_EQ(3, utf16toutf8(i, is, o, os, &errors));
	EXPECT_MEMEQ("A\0n\0s\0Ans", (const char*)data, 9);
	EXPECT_EQ(0, errors);
}

TEST(Utf16ToUtf8, OverlappingParametersStartsEqual)
{
	int32_t errors = 0;

	uint8_t data[128] = { 0 };

	const utf16_t* i = (const utf16_t*)data;
	size_t is = 24;
	char* o = (char*)data;
	size_t os = 48;

	EXPECT_EQ(0, utf16toutf8(i, is, o, os, &errors));
	EXPECT_EQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST(Utf16ToUtf8, OverlappingParametersEndsEqual)
{
	int32_t errors = 0;

	uint8_t data[128] = { 0 };

	const utf16_t* i = (const utf16_t*)(data + 20);
	size_t is = 30;
	char* o = (char*)(data + 10);
	size_t os = 40;

	EXPECT_EQ(0, utf16toutf8(i, is, o, os, &errors));
	EXPECT_EQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST(Utf16ToUtf8, OverlappingParametersInputStartsInTarget)
{
	int32_t errors = 0;

	uint8_t data[128] = { 0 };

	const utf16_t* i = (const utf16_t*)(data + 21);
	size_t is = 76;
	char* o = (char*)(data + 12);
	size_t os = 61;

	EXPECT_EQ(0, utf16toutf8(i, is, o, os, &errors));
	EXPECT_EQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST(Utf16ToUtf8, OverlappingParametersInputEndsInTarget)
{
	int32_t errors = 0;

	uint8_t data[128] = { 0 };

	const utf16_t* i = (const utf16_t*)(data + 39);
	size_t is = 11;
	char* o = (char*)(data + 45);
	size_t os = 23;

	EXPECT_EQ(0, utf16toutf8(i, is, o, os, &errors));
	EXPECT_EQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST(Utf16ToUtf8, OverlappingParametersInputInsideTarget)
{
	int32_t errors = 0;

	uint8_t data[128] = { 0 };

	const utf16_t* i = (const utf16_t*)(data + 22);
	size_t is = 15;
	char* o = (char*)(data + 18);
	size_t os = 40;

	EXPECT_EQ(0, utf16toutf8(i, is, o, os, &errors));
	EXPECT_EQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST(Utf16ToUtf8, OverlappingParametersTargetStartsInInput)
{
	int32_t errors = 0;

	uint8_t data[128] = { 0 };

	const utf16_t* i = (const utf16_t*)(data + 15);
	size_t is = 20;
	char* o = (char*)(data + 21);
	size_t os = 41;

	EXPECT_EQ(0, utf16toutf8(i, is, o, os, &errors));
	EXPECT_EQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST(Utf16ToUtf8, OverlappingParametersTargetEndsInInput)
{
	int32_t errors = 0;

	uint8_t data[128] = { 0 };

	const utf16_t* i = (const utf16_t*)(data + 67);
	size_t is = 28;
	char* o = (char*)(data + 49);
	size_t os = 31;

	EXPECT_EQ(0, utf16toutf8(i, is, o, os, &errors));
	EXPECT_EQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST(Utf16ToUtf8, OverlappingParametersTargetInsideInput)
{
	int32_t errors = 0;

	uint8_t data[128] = { 0 };

	const utf16_t* i = (const utf16_t*)(data + 41);
	size_t is = 28;
	char* o = (char*)(data + 48);
	size_t os = 10;

	EXPECT_EQ(0, utf16toutf8(i, is, o, os, &errors));
	EXPECT_EQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}