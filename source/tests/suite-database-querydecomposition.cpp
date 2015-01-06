#include "tests-base.hpp"

extern "C" {
	#include "../internal/database.h"
}

#include "helpers-strings.hpp"

TEST(QueryDecomposition, PropertyComposeProperty)
{
	EXPECT_EQ(nullptr, database_querydecomposition(0x0000011A, UnicodeProperty_Normalization_Compose));
}

TEST(QueryDecomposition, PropertyCompatibilityComposeProperty)
{
	EXPECT_EQ(nullptr, database_querydecomposition(0x0000009B, UnicodeProperty_Normalization_Compatibility_Compose));
}

TEST(QueryDecomposition, PropertyInvalid)
{
	EXPECT_EQ(nullptr, database_querydecomposition(0x0002F974, 33));
}

TEST(QueryDecompositionDecomposed, Found)
{
	EXPECT_UTF8EQ("\xD0\x98\xCC\x88", database_querydecomposition(0x000004E4, UnicodeProperty_Normalization_Decompose));
}

TEST(QueryDecompositionDecomposed, FoundFirst)
{
	EXPECT_UTF8EQ("A\xCC\x80", database_querydecomposition(0x000000C0, UnicodeProperty_Normalization_Decompose));
}

TEST(QueryDecompositionDecomposed, FoundLast)
{
	EXPECT_UTF8EQ("\xF0\xAA\x98\x80", database_querydecomposition(0x0002FA1D, UnicodeProperty_Normalization_Decompose));
}

TEST(QueryDecompositionDecomposed, FoundPivot)
{
	EXPECT_UTF8EQ("\xE7\x88\x9B", database_querydecomposition(0x0000F91E, UnicodeProperty_Normalization_Decompose));
}

TEST(QueryDecompositionDecomposed, FoundPivotUp)
{
	EXPECT_UTF8EQ("s\xCC\xA3", database_querydecomposition(0x00001E63, UnicodeProperty_Normalization_Decompose));
}

TEST(QueryDecompositionDecomposed, FoundPivotDown)
{
	EXPECT_UTF8EQ("\xE5\x86\xAC", database_querydecomposition(0x0002F81A, UnicodeProperty_Normalization_Decompose));
}

TEST(QueryDecompositionDecomposed, FoundPivotDownDown)
{
	EXPECT_UTF8EQ(";", database_querydecomposition(0x0000037E, UnicodeProperty_Normalization_Decompose));
}

TEST(QueryDecompositionDecomposed, FoundMaxDepth)
{
	EXPECT_UTF8EQ("\xE8\xBC\xB8", database_querydecomposition(0x0002F9DF, UnicodeProperty_Normalization_Decompose));
}

TEST(QueryDecompositionDecomposed, Missing)
{
	EXPECT_EQ(nullptr, database_querydecomposition(0x0001FFFF, UnicodeProperty_Normalization_Decompose));
}

TEST(QueryDecompositionDecomposed, MissingOutOfLowerBounds)
{
	EXPECT_EQ(nullptr, database_querydecomposition(0x00000067, UnicodeProperty_Normalization_Decompose));
}

TEST(QueryDecompositionDecomposed, MissingOutOfUpperBounds)
{
	EXPECT_EQ(nullptr, database_querydecomposition(0x0011A26D, UnicodeProperty_Normalization_Decompose));
}

TEST(QueryDecompositionCompatibilityDecomposed, Found)
{
	EXPECT_UTF8EQ("\xD0\x98\xCC\x88", database_querydecomposition(0x000004E4, UnicodeProperty_Normalization_Compatibility_Decompose));
}

TEST(QueryDecompositionCompatibilityDecomposed, FoundFirst)
{
	EXPECT_UTF8EQ(" ", database_querydecomposition(0x000000A0, UnicodeProperty_Normalization_Compatibility_Decompose));
}

TEST(QueryDecompositionCompatibilityDecomposed, FoundLast)
{
	EXPECT_UTF8EQ("\xF0\xAA\x98\x80", database_querydecomposition(0x0002FA1D, UnicodeProperty_Normalization_Compatibility_Decompose));
}

TEST(QueryDecompositionCompatibilityDecomposed, FoundPivot)
{
	EXPECT_UTF8EQ("\xD7\x90\xD7\x9C", database_querydecomposition(0x0000FB4F, UnicodeProperty_Normalization_Compatibility_Decompose));
}

