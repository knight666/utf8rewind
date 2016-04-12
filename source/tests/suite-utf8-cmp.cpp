#include "tests-base.hpp"

#include "../helpers/helpers-compare.hpp"

TEST(Utf8CompareCaseFolded, BasicLatinSingle)
{
	EXPECT_CMP_EQ("Y", "Y", 1, UTF8_COMPARE_CASE_FOLDED, 0);
}

TEST(Utf8CompareCaseFolded, BasicLatinSingleInvalid)
{
	EXPECT_CMP_EQ("\xFF", "\xFF", 1, UTF8_COMPARE_CASE_FOLDED, 0);
}

TEST(Utf8CompareCaseFolded, BasicLatinSingleMismatch)
{
	EXPECT_CMP_EQ("c", "b", 1, UTF8_COMPARE_CASE_FOLDED, 1);
}

TEST(Utf8CompareCaseFolded, BasicLatinSingleMismatchCaseFoldLeft)
{
	EXPECT_CMP_EQ("Z", "z", 1, UTF8_COMPARE_CASE_FOLDED, 0);
}

TEST(Utf8CompareCaseFolded, BasicLatinSingleMismatchCaseFoldRight)
{
	EXPECT_CMP_EQ("d", "D", 1, UTF8_COMPARE_CASE_FOLDED, 0);
}

TEST(Utf8CompareCaseFolded, BasicLatinSingleMismatchInvalidLeft)
{
	EXPECT_CMP_EQ("\xFE", "B", 1, UTF8_COMPARE_CASE_FOLDED, 1);
}

TEST(Utf8CompareCaseFolded, BasicLatinSingleMismatchInvalidRight)
{
	EXPECT_CMP_EQ("1", "\xFF", 1, UTF8_COMPARE_CASE_FOLDED, 1);
}

TEST(Utf8CompareCaseFolded, BasicLatinSingleMismatchInvalidBoth)
{
	EXPECT_CMP_EQ("\xFE", "\xFF", 1, UTF8_COMPARE_CASE_FOLDED, 1);
}

TEST(Utf8CompareCaseFolded, BasicLatinMultiple)
{
	EXPECT_CMP_EQ("Work", "Work", 4, UTF8_COMPARE_CASE_FOLDED, 0);
}

TEST(Utf8CompareCaseFolded, BasicLatinMultipleMismatch)
{
	EXPECT_CMP_EQ("oOo", "oBo", 3, UTF8_COMPARE_CASE_FOLDED, 2);
}

TEST(Utf8CompareCaseFolded, BasicLatinMultipleMismatchFirst)
{
	EXPECT_CMP_EQ("drank", "orank", 4, UTF8_COMPARE_CASE_FOLDED, 1);
}

TEST(Utf8CompareCaseFolded, BasicLatinMultipleMismatchLast)
{
	EXPECT_CMP_EQ("magic", "magik", 5, UTF8_COMPARE_CASE_FOLDED, 5);
}

TEST(Utf8CompareCaseFolded, BasicLatinMultipleMismatchCaseFold)
{
	EXPECT_CMP_EQ("DroughT", "dROUGHt", 7, UTF8_COMPARE_CASE_FOLDED, 0);
}

TEST(Utf8CompareCaseFolded, BasicLatinMultipleMismatchCaseFoldFirst)
{
	EXPECT_CMP_EQ("Business", "business", 8, UTF8_COMPARE_CASE_FOLDED, 0);
}

TEST(Utf8CompareCaseFolded, BasicLatinMultipleMismatchCaseFoldLast)
{
	EXPECT_CMP_EQ("lasT", "last", 8, UTF8_COMPARE_CASE_FOLDED, 0);
}

TEST(Utf8CompareCaseFolded, MultiByteSingle)
{
	EXPECT_CMP_EQ("\xC7\xB7", "\xC7\xB7", 2, UTF8_COMPARE_CASE_FOLDED, 0);
}

TEST(Utf8CompareCaseFolded, MultiByteSingleCasingExpansion)
{
	EXPECT_CMP_EQ("\xC3\x9F", "ss", 2, UTF8_COMPARE_CASE_FOLDED, 0);
}

TEST(Utf8CompareCaseFolded, MultiByteSingleMismatch)
{
	EXPECT_CMP_EQ("\xEF\xBB\xA2", "\xEF\xB9\x91", 3, UTF8_COMPARE_CASE_FOLDED, 1);
}

TEST(Utf8CompareCaseFolded, MultiByteSingleMismatchCaseFoldLeft)
{
	EXPECT_CMP_EQ("\xE1\xBD\x80", "\xE1\xBD\x88", 3, UTF8_COMPARE_CASE_FOLDED, 0);
}

TEST(Utf8CompareCaseFolded, MultiByteSingleMismatchCaseFoldRight)
{
	EXPECT_CMP_EQ("\xE1\xBC\xA5", "\xE1\xBC\xAD", 3, UTF8_COMPARE_CASE_FOLDED, 1);
}

TEST(Utf8CompareCaseFolded, MultiByteMultiple)
{
	EXPECT_CMP_EQ("\xE1\xB6\xA6\xE2\x98\xB5\xE2\x8B\x9F\xE2\x99\x98", "\xE1\xB6\xA6\xE2\x98\xB5\xE2\x8B\x9F\xE2\x99\x98", 12, UTF8_COMPARE_CASE_FOLDED, 0);
}

TEST(Utf8CompareCaseFolded, MultiByteMultipleBad)
{
	EXPECT_CMP_EQ("\xE1\xB6\xA6\xE2\x98\xB5\xE2\x8B\x9F\xE2\x99\x98", "\xE1\xB6\xA6\xE2\x98\xB5\xE2\x8B\x9F\xE2\x99\x97", 12, UTF8_COMPARE_CASE_FOLDED, 0);
}