#include "tests-base.hpp"

extern "C" {
	#include "../internal/database.h"
}

#include "helpers-strings.hpp"

TEST(FindDecomposition, Compose)
{
	EXPECT_EQ(nullptr, database_querydecomposition(0x0000011A, UnicodeProperty_Normalization_Compose));
}

TEST(FindDecomposition, CompatibilityCompose)
{
	EXPECT_EQ(nullptr, database_querydecomposition(0x0000009B, UnicodeProperty_Normalization_Compatibility_Compose));
}

TEST(FindDecomposition, InvalidQuery)
{
	EXPECT_EQ(nullptr, database_querydecomposition(0x0002F974, 33));
}

TEST(FindDecomposition, DecomposedFound)
{
	EXPECT_UTF8EQ("\xD0\x98\xCC\x88", database_querydecomposition(0x000004E4, UnicodeProperty_Normalization_Decompose));
}

TEST(FindDecomposition, DecomposedFoundFirst)
{
	EXPECT_UTF8EQ("A\xCC\x80", database_querydecomposition(0x000000C0, UnicodeProperty_Normalization_Decompose));
}

TEST(FindDecomposition, DecomposedFoundLast)
{
	EXPECT_UTF8EQ("\xF0\xAA\x98\x80", database_querydecomposition(0x0002FA1D, UnicodeProperty_Normalization_Decompose));
}

TEST(FindDecomposition, DecomposedFoundPivot)
{
	EXPECT_UTF8EQ("\xE7\x88\x9B", database_querydecomposition(0x0000F91E, UnicodeProperty_Normalization_Decompose));
}

TEST(FindDecomposition, DecomposedFoundPivotUp)
{
	EXPECT_UTF8EQ("s\xCC\xA3", database_querydecomposition(0x00001E63, UnicodeProperty_Normalization_Decompose));
}

TEST(FindDecomposition, DecomposedFoundPivotDown)
{
	EXPECT_UTF8EQ("\xE5\x86\xAC", database_querydecomposition(0x0002F81A, UnicodeProperty_Normalization_Decompose));
}

TEST(FindDecomposition, DecomposedFoundPivotDownDown)
{
	EXPECT_UTF8EQ(";", database_querydecomposition(0x0000037E, UnicodeProperty_Normalization_Decompose));
}

TEST(FindDecomposition, DecomposedFoundMaxDepth)
{
	EXPECT_UTF8EQ("\xE8\xBC\xB8", database_querydecomposition(0x0002F9DF, UnicodeProperty_Normalization_Decompose));
}

TEST(FindDecomposition, DecomposedMissing)
{
	EXPECT_EQ(nullptr, database_querydecomposition(0x0001FFFF, UnicodeProperty_Normalization_Decompose));
}

TEST(FindDecomposition, DecomposedMissingOutOfLowerBounds)
{
	EXPECT_EQ(nullptr, database_querydecomposition(0x00000067, UnicodeProperty_Normalization_Decompose));
}

TEST(FindDecomposition, DecomposedMissingOutOfUpperBounds)
{
	EXPECT_EQ(nullptr, database_querydecomposition(0x0011A26D, UnicodeProperty_Normalization_Decompose));
}

TEST(FindDecomposition, CompatibilityDecomposedFound)
{
	EXPECT_UTF8EQ("\xD0\x98\xCC\x88", database_querydecomposition(0x000004E4, UnicodeProperty_Normalization_Compatibility_Decompose));
}

TEST(FindDecomposition, CompatibilityDecomposedFoundFirst)
{
	EXPECT_UTF8EQ(" ", database_querydecomposition(0x000000A0, UnicodeProperty_Normalization_Compatibility_Decompose));
}

TEST(FindDecomposition, CompatibilityDecomposedFoundLast)
{
	EXPECT_UTF8EQ("\xF0\xAA\x98\x80", database_querydecomposition(0x0002FA1D, UnicodeProperty_Normalization_Compatibility_Decompose));
}

TEST(FindDecomposition, CompatibilityDecomposedFoundPivot)
{
	EXPECT_UTF8EQ("\xD7\x90\xD7\x9C", database_querydecomposition(0x0000FB4F, UnicodeProperty_Normalization_Compatibility_Decompose));
}

TEST(FindDecomposition, CompatibilityDecomposedFoundPivotUp)
{
	EXPECT_UTF8EQ("w", database_querydecomposition(0x000024E6, UnicodeProperty_Normalization_Compatibility_Decompose));
}

