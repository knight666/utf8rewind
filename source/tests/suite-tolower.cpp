#include "tests-base.hpp"

#include "utf8rewind.h"

TEST(ToLower, BasicLatin)
{
	char* c = "Hello World!";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(12, utf8tolower(c, strlen(c), b, s, &errors));
	EXPECT_STREQ("hello world!", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLower, BasicLatinAllUppercase)
{
	char* c = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(26, utf8tolower(c, strlen(c), b, s, &errors));
	EXPECT_STREQ("abcdefghijklmnopqrstuvwxyz", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLower, BasicLatinAllLowercase)
{
	char* c = "abcdefghijklmnopqrstuvwxyz";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(26, utf8tolower(c, strlen(c), b, s, &errors));
	EXPECT_STREQ("abcdefghijklmnopqrstuvwxyz", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLower, BasicLatinAllOther)
{
	char* c = " !\"#$%&'()*+,-./0123456789:;<=>?@[\\]^_`{|}`";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(43, utf8tolower(c, strlen(c), b, s, &errors));
	EXPECT_STREQ(" !\"#$%&'()*+,-./0123456789:;<=>?@[\\]^_`{|}`", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLower, BasicLatinAmountOfBytes)
{
	char* c = "Playable";
	int32_t errors = 0;

	EXPECT_EQ(8, utf8tolower(c, strlen(c), nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(ToLower, BasicLatinNotEnoughSpace)
{
	char* c = "VERY important";
	const size_t s = 9;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(8, utf8tolower(c, strlen(c), b, s - 1, &errors));
	EXPECT_STREQ("very imp", b);
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(ToLower, InvalidData)
{
	int32_t errors = 0;

	EXPECT_EQ(0, utf8tolower(nullptr, 1, nullptr, 0, &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}