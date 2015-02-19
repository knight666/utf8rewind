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

TEST(ToLower, GeneralCategoryCaseMappedMultipleLowercase)
{
	// CYRILLIC SMALL LETTER SHORT I
	// ARMENIAN SMALL LETTER PIWR
	// GREEK SMALL LETTER PHI

	const char* c = "\xD0\xB9\xD6\x83\xCF\x86";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(6, utf8tolower(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("\xD0\xB9\xD6\x83\xCF\x86", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLower, GeneralCategoryCaseMappedMultipleTitlecase)
{
	// LATIN SMALL LETTER SHARP S
	// LATIN SMALL LETTER DZ
	// LATIN SMALL LIGATURE IJ

	const char* c = "\xC3\x9F\xC7\xB2\xC4\xB3";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(6, utf8tolower(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("\xC3\x9F\xC7\xB3\xC4\xB3", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLower, GeneralCategoryCaseMappedMultipleUnaffected)
{
	// COMBINING CYRILLIC TITLO
	// SIDEWAYS BLACK UP POINTING INDEX
	// MANICHAEAN LETTER SADHE
	// ARABIC LETTER DAL WITH INVERTED V

	const char* c = "\xD2\x83\xF0\x9F\x96\xA0\xF0\x90\xAB\x9D\xDB\xAE";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(12, utf8tolower(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("\xD2\x83\xF0\x9F\x96\xA0\xF0\x90\xAB\x9D\xDB\xAE", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLower, GeneralCategoryCaseMappedWord)
{
	const char* c = "\xCF\x88\xCF\x85\xCF\x87\xCE\xBF\xCF\x86\xCE\xB8\xCF\x8C\xCF\x81\xCE\xB1";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(18, utf8tolower(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("\xCF\x88\xCF\x85\xCF\x87\xCE\xBF\xCF\x86\xCE\xB8\xCF\x8C\xCF\x81\xCE\xB1", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLower, GeneralCategoryCaseMappedSentence)
{
	const char* c = "\xCE\x93\xCE\xB1\xCE\xB6\xCE\xAD\xCE\xB5\xCF\x82 \xCE\xBA\xCE\xB1\xE1\xBD\xB6 \xCE\xBC\xCF\x85\xCF\x81\xCF\x84\xCE\xB9\xE1\xBD\xB2\xCF\x82";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(36, utf8tolower(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("\xCE\xB3\xCE\xB1\xCE\xB6\xCE\xAD\xCE\xB5\xCF\x82 \xCE\xBA\xCE\xB1\xE1\xBD\xB6 \xCE\xBC\xCF\x85\xCF\x81\xCF\x84\xCE\xB9\xE1\xBD\xB2\xCF\x82", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLower, GeneralCategoryCaseMappedAmountOfBytes)
{
	// 0130 01A2 24D2
	// 0069 0307 01A3 24D2

	const char* c = "\xC4\xB0\xC6\xA2\xE2\x93\x92";
	int32_t errors = 0;

	EXPECT_EQ(8, utf8tolower(c, strlen(c), nullptr, 0, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(ToLower, GeneralCategoryCaseMappedNotEnoughSpace)
{
	// 1FE4 1E54 1F4D
	// 1FE4 1E55 1F45

	const char* c = "\xE1\xBF\xA4\xE1\xB9\x94\xE1\xBD\x8D";
	const size_t s = 8;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(6, utf8tolower(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("\xE1\xBF\xA4\xE1\xB9\x95", b);
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(ToLower, InvalidCodepointSingle)
{
	const char* c = "\xF0\x92";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8tolower(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("\xEF\xBF\xBD", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLower, InvalidCodepointMultiple)
{
	const char* c = "\xED\x89\xC0\x9A\xCA";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(9, utf8tolower(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("\xEF\xBF\xBD\xEF\xBF\xBD\xEF\xBF\xBD", b);
	EXPECT_EQ(0, errors);
}

TEST(ToLower, InvalidCodepointAmountOfBytes)
{
	const char* c = "\xDE\xDE\xDA\xCA";
	int32_t errors = 0;

	EXPECT_EQ(12, utf8tolower(c, strlen(c), nullptr, 0, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(ToLower, InvalidCodepointNotEnoughSpace)
{
	const char* c = "\xDF\xDF\xDF";
	const size_t s = 8;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(6, utf8tolower(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("\xEF\xBF\xBD\xEF\xBF\xBD", b);
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(ToLower, InvalidData)
{
	int32_t errors = 0;

	EXPECT_EQ(0, utf8tolower(nullptr, 1, nullptr, 0, 0, &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}