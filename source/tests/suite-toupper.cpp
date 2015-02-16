#include "tests-base.hpp"

#include "utf8rewind.h"

#include "helpers-strings.hpp"

TEST(ToUpper, BasicLatinSingleUppercase)
{
	const char* c = "B";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1, utf8toupper(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("B", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpper, BasicLatinSingleLowercase)
{
	const char* c = "w";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1, utf8toupper(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("W", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpper, BasicLatinSingleUnaffected)
{
	const char* c = ")";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1, utf8toupper(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ(")", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpper, BasicLatinMultipleUppercase)
{
	const char* c = "CARS";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toupper(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("CARS", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpper, BasicLatinMultipleLowercase)
{
	const char* c = "id";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8toupper(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("ID", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpper, BasicLatinMultipleUnaffected)
{
	const char* c = "#@!&%";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(5, utf8toupper(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("#@!&%", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpper, BasicLatinSequence)
{
	const char* c = "Holiday Special!";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(16, utf8toupper(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("HOLIDAY SPECIAL!", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpper, BasicLatinAmountOfBytes)
{
	const char* c = "Magic";
	int32_t errors = 0;

	EXPECT_EQ(5, utf8toupper(c, strlen(c), nullptr, 0, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(ToUpper, BasicLatinNotEnoughSpace)
{
	const char* c = "Merde";
	const size_t s = 4;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8toupper(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("MER", b);
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(ToUpper, GeneralCategoryCaseMappedSingleUppercase)
{
	// CYRILLIC CAPITAL LETTER EL

	const char* c = "\xD0\x9B";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8toupper(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("\xD0\x9B", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpper, GeneralCategoryCaseMappedSingleLowercase)
{
	// GREEK SMALL LETTER UPSILON WITH DIALYTIKA AND TONOS

	const char* c = "\xCE\xB0";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(6, utf8toupper(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("\xCE\xA5\xCC\x88\xCC\x81", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpper, GeneralCategoryCaseMappedSingleUnaffected)
{
	// COMBINING BREVE

	const char* c = "\xCC\x86";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8toupper(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("\xCC\x86", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpper, GeneralCategoryCaseMappedMultipleUppercase)
{
	// GREEK CAPITAL LETTER PI
	// GREEK CAPITAL LETTER TAU
	// GREEK CAPITAL LETTER OMEGA

	const char* c = "\xCE\xA0\xCE\xA4\xCE\xA9";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(6, utf8toupper(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("\xCE\xA0\xCE\xA4\xCE\xA9", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpper, GeneralCategoryCaseMappedMultipleLowercase)
{
	// DESERET SMALL LETTER LONG I
	// FULLWIDTH LATIN SMALL LETTER A
	// LATIN SMALL LETTER SALTILLO
	// CYRILLIC SMALL LETTER MONOGRAPH UK

	const char* c = "\xF0\x90\x90\xA8\xEF\xBD\x81\xEA\x9E\x8C\xEA\x99\x8B";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(13, utf8toupper(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("\xF0\x90\x90\x80\xEF\xBC\xA1\xEA\x9E\x8B\xEA\x99\x8A", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpper, GeneralCategoryCaseMappedMultipleUnaffected)
{
	// SUPERSET ABOVE SUPERSET
	// KATAKANA LETTER RE
	// RIGHTWARDS TRIANGLE-HEADED PAIRED ARROWS

	const char* c = "\xE2\xAB\x96\xE3\x83\xAC\xE2\xAE\x86";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(9, utf8toupper(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("\xE2\xAB\x96\xE3\x83\xAC\xE2\xAE\x86", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpper, GeneralCategoryCaseMappedSequence)
{
	const char* c = "\xD1\x8D\xD1\x82\xD0\xB8\xD1\x85 \xD0\xBC\xD1\x8F\xD0\xB3\xD0\xBA\xD0\xB8\xD1\x85";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(21, utf8toupper(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("\xD0\xAD\xD0\xA2\xD0\x98\xD0\xA5 \xD0\x9C\xD0\xAF\xD0\x93\xD0\x9A\xD0\x98\xD0\xA5", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpper, GeneralCategoryCaseMappedAmountOfBytes)
{
	// 2CC3 1FE2 2C5A
	// 2CC2 03A5 0308 0300 2C5A

	const char* c = "\xE2\xB3\x83\xE1\xBF\xA2\xE2\xB1\x9A";
	int32_t errors = 0;

	EXPECT_EQ(12, utf8toupper(c, strlen(c), nullptr, 0, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(ToUpper, GeneralCategoryCaseMappedNotEnoughSpace)
{
	// 2163 1FD2
	// 2163 0399 0308 0300

	const char* c = "\xE2\x85\xA3\xE1\xBF\x92";
	const size_t s = 6;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8toupper(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("\xE2\x85\xA3", b);
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(ToUpper, InvalidCodepointSingle)
{
	const char* c = "\xCD";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8toupper(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("\xEF\xBF\xBD", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpper, InvalidCodepointMultiple)
{
	const char* c = "\xED\x89\xC0\x9A\xCA";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(9, utf8toupper(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("\xEF\xBF\xBD\xEF\xBF\xBD\xEF\xBF\xBD", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpper, InvalidCodepointAmountOfBytes)
{
	const char* c = "\xDE\xDE\xDA\xCA";
	int32_t errors = 0;

	EXPECT_EQ(12, utf8toupper(c, strlen(c), nullptr, 0, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(ToUpper, InvalidCodepointNotEnoughSpace)
{
	const char* c = "\xDF\xDF\xDF";
	const size_t s = 8;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(6, utf8toupper(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("\xEF\xBF\xBD\xEF\xBF\xBD", b);
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(ToUpper, InvalidData)
{
	int32_t errors = 0;

	EXPECT_EQ(0, utf8toupper(nullptr, 1, nullptr, 0, 0, &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}