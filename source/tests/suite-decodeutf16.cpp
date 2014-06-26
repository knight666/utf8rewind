#include "tests-base.hpp"

#include "utf8rewind.h"

TEST(DecodeUtf16, Character)
{
	const char* i = "\xE0\xA4\x9C";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8decodeutf16(i, strlen(i), o, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x091C, o[0]);
}

TEST(DecodeUtf16, String)
{
	const char* i = "\xE0\xA4\x9C\xE0\xA4\xA1\xE0\xA4\xA4";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(6, utf8decodeutf16(i, strlen(i), o, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x091C, o[0]);
	EXPECT_EQ(0x0921, o[1]);
	EXPECT_EQ(0x0924, o[2]);
}

TEST(DecodeUtf16, Ascii)
{
	const char* i = "k";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8decodeutf16(i, strlen(i), o, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ('k', o[0]);
}

TEST(DecodeUtf16, AsciiFirst)
{
	const char* i = "\0";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf8decodeutf16(i, strlen(i), o, s, &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
	EXPECT_EQ(0x0000, o[0]);
}

TEST(DecodeUtf16, AsciiLast)
{
	const char* i = "\x7F";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8decodeutf16(i, strlen(i), o, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x007F, o[0]);
}

TEST(DecodeUtf16, AsciiInvalid)
{
	const char* i = "\x97";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf8decodeutf16(i, strlen(i), o, s, &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_CHARACTER, errors);
	EXPECT_EQ(0x0000, o[0]);
}

TEST(DecodeUtf16, TwoBytes)
{
	const char* i = "\xC5\xA9";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8decodeutf16(i, strlen(i), o, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0169, o[0]);
}

TEST(DecodeUtf16, TwoBytesFirst)
{
	const char* i = "\xC2\x80";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8decodeutf16(i, strlen(i), o, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0080, o[0]);
}

TEST(DecodeUtf16, TwoBytesLast)
{
	const char* i = "\xDF\xBF";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8decodeutf16(i, strlen(i), o, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x07FF, o[0]);
}

TEST(DecodeUtf16, TwoBytesNotEnoughData)
{
	const char* i = "\xDA";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf8decodeutf16(i, strlen(i), o, s, &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
	EXPECT_EQ(0x0000, o[0]);
}

TEST(DecodeUtf16, ThreeBytes)
{
	const char* i = "\xE1\x8C\x8A";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8decodeutf16(i, strlen(i), o, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x130A, o[0]);
}

TEST(DecodeUtf16, ThreeBytesFirst)
{
	const char* i = "\xE0\xA0\x80";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8decodeutf16(i, strlen(i), o, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0800, o[0]);
}

TEST(DecodeUtf16, ThreeBytesLast)
{
	const char* i = "\xEF\xBF\xBF";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8decodeutf16(i, strlen(i), o, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0xFFFF, o[0]);
}

TEST(DecodeUtf16, ThreeBytesNotEnoughData)
{
	const char* i = "\xEF\xA9";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf8decodeutf16(i, strlen(i), o, s, &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
	EXPECT_EQ(0x0000, o[0]);
}

TEST(DecodeUtf16, SurrogatePair)
{
	const char* i = "\xF0\x9F\x98\xA4";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8decodeutf16(i, strlen(i), o, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0xD83D, o[0]);
	EXPECT_EQ(0xDE24, o[1]);
}

TEST(DecodeUtf16, SurrogatePairFirst)
{
	const char* i = "\xF0\x90\x80\x80";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8decodeutf16(i, strlen(i), o, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0xD800, o[0]);
	EXPECT_EQ(0xDC00, o[1]);
}

TEST(DecodeUtf16, SurrogatePairLast)
{
	const char* i = "\xF4\x8F\xBF\xBF";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8decodeutf16(i, strlen(i), o, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0xDBFF, o[0]);
	EXPECT_EQ(0xDFFF, o[1]);
}

TEST(DecodeUtf16, SurrogatePairNotEnoughData)
{
	const char* i = "\xF0\x9F\x98";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf8decodeutf16(i, strlen(i), o, s, &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
	EXPECT_EQ(0x0000, o[0]);
}