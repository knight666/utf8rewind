#include "tests-base.hpp"

extern "C" {
	#include "../internal/database.h"
}

TEST(QueryProperty, InvalidProperty)
{
	EXPECT_EQ(UTF8_INVALID_PROPERTY, database_queryproperty(0x00C0, 198));
}

// Composed

TEST(QueryPropertyComposed, FoundNo)
{
	EXPECT_EQ(QuickCheckResult_No, database_queryproperty(0x1FEE, UnicodeProperty_Normalization_Compose));
}

TEST(QueryPropertyComposed, FoundMaybe)
{
	EXPECT_EQ(QuickCheckResult_Maybe, database_queryproperty(0x0DCF, UnicodeProperty_Normalization_Compose));
}

TEST(QueryPropertyComposed, FoundFirst)
{
	EXPECT_EQ(QuickCheckResult_Maybe, database_queryproperty(0x0300, UnicodeProperty_Normalization_Compose));
}

TEST(QueryPropertyComposed, FoundLast)
{
	EXPECT_EQ(QuickCheckResult_No, database_queryproperty(0x2FA1D, UnicodeProperty_Normalization_Compose));
}

TEST(QueryPropertyComposed, OutOfLowerBounds)
{
	EXPECT_EQ(QuickCheckResult_Yes, database_queryproperty(0x00FF, UnicodeProperty_Normalization_Compose));
}

TEST(QueryPropertyComposed, OutOfUpperBounds)
{
	EXPECT_EQ(QuickCheckResult_Yes, database_queryproperty(0x30000, UnicodeProperty_Normalization_Compose));
}

TEST(QueryPropertyComposed, BetweenBlocks)
{
	EXPECT_EQ(QuickCheckResult_Yes, database_queryproperty(0x2B81, UnicodeProperty_Normalization_Compose));
}

// Decomposed

TEST(QueryPropertyDecomposed, FoundNo)
{
	EXPECT_EQ(QuickCheckResult_No, database_queryproperty(0x0374, UnicodeProperty_Normalization_Decompose));
}

TEST(QueryPropertyDecomposed, FoundFirst)
{
	EXPECT_EQ(QuickCheckResult_No, database_queryproperty(0x00C0, UnicodeProperty_Normalization_Decompose));
}

TEST(QueryPropertyDecomposed, FoundLast)
{
	EXPECT_EQ(QuickCheckResult_No, database_queryproperty(0x2FA1D, UnicodeProperty_Normalization_Decompose));
}

TEST(QueryPropertyDecomposed, OutOfLowerBounds)
{
	EXPECT_EQ(QuickCheckResult_Yes, database_queryproperty(0x007F, UnicodeProperty_Normalization_Decompose));
}

TEST(QueryPropertyDecomposed, OutOfUpperBounds)
{
	EXPECT_EQ(QuickCheckResult_Yes, database_queryproperty(0x30000, UnicodeProperty_Normalization_Decompose));
}

TEST(QueryPropertyDecomposed, BetweenBlocks)
{
	EXPECT_EQ(QuickCheckResult_Yes, database_queryproperty(0xFB20, UnicodeProperty_Normalization_Decompose));
}

TEST(QueryPropertyDecomposed, HangulSyllable)
{
	EXPECT_EQ(QuickCheckResult_No, database_queryproperty(0xB1A1, UnicodeProperty_Normalization_Decompose));
}

TEST(QueryPropertyDecomposed, HangulSyllableStart)
{
	EXPECT_EQ(QuickCheckResult_No, database_queryproperty(0xAC00, UnicodeProperty_Normalization_Decompose));
}

TEST(QueryPropertyDecomposed, HangulSyllableEnd)
{
	EXPECT_EQ(QuickCheckResult_No, database_queryproperty(0xD7A3, UnicodeProperty_Normalization_Decompose));
}

// Compatibility composed

TEST(QueryPropertyCompatibilityComposed, FoundNo)
{
	EXPECT_EQ(QuickCheckResult_No, database_queryproperty(0x0A5B, UnicodeProperty_Normalization_Compatibility_Compose));
}

TEST(QueryPropertyCompatibilityComposed, FoundMaybe)
{
	EXPECT_EQ(QuickCheckResult_Maybe, database_queryproperty(0x0B57, UnicodeProperty_Normalization_Compatibility_Compose));
}

TEST(QueryPropertyCompatibilityComposed, FoundFirst)
{
	EXPECT_EQ(QuickCheckResult_No, database_queryproperty(0x00A0, UnicodeProperty_Normalization_Compatibility_Compose));
}

TEST(QueryPropertyCompatibilityComposed, FoundLast)
{
	EXPECT_EQ(QuickCheckResult_No, database_queryproperty(0x2FA1D, UnicodeProperty_Normalization_Compatibility_Compose));
}

TEST(QueryPropertyCompatibilityComposed, OutOfLowerBounds)
{
	EXPECT_EQ(QuickCheckResult_Yes, database_queryproperty(0x007F, UnicodeProperty_Normalization_Compatibility_Compose));
}

TEST(QueryPropertyCompatibilityComposed, OutOfUpperBounds)
{
	EXPECT_EQ(QuickCheckResult_Yes, database_queryproperty(0x30000, UnicodeProperty_Normalization_Compatibility_Compose));
}

TEST(QueryPropertyCompatibilityComposed, BetweenBlocks)
{
	EXPECT_EQ(QuickCheckResult_Yes, database_queryproperty(0x1EE5C, UnicodeProperty_Normalization_Compatibility_Compose));
}

// Compatibility decomposed

