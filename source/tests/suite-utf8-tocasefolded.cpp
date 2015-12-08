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

TEST_F(Utf8ToCaseFolded, GeneralCategoryCaseMappedSingleUppercase)
{
	// LATIN CAPITAL LETTER N WITH LEFT HOOK

	const char* c = "\xC6\x9D";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8tocasefolded(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC9\xB2", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToCaseFolded, GeneralCategoryCaseMappedSingleLowercase)
{
	// LATIN SMALL LETTER DOTLESS I

	const char* c = "\xC4\xB1";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8tocasefolded(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC4\xB1", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToCaseFolded, GeneralCategoryCaseMappedSingleTitlecase)
{
	// 01C5;LATIN CAPITAL LETTER D WITH SMALL LETTER Z WITH CARON

	const char* c = "\xC7\x85";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8tocasefolded(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC7\x86", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToCaseFolded, GeneralCategoryCaseMappedSingleUnaffected)
{
	// COMBINING VERTICAL LINE ABOVE

	const char* c = "\xCC\x8D";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8tocasefolded(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xCC\x8D", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToCaseFolded, GeneralCategoryCaseMappedSingleAmountOfBytes)
{
	// LATIN CAPITAL LETTER WYNN

	const char* c = "\xC7\xB7";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8tocasefolded(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToCaseFolded, GeneralCategoryCaseMappedSingleNotEnoughSpace)
{
	// WARANG CITI CAPITAL LETTER UU

	const char* c = "\xF0\x91\xA2\xA7";
	const size_t s = 3;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8tocasefolded(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST_F(Utf8ToCaseFolded, GeneralCategoryCaseMappedMultipleUppercase)
{
	// LATIN LETTER YR
	// GREEK CAPITAL LETTER OMEGA
	// LATIN CAPITAL LETTER I WITH DOT ABOVE
	// LATIN CAPITAL LETTER N WITH TILDE

	const char* c = "\xC6\xA6\xCE\xA9\xC4\xB0\xC3\x91";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(9, utf8tocasefolded(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xCA\x80\xCF\x89i\xCC\x87\xC3\xB1", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToCaseFolded, GeneralCategoryCaseMappedMultipleLowercase)
{
	// LATIN SMALL LETTER EZH WITH TAIL
	// GREEK SMALL LETTER THETA
	// LATIN SMALL LETTER REVERSED OPEN E WITH HOOK

	const char* c = "\xC6\xBA\xCE\xB8\xC9\x9D";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(6, utf8tocasefolded(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC6\xBA\xCE\xB8\xC9\x9D", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToCaseFolded, GeneralCategoryCaseMappedMultipleTitlecase)
{
	// LATIN CAPITAL LETTER D WITH SMALL LETTER Z
	// LATIN CAPITAL LETTER D WITH SMALL LETTER Z WITH CARON
	// LATIN CAPITAL LETTER D WITH SMALL LETTER Z

	const char* c = "\xC7\xB2\xC7\x85\xC7\xB2";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(6, utf8tocasefolded(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC7\xB3\xC7\x86\xC7\xB3", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToCaseFolded, GeneralCategoryCaseMappedMultipleUnaffected)
{
	// COMBINING HOMOTHETIC ABOVE
	// ARABIC LETTER WAW WITH EXTENDED ARABIC-INDIC DIGIT TWO ABOVE
	// NKO LAJANYALAN
	// SYRIAC SUPRALINEAR FULL STOP

	const char* c = "\xCD\x8B\xDD\xB8\xDF\xBA\xDC\x81";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(8, utf8tocasefolded(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xCD\x8B\xDD\xB8\xDF\xBA\xDC\x81", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToCaseFolded, GeneralCategoryCaseMappedMultipleAmountOfBytes)
{
	// 0471;CYRILLIC SMALL LETTER PSI
	// 043F;CYRILLIC SMALL LETTER PE
	// 0460;CYRILLIC CAPITAL LETTER OMEGA
	// 041C;CYRILLIC CAPITAL LETTER EM

	const char* c = "\xD1\xB1\xD0\xBF\xD1\xA0\xD0\x9C";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(8, utf8tocasefolded(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToCaseFolded, GeneralCategoryCaseMappedMultipleNotEnoughSpace)
{
	// GREEK SMALL LETTER PAMPHYLIAN DIGAMMA
	// LATIN SMALL LETTER UPSILON
	// GREEK QUESTION MARK
	// COMBINING LEFT HALF RING ABOVE

	const char* c = "\xCD\xB7\xCA\x8A\xCD\xBE\xCD\x91";
	const size_t s = 5;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8tocasefolded(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xCD\xB7\xCA\x8A", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}