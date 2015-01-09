#include "tests-base.hpp"

#include "utf8rewind.h"

#include "helpers-strings.hpp"

TEST(ToTitle, BasicLatinUppercase)
{
	const char* c = "J";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1, utf8totitle(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("J", b);
	EXPECT_EQ(0, errors);
}

TEST(ToTitle, BasicLatinLowercase)
{
	const char* c = "z";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1, utf8totitle(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("Z", b);
	EXPECT_EQ(0, errors);
}

TEST(ToTitle, BasicLatinUnaffected)
{
	const char* c = "$";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1, utf8totitle(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("$", b);
	EXPECT_EQ(0, errors);
}

TEST(ToTitle, MultiByteUppercase)
{
	const char* c = "\xC7\x84";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8totitle(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("\xC7\x85", b);
	EXPECT_EQ(0, errors);
}

TEST(ToTitle, MultiByteLowercase)
{
	const char* c = "\xEF\xAC\x97";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8totitle(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("\xD5\x84\xD5\xAD", b);
	EXPECT_EQ(0, errors);
}

TEST(ToTitle, MultiByteTitlecase)
{
	const char* c = "\xC7\xB2";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8totitle(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("\xC7\xB2", b);
	EXPECT_EQ(0, errors);
}

TEST(ToTitle, Word)
{
	const char* c = "ApplE";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(5, utf8totitle(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("Apple", b);
	EXPECT_EQ(0, errors);
}

TEST(ToTitle, WordAllUppercase)
{
	const char* c = "BROWN";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(5, utf8totitle(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("Brown", b);
	EXPECT_EQ(0, errors);
}

TEST(ToTitle, WordAllLowercase)
{
	const char* c = "lady";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8totitle(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("Lady", b);
	EXPECT_EQ(0, errors);
}

TEST(ToTitle, SentenceTwoWords)
{
	const char* c = "PRINTER INK";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(11, utf8totitle(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("Printer Ink", b);
	EXPECT_EQ(0, errors);
}

TEST(ToTitle, SentencePunctuationInMiddle)
{
	const char* c = "RE/wind=cool";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(12, utf8totitle(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("Re/Wind=Cool", b);
	EXPECT_EQ(0, errors);
}

TEST(ToTitle, SentencePunctuationAtStart)
{
	const char* c = "/!\\alert imminent";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(17, utf8totitle(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("/!\\Alert Imminent", b);
	EXPECT_EQ(0, errors);
}

TEST(ToTitle, SentencePunctuationAtEnd)
{
	const char* c = "you tell me ;)";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(14, utf8totitle(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("You Tell Me ;)", b);
	EXPECT_EQ(0, errors);
}

TEST(ToTitle, AmountOfBytes)
{
	const char* c = "tiny \xEA\x9E\xAA";
	int32_t errors = 0;

	EXPECT_EQ(8, utf8totitle(c, strlen(c), nullptr, 0, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(ToTitle, NotEnoughSpace)
{
	const char* c = "SMALL \xCE\x90";
	const size_t s = 7;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(6, utf8totitle(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("Small ", b);
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(ToTitle, InvalidCodepoint)
{
	const char* c = "\xF0\x92";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("\xEF\xBF\xBD", b);
	EXPECT_EQ(0, errors);
}

TEST(ToTitle, InvalidData)
{
	int32_t errors = 0;

	EXPECT_EQ(0, utf8totitle(nullptr, 1, nullptr, 0, 0, &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}