TEST(QueryPropertyCompatibilityDecomposed, FoundNo)
{
	EXPECT_EQ(QuickCheckResult_No, database_queryproperty(0x03F9, UnicodeProperty_Normalization_Compatibility_Decompose));
}

TEST(QueryPropertyCompatibilityDecomposed, FoundFirst)
{
	EXPECT_EQ(QuickCheckResult_No, database_queryproperty(0x00A0, UnicodeProperty_Normalization_Compatibility_Decompose));
}

TEST(QueryPropertyCompatibilityDecomposed, FoundLast)
{
	EXPECT_EQ(QuickCheckResult_No, database_queryproperty(0x2FA1D, UnicodeProperty_Normalization_Compatibility_Decompose));
}

TEST(QueryPropertyCompatibilityDecomposed, OutOfLowerBounds)
{
	EXPECT_EQ(QuickCheckResult_Yes, database_queryproperty(0x007F, UnicodeProperty_Normalization_Compatibility_Decompose));
}

TEST(QueryPropertyCompatibilityDecomposed, OutOfUpperBounds)
{
	EXPECT_EQ(QuickCheckResult_Yes, database_queryproperty(0x30000, UnicodeProperty_Normalization_Compatibility_Decompose));
}

TEST(QueryPropertyCompatibilityDecomposed, BetweenBlocks)
{
	EXPECT_EQ(QuickCheckResult_Yes, database_queryproperty(0x038D, UnicodeProperty_Normalization_Compatibility_Decompose));
}

TEST(QueryPropertyCompatibilityDecomposed, HangulSyllable)
{
	EXPECT_EQ(QuickCheckResult_No, database_queryproperty(0xCCCA, UnicodeProperty_Normalization_Compatibility_Decompose));
}

TEST(QueryPropertyCompatibilityDecomposed, HangulSyllableStart)
{
	EXPECT_EQ(QuickCheckResult_No, database_queryproperty(0xAC00, UnicodeProperty_Normalization_Compatibility_Decompose));
}

TEST(QueryPropertyCompatibilityDecomposed, HangulSyllableEnd)
{
	EXPECT_EQ(QuickCheckResult_No, database_queryproperty(0xD7A3, UnicodeProperty_Normalization_Compatibility_Decompose));
}

// Uppercase

TEST(QueryPropertyUppercase, Found)
{
	EXPECT_EQ(1, database_queryproperty(0x0101, UnicodeProperty_Uppercase));
}

TEST(QueryPropertyUppercase, FoundFirst)
{
	EXPECT_EQ(1, database_queryproperty(0x0061, UnicodeProperty_Uppercase));
}

TEST(QueryPropertyUppercase, FoundLast)
{
	EXPECT_EQ(1, database_queryproperty(0x118DF, UnicodeProperty_Uppercase));
}

TEST(QueryPropertyUppercase, OutOfLowerBounds)
{
	EXPECT_EQ(0, database_queryproperty(0x0020, UnicodeProperty_Uppercase));
}

TEST(QueryPropertyUppercase, OutOfUpperBounds)
{
	EXPECT_EQ(0, database_queryproperty(0x0020, UnicodeProperty_Uppercase));
}

TEST(QueryPropertyUppercase, BetweenBlocks)
{
	EXPECT_EQ(0, database_queryproperty(0x2C5F, UnicodeProperty_Uppercase));
}

// Lowercase

TEST(QueryPropertyLowercase, Found)
{
	EXPECT_EQ(1, database_queryproperty(0x01B5, UnicodeProperty_Lowercase));
}

TEST(QueryPropertyLowercase, FoundFirst)
{
	EXPECT_EQ(1, database_queryproperty(0x0041, UnicodeProperty_Lowercase));
}

TEST(QueryPropertyLowercase, FoundLast)
{
	EXPECT_EQ(1, database_queryproperty(0x118Bf, UnicodeProperty_Lowercase));
}

TEST(QueryPropertyLowercase, OutOfLowerBounds)
{
	EXPECT_EQ(0, database_queryproperty(0x002F, UnicodeProperty_Lowercase));
}

TEST(QueryPropertyLowercase, OutOfUpperBounds)
{
	EXPECT_EQ(0, database_queryproperty(0x121F4, UnicodeProperty_Lowercase));
}

TEST(QueryPropertyLowercase, BetweenBlocks)
{
	EXPECT_EQ(0, database_queryproperty(0x2180, UnicodeProperty_Lowercase));
}

// Titlecase

TEST(QueryPropertyTitlecase, Found)
{
	EXPECT_EQ(1, database_queryproperty(0x028B, UnicodeProperty_Titlecase));
}

TEST(QueryPropertyTitlecase, FoundFirst)
{
	EXPECT_EQ(1, database_queryproperty(0x0061, UnicodeProperty_Titlecase));
}

TEST(QueryPropertyTitlecase, FoundLast)
{
	EXPECT_EQ(1, database_queryproperty(0x118DF, UnicodeProperty_Titlecase));
}

TEST(QueryPropertyTitlecase, OutOfLowerBounds)
{
	EXPECT_EQ(0, database_queryproperty(0x0018, UnicodeProperty_Titlecase));
}

TEST(QueryPropertyTitlecase, OutOfUpperBounds)
{
	EXPECT_EQ(0, database_queryproperty(0x1D20FF, UnicodeProperty_Titlecase));
}

TEST(QueryPropertyTitlecase, BetweenBlocks)
{
	EXPECT_EQ(0, database_queryproperty(0x2180, UnicodeProperty_Titlecase));
}