TEST(FindDecomposition, CompatibilityDecomposedFoundPivotDown)
{
	EXPECT_UTF8EQ("q", database_querydecomposition(0x0001D596, UnicodeProperty_Normalization_Compatibility_Decompose));
}

TEST(FindDecomposition, CompatibilityDecomposedFoundPivotDownUp)
{
	EXPECT_UTF8EQ("\xE3\x83\x9B\xE3\x83\xBC\xE3\x83\xB3", database_querydecomposition(0x00003342, UnicodeProperty_Normalization_Compatibility_Decompose));
}

TEST(FindDecomposition, CompatibilityDecomposedFoundMaxDepth)
{
	EXPECT_UTF8EQ("u", database_querydecomposition(0x0001D4CA, UnicodeProperty_Normalization_Compatibility_Decompose));
}

TEST(FindDecomposition, CompatibilityDecomposedMissing)
{
	EXPECT_EQ(nullptr, database_querydecomposition(0x0001A2AF, UnicodeProperty_Normalization_Compatibility_Decompose));
}

TEST(FindDecomposition, CompatibilityDecomposedMissingOutOfLowerBounds)
{
	EXPECT_EQ(nullptr, database_querydecomposition(0x00000023, UnicodeProperty_Normalization_Compatibility_Decompose));
}

TEST(FindDecomposition, CompatibilityDecomposedMissingOutOfUpperBounds)
{
	EXPECT_EQ(nullptr, database_querydecomposition(0x00DD2A5D, UnicodeProperty_Normalization_Compatibility_Decompose));
}

TEST(FindDecomposition, UppercaseFound)
{
	EXPECT_UTF8EQ("\xE1\xB8\x8A", database_querydecomposition(0x00001E0B, UnicodeProperty_Uppercase));
}

TEST(FindDecomposition, UppercaseFoundFirst)
{
	EXPECT_UTF8EQ("\xCE\x9C", database_querydecomposition(0x000000B5, UnicodeProperty_Uppercase));
}

TEST(FindDecomposition, UppercaseFoundLast)
{
	EXPECT_UTF8EQ("\xF0\x91\xA2\xBF", database_querydecomposition(0x000118DF, UnicodeProperty_Uppercase));
}

TEST(FindDecomposition, UppercaseFoundPivot)
{
	EXPECT_UTF8EQ("\xE1\xBB\x84", database_querydecomposition(0x00001EC5, UnicodeProperty_Uppercase));
}

TEST(FindDecomposition, UppercaseFoundPivotUp)
{
	EXPECT_UTF8EQ("\xD0\x91", database_querydecomposition(0x00000431, UnicodeProperty_Uppercase));
}

TEST(FindDecomposition, UppercaseFoundPivotDown)
{
	EXPECT_UTF8EQ("\xE1\xBD\xAF\xCE\x99", database_querydecomposition(0x00001FAF, UnicodeProperty_Uppercase));
}

TEST(FindDecomposition, UppercaseFoundPivotDownUp)
{
	EXPECT_UTF8EQ("\xE2\x93\x88", database_querydecomposition(0x000024E2, UnicodeProperty_Uppercase));
}

TEST(FindDecomposition, UppercaseFoundMaxDepth)
{
	EXPECT_UTF8EQ("\xE2\x92\xBD", database_querydecomposition(0x000024D7, UnicodeProperty_Uppercase));
}

TEST(FindDecomposition, UppercaseMissing)
{
	EXPECT_EQ(nullptr, database_querydecomposition(0x00002BAD, UnicodeProperty_Uppercase));
}

TEST(FindDecomposition, UppercaseMissingOutOfLowerBounds)
{
	EXPECT_EQ(nullptr, database_querydecomposition(0x00000020, UnicodeProperty_Uppercase));
}

TEST(FindDecomposition, UppercaseMissingOutOfUpperBounds)
{
	EXPECT_EQ(nullptr, database_querydecomposition(0x00101111, UnicodeProperty_Uppercase));
}

TEST(FindDecomposition, LowercaseFound)
{
	EXPECT_UTF8EQ("\xCE\xB3", database_querydecomposition(0x00000393, UnicodeProperty_Lowercase));
}

TEST(FindDecomposition, LowercaseFoundFirst)
{
	EXPECT_UTF8EQ("\xC3\xA0", database_querydecomposition(0x000000C0, UnicodeProperty_Lowercase));
}

