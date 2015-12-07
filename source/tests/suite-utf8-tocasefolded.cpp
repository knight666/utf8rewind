#include "tests-base.hpp"

#include "utf8rewind.h"

#include "../helpers/helpers-locale.hpp"
#include "../helpers/helpers-strings.hpp"

class Utf8ToCaseFolded
	: public ::testing::Test
{

protected:

	void SetUp()
	{
		SET_LOCALE_ENGLISH();
	}

	void TearDown()
	{
		RESET_LOCALE();
	}

};

TEST_F(Utf8ToCaseFolded, BasicLatinSingleUppercase)
{
	const char* c = "G";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(1, utf8tocasefolded(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("g", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToCaseFolded, BasicLatinSingleLowercase)
{
	const char* c = "k";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(1, utf8tocasefolded(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("k", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToCaseFolded, BasicLatinSingleUnaffected)
{
	const char* c = "0";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(1, utf8tocasefolded(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("0", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToCaseFolded, BasicLatinSingleAmountOfBytes)
{
	const char* c = "&";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(1, utf8tocasefolded(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToCaseFolded, BasicLatinSingleNotEnoughSpace)
{
	const char* c = "d";
	const size_t s = 0;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8tocasefolded(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST_F(Utf8ToCaseFolded, BasicLatinMultipleUppercase)
{
	const char* c = "HODR";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8tocasefolded(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("hodr", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToCaseFolded, BasicLatinMultipleLowercase)
{
	const char* c = "junk";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8tocasefolded(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("junk", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToCaseFolded, BasicLatinMultipleUnaffected)
{
	const char* c = "$ 1500";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(6, utf8tocasefolded(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("$ 1500", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToCaseFolded, BasicLatinMultipleAmountOfBytes)
{
	const char* c = "Skirt";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(5, utf8tocasefolded(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToCaseFolded, BasicLatinMultipleNotEnoughSpace)
{
	const char* c = "7bD122";
	const size_t s = 3;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8tocasefolded(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("7bd", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}