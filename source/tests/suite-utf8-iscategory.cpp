#include "tests-base.hpp"

#include "../helpers/helpers-strings.hpp"

TEST(Utf8IsCategory, BasicLatinSingle)
{
	const char* i = "L";
	size_t is = strlen(i);

	EXPECT_GCEQ(1, i, is, UTF8_CATEGORY_LETTER_UPPERCASE);
}

TEST(Utf8IsCategory, BasicLatinSingleMismatch)
{
	const char* i = ")";
	size_t is = strlen(i);

	EXPECT_GCEQ(0, i, is, UTF8_CATEGORY_LETTER_LOWERCASE);
}

TEST(Utf8IsCategory, BasicLatinMultiple)
{
	const char* i = "123";
	size_t is = strlen(i);

	EXPECT_GCEQ(3, i, is, UTF8_CATEGORY_NUMBER_DECIMAL);
}

TEST(Utf8IsCategory, BasicLatinMultipleMismatchAtStart)
{
	const char* i = "(good)";
	size_t is = strlen(i);

	EXPECT_GCEQ(0, i, is, UTF8_CATEGORY_LETTER_LOWERCASE);
}

TEST(Utf8IsCategory, BasicLatinMultipleMismatchBeforeEnd)
{
	const char* i = "    %";
	size_t is = strlen(i);

	EXPECT_GCEQ(4, i, is, UTF8_CATEGORY_SEPARATOR_SPACE);
}

TEST(Utf8IsCategory, MultiByteSingle)
{
	const char* i = "\xC7\x85";
	size_t is = strlen(i);

	EXPECT_GCEQ(2, i, is, UTF8_CATEGORY_LETTER_TITLECASE);
}

TEST(Utf8IsCategory, MultiByteSingleMismatch)
{
	const char* i = "\xCA\xB3";
	size_t is = strlen(i);

	EXPECT_GCEQ(0, i, is, UTF8_CATEGORY_SYMBOL_MATH);
}

TEST(Utf8IsCategory, MultiByteMultiple)
{
	const char* i = "\xD8\x87\xC2\xAC\xCF\xB6";
	size_t is = strlen(i);

	EXPECT_GCEQ(6, i, is, UTF8_CATEGORY_SYMBOL_MATH);
}

TEST(Utf8IsCategory, MultiByteMultipleMismatchAtStart)
{
	const char* i = "\xD9\xA0\xDB\xAD\xD9\xAC\xDC\x84";
	size_t is = strlen(i);

	EXPECT_GCEQ(0, i, is, UTF8_CATEGORY_LETTER_OTHER);
}

TEST(Utf8IsCategory, MultiByteMultipleMismatchBeforeEnd)
{
	const char* i = "\xDE\x8A\xF0\x9B\xB0\xA2\xC2\xAA\xF0\x96\xAC\xBE";
	size_t is = strlen(i);

	EXPECT_GCEQ(8, i, is, UTF8_CATEGORY_LETTER_OTHER);
}

TEST(Utf8IsCategory, InvalidCodePointSingle)
{
	const char* i = "\xC5";
	size_t is = strlen(i);

	EXPECT_GCEQ(1, i, is, UTF8_CATEGORY_SYMBOL_OTHER);
}

TEST(Utf8IsCategory, InvalidCodePointSingleMismatch)
{
	const char* i = "\xE5\x81";
	size_t is = strlen(i);

	EXPECT_GCEQ(0, i, is, UTF8_CATEGORY_LETTER_OTHER);
}

TEST(Utf8IsCategory, InvalidCodePointMultiple)
{
	const char* i = "\xF8\x81\xC4\xE4\xA1";
	size_t is = strlen(i);

	EXPECT_GCEQ(5, i, is, UTF8_CATEGORY_SYMBOL_OTHER);
}

TEST(Utf8IsCategory, InvalidCodePointMultipleMismatch)
{
	const char* i = "\xDE\xAD\xF8";
	size_t is = strlen(i);

	EXPECT_GCEQ(0, i, is, UTF8_CATEGORY_NUMBER_LETTER);
}

TEST(Utf8IsCategory, InvalidData)
{
	EXPECT_GCEQ(0, nullptr, 3, UTF8_CATEGORY_LETTER_UPPERCASE);
}

TEST(Utf8IsCategory, InvalidLength)
{
	EXPECT_GCEQ(0, nullptr, 3, UTF8_CATEGORY_LETTER_UPPERCASE);
}