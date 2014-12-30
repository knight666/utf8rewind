#include "tests-base.hpp"

extern "C" {
	#include "../normalization.h"
}

TEST(QuickCheck, NoResult)
{
	uint8_t r = quickcheck(0x81288, QuickCheck_Normalize_Composed);

	EXPECT_EQ(QuickCheckResult_Yes, r);
}

TEST(QuickCheck, InvalidNormalizationForm)
{
	uint8_t r = quickcheck(0xC0, 198);

	EXPECT_EQ(QuickCheckResult_Invalid, r);
}

// Composed

TEST(QuickCheck, ComposedFoundNo)
{
	uint8_t r = quickcheck(0x1FEE, QuickCheck_Normalize_Composed);

	EXPECT_EQ(QuickCheckResult_No, r);
}

TEST(QuickCheck, ComposedFoundMaybe)
{
	uint8_t r = quickcheck(0x0DCF, QuickCheck_Normalize_Composed);

	EXPECT_EQ(QuickCheckResult_Maybe, r);
}

TEST(QuickCheck, ComposedFoundFirst)
{
	uint8_t r = quickcheck(0x0300, QuickCheck_Normalize_Composed);

	EXPECT_EQ(QuickCheckResult_Maybe, r);
}

TEST(QuickCheck, ComposedFoundLast)
{
	uint8_t r = quickcheck(0x2FA1D, QuickCheck_Normalize_Composed);

	EXPECT_EQ(QuickCheckResult_No, r);
}

TEST(QuickCheck, ComposedOutOfLowerBounds)
{
	uint8_t r = quickcheck(0x00FF, QuickCheck_Normalize_Composed);

	EXPECT_EQ(QuickCheckResult_Yes, r);
}

TEST(QuickCheck, ComposedOutOfUpperBounds)
{
	uint8_t r = quickcheck(0x30000, QuickCheck_Normalize_Composed);

	EXPECT_EQ(QuickCheckResult_Yes, r);
}

TEST(QuickCheck, ComposedBetweenBlocks)
{
	uint8_t r = quickcheck(0x2B81, QuickCheck_Normalize_Composed);

	EXPECT_EQ(QuickCheckResult_Yes, r);
}

// Decomposed

TEST(QuickCheck, DecomposedFoundNo)
{
	uint8_t r = quickcheck(0x0374, QuickCheck_Normalize_Decomposed);

	EXPECT_EQ(QuickCheckResult_No, r);
}

TEST(QuickCheck, DecomposedFoundFirst)
{
	uint8_t r = quickcheck(0x00C0, QuickCheck_Normalize_Decomposed);

	EXPECT_EQ(QuickCheckResult_No, r);
}

TEST(QuickCheck, DecomposedFoundLast)
{
	uint8_t r = quickcheck(0x2FA1D, QuickCheck_Normalize_Decomposed);

	EXPECT_EQ(QuickCheckResult_No, r);
}

TEST(QuickCheck, DecomposedOutOfLowerBounds)
{
	uint8_t r = quickcheck(0x007F, QuickCheck_Normalize_Decomposed);

	EXPECT_EQ(QuickCheckResult_Yes, r);
}

TEST(QuickCheck, DecomposedOutOfUpperBounds)
{
	uint8_t r = quickcheck(0x30000, QuickCheck_Normalize_Decomposed);

	EXPECT_EQ(QuickCheckResult_Yes, r);
}

TEST(QuickCheck, DecomposedBetweenBlocks)
{
	uint8_t r = quickcheck(0xFB20, QuickCheck_Normalize_Decomposed);

	EXPECT_EQ(QuickCheckResult_Yes, r);
}

TEST(QuickCheck, DecomposedHangulSyllable)
{
	uint8_t r = quickcheck(0xB1A1, QuickCheck_Normalize_Decomposed);

	EXPECT_EQ(QuickCheckResult_No, r);
}

TEST(QuickCheck, DecomposedHangulSyllableStart)
{
	uint8_t r = quickcheck(0xAC00, QuickCheck_Normalize_Decomposed);

	EXPECT_EQ(QuickCheckResult_No, r);
}

TEST(QuickCheck, DecomposedHangulSyllableEnd)
{
	uint8_t r = quickcheck(0xD7A3, QuickCheck_Normalize_Decomposed);

	EXPECT_EQ(QuickCheckResult_No, r);
}