TEST(QueryDecompositionCompatibilityDecomposed, FoundPivotUp)
{
	EXPECT_UTF8EQ("w", database_querydecomposition(0x000024E6, UnicodeProperty_Normalization_Compatibility_Decompose));
}

TEST(QueryDecompositionCompatibilityDecomposed, FoundPivotDown)
{
	EXPECT_UTF8EQ("q", database_querydecomposition(0x0001D596, UnicodeProperty_Normalization_Compatibility_Decompose));
}

TEST(QueryDecompositionCompatibilityDecomposed, FoundPivotDownUp)
{
	EXPECT_UTF8EQ("\xE3\x83\x9B\xE3\x83\xBC\xE3\x83\xB3", database_querydecomposition(0x00003342, UnicodeProperty_Normalization_Compatibility_Decompose));
}

TEST(QueryDecompositionCompatibilityDecomposed, FoundMaxDepth)
{
	EXPECT_UTF8EQ("u", database_querydecomposition(0x0001D4CA, UnicodeProperty_Normalization_Compatibility_Decompose));
}

TEST(QueryDecompositionCompatibilityDecomposed, Missing)
{
	EXPECT_EQ(nullptr, database_querydecomposition(0x0001A2AF, UnicodeProperty_Normalization_Compatibility_Decompose));
}

TEST(QueryDecompositionCompatibilityDecomposed, MissingOutOfLowerBounds)
{
	EXPECT_EQ(nullptr, database_querydecomposition(0x00000023, UnicodeProperty_Normalization_Compatibility_Decompose));
}

TEST(QueryDecompositionCompatibilityDecomposed, MissingOutOfUpperBounds)
{
	EXPECT_EQ(nullptr, database_querydecomposition(0x00DD2A5D, UnicodeProperty_Normalization_Compatibility_Decompose));
}

TEST(QueryDecompositionUppercase, Found)
{
	EXPECT_UTF8EQ("\xE1\xB8\x8A", database_querydecomposition(0x00001E0B, UnicodeProperty_Uppercase));
}

TEST(QueryDecompositionUppercase, FoundFirst)
{
	EXPECT_UTF8EQ("\xCE\x9C", database_querydecomposition(0x000000B5, UnicodeProperty_Uppercase));
}

TEST(QueryDecompositionUppercase, FoundLast)
{
	EXPECT_UTF8EQ("\xF0\x91\xA2\xBF", database_querydecomposition(0x000118DF, UnicodeProperty_Uppercase));
}

TEST(QueryDecompositionUppercase, FoundPivot)
{
	EXPECT_UTF8EQ("\xE1\xBB\x84", database_querydecomposition(0x00001EC5, UnicodeProperty_Uppercase));
}

TEST(QueryDecompositionUppercase, FoundPivotUp)
{
	EXPECT_UTF8EQ("\xD0\x91", database_querydecomposition(0x00000431, UnicodeProperty_Uppercase));
}

TEST(QueryDecompositionUppercase, FoundPivotDown)
{
	EXPECT_UTF8EQ("\xE1\xBD\xAF\xCE\x99", database_querydecomposition(0x00001FAF, UnicodeProperty_Uppercase));
}

TEST(QueryDecompositionUppercase, FoundPivotDownUp)
{
	EXPECT_UTF8EQ("\xE2\x93\x88", database_querydecomposition(0x000024E2, UnicodeProperty_Uppercase));
}

TEST(QueryDecompositionUppercase, FoundMaxDepth)
{
	EXPECT_UTF8EQ("\xE2\x92\xBD", database_querydecomposition(0x000024D7, UnicodeProperty_Uppercase));
}

TEST(QueryDecompositionUppercase, Missing)
{
	EXPECT_EQ(nullptr, database_querydecomposition(0x00002BAD, UnicodeProperty_Uppercase));
}

TEST(QueryDecompositionUppercase, MissingOutOfLowerBounds)
{
	EXPECT_EQ(nullptr, database_querydecomposition(0x00000020, UnicodeProperty_Uppercase));
}

TEST(QueryDecompositionUppercase, MissingOutOfUpperBounds)
{
	EXPECT_EQ(nullptr, database_querydecomposition(0x00101111, UnicodeProperty_Uppercase));
}

TEST(QueryDecompositionLowercase, Found)
{
	EXPECT_UTF8EQ("\xCE\xB3", database_querydecomposition(0x00000393, UnicodeProperty_Lowercase));
}

TEST(QueryDecompositionLowercase, FoundFirst)
{
	EXPECT_UTF8EQ("\xC3\xA0", database_querydecomposition(0x000000C0, UnicodeProperty_Lowercase));
}

