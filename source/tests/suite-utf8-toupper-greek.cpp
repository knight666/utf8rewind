#include "tests-base.hpp"

#include "../helpers/helpers-casemapping.hpp"
#include "../helpers/helpers-locale.hpp"
#include "../helpers/helpers-strings.hpp"

class Utf8ToUpperGreek
	: public ::testing::Test
{

protected:

	void SetUp()
	{
		// Greek grammar rules are not exclusive to Greek locale

		SET_LOCALE_ENGLISH();
	}

	void TearDown()
	{
		RESET_LOCALE();
	}

};

TEST_F(Utf8ToUpperGreek, SingleCapitalLetterSigma)
{
	// 03A3

	const char* c = "\xCE\xA3";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toupper(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("\xCE\xA3", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperGreek, SingleSmallLetterSigma)
{
	// 03C3

	const char* c = "\xCF\x83";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toupper(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("\xCE\xA3", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperGreek, SingleSmallLetterFinalSigma)
{
	// 03C2

	const char* c = "\xCF\x82";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toupper(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("\xCE\xA3", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperGreek, WordCapitalLetterSigma)
{
	// 0399 03A3 03B7 03A6
	// 0399 03A3 0397 03A6

	const char* c = "\xCE\x99\xCE\xA3\xCE\xB7\xCE\xA6";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(8, utf8toupper(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("\xCE\x99\xCE\xA3\xCE\x97\xCE\xA6", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperGreek, WordCapitalLetterSigmaAtEnd)
{
	// 0396 03A0 03A4 03A3
	// 0396 03A0 03A4 03A3

	const char* c = "\xCE\x96\xCE\xA0\xCE\xA4\xCE\xA3";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(8, utf8toupper(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("\xCE\x96\xCE\xA0\xCE\xA4\xCE\xA3", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperGreek, WordSmallLetterSigma)
{
	// 03C5 03BD 03B6 03B6 03C3
	// 03A5 039D 0396 0396 03A3

	const char* c = "\xCF\x85\xCE\xBD\xCE\xB6\xCE\xB6\xCF\x83";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(10, utf8toupper(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("\xCE\xA5\xCE\x9D\xCE\x96\xCE\x96\xCE\xA3", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperGreek, WordSmallLetterFinalSigma)
{
	// 03B6 03B3 03C2
	// 0396 0393 03A3

	const char* c = "\xCE\xB6\xCE\xB3\xCF\x82";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(6, utf8toupper(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("\xCE\x96\xCE\x93\xCE\xA3", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperGreek, SentenceCapitalLetterSigma)
{
	// 0399 0399 03A3 0020 0394 03A0 03AB
	// 0399 0399 03A3 0020 0394 03A0 03AB

	const char* c = "\xCE\x99\xCE\x99\xCE\xA3 \xCE\x94\xCE\xA0\xCE\xAB";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(13, utf8toupper(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("\xCE\x99\xCE\x99\xCE\xA3 \xCE\x94\xCE\xA0\xCE\xAB", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperGreek, SentenceSmallLetterSigma)
{
	// 0397 03A3 03A7 03BF 03C3 0020 03B8 03C3 03C6
	// 0397 03A3 03A7 039F 03A3 0020 0398 03A3 03A6

	const char* c = "\xCE\x97\xCE\xA3\xCE\xA7\xCE\xBF\xCF\x83 \xCE\xB8\xCF\x83\xCF\x86";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(17, utf8toupper(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("\xCE\x97\xCE\xA3\xCE\xA7\xCE\x9F\xCE\xA3 \xCE\x98\xCE\xA3\xCE\xA6", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperGreek, SentenceSmallLetterFinalSigma)
{
	// 03C3 03C2 0020 03BA 03B7 03CB 03CB
	// 03A3 03A3 0020 039A 0397 03AB 03AB

	const char* c = "\xCF\x83\xCF\x82 \xCE\xBA\xCE\xB7\xCF\x8B\xCF\x8B";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(13, utf8toupper(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("\xCE\xA3\xCE\xA3 \xCE\x9A\xCE\x97\xCE\xAB\xCE\xAB", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}