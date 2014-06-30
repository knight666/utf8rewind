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
	EXPECT_EQ(0x5F, o[0]);
}

TEST(DecodeUtf32, AsciiFirst)
{
	const char* i = "\0";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
	EXPECT_EQ(0, o[0]);
}

TEST(DecodeUtf32, AsciiLast)
{
	const char* i = "\x7F";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x7F, o[0]);
}

TEST(DecodeUtf32, AsciiInvalid)
{
	const char* i = "\x88";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_CHARACTER, errors);
	EXPECT_EQ(0x00, o[0]);
}

TEST(DecodeUtf32, TwoBytes)
{
	const char* i = "\xC2\xA2";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x00A2, o[0]);
}

TEST(DecodeUtf32, TwoBytesFirst)
{
	const char* i = "\xC2\x80";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0080, o[0]);
}

TEST(DecodeUtf32, TwoBytesLast)
{
	const char* i = "\xDF\xBF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x07FF, o[0]);
}

TEST(DecodeUtf32, TwoBytesNotEnoughData)
{
	const char* i = "\xC2";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
	EXPECT_EQ(0x00, o[0]);
}

TEST(DecodeUtf32, ThreeBytes)
{
	const char* i = "\xE1\x8C\x8A";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x130A, o[0]);
}

TEST(DecodeUtf32, ThreeBytesFirst)
{
	const char* i = "\xE0\xA0\x80";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0800, o[0]);
}

TEST(DecodeUtf32, ThreeBytesLast)
{
	const char* i = "\xEF\xBF\xBF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0xFFFF, o[0]);
}

TEST(DecodeUtf32, ThreeBytesNotEnoughData)
{
	const char* i = "\xEF\xBF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
	EXPECT_EQ(0x00, o[0]);
}

TEST(DecodeUtf32, FourBytes)
{
	const char* i = "\xF0\x9F\x98\x8E";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x01F60E, o[0]);
}

TEST(DecodeUtf32, FourBytesFirst)
{
	const char* i = "\xF0\x90\x80\x80";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x010000, o[0]);
}

TEST(DecodeUtf32, FourBytesLast)
{
	const char* i = "\xF4\x80\x83\xBF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x1000FF, o[0]);
}

TEST(DecodeUtf32, FourBytesNotEnoughData)
{
	const char* i = "\xF0\x90\x80";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
	EXPECT_EQ(0x00, o[0]);
}

TEST(DecodeUtf32, LengthAscii)
{
	const char* i = "I";
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(DecodeUtf32, LengthAsciiFirst)
{
	const char* i = "\0";
	int32_t errors = 0;

	EXPECT_EQ(0, utf8toutf32(i, 1, nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(DecodeUtf32, LengthAsciiLast)
{
	const char* i = "\x7F";
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, 1, nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(DecodeUtf32, LengthTwoBytes)
{
	const char* i = "\xCB\x9B";
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(DecodeUtf32, LengthTwoBytesFirst)
{
	const char* i = "\xC2\x80";
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(DecodeUtf32, LengthTwoBytesLast)
{
	const char* i = "\xDF\xBF";
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(DecodeUtf32, LengthTwoBytesNotEnoughData)
{
	const char* i = "\xC8";
	int32_t errors = 0;

	EXPECT_EQ(0, utf8toutf32(i, strlen(i), nullptr, 0, &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(DecodeUtf32, LengthThreeBytes)
{
	const char* i = "\xE0\xB4\x8B";
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(DecodeUtf32, LengthThreeBytesFirst)
{
	const char* i = "\xE0\xA0\x80";
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(DecodeUtf32, LengthThreeBytesLast)
{
	const char* i = "\xEF\xBF\xBF";
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(DecodeUtf32, LengthThreeBytesNotEnoughData)
{
	const char* i = "\xE0\xB4";
	int32_t errors = 0;

	EXPECT_EQ(0, utf8toutf32(i, strlen(i), nullptr, 0, &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(DecodeUtf32, LengthFourBytes)
{
	const char* i = "\xF0\x90\xB0\xAC";
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(DecodeUtf32, LengthFourBytesFirst)
{
	const char* i = "\xF0\x90\x80\x80";
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(DecodeUtf32, LengthFourBytesLast)
{
	const char* i = "\xF4\x80\x83\xBF";
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(DecodeUtf32, LengthFourBytesNotEnoughData)
{
	const char* i = "\xF0\x90\xB1";
	int32_t errors = 0;

	EXPECT_EQ(0, utf8toutf32(i, strlen(i), nullptr, 0, &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(DecodeUtf32, LengthNotEnoughData)
{
	int32_t errors = 0;

	EXPECT_EQ(0, utf8toutf32(nullptr, 1, nullptr, 0, &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}