TEST(QueryDecompositionLowercase, FoundLast)
{
	EXPECT_UTF8EQ("\xF0\x91\xA3\x9F", database_querydecomposition(0x000118BF, UnicodeProperty_Lowercase));
}

TEST(QueryDecompositionLowercase, FoundPivot)
{
	EXPECT_UTF8EQ("\xE1\xBA\xA1", database_querydecomposition(0x00001EA0, UnicodeProperty_Lowercase));
}

TEST(QueryDecompositionLowercase, FoundPivotUp)
{
	EXPECT_UTF8EQ("\xD0\xB7", database_querydecomposition(0x00000417, UnicodeProperty_Lowercase));
}

TEST(QueryDecompositionLowercase, FoundPivotDown)
{
	EXPECT_UTF8EQ("\xE2\xB1\x9A", database_querydecomposition(0x00002C2A, UnicodeProperty_Lowercase));
}

TEST(QueryDecompositionLowercase, FoundPivotDownUp)
{
	EXPECT_UTF8EQ("\xE1\xBE\xA2", database_querydecomposition(0x00001FAA, UnicodeProperty_Lowercase));
}

TEST(QueryDecompositionLowercase, FoundMaxDepth)
{
	EXPECT_UTF8EQ("\xEA\x9C\xA9", database_querydecomposition(0x0000A728, UnicodeProperty_Lowercase));
}

TEST(QueryDecompositionLowercase, Missing)
{
	EXPECT_EQ(nullptr, database_querydecomposition(0x0000F3AA, UnicodeProperty_Lowercase));
}

TEST(QueryDecompositionLowercase, MissingOutOfLowerBounds)
{
	EXPECT_EQ(nullptr, database_querydecomposition(0x00000021, UnicodeProperty_Lowercase));
}

TEST(QueryDecompositionLowercase, MissingOutOfUpperBounds)
{
	EXPECT_EQ(nullptr, database_querydecomposition(0x00012000, UnicodeProperty_Lowercase));
}

TEST(QueryDecompositionTitlecase, Found)
{
	EXPECT_UTF8EQ("\xD0\x80", database_querydecomposition(0x00000450, UnicodeProperty_Titlecase));
}

TEST(QueryDecompositionTitlecase, FoundFirst)
{
	EXPECT_UTF8EQ("\xCE\x9C", database_querydecomposition(0x000000B5, UnicodeProperty_Titlecase));
}

TEST(QueryDecompositionTitlecase, FoundLast)
{
	EXPECT_UTF8EQ("\xF0\x91\xA2\xBF", database_querydecomposition(0x000118DF, UnicodeProperty_Titlecase));
}

TEST(QueryDecompositionTitlecase, FoundPivot)
{
	EXPECT_UTF8EQ("\xE1\xBB\x80", database_querydecomposition(0x00001EC1, UnicodeProperty_Titlecase));
}

TEST(QueryDecompositionTitlecase, FoundPivotUp)
{
	EXPECT_UTF8EQ("\xCF\xB7", database_querydecomposition(0x000003F8, UnicodeProperty_Titlecase));
}

TEST(QueryDecompositionTitlecase, FoundPivotDown)
{
	EXPECT_UTF8EQ("\xE2\xB2\x90", database_querydecomposition(0x00002C91, UnicodeProperty_Titlecase));
}

TEST(QueryDecompositionTitlecase, FoundPivotDownUp)
{
	EXPECT_UTF8EQ("\xE1\xBE\xAE", database_querydecomposition(0x00001FAE, UnicodeProperty_Titlecase));
}

TEST(QueryDecompositionTitlecase, FoundMaxDepth)
{
	EXPECT_UTF8EQ("\xE1\xBE\x9F", database_querydecomposition(0x00001F9F, UnicodeProperty_Titlecase));
}

TEST(QueryDecompositionTitlecase, Missing)
{
	EXPECT_EQ(nullptr, database_querydecomposition(0x0000ABED, UnicodeProperty_Titlecase));
}

TEST(QueryDecompositionTitlecase, MissingOutOfLowerBounds)
{
	EXPECT_EQ(nullptr, database_querydecomposition(0x0000001A, UnicodeProperty_Titlecase));
}

TEST(QueryDecompositionTitlecase, MissingOutOfUpperBounds)
{
	EXPECT_EQ(nullptr, database_querydecomposition(0x0002112A, UnicodeProperty_Titlecase));
}