TEST(FindDecomposition, LowercaseFoundLast)
{
	EXPECT_UTF8EQ("\xF0\x91\xA3\x9F", database_querydecomposition(0x000118BF, UnicodeProperty_Lowercase));
}

TEST(FindDecomposition, LowercaseFoundPivot)
{
	EXPECT_UTF8EQ("\xE1\xBA\xA1", database_querydecomposition(0x00001EA0, UnicodeProperty_Lowercase));
}

TEST(FindDecomposition, LowercaseFoundPivotUp)
{
	EXPECT_UTF8EQ("\xD0\xB7", database_querydecomposition(0x00000417, UnicodeProperty_Lowercase));
}

TEST(FindDecomposition, LowercaseFoundPivotDown)
{
	EXPECT_UTF8EQ("\xE2\xB1\x9A", database_querydecomposition(0x00002C2A, UnicodeProperty_Lowercase));
}

TEST(FindDecomposition, LowercaseFoundPivotDownUp)
{
	EXPECT_UTF8EQ("\xE1\xBE\xA2", database_querydecomposition(0x00001FAA, UnicodeProperty_Lowercase));
}

TEST(FindDecomposition, LowercaseFoundMaxDepth)
{
	EXPECT_UTF8EQ("\xEA\x9C\xA9", database_querydecomposition(0x0000A728, UnicodeProperty_Lowercase));
}

TEST(FindDecomposition, LowercaseMissing)
{
	EXPECT_EQ(nullptr, database_querydecomposition(0x0000F3AA, UnicodeProperty_Lowercase));
}

TEST(FindDecomposition, LowercaseMissingOutOfLowerBounds)
{
	EXPECT_EQ(nullptr, database_querydecomposition(0x00000021, UnicodeProperty_Lowercase));
}

TEST(FindDecomposition, LowercaseMissingOutOfUpperBounds)
{
	EXPECT_EQ(nullptr, database_querydecomposition(0x00012000, UnicodeProperty_Lowercase));
}

TEST(FindDecomposition, TitlecaseFound)
{
	EXPECT_UTF8EQ("\xD0\x80", database_querydecomposition(0x00000450, UnicodeProperty_Titlecase));
}

TEST(FindDecomposition, TitlecaseFoundFirst)
{
	EXPECT_UTF8EQ("\xCE\x9C", database_querydecomposition(0x000000B5, UnicodeProperty_Titlecase));
}

TEST(FindDecomposition, TitlecaseFoundLast)
{
	EXPECT_UTF8EQ("\xF0\x91\xA2\xBF", database_querydecomposition(0x000118DF, UnicodeProperty_Titlecase));
}

TEST(FindDecomposition, TitlecaseFoundPivot)
{
	EXPECT_UTF8EQ("\xE1\xBB\x80", database_querydecomposition(0x00001EC1, UnicodeProperty_Titlecase));
}

TEST(FindDecomposition, TitlecaseFoundPivotUp)
{
	EXPECT_UTF8EQ("\xCF\xB7", database_querydecomposition(0x000003F8, UnicodeProperty_Titlecase));
}

TEST(FindDecomposition, TitlecaseFoundPivotDown)
{
	EXPECT_UTF8EQ("\xE2\xB2\x90", database_querydecomposition(0x00002C91, UnicodeProperty_Titlecase));
}

TEST(FindDecomposition, TitlecaseFoundPivotDownUp)
{
	EXPECT_UTF8EQ("\xE1\xBE\xAE", database_querydecomposition(0x00001FAE, UnicodeProperty_Titlecase));
}

TEST(FindDecomposition, TitlecaseFoundMaxDepth)
{
	EXPECT_UTF8EQ("\xE1\xBE\x9F", database_querydecomposition(0x00001F9F, UnicodeProperty_Titlecase));
}

TEST(FindDecomposition, TitlecaseMissing)
{
	EXPECT_EQ(nullptr, database_querydecomposition(0x0000ABED, UnicodeProperty_Titlecase));
}

TEST(FindDecomposition, TitlecaseMissingOutOfLowerBounds)
{
	EXPECT_EQ(nullptr, database_querydecomposition(0x0000001A, UnicodeProperty_Titlecase));
}

TEST(FindDecomposition, TitlecaseMissingOutOfUpperBounds)
{
	EXPECT_EQ(nullptr, database_querydecomposition(0x0002112A, UnicodeProperty_Titlecase));
}