// Compatibility composed

TEST(QuickCheck, CompatibilityComposedFoundNo)
{
	uint8_t r = quickcheck(0x0A5B, QuickCheck_Normalize_Compatibility_Composed);

	EXPECT_EQ(QuickCheckResult_No, r);
}

TEST(QuickCheck, CompatibilityComposedFoundMaybe)
{
	uint8_t r = quickcheck(0x0B57, QuickCheck_Normalize_Compatibility_Composed);

	EXPECT_EQ(QuickCheckResult_Maybe, r);
}

TEST(QuickCheck, CompatibilityComposedFoundFirst)
{
	uint8_t r = quickcheck(0x00A0, QuickCheck_Normalize_Compatibility_Composed);

	EXPECT_EQ(QuickCheckResult_No, r);
}

TEST(QuickCheck, CompatibilityComposedFoundLast)
{
	uint8_t r = quickcheck(0x2FA1D, QuickCheck_Normalize_Compatibility_Composed);

	EXPECT_EQ(QuickCheckResult_No, r);
}

TEST(QuickCheck, CompatibilityComposedOutOfLowerBounds)
{
	uint8_t r = quickcheck(0x007F, QuickCheck_Normalize_Compatibility_Composed);

	EXPECT_EQ(QuickCheckResult_Yes, r);
}

TEST(QuickCheck, CompatibilityComposedOutOfUpperBounds)
{
	uint8_t r = quickcheck(0x30000, QuickCheck_Normalize_Compatibility_Composed);

	EXPECT_EQ(QuickCheckResult_Yes, r);
}

TEST(QuickCheck, CompatibilityComposedBetweenBlocks)
{
	uint8_t r = quickcheck(0x1EE5C, QuickCheck_Normalize_Compatibility_Composed);

	EXPECT_EQ(QuickCheckResult_Yes, r);
}

// Compatibility decomposed

TEST(QuickCheck, CompatibilityDecomposedFoundNo)
{
	uint8_t r = quickcheck(0x03F9, QuickCheck_Normalize_Compatibility_Decomposed);

	EXPECT_EQ(QuickCheckResult_No, r);
}

TEST(QuickCheck, CompatibilityDecomposedFoundFirst)
{
	uint8_t r = quickcheck(0x00A0, QuickCheck_Normalize_Compatibility_Decomposed);

	EXPECT_EQ(QuickCheckResult_No, r);
}

TEST(QuickCheck, CompatibilityDecomposedFoundLast)
{
	uint8_t r = quickcheck(0x2FA1D, QuickCheck_Normalize_Compatibility_Decomposed);

	EXPECT_EQ(QuickCheckResult_No, r);
}

TEST(QuickCheck, CompatibilityDecomposedOutOfLowerBounds)
{
	uint8_t r = quickcheck(0x007F, QuickCheck_Normalize_Compatibility_Decomposed);

	EXPECT_EQ(QuickCheckResult_Yes, r);
}

TEST(QuickCheck, CompatibilityDecomposedOutOfUpperBounds)
{
	uint8_t r = quickcheck(0x30000, QuickCheck_Normalize_Compatibility_Decomposed);

	EXPECT_EQ(QuickCheckResult_Yes, r);
}

TEST(QuickCheck, CompatibilityDecomposedBetweenBlocks)
{
	uint8_t r = quickcheck(0x038D, QuickCheck_Normalize_Compatibility_Decomposed);

	EXPECT_EQ(QuickCheckResult_Yes, r);
}

TEST(QuickCheck, CompatibilityDecomposedHangulSyllable)
{
	uint8_t r = quickcheck(0xCCCA, QuickCheck_Normalize_Compatibility_Decomposed);

	EXPECT_EQ(QuickCheckResult_No, r);
}

TEST(QuickCheck, CompatibilityDecomposedHangulSyllableStart)
{
	uint8_t r = quickcheck(0xAC00, QuickCheck_Normalize_Compatibility_Decomposed);

	EXPECT_EQ(QuickCheckResult_No, r);
}

TEST(QuickCheck, CompatibilityDecomposedHangulSyllableEnd)
{
	uint8_t r = quickcheck(0xD7A3, QuickCheck_Normalize_Compatibility_Decomposed);

	EXPECT_EQ(QuickCheckResult_No, r);
}