#include "tests-base.hpp"

#include "helpers-casemapping.hpp"
#include "helpers-locale.hpp"
#include "helpers-strings.hpp"

class Utf8ToUpperGreek
	: public ::testing::Test
{

protected:

	void SetUp()
	{
		SET_LOCALE_GREEK();
	}

	void TearDown()
	{
		RESET_LOCALE();
	}

};

TEST_F(Utf8ToUpperGreek, GreekSingleUppercase)
{
	// U+039C GREEK CAPITAL LETTER MU

	const char* c = "\xCE\x9C";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toupper(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("\xCE\x9C", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperGreek, GreekSingleLowercase)
{
	// U+03CA GREEK SMALL LETTER IOTA WITH DIALYTIKA

	const char* c = "\xCF\x8A";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toupper(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("\xCE\xAA", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperGreek, GreekSingleUnaffected)
{
	// U+0387 GREEK ANO TELEIA

	const char* c = "\xCE\x87";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toupper(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("\xCE\x87", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}