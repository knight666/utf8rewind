#include "tests-base.hpp"

extern "C" {
	#include "../normalization.h"
}

TEST(QuickCheck, NoResult)
{
	EXPECT_EQ(0, quickcheck(0x81288, QuickCheck_Normalize_Composed));
}

TEST(QuickCheck, InvalidNormalizationForm)
{
	EXPECT_EQ(QuickCheckResult_Invalid, quickcheck(0x00C0, 198));
}

// Composed

TEST(QuickCheck, ComposedFoundNo)
{
	EXPECT_EQ(QuickCheckResult_No, quickcheck(0x1FEE, QuickCheck_Normalize_Composed));
}

TEST(QuickCheck, ComposedFoundMaybe)
{
	EXPECT_EQ(QuickCheckResult_Maybe, quickcheck(0x0DCF, QuickCheck_Normalize_Composed));
}

TEST(QuickCheck, ComposedFoundFirst)
{
	EXPECT_EQ(QuickCheckResult_Maybe, quickcheck(0x0300, QuickCheck_Normalize_Composed));
}

TEST(QuickCheck, ComposedFoundLast)
{
	EXPECT_EQ(QuickCheckResult_No, quickcheck(0x2FA1D, QuickCheck_Normalize_Composed));
}

TEST(QuickCheck, ComposedOutOfLowerBounds)
{
	EXPECT_EQ(QuickCheckResult_Yes, quickcheck(0x00FF, QuickCheck_Normalize_Composed));
}

TEST(QuickCheck, ComposedOutOfUpperBounds)
{
	EXPECT_EQ(QuickCheckResult_Yes, quickcheck(0x30000, QuickCheck_Normalize_Composed));
}

TEST(QuickCheck, ComposedBetweenBlocks)
{
	EXPECT_EQ(QuickCheckResult_Yes, quickcheck(0x2B81, QuickCheck_Normalize_Composed));
}

// Decomposed

TEST(QuickCheck, DecomposedFoundNo)
{
	EXPECT_EQ(QuickCheckResult_No, quickcheck(0x0374, QuickCheck_Normalize_Decomposed));
}

TEST(QuickCheck, DecomposedFoundFirst)
{
	EXPECT_EQ(QuickCheckResult_No, quickcheck(0x00C0, QuickCheck_Normalize_Decomposed));
}

TEST(QuickCheck, DecomposedFoundLast)
{
	EXPECT_EQ(QuickCheckResult_No, quickcheck(0x2FA1D, QuickCheck_Normalize_Decomposed));
}

TEST(QuickCheck, DecomposedOutOfLowerBounds)
{
	EXPECT_EQ(QuickCheckResult_Yes, quickcheck(0x007F, QuickCheck_Normalize_Decomposed));
}

TEST(QuickCheck, DecomposedOutOfUpperBounds)
{
	EXPECT_EQ(QuickCheckResult_Yes, quickcheck(0x30000, QuickCheck_Normalize_Decomposed));
}

TEST(QuickCheck, DecomposedBetweenBlocks)
{
	EXPECT_EQ(QuickCheckResult_Yes, quickcheck(0xFB20, QuickCheck_Normalize_Decomposed));
}

TEST(QuickCheck, DecomposedHangulSyllable)
{
	EXPECT_EQ(QuickCheckResult_No, quickcheck(0xB1A1, QuickCheck_Normalize_Decomposed));
}

TEST(QuickCheck, DecomposedHangulSyllableStart)
{
	EXPECT_EQ(QuickCheckResult_No, quickcheck(0xAC00, QuickCheck_Normalize_Decomposed));
}

TEST(QuickCheck, DecomposedHangulSyllableEnd)
{
	EXPECT_EQ(QuickCheckResult_No, quickcheck(0xD7A3, QuickCheck_Normalize_Decomposed));
}

// Compatibility composed

TEST(QuickCheck, CompatibilityComposedFoundNo)
{
	EXPECT_EQ(QuickCheckResult_No, quickcheck(0x0A5B, QuickCheck_Normalize_Compatibility_Composed));
}

TEST(QuickCheck, CompatibilityComposedFoundMaybe)
{
	EXPECT_EQ(QuickCheckResult_Maybe, quickcheck(0x0B57, QuickCheck_Normalize_Compatibility_Composed));
}

TEST(QuickCheck, CompatibilityComposedFoundFirst)
{
	EXPECT_EQ(QuickCheckResult_No, quickcheck(0x00A0, QuickCheck_Normalize_Compatibility_Composed));
}

TEST(QuickCheck, CompatibilityComposedFoundLast)
{
	EXPECT_EQ(QuickCheckResult_No, quickcheck(0x2FA1D, QuickCheck_Normalize_Compatibility_Composed));
}

TEST(QuickCheck, CompatibilityComposedOutOfLowerBounds)
{
	EXPECT_EQ(QuickCheckResult_Yes, quickcheck(0x007F, QuickCheck_Normalize_Compatibility_Composed));
}

TEST(QuickCheck, CompatibilityComposedOutOfUpperBounds)
{
	EXPECT_EQ(QuickCheckResult_Yes, quickcheck(0x30000, QuickCheck_Normalize_Compatibility_Composed));
}

TEST(QuickCheck, CompatibilityComposedBetweenBlocks)
{
	EXPECT_EQ(QuickCheckResult_Yes, quickcheck(0x1EE5C, QuickCheck_Normalize_Compatibility_Composed));
}

