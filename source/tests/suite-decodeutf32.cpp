#include "tests-base.hpp"

#include "utf8rewind.h"

TEST(DecodeUtf32, Character)
{
	const char* i = "\xF0\x9F\x98\xA4";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x1F624, o[0]);
}

TEST(DecodeUtf32, String)
{
	const char* i = "\xE0\xA4\x9C\xE0\xA4\xA1\xE0\xA4\xA4";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(12, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x091C, o[0]);
	EXPECT_EQ(0x0921, o[1]);
	EXPECT_EQ(0x0924, o[2]);
}

TEST(DecodeUtf32, StringEndsInMiddle)
{
	const char* i = "How un\0fortunate";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(24, utf8toutf32(i, 16, o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ('H', o[0]);
	EXPECT_EQ('o', o[1]);
	EXPECT_EQ('w', o[2]);
	EXPECT_EQ(' ', o[3]);
	EXPECT_EQ('u', o[4]);
	EXPECT_EQ('n', o[5]);
}

TEST(DecodeUtf32, StringDataSizeUnder)
{
	const char* i = "Tree";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(12, utf8toutf32(i, 3, o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ('T', o[0]);
	EXPECT_EQ('r', o[1]);
	EXPECT_EQ('e', o[2]);
}

TEST(DecodeUtf32, StringDataSizeOver)
{
	const char* i = "Bark";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(16, utf8toutf32(i, 8, o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ('B', o[0]);
	EXPECT_EQ('a', o[1]);
	EXPECT_EQ('r', o[2]);
	EXPECT_EQ('k', o[3]);
}

TEST(DecodeUtf32, Ascii)
{
	const char* i = "\x5F";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000005F, o[0]);
}

TEST(DecodeUtf32, AsciiFirst)
{
	const char* i = "\0";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
	EXPECT_EQ(0x00000000, o[0]);
}

TEST(DecodeUtf32, AsciiLast)
{
	const char* i = "\x7F";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000007F, o[0]);
}

TEST(DecodeUtf32, AsciiInvalid)
{
	const char* i = "\x88";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_CHARACTER, errors);
	EXPECT_EQ(0x00000000, o[0]);
}

TEST(DecodeUtf32, TwoBytes)
{
	const char* i = "\xC2\xA2";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x000000A2, o[0]);
}

TEST(DecodeUtf32, TwoBytesFirst)
{
	const char* i = "\xC2\x80";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x00000080, o[0]);
}

TEST(DecodeUtf32, TwoBytesLast)
{
	const char* i = "\xDF\xBF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x000007FF, o[0]);
}

TEST(DecodeUtf32, TwoBytesNotEnoughData)
{
	const char* i = "\xC2";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
	EXPECT_EQ(0x00000000, o[0]);
}

TEST(DecodeUtf32, ThreeBytes)
{
	const char* i = "\xE1\x8C\x8A";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000130A, o[0]);
}

TEST(DecodeUtf32, ThreeBytesFirst)
{
	const char* i = "\xE0\xA0\x80";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x00000800, o[0]);
}

TEST(DecodeUtf32, ThreeBytesLast)
{
	const char* i = "\xEF\xBF\xBF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000FFFF, o[0]);
}

TEST(DecodeUtf32, ThreeBytesNotEnoughData)
{
	const char* i = "\xEF\xBF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
	EXPECT_EQ(0x00000000, o[0]);
}

TEST(DecodeUtf32, FourBytes)
{
	const char* i = "\xF0\x9F\x98\x8E";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0001F60E, o[0]);
}

TEST(DecodeUtf32, FourBytesFirst)
{
	const char* i = "\xF0\x90\x80\x80";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x00010000, o[0]);
}

TEST(DecodeUtf32, FourBytesLast)
{
	const char* i = "\xF7\xBF\xBF\xBF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x001FFFFF, o[0]);
}

TEST(DecodeUtf32, FourBytesNotEnoughData)
{
	const char* i = "\xF0\x90\x80";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
	EXPECT_EQ(0x00000000, o[0]);
}

TEST(DecodeUtf32, FiveBytesFirst)
{
	const char* i = "\xF8\x88\x80\x80\x80";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x00200000, o[0]);
}

TEST(DecodeUtf32, FiveBytesLast)
{
	const char* i = "\xFB\xBF\xBF\xBF\xBF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x03FFFFFF, o[0]);
}

TEST(DecodeUtf32, FiveBytesNotEnoughData)
{
	const char* i = "\xF8\x88\x80\x80";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
	EXPECT_EQ(0x00000000, o[0]);
}

TEST(DecodeUtf32, SixBytesFirst)
{
	const char* i = "\xFC\x84\x80\x80\x80\x80";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x04000000, o[0]);
}

TEST(DecodeUtf32, SixBytesLast)
{
	const char* i = "\xFD\xBF\xBF\xBF\xBF\xBF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x7FFFFFFF, o[0]);
}

TEST(DecodeUtf32, SixBytesNotEnoughData)
{
	const char* i = "\xFD\xBF\xBF\xBF\xBF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
	EXPECT_EQ(0x00000000, o[0]);
}

TEST(DecodeUtf32, AmountOfBytes)
{
	const char* i = "\xF0\x90\xB0\xAC";
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(DecodeUtf32, AmountOfBytesNotEnoughData)
{
	const char* i = "\xC8";
	int32_t errors = 0;

	EXPECT_EQ(0, utf8toutf32(i, strlen(i), nullptr, 0, &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(DecodeUtf32, AmountOfBytesNoData)
{
	int32_t errors = 0;

	EXPECT_EQ(0, utf8toutf32(nullptr, 1, nullptr, 0, &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}