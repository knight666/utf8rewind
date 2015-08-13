#include "tests-base.hpp"

extern "C" {
	#include "../internal/database.h"
}

#include "helpers-strings.hpp"

TEST(QueryDecomposition, ComposeProperty)
{
	size_t t;

	EXPECT_EQ(nullptr, database_querydecomposition(0x0000011A, UnicodeProperty_Normalization_Compose, &t));
	EXPECT_EQ(0, t);
}

TEST(QueryDecomposition, CompatibilityComposeProperty)
{
	size_t t;

	EXPECT_EQ(nullptr, database_querydecomposition(0x0000009B, UnicodeProperty_Normalization_Compatibility_Compose, &t));
	EXPECT_EQ(0, t);
}

TEST(QueryDecomposition, InvalidProperty)
{
	size_t t;

	EXPECT_EQ(nullptr, database_querydecomposition(0x0002F974, 33, &t));
	EXPECT_EQ(0, t);
}

TEST(QueryDecomposition, MissingLength)
{
	EXPECT_EQ(nullptr, database_querydecomposition(0x0000A757, UnicodeProperty_Titlecase, nullptr));
}

// Decomposed

TEST(QueryDecompositionDecomposed, Found)
{
	size_t t;

	EXPECT_UTF8EQ("\xD0\x98\xCC\x88", database_querydecomposition(0x000004E4, UnicodeProperty_Normalization_Decompose, &t));
	EXPECT_EQ(4, t);
}

TEST(QueryDecompositionDecomposed, FoundFirst)
{
	size_t t;

	EXPECT_UTF8EQ("A\xCC\x80", database_querydecomposition(0x000000C0, UnicodeProperty_Normalization_Decompose, &t));
	EXPECT_EQ(3, t);
}

TEST(QueryDecompositionDecomposed, FoundLast)
{
	size_t t;

	EXPECT_UTF8EQ("\xF0\xAA\x98\x80", database_querydecomposition(0x0002FA1D, UnicodeProperty_Normalization_Decompose, &t));
	EXPECT_EQ(4, t);
}

TEST(QueryDecompositionDecomposed, FoundPivot)
{
	size_t t;

	EXPECT_UTF8EQ("\xE7\x88\x9B", database_querydecomposition(0x0000F91E, UnicodeProperty_Normalization_Decompose, &t));
	EXPECT_EQ(3, t);
}

TEST(QueryDecompositionDecomposed, FoundPivotUp)
{
	size_t t;

	EXPECT_UTF8EQ("s\xCC\xA3", database_querydecomposition(0x00001E63, UnicodeProperty_Normalization_Decompose, &t));
	EXPECT_EQ(3, t);
}

TEST(QueryDecompositionDecomposed, FoundPivotDown)
{
	size_t t;

	EXPECT_UTF8EQ("\xE5\x86\xAC", database_querydecomposition(0x0002F81A, UnicodeProperty_Normalization_Decompose, &t));
	EXPECT_EQ(3, t);
}

TEST(QueryDecompositionDecomposed, FoundPivotDownDown)
{
	size_t t;

	EXPECT_UTF8EQ(";", database_querydecomposition(0x0000037E, UnicodeProperty_Normalization_Decompose, &t));
	EXPECT_EQ(1, t);
}

TEST(QueryDecompositionDecomposed, FoundMaxDepth)
{
	size_t t;

	EXPECT_UTF8EQ("\xE8\xBC\xB8", database_querydecomposition(0x0002F9DF, UnicodeProperty_Normalization_Decompose, &t));
	EXPECT_EQ(3, t);
}

TEST(QueryDecompositionDecomposed, Missing)
{
	size_t t;

	EXPECT_EQ(nullptr, database_querydecomposition(0x0001FFFF, UnicodeProperty_Normalization_Decompose, &t));
	EXPECT_EQ(0, t);
}

TEST(QueryDecompositionDecomposed, MissingOutOfLowerBounds)
{
	size_t t;

	EXPECT_EQ(nullptr, database_querydecomposition(0x00000067, UnicodeProperty_Normalization_Decompose, &t));
	EXPECT_EQ(0, t);
}

TEST(QueryDecompositionDecomposed, MissingOutOfUpperBounds)
{
	size_t t;

	EXPECT_EQ(nullptr, database_querydecomposition(0x0011A26D, UnicodeProperty_Normalization_Decompose, &t));
	EXPECT_EQ(0, t);
}

