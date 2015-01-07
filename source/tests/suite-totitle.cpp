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

TEST(ToTitle, SentencePunctuation)
{
	const char* c = "RE/wind=cool";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(12, utf8totitle(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("Re/Wind=Cool", b);
	EXPECT_EQ(0, errors);
}