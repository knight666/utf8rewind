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

TEST(ToUpper, Decomposed)
{
	const char* c = "O\xCC\x9Bu\xCC\x88\xCC\x84" "A\xCC\x87\xCC\x84";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(13, utf8toupper(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("O\xCC\x9BU\xCC\x88\xCC\x84" "A\xCC\x87\xCC\x84", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpper, DecomposedUppercase)
{
	const char* c = "A\xCC\x80";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8toupper(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("A\xCC\x80", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpper, DecomposedLowercase)
{
	const char* c = "i\xCC\x8C";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8toupper(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("I\xCC\x8C", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpper, DecomposedCouldNotCompose)
{
	const char* c = "\xE1\xBF\xBD";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8toupper(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("\xE1\xBF\xBD", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpper, AmountOfBytes)
{
	const char* c = "Magic";
	int32_t errors = 0;

	EXPECT_EQ(5, utf8toupper(c, strlen(c), nullptr, 0, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(ToUpper, NotEnoughSpace)
{
	const char* c = "Merde";
	const size_t s = 4;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8toupper(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("MER", b);
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(ToUpper, InvalidData)
{
	int32_t errors = 0;

	EXPECT_EQ(0, utf8toupper(nullptr, 1, nullptr, 0, 0, &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(ToUpperNormalize, BasicLatin)
{
	const char* c = "christmas SWEATer";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(17, utf8toupper(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("CHRISTMAS SWEATER", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpperNormalize, BasicLatinUppercase)
{
	const char* c = "B";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1, utf8toupper(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("B", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpperNormalize, BasicLatinLowercase)
{
	const char* c = "f";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1, utf8toupper(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("F", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpperNormalize, BasicLatinUnaffected)
{
	const char* c = "\"";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1, utf8toupper(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("\"", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpperNormalize, TwoBytesUppercase)
{
	const char* c = "\xD2\xA8";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8toupper(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("\xD2\xA8", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpperNormalize, TwoBytesLowercase)
{
	const char* c = "\xD3\x8F";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8toupper(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("\xD3\x80", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpperNormalize, TwoBytesUnaffected)
{
	const char* c = "\xD6\x8F";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8toupper(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("\xD6\x8F", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpperNormalize, ThreeBytesUppercase)
{
	const char* c = "\xE1\x82\xAC";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8toupper(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("\xE1\x82\xAC", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpperNormalize, ThreeBytesLowercase)
{
	const char* c = "\xE1\xBA\xA1";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8toupper(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("\xE1\xBA\xA0", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpperNormalize, ThreeBytesUnaffected)
{
	const char* c = "\xE1\x83\xBB";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8toupper(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("\xE1\x83\xBB", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpperNormalize, FourBytesUppercase)
{
	const char* c = "\xF0\x90\x90\x88";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toupper(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("\xF0\x90\x90\x88", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpperNormalize, FourBytesLowercase)
{
	const char* c = "\xF0\x90\x90\xBC";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toupper(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("\xF0\x90\x90\x94", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpperNormalize, FourBytesUnaffected)
{
	const char* c = "\xF0\x91\xA3\xB2";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toupper(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("\xF0\x91\xA3\xB2", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpperNormalize, Decomposed)
{
	const char* c = "a\xCC\x91\xD0\xB5\xCC\x88\xC4\xB3";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(6, utf8toupper(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("\xC8\x82\xD0\x81\xC4\xB2", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpperNormalize, DecomposedUppercase)
{
	const char* c = "N\xCC\xA7";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8toupper(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("\xC5\x85", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpperNormalize, DecomposedLowercase)
{
	const char* c = "t\xCC\xA7";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8toupper(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("\xC5\xA2", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpperNormalize, DecomposedCouldNotCompose)
{
	const char* c = "\xE1\xBF\xBD";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf8toupper(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpperNormalize, AmountOfBytes)
{
	const char* c = "U\xCC\x88\xCC\x84";
	int32_t errors = 0;

	EXPECT_EQ(2, utf8toupper(c, strlen(c), nullptr, 0, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_EQ(0, errors);
}

TEST(ToUpperNormalize, NotEnoughSpace)
{
	const char* c = "a\xCC\x8A\xCC\x81i\xCC\x8F";
	const size_t s = 3;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8toupper(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("\xC7\xBA", b);
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(ToUpperNormalize, InvalidCodepoint)
{
	const char* c = "\xF0\x94\x99";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8toupper(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("\xEF\xBF\xBD", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpperNormalize, InvalidData)
{
	int32_t errors = 0;

	EXPECT_EQ(0, utf8toupper(nullptr, 1, nullptr, 0, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}