// Compatibility decomposed

TEST(QueryDecompositionCompatibilityDecomposed, Found)
{
	size_t t;

	EXPECT_UTF8EQ("\xD0\x98\xCC\x88", database_querydecomposition(0x000004E4, UnicodeProperty_Normalization_Compatibility_Decompose, &t));
	EXPECT_EQ(4, t);
}

TEST(QueryDecompositionCompatibilityDecomposed, FoundFirst)
{
	size_t t;

	EXPECT_UTF8EQ(" ", database_querydecomposition(0x000000A0, UnicodeProperty_Normalization_Compatibility_Decompose, &t));
	EXPECT_EQ(1, t);
}

TEST(QueryDecompositionCompatibilityDecomposed, FoundLast)
{
	size_t t;

	EXPECT_UTF8EQ("\xF0\xAA\x98\x80", database_querydecomposition(0x0002FA1D, UnicodeProperty_Normalization_Compatibility_Decompose, &t));
	EXPECT_EQ(4, t);
}

TEST(QueryDecompositionCompatibilityDecomposed, FoundPivot)
{
	size_t t;

	EXPECT_UTF8EQ("\xD7\x90\xD7\x9C", database_querydecomposition(0x0000FB4F, UnicodeProperty_Normalization_Compatibility_Decompose, &t));
	EXPECT_EQ(4, t);
}

TEST(QueryDecompositionCompatibilityDecomposed, FoundPivotUp)
{
	size_t t;

	EXPECT_UTF8EQ("w", database_querydecomposition(0x000024E6, UnicodeProperty_Normalization_Compatibility_Decompose, &t));
	EXPECT_EQ(1, t);
}

TEST(QueryDecompositionCompatibilityDecomposed, FoundPivotDown)
{
	size_t t;

	EXPECT_UTF8EQ("q", database_querydecomposition(0x0001D596, UnicodeProperty_Normalization_Compatibility_Decompose, &t));
	EXPECT_EQ(1, t);
}

TEST(QueryDecompositionCompatibilityDecomposed, FoundPivotDownUp)
{
	size_t t;

	EXPECT_UTF8EQ("\xE3\x83\x9B\xE3\x83\xBC\xE3\x83\xB3", database_querydecomposition(0x00003342, UnicodeProperty_Normalization_Compatibility_Decompose, &t));
	EXPECT_EQ(9, t);
}

TEST(QueryDecompositionCompatibilityDecomposed, FoundMaxDepth)
{
	size_t t;

	EXPECT_UTF8EQ("u", database_querydecomposition(0x0001D4CA, UnicodeProperty_Normalization_Compatibility_Decompose, &t));
	EXPECT_EQ(1, t);
}

TEST(QueryDecompositionCompatibilityDecomposed, Missing)
{
	size_t t;

	EXPECT_EQ(nullptr, database_querydecomposition(0x0001A2AF, UnicodeProperty_Normalization_Compatibility_Decompose, &t));
	EXPECT_EQ(0, t);
}

TEST(QueryDecompositionCompatibilityDecomposed, MissingOutOfLowerBounds)
{
	size_t t;

	EXPECT_EQ(nullptr, database_querydecomposition(0x00000023, UnicodeProperty_Normalization_Compatibility_Decompose, &t));
	EXPECT_EQ(0, t);
}

TEST(QueryDecompositionCompatibilityDecomposed, MissingOutOfUpperBounds)
{
	size_t t;

	EXPECT_EQ(nullptr, database_querydecomposition(0x00DD2A5D, UnicodeProperty_Normalization_Compatibility_Decompose, &t));
	EXPECT_EQ(0, t);
}

// Uppercase

TEST(QueryDecompositionUppercase, Found)
{
	size_t t;

	EXPECT_UTF8EQ("\xE1\xB8\x8A", database_querydecomposition(0x00001E0B, UnicodeProperty_Uppercase, &t));
	EXPECT_EQ(3, t);
}

TEST(QueryDecompositionUppercase, FoundFirst)
{
	size_t t;

	EXPECT_UTF8EQ("\xCE\x9C", database_querydecomposition(0x000000B5, UnicodeProperty_Uppercase, &t));
	EXPECT_EQ(2, t);
}

