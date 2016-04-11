#include "tests-base.hpp"

#include "utf8rewind.h"

TEST(Utf8CompareCaseFolded, BasicLatinSingle)
{
	EXPECT_EQ(0, utf8cmp("Y", "Y", 1, UTF8_COMPARE_CASE_FOLDED));
}

TEST(Utf8CompareCaseFolded, BasicLatinSingleInvalid)
{
	EXPECT_EQ(0, utf8cmp("\xFF", "\xFF", 1, UTF8_COMPARE_CASE_FOLDED));
}

TEST(Utf8CompareCaseFolded, BasicLatinSingleMismatch)
{
	EXPECT_EQ(1, utf8cmp("c", "b", 1, UTF8_COMPARE_CASE_FOLDED));
}

TEST(Utf8CompareCaseFolded, BasicLatinSingleMismatchCaseFoldLeft)
{
	EXPECT_EQ(0, utf8cmp("Z", "z", 1, UTF8_COMPARE_CASE_FOLDED));
}

TEST(Utf8CompareCaseFolded, BasicLatinSingleMismatchCaseFoldRight)
{
	EXPECT_EQ(0, utf8cmp("d", "D", 1, UTF8_COMPARE_CASE_FOLDED));
}

TEST(Utf8CompareCaseFolded, BasicLatinSingleMismatchInvalidLeft)
{
	EXPECT_EQ(1, utf8cmp("\xFE", "B", 1, UTF8_COMPARE_CASE_FOLDED));
}

TEST(Utf8CompareCaseFolded, BasicLatinSingleMismatchInvalidRight)
{
	EXPECT_EQ(1, utf8cmp("1", "\xFF", 1, UTF8_COMPARE_CASE_FOLDED));
}

TEST(Utf8CompareCaseFolded, BasicLatinSingleMismatchInvalidBoth)
{
	EXPECT_EQ(1, utf8cmp("\xFE", "\xFF", 1, UTF8_COMPARE_CASE_FOLDED));
}

TEST(Utf8CompareCaseFolded, BasicLatinMultiple)
{
	EXPECT_EQ(0, utf8cmp("Work", "Work", 4, UTF8_COMPARE_CASE_FOLDED));
}

TEST(Utf8CompareCaseFolded, BasicLatinMultipleMismatch)
{
	EXPECT_EQ(2, utf8cmp("oOo", "oBo", 3, UTF8_COMPARE_CASE_FOLDED));
}

TEST(Utf8CompareCaseFolded, BasicLatinMultipleMismatchFirst)
{
	EXPECT_EQ(1, utf8cmp("drank", "orank", 4, UTF8_COMPARE_CASE_FOLDED));
}

TEST(Utf8CompareCaseFolded, BasicLatinMultipleMismatchLast)
{
	EXPECT_EQ(5, utf8cmp("magic", "magik", 5, UTF8_COMPARE_CASE_FOLDED));
}

TEST(Utf8CompareCaseFolded, BasicLatinMultipleMismatchCaseFold)
{
	EXPECT_EQ(0, utf8cmp("DroughT", "dROUGHt", 7, UTF8_COMPARE_CASE_FOLDED));
}

TEST(Utf8CompareCaseFolded, BasicLatinMultipleMismatchCaseFoldFirst)
{
	EXPECT_EQ(0, utf8cmp("Business", "business", 8, UTF8_COMPARE_CASE_FOLDED));
}

TEST(Utf8CompareCaseFolded, BasicLatinMultipleMismatchCaseFoldLast)
{
	EXPECT_EQ(0, utf8cmp("lasT", "last", 8, UTF8_COMPARE_CASE_FOLDED));
}

TEST(Utf8CompareCaseFolded, MultiByteSingle)
{
	EXPECT_EQ(0, utf8cmp("\xC7\xB7", "\xC7\xB7", 2, UTF8_COMPARE_CASE_FOLDED));
}

TEST(Utf8CompareCaseFolded, MultiByteSingleCasingExpansion)
{
	EXPECT_EQ(0, utf8cmp("\xC3\x9F", "ss", 2, UTF8_COMPARE_CASE_FOLDED));
}

TEST(Utf8CompareCaseFolded, MultiByteSingleMismatch)
{
	EXPECT_EQ(1, utf8cmp("\xEF\xBB\xA2", "\xEF\xB9\x91", 3, UTF8_COMPARE_CASE_FOLDED));
}

TEST(Utf8CompareCaseFolded, MultiByteSingleMismatchCaseFoldLeft)
{
	EXPECT_EQ(0, utf8cmp("\xE1\xBD\x80", "\xE1\xBD\x88", 3, UTF8_COMPARE_CASE_FOLDED));
}

TEST(Utf8CompareCaseFolded, MultiByteSingleMismatchCaseFoldRight)
{
	EXPECT_EQ(0, utf8cmp("\xE1\xBC\xA5", "\xE1\xBC\xAD", 3, UTF8_COMPARE_CASE_FOLDED));
}

TEST(Utf8CompareCaseFolded, MultiByteMultiple)
{
	EXPECT_EQ(0, utf8cmp("\xE1\xB6\xA6\xE2\x98\xB5\xE2\x8B\x9F\xE2\x99\x98", "\xE1\xB6\xA6\xE2\x98\xB5\xE2\x8B\x9F\xE2\x99\x98", 12, UTF8_COMPARE_CASE_FOLDED));
}