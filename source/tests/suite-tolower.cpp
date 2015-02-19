#include "tests-base.hpp"

#include "utf8rewind.h"

#include "helpers-strings.hpp"

TEST(ToLower, BasicLatinSingleUppercase)
{
	const char* c = "G";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1, utf8tolower(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("g", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLower, BasicLatinSingleLowercase)
{
	const char* c = "y";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1, utf8tolower(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("y", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLower, BasicLatinSingleUnaffected)
{
	const char* c = "@";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1, utf8tolower(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("@", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLower, BasicLatinMultipleUppercase)
{
	const char* c = "MULTI";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(5, utf8tolower(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("multi", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLower, BasicLatinMultipleLowercase)
{
	const char* c = "jazz";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8tolower(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("jazz", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLower, BasicLatinMultipleUnaffected)
{
	const char* c = "(-(#)-)";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(7, utf8tolower(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("(-(#)-)", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLower, BasicLatinWord)
{
	const char* c = "MuMbLinG";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(8, utf8tolower(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("mumbling", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLower, BasicLatinSentence)
{
	const char* c = "Hello World!";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(12, utf8tolower(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("hello world!", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLower, BasicLatinAmountOfBytes)
{
	const char* c = "Houten";
	int32_t errors = 0;

	EXPECT_EQ(6, utf8tolower(c, strlen(c), nullptr, 0, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(ToLower, BasicLatinNotEnoughSpace)
{
	const char* c = "Interested?";
	const size_t s = 4;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8tolower(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("int", b);
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(ToLower, GeneralCategoryCaseMappedSingleUppercase)
{
	// LATIN CAPITAL LETTER A WITH CIRCUMFLEX

	const char* c = "\xC3\x82";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8tolower(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("\xC3\xA2", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLower, GeneralCategoryCaseMappedSingleLowercase)
{
	// DESERET SMALL LETTER WU

	const char* c = "\xF0\x90\x90\xB6";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8tolower(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("\xF0\x90\x90\xB6", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLower, GeneralCategoryCaseMappedSingleTitlecase)
{
	// LATIN CAPITAL LETTER D WITH SMALL LETTER Z WITH CARON

	const char* c = "\xC7\x85";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8tolower(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("\xC7\x86", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLower, GeneralCategoryCaseMappedSingleUnaffected)
{
	// VULGAR FRACTION ZERO THIRDS

	const char* c = "\xE2\x86\x89";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8tolower(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("\xE2\x86\x89", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLower, GeneralCategoryCaseMappedMultipleUppercase)
{
	// LATIN CAPITAL LETTER R WITH INVERTED BREVE
	// LATIN CAPITAL LETTER EZH WITH CARON
	// LATIN CAPITAL LETTER TURNED V
	// GREEK CAPITAL LETTER OMICRON WITH TONOS

	const char* c = "\xC8\x92\xC7\xAE\xC9\x85\xCE\x8C";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(8, utf8tolower(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("\xC8\x93\xC7\xAF\xCA\x8C\xCF\x8C", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLower, AmountOfBytes)
{
	const char* c = "Say \xF0\x91\xA2\xB9";
	int32_t errors = 0;

	EXPECT_EQ(8, utf8tolower(c, strlen(c), nullptr, 0, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(ToLower, NotEnoughSpace)
{
	const char* c = "Whole Lotta \xE2\x93\xA1osie";
	const size_t s = 14;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(12, utf8tolower(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("whole lotta ", b);
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(ToLower, InvalidCodepoint)
{
	const char* c = "\xF0\x92";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8tolower(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("\xEF\xBF\xBD", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLower, InvalidData)
{
	int32_t errors = 0;

	EXPECT_EQ(0, utf8tolower(nullptr, 1, nullptr, 0, 0, &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(ToLowerNormalize, BasicLatin)
{
	const char* c = "MaGiC NuMbErS";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(13, utf8tolower(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("magic numbers", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLowerNormalize, BasicLatinUppercase)
{
	const char* c = "Z";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1, utf8tolower(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("z", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLowerNormalize, BasicLatinLowercase)
{
	const char* c = "j";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1, utf8tolower(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("j", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLowerNormalize, BasicLatinUnaffected)
{
	const char* c = "-";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1, utf8tolower(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("-", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLowerNormalize, TwoBytesUppercase)
{
	const char* c = "\xC7\x8A";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8tolower(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("\xC7\x8C", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLowerNormalize, TwoBytesLowercase)
{
	const char* c = "\xC9\xA7";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8tolower(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("\xC9\xA7", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLowerNormalize, TwoBytesUnaffected)
{
	const char* c = "\xC2\xA0";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8tolower(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("\xC2\xA0", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLowerNormalize, ThreeBytesUppercase)
{
	const char* c = "\xE1\xB9\xBC";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8tolower(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("\xE1\xB9\xBD", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLowerNormalize, ThreeBytesLowercase)
{
	const char* c = "\xE1\xBC\x80";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8tolower(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("\xE1\xBC\x80", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLowerNormalize, ThreeBytesUnaffected)
{
	const char* c = "\xE2\x84\x80";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8tolower(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("\xE2\x84\x80", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLowerNormalize, FourBytesUppercase)
{
	const char* c = "\xF0\x90\x90\x8A";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8tolower(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("\xF0\x90\x90\xB2", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLowerNormalize, FourBytesLowercase)
{
	const char* c = "\xF0\x91\xA3\x85";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8tolower(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("\xF0\x91\xA3\x85", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLowerNormalize, FourBytesUnaffected)
{
	const char* c = "\xF0\x91\xA3\xA4";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8tolower(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("\xF0\x91\xA3\xA4", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLowerNormalize, Decomposed)
{
	const char* c = "\xC6\xB7\xC7\x8C" "A\xCC\x87\xCC\x84";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(6, utf8tolower(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("\xCA\x92\xC7\x8C\xC7\xA1", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLowerNormalize, DecomposedUppercase)
{
	const char* c = "A\xCC\x80";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8tolower(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("\xC3\xA0", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLowerNormalize, DecomposedLowercase)
{
	const char* c = "a\xCC\x81";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8tolower(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("\xC3\xA1", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLowerNormalize, DecomposedCouldNotCompose)
{
	const char* c = "\xE1\xBF\xBD";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf8tolower(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLowerNormalize, AmountOfBytes)
{
	const char* c = "\xE1\x80\xA5\xE1\x80\xAE";
	int32_t errors = 0;

	EXPECT_EQ(3, utf8tolower(c, strlen(c), nullptr, 0, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_EQ(0, errors);
}

TEST(ToLowerNormalize, NotEnoughSpace)
{
	const char* c = "U\xCC\x82o\xCC\x82" "c\xCC\xA7";
	const size_t s = 5;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8tolower(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("\xC3\xBB\xC3\xB4", b);
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(ToLowerNormalize, InvalidCodepoint)
{
	const char* c = "\xF0\x94\x99";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8tolower(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("\xEF\xBF\xBD", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLowerNormalize, InvalidData)
{
	int32_t errors = 0;

	EXPECT_EQ(0, utf8tolower(nullptr, 1, nullptr, 0, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}