TEST(QueryDecompositionUppercase, FoundLast)
{
	size_t t;

	EXPECT_UTF8EQ("\xF0\x91\xA2\xBF", database_querydecomposition(0x000118DF, UnicodeProperty_Uppercase, &t));
	EXPECT_EQ(4, t);
}

TEST(QueryDecompositionUppercase, FoundPivot)
{
	size_t t;

	EXPECT_UTF8EQ("\xE1\xBB\x84", database_querydecomposition(0x00001EC5, UnicodeProperty_Uppercase, &t));
	EXPECT_EQ(3, t);
}

TEST(QueryDecompositionUppercase, FoundPivotUp)
{
	size_t t;

	EXPECT_UTF8EQ("\xD0\x91", database_querydecomposition(0x00000431, UnicodeProperty_Uppercase, &t));
	EXPECT_EQ(2, t);
}

TEST(QueryDecompositionUppercase, FoundPivotDown)
{
	size_t t;

	EXPECT_UTF8EQ("\xE1\xBD\xAF\xCE\x99", database_querydecomposition(0x00001FAF, UnicodeProperty_Uppercase, &t));
	EXPECT_EQ(5, t);
}

TEST(QueryDecompositionUppercase, FoundPivotDownUp)
{
	size_t t;

	EXPECT_UTF8EQ("\xE2\x93\x88", database_querydecomposition(0x000024E2, UnicodeProperty_Uppercase, &t));
	EXPECT_EQ(3, t);
}

TEST(QueryDecompositionUppercase, FoundMaxDepth)
{
	size_t t;

	EXPECT_UTF8EQ("\xE2\x92\xBD", database_querydecomposition(0x000024D7, UnicodeProperty_Uppercase, &t));
	EXPECT_EQ(3, t);
}

TEST(QueryDecompositionUppercase, Missing)
{
	size_t t;

	EXPECT_EQ(nullptr, database_querydecomposition(0x00002BAD, UnicodeProperty_Uppercase, &t));
	EXPECT_EQ(0, t);
}

TEST(QueryDecompositionUppercase, MissingOutOfLowerBounds)
{
	size_t t;

	EXPECT_EQ(nullptr, database_querydecomposition(0x00000020, UnicodeProperty_Uppercase, &t));
	EXPECT_EQ(0, t);
}

TEST(QueryDecompositionUppercase, MissingOutOfUpperBounds)
{
	size_t t;

	EXPECT_EQ(nullptr, database_querydecomposition(0x00101111, UnicodeProperty_Uppercase, &t));
	EXPECT_EQ(0, t);
}

// Lowercase

TEST(QueryDecompositionLowercase, Found)
{
	size_t t;

	EXPECT_UTF8EQ("\xCE\xB3", database_querydecomposition(0x00000393, UnicodeProperty_Lowercase, &t));
	EXPECT_EQ(2, t);
}

TEST(QueryDecompositionLowercase, FoundFirst)
{
	size_t t;

	EXPECT_UTF8EQ("\xC3\xA0", database_querydecomposition(0x000000C0, UnicodeProperty_Lowercase, &t));
	EXPECT_EQ(2, t);
}

TEST(QueryDecompositionLowercase, FoundLast)
{
	size_t t;

	EXPECT_UTF8EQ("\xF0\x91\xA3\x9F", database_querydecomposition(0x000118BF, UnicodeProperty_Lowercase, &t));
	EXPECT_EQ(4, t);
}

TEST(QueryDecompositionLowercase, FoundPivot)
{
	size_t t;

	EXPECT_UTF8EQ("\xE1\xBA\xA1", database_querydecomposition(0x00001EA0, UnicodeProperty_Lowercase, &t));
	EXPECT_EQ(3, t);
}

TEST(QueryDecompositionLowercase, FoundPivotUp)
{
	size_t t;

	EXPECT_UTF8EQ("\xD0\xB7", database_querydecomposition(0x00000417, UnicodeProperty_Lowercase, &t));
	EXPECT_EQ(2, t);
}

TEST(QueryDecompositionLowercase, FoundPivotDown)
{
	size_t t;

	EXPECT_UTF8EQ("\xE2\xB1\x9A", database_querydecomposition(0x00002C2A, UnicodeProperty_Lowercase, &t));
	EXPECT_EQ(3, t);
}

TEST(QueryDecompositionLowercase, FoundPivotDownUp)
{
	size_t t;

	EXPECT_UTF8EQ("\xE1\xBE\xA2", database_querydecomposition(0x00001FAA, UnicodeProperty_Lowercase, &t));
	EXPECT_EQ(3, t);
}

