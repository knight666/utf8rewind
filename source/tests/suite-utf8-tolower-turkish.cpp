#include "tests-base.hpp"

#include "../helpers/helpers-locale.hpp"
#include "../helpers/helpers-strings.hpp"

class Utf8ToLowerTurkish
	: public ::testing::Test
{

protected:

	void SetUp()
	{
		SET_LOCALE_TURKISH();
	}

	void TearDown()
	{
		RESET_LOCALE();
	}

};

TEST_F(Utf8ToLowerTurkish, SingleCapitalLetterI)
{
	// 0049
	// 0131

	const char* c = "I";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8tolower(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("\xC4\xB1", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToLowerTurkish, SingleCapitalLetterIAndDotAbove)
{
	// 0049 0307
	// 0069

	const char* c = "I\xCC\x87";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(1, utf8tolower(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("i", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToLowerTurkish, SingleCapitalLetterIWithDotAbove)
{
	// 0130
	// 0069

	const char* c = "\xC4\xB0";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(1, utf8tolower(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("i", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToLowerTurkish, SingleCapitalLetterIWithDotAboveAndDotAbove)
{
	// 0130 0307
	// 0069 0307

	const char* c = "\xC4\xB0\xCC\x87";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8tolower(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("i\xCC\x87", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToLowerTurkish, SingleSmallLetterI)
{
	// 0069
	// 0069

	const char* c = "i";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(1, utf8tolower(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("i", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToLowerTurkish, SingleSmallLetterIWithDotAbove)
{
	// 0069 0307
	// 0069 0307

	const char* c = "i\xCC\x87";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8tolower(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("i\xCC\x87", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToLowerTurkish, SingleSmallLetterDotlessI)
{
	// 0131
	// 0131

	const char* c = "\xC4\xB1";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8tolower(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("\xC4\xB1", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToLowerTurkish, SingleSmallLetterDotlessIWithDotAbove)
{
	// 0131 0307
	// 0131 0307

	const char* c = "\xC4\xB1\xCC\x87";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8tolower(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("\xC4\xB1\xCC\x87", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}