// Compatibility decomposed

TEST(QuickCheck, CompatibilityDecomposedFoundNo)
{
	EXPECT_EQ(QuickCheckResult_No, quickcheck(0x03F9, QuickCheck_Normalize_Compatibility_Decomposed));
}

TEST(QuickCheck, CompatibilityDecomposedFoundFirst)
{
	EXPECT_EQ(QuickCheckResult_No, quickcheck(0x00A0, QuickCheck_Normalize_Compatibility_Decomposed));
}

TEST(QuickCheck, CompatibilityDecomposedFoundLast)
{
	EXPECT_EQ(QuickCheckResult_No, quickcheck(0x2FA1D, QuickCheck_Normalize_Compatibility_Decomposed));
}

TEST(QuickCheck, CompatibilityDecomposedOutOfLowerBounds)
{
	EXPECT_EQ(QuickCheckResult_Yes, quickcheck(0x007F, QuickCheck_Normalize_Compatibility_Decomposed));
}

TEST(QuickCheck, CompatibilityDecomposedOutOfUpperBounds)
{
	EXPECT_EQ(QuickCheckResult_Yes, quickcheck(0x30000, QuickCheck_Normalize_Compatibility_Decomposed));
}

TEST(QuickCheck, CompatibilityDecomposedBetweenBlocks)
{
	EXPECT_EQ(QuickCheckResult_Yes, quickcheck(0x038D, QuickCheck_Normalize_Compatibility_Decomposed));
}

TEST(QuickCheck, CompatibilityDecomposedHangulSyllable)
{
	EXPECT_EQ(QuickCheckResult_No, quickcheck(0xCCCA, QuickCheck_Normalize_Compatibility_Decomposed));
}

TEST(QuickCheck, CompatibilityDecomposedHangulSyllableStart)
{
	EXPECT_EQ(QuickCheckResult_No, quickcheck(0xAC00, QuickCheck_Normalize_Compatibility_Decomposed));
}

TEST(QuickCheck, CompatibilityDecomposedHangulSyllableEnd)
{
	EXPECT_EQ(QuickCheckResult_No, quickcheck(0xD7A3, QuickCheck_Normalize_Compatibility_Decomposed));
}

// Uppercase

TEST(QuickCheck, UppercaseFound)
{
	EXPECT_EQ(1, quickcheck(0x0101, QuickCheck_Uppercase));
}

TEST(QuickCheck, UppercaseFoundFirst)
{
	EXPECT_EQ(1, quickcheck(0x0061, QuickCheck_Uppercase));
}

TEST(QuickCheck, UppercaseFoundLast)
{
	EXPECT_EQ(1, quickcheck(0x118DF, QuickCheck_Uppercase));
}

TEST(QuickCheck, UppercaseOutOfLowerBounds)
{
	EXPECT_EQ(0, quickcheck(0x0020, QuickCheck_Uppercase));
}

TEST(QuickCheck, UppercaseOutOfUpperBounds)
{
	EXPECT_EQ(0, quickcheck(0x0020, QuickCheck_Uppercase));
}

TEST(QuickCheck, UppercaseBetweenBlocks)
{
	EXPECT_EQ(0, quickcheck(0x2C5F, QuickCheck_Uppercase));
}

// Lowercase

TEST(QuickCheck, LowercaseFound)
{
	EXPECT_EQ(1, quickcheck(0x01B5, QuickCheck_Lowercase));
}

TEST(QuickCheck, LowercaseFoundFirst)
{
	EXPECT_EQ(1, quickcheck(0x0041, QuickCheck_Lowercase));
}

TEST(QuickCheck, LowercaseFoundLast)
{
	EXPECT_EQ(1, quickcheck(0x118Bf, QuickCheck_Lowercase));
}

TEST(QuickCheck, LowercaseOutOfLowerBounds)
{
	EXPECT_EQ(0, quickcheck(0x002F, QuickCheck_Lowercase));
}

TEST(QuickCheck, LowercaseOutOfUpperBounds)
{
	EXPECT_EQ(0, quickcheck(0x121F4, QuickCheck_Lowercase));
}

TEST(QuickCheck, LowercaseBetweenBlocks)
{
	EXPECT_EQ(0, quickcheck(0x2180, QuickCheck_Lowercase));
}

// Titlecase

TEST(QuickCheck, TitlecaseFound)
{
	EXPECT_EQ(1, quickcheck(0x028B, QuickCheck_Titlecase));
}

TEST(QuickCheck, TitlecaseFoundFirst)
{
	EXPECT_EQ(1, quickcheck(0x0061, QuickCheck_Titlecase));
}

TEST(QuickCheck, TitlecaseFoundLast)
{
	EXPECT_EQ(1, quickcheck(0x118DF, QuickCheck_Titlecase));
}

TEST(QuickCheck, TitlecaseOutOfLowerBounds)
{
	EXPECT_EQ(0, quickcheck(0x0018, QuickCheck_Titlecase));
}

TEST(QuickCheck, TitlecaseOutOfUpperBounds)
{
	EXPECT_EQ(0, quickcheck(0x1D20FF, QuickCheck_Titlecase));
}

TEST(QuickCheck, TitlecaseBetweenBlocks)
{
	EXPECT_EQ(0, quickcheck(0x2180, QuickCheck_Titlecase));
}