TEST(QueryDecompositionLowercase, FoundMaxDepth)
{
	size_t t;

	EXPECT_UTF8EQ("\xEA\x9C\xA9", database_querydecomposition(0x0000A728, UnicodeProperty_Lowercase, &t));
	EXPECT_EQ(3, t);
}

TEST(QueryDecompositionLowercase, Missing)
{
	size_t t;

	EXPECT_EQ(nullptr, database_querydecomposition(0x0000F3AA, UnicodeProperty_Lowercase, &t));
	EXPECT_EQ(0, t);
}

TEST(QueryDecompositionLowercase, MissingOutOfLowerBounds)
{
	size_t t;

	EXPECT_EQ(nullptr, database_querydecomposition(0x00000021, UnicodeProperty_Lowercase, &t));
	EXPECT_EQ(0, t);
}

TEST(QueryDecompositionLowercase, MissingOutOfUpperBounds)
{
	size_t t;

	EXPECT_EQ(nullptr, database_querydecomposition(0x00012000, UnicodeProperty_Lowercase, &t));
	EXPECT_EQ(0, t);
}

// Titlecase

TEST(QueryDecompositionTitlecase, Found)
{
	size_t t;

	EXPECT_UTF8EQ("\xD0\x80", database_querydecomposition(0x00000450, UnicodeProperty_Titlecase, &t));
	EXPECT_EQ(2, t);
}

TEST(QueryDecompositionTitlecase, FoundFirst)
{
	size_t t;

	EXPECT_UTF8EQ("\xCE\x9C", database_querydecomposition(0x000000B5, UnicodeProperty_Titlecase, &t));
	EXPECT_EQ(2, t);
}

TEST(QueryDecompositionTitlecase, FoundLast)
{
	size_t t;

	EXPECT_UTF8EQ("\xF0\x91\xA2\xBF", database_querydecomposition(0x000118DF, UnicodeProperty_Titlecase, &t));
	EXPECT_EQ(4, t);
}

TEST(QueryDecompositionTitlecase, FoundPivot)
{
	size_t t;

	EXPECT_UTF8EQ("\xE1\xBB\x80", database_querydecomposition(0x00001EC1, UnicodeProperty_Titlecase, &t));
	EXPECT_EQ(3, t);
}

TEST(QueryDecompositionTitlecase, FoundPivotUp)
{
	size_t t;

	EXPECT_UTF8EQ("\xCF\xB7", database_querydecomposition(0x000003F8, UnicodeProperty_Titlecase, &t));
	EXPECT_EQ(2, t);
}

TEST(QueryDecompositionTitlecase, FoundPivotDown)
{
	size_t t;

	EXPECT_UTF8EQ("\xE2\xB2\x90", database_querydecomposition(0x00002C91, UnicodeProperty_Titlecase, &t));
	EXPECT_EQ(3, t);
}

TEST(QueryDecompositionTitlecase, FoundPivotDownUp)
{
	size_t t;

	EXPECT_UTF8EQ("\xE1\xBE\xAE", database_querydecomposition(0x00001FAE, UnicodeProperty_Titlecase, &t));
	EXPECT_EQ(3, t);
}

TEST(QueryDecompositionTitlecase, FoundMaxDepth)
{
	size_t t;

	EXPECT_UTF8EQ("\xE1\xBE\x9F", database_querydecomposition(0x00001F9F, UnicodeProperty_Titlecase, &t));
	EXPECT_EQ(3, t);
}

TEST(QueryDecompositionTitlecase, Missing)
{
	size_t t;

	EXPECT_EQ(nullptr, database_querydecomposition(0x0000ABED, UnicodeProperty_Titlecase, &t));
	EXPECT_EQ(0, t);
}

TEST(QueryDecompositionTitlecase, MissingOutOfLowerBounds)
{
	size_t t;

	EXPECT_EQ(nullptr, database_querydecomposition(0x0000001A, UnicodeProperty_Titlecase, &t));
	EXPECT_EQ(0, t);
}

TEST(QueryDecompositionTitlecase, MissingOutOfUpperBounds)
{
	size_t t;

	EXPECT_EQ(nullptr, database_querydecomposition(0x0002112A, UnicodeProperty_Titlecase, &t));
	EXPECT_EQ(0, t);
}