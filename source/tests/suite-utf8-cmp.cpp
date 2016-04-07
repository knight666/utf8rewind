#include "tests-base.hpp"

#include "utf8rewind.h"

TEST(Utf8CompareCaseFolded, BasicLatinSingle)
{
	EXPECT_EQ(0, utf8cmp("Y", "Y", 1, UTF8_COMPARE_CASE_FOLDED));
}

TEST(Utf8CompareCaseFolded, BasicLatinSingleMismatch)
{
	EXPECT_EQ(1, utf8cmp("c", "b", 1, UTF8_COMPARE_CASE_FOLDED));
}

TEST(Utf8CompareCaseFolded, BasicLatinSingleMismatchCasing)
{
	EXPECT_EQ(0, utf8cmp("d", "D", 1, UTF8_COMPARE_CASE_FOLDED));
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

TEST(Utf8CompareCaseFolded, BasicLatinMultipleMismatchCasing)
{
	EXPECT_EQ(0, utf8cmp("DroughT", "dROUGHt", 7, UTF8_COMPARE_CASE_FOLDED));
}

TEST(Utf8CompareCaseFolded, BasicLatinMultipleMismatchCasingFirst)
{
	EXPECT_EQ(0, utf8cmp("Business", "business", 8, UTF8_COMPARE_CASE_FOLDED));
}

TEST(Utf8CompareCaseFolded, BasicLatinMultipleMismatchCasingLast)
{
	EXPECT_EQ(0, utf8cmp("lasT", "last", 8, UTF8_COMPARE_CASE_FOLDED));
}