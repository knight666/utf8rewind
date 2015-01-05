#include "tests-base.hpp"

#include "utf8rewind.h"

#include "helpers-strings.hpp"

TEST(ToLower, BasicLatin)
{
	const char* c = "Hello World!";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(12, utf8transform(c, strlen(c), b, s - 1, UTF8_TRANSFORM_LOWERCASE, &errors));
	EXPECT_UTF8EQ("hello world!", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLower, BasicLatinUppercase)
{
	const char* c = "G";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1, utf8transform(c, strlen(c), b, s - 1, UTF8_TRANSFORM_LOWERCASE, &errors));
	EXPECT_UTF8EQ("g", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLower, BasicLatinLowercase)
{
	const char* c = "y";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1, utf8transform(c, strlen(c), b, s - 1, UTF8_TRANSFORM_LOWERCASE, &errors));
	EXPECT_UTF8EQ("y", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLower, BasicLatinUnaffected)
{
	const char* c = "@";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1, utf8transform(c, strlen(c), b, s - 1, UTF8_TRANSFORM_LOWERCASE, &errors));
	EXPECT_UTF8EQ("@", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLower, BasicLatinAmountOfBytes)
{
	const char* c = "Playable";
	int32_t errors = 0;

	EXPECT_EQ(8, utf8transform(c, strlen(c), nullptr, 0, UTF8_TRANSFORM_LOWERCASE, &errors));
	EXPECT_EQ(0, errors);
}

TEST(ToLower, BasicLatinNotEnoughSpace)
{
	const char* c = "VERY important";
	const size_t s = 9;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(8, utf8transform(c, strlen(c), b, s - 1, UTF8_TRANSFORM_LOWERCASE, &errors));
	EXPECT_UTF8EQ("very imp", b);
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(ToLower, TwoBytes)
{
	const char* c = "Rac\xC3\x88r";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(6, utf8transform(c, strlen(c), b, s - 1, UTF8_TRANSFORM_LOWERCASE, &errors));
	EXPECT_UTF8EQ("rac\xC3\xA8r", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLower, TwoBytesUppercase)
{
	const char* c = "\xC3\x82";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8transform(c, strlen(c), b, s - 1, UTF8_TRANSFORM_LOWERCASE, &errors));
	EXPECT_UTF8EQ("\xC3\xA2", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLower, TwoBytesLowercase)
{
	const char* c = "\xC3\xA7";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8transform(c, strlen(c), b, s - 1, UTF8_TRANSFORM_LOWERCASE, &errors));
	EXPECT_UTF8EQ("\xC3\xA7", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLower, TwoBytesUnaffected)
{
	const char* c = "\xC3\xB7";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8transform(c, strlen(c), b, s - 1, UTF8_TRANSFORM_LOWERCASE, &errors));
	EXPECT_UTF8EQ("\xC3\xB7", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLower, TwoBytesAmountOfBytes)
{
	const char* c = "\xC3\x8Dnteresting";
	int32_t errors = 0;

	EXPECT_EQ(12, utf8transform(c, strlen(c), nullptr, 0, UTF8_TRANSFORM_LOWERCASE, &errors));
	EXPECT_EQ(0, errors);
}

TEST(ToLower, TwoBytesNotEnoughSpace)
{
	const char* c = "Atl\xC3\x84ntis";
	const size_t s = 4;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8transform(c, strlen(c), b, s - 1, UTF8_TRANSFORM_LOWERCASE, &errors));
	EXPECT_UTF8EQ("atl", b);
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(ToLower, ThreeBytes)
{
	const char* c = "You're number \xE2\x85\xA0";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(17, utf8transform(c, strlen(c), b, s - 1, UTF8_TRANSFORM_LOWERCASE, &errors));
	EXPECT_UTF8EQ("you're number \xE2\x85\xB0", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLower, ThreeBytesUppercase)
{
	const char* c = "\xE2\x85\xAF";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8transform(c, strlen(c), b, s - 1, UTF8_TRANSFORM_LOWERCASE, &errors));
	EXPECT_UTF8EQ("\xE2\x85\xBF", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLower, ThreeBytesLowercase)
{
	const char* c = "\xE2\x85\xB6";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8transform(c, strlen(c), b, s - 1, UTF8_TRANSFORM_LOWERCASE, &errors));
	EXPECT_UTF8EQ("\xE2\x85\xB6", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLower, ThreeBytesUnaffected)
{
	const char* c = "\xE2\x86\x89";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8transform(c, strlen(c), b, s - 1, UTF8_TRANSFORM_LOWERCASE, &errors));
	EXPECT_UTF8EQ("\xE2\x86\x89", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLower, ThreeBytesAmountOfBytes)
{
	const char* c = "That's \xE2\x93\xA9" "any";
	int32_t errors = 0;

	EXPECT_EQ(13, utf8transform(c, strlen(c), nullptr, 0, UTF8_TRANSFORM_LOWERCASE, &errors));
	EXPECT_EQ(0, errors);
}

TEST(ToLower, ThreeBytesNotEnoughSpace)
{
	const char* c = "Whole Lotta \xE2\x93\xA1osie";
	const size_t s = 14;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(12, utf8transform(c, strlen(c), b, s - 1, UTF8_TRANSFORM_LOWERCASE, &errors));
	EXPECT_UTF8EQ("whole lotta ", b);
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(ToLower, FourBytes)
{
	const char* c = "Bring a t\xF0\x91\xA2\xA9wel";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(16, utf8transform(c, strlen(c), b, s - 1, UTF8_TRANSFORM_LOWERCASE, &errors));
	EXPECT_UTF8EQ("bring a t\xF0\x91\xA3\x89wel", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLower, FourBytesUppercase)
{
	const char* c = "\xF0\x91\xA2\xB2";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8transform(c, strlen(c), b, s - 1, UTF8_TRANSFORM_LOWERCASE, &errors));
	EXPECT_UTF8EQ("\xF0\x91\xA3\x92", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLower, FourBytesLowercase)
{
	const char* c = "\xF0\x90\x90\xB6";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8transform(c, strlen(c), b, s - 1, UTF8_TRANSFORM_LOWERCASE, &errors));
	EXPECT_UTF8EQ("\xF0\x90\x90\xB6", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLower, FourBytesUnaffected)
{
	const char* c = "\xF0\x91\xA3\xA4";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8transform(c, strlen(c), b, s - 1, UTF8_TRANSFORM_LOWERCASE, &errors));
	EXPECT_UTF8EQ("\xF0\x91\xA3\xA4", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLower, FourBytesAmountOfBytes)
{
	const char* c = "Say \xF0\x91\xA2\xB9";
	int32_t errors = 0;

	EXPECT_EQ(8, utf8transform(c, strlen(c), nullptr, 0, UTF8_TRANSFORM_LOWERCASE, &errors));
	EXPECT_EQ(0, errors);
}

TEST(ToLower, FourBytesNotEnoughSpace)
{
	const char* c = "My \xF0\x91\xA3\x8Eurt";
	const size_t s = 5;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8transform(c, strlen(c), b, s - 1, UTF8_TRANSFORM_LOWERCASE, &errors));
	EXPECT_UTF8EQ("my ", b);
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(ToLower, InvalidCodepoint)
{
	const char* c = "\xF0\x92";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8transform(c, strlen(c), b, s - 1, UTF8_TRANSFORM_LOWERCASE, &errors));
	EXPECT_UTF8EQ("\xEF\xBF\xBD", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLower, InvalidData)
{
	int32_t errors = 0;

	EXPECT_EQ(0, utf8transform(nullptr, 1, nullptr, 0, UTF8_TRANSFORM_LOWERCASE, &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}