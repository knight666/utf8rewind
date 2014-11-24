#include "tests-base.hpp"

#include "utf8rewind.h"

TEST(ToUpper, Ascii)
{
	char* c = "Hello World!";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(12, utf8toupper(c, strlen(c), b, s, &errors));
	EXPECT_STREQ("HELLO WORLD!", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpper, AsciiAllLowercase)
{
	char* c = "abcdefghijklmnopqrstuvwxyz";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(26, utf8toupper(c, strlen(c), b, s, &errors));
	EXPECT_STREQ("ABCDEFGHIJKLMNOPQRSTUVWXYZ", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpper, AsciiAllUppercase)
{
	char* c = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(26, utf8toupper(c, strlen(c), b, s, &errors));
	EXPECT_STREQ("ABCDEFGHIJKLMNOPQRSTUVWXYZ", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpper, AsciiAllOther)
{
	char* c = " !\"#$%&'()*+,-./0123456789:;<=>?@[\\]^_`{|}`";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(43, utf8toupper(c, strlen(c), b, s, &errors));
	EXPECT_STREQ(" !\"#$%&'()*+,-./0123456789:;<=>?@[\\]^_`{|}`", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpper, AsciiAmountOfBytes)
{
	char* c = "Magic";
	int32_t errors = 0;

	EXPECT_EQ(5, utf8toupper(c, strlen(c), nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(ToUpper, AsciiNotEnoughSpace)
{
	char* c = "Merde";
	const size_t s = 4;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8toupper(c, strlen(c), b, s - 1, &errors));
	EXPECT_STREQ("MER", b);
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(ToUpper, AsciiInvalidData)
{
	int32_t errors = 0;

	EXPECT_EQ(0, utf8toupper(nullptr, 1, nullptr, 0, &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}