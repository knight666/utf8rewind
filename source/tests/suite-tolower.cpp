#include "tests-base.hpp"

#include "utf8rewind.h"

#include "helpers-strings.hpp"

TEST(ToLower, BasicLatin)
{
	const char* c = "Hello World!";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(12, utf8tolower(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("hello world!", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLower, BasicLatinUppercase)
{
	const char* c = "G";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1, utf8tolower(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("g", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLower, BasicLatinLowercase)
{
	const char* c = "y";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1, utf8tolower(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("y", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLower, BasicLatinUnaffected)
{
	const char* c = "@";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1, utf8tolower(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("@", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLower, BasicLatinAmountOfBytes)
{
	const char* c = "Playable";
	int32_t errors = 0;

	EXPECT_EQ(8, utf8tolower(c, strlen(c), nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(ToLower, BasicLatinNotEnoughSpace)
{
	const char* c = "VERY important";
	const size_t s = 9;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(8, utf8tolower(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("very imp", b);
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(ToLower, TwoBytes)
{
	const char* c = "Rac\xC3\x88r";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(6, utf8tolower(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("rac\xC3\xA8r", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLower, TwoBytesUppercase)
{
	const char* c = "\xC3\x82";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8tolower(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("\xC3\xA2", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLower, TwoBytesLowercase)
{
	const char* c = "\xC3\xA7";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8tolower(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("\xC3\xA7", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLower, TwoBytesUnaffected)
{
	const char* c = "\xC3\xB7";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8tolower(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("\xC3\xB7", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLower, TwoBytesAmountOfBytes)
{
	const char* c = "\xC3\x8Dnteresting";
	int32_t errors = 0;

	EXPECT_EQ(12, utf8tolower(c, strlen(c), nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(ToLower, TwoBytesNotEnoughSpace)
{
	const char* c = "Atl\xC3\x84ntis";
	const size_t s = 4;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8tolower(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("atl", b);
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(ToLower, InvalidData)
{
	int32_t errors = 0;

	EXPECT_EQ(0, utf8tolower(nullptr, 1, nullptr, 0, &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}