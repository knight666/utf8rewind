#include "tests-base.hpp"

extern "C" {
	#include "../normalization.h"
}

TEST(QuickCheck, NoResult)
{
	uint8_t r = quickcheck(0x81288, NormalizationForm_Composed);

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
	uint8_t r = quickcheck(0x1FEE, NormalizationForm_Composed);

	EXPECT_EQ(QuickCheckResult_No, r);
}

TEST(QuickCheck, ComposedFoundMaybe)
{
	uint8_t r = quickcheck(0x0DCF, NormalizationForm_Composed);

	EXPECT_EQ(QuickCheckResult_Maybe, r);
}

TEST(QuickCheck, ComposedOutOfLowerBounds)
{
	uint8_t r = quickcheck(0x00FF, NormalizationForm_Composed);

	EXPECT_EQ(QuickCheckResult_Yes, r);
}

TEST(QuickCheck, ComposedOutOfUpperBounds)
{
	uint8_t r = quickcheck(0x30000, NormalizationForm_Composed);

	EXPECT_EQ(QuickCheckResult_Yes, r);
}

// Decomposed

TEST(QuickCheck, DecomposedFoundNo)
{
	uint8_t r = quickcheck(0x0374, NormalizationForm_Decomposed);

	EXPECT_EQ(QuickCheckResult_No, r);
}

TEST(QuickCheck, DecomposedOutOfLowerBounds)
{
	uint8_t r = quickcheck(0x007F, NormalizationForm_Decomposed);

	EXPECT_EQ(QuickCheckResult_Yes, r);
}

TEST(QuickCheck, DecomposedOutOfUpperBounds)
{
	uint8_t r = quickcheck(0x30000, NormalizationForm_Decomposed);

	EXPECT_EQ(QuickCheckResult_Yes, r);
}

// Compatibility composed

TEST(QuickCheck, CompatibilityComposedFoundNo)
{
	uint8_t r = quickcheck(0x0A5B, NormalizationForm_Compatibility_Composed);

	EXPECT_EQ(QuickCheckResult_No, r);
}

TEST(QuickCheck, CompatibilityComposedFoundMaybe)
{
	uint8_t r = quickcheck(0x0B57, NormalizationForm_Compatibility_Composed);

	EXPECT_EQ(QuickCheckResult_Maybe, r);
}

TEST(QuickCheck, CompatibilityComposedOutOfLowerBounds)
{
	uint8_t r = quickcheck(0x007F, NormalizationForm_Compatibility_Composed);

	EXPECT_EQ(QuickCheckResult_Yes, r);
}

TEST(QuickCheck, CompatibilityComposedOutOfUpperBounds)
{
	uint8_t r = quickcheck(0x30000, NormalizationForm_Compatibility_Composed);

	EXPECT_EQ(QuickCheckResult_Yes, r);
}

// Compatibility decomposed

TEST(QuickCheck, CompatibilityDecomposedFoundNo)
{
	uint8_t r = quickcheck(0x03F9, NormalizationForm_Compatibility_Decomposed);

	EXPECT_EQ(QuickCheckResult_No, r);
}

TEST(QuickCheck, CompatibilityDecomposedOutOfLowerBounds)
{
	uint8_t r = quickcheck(0x007F, NormalizationForm_Compatibility_Decomposed);

	EXPECT_EQ(QuickCheckResult_Yes, r);
}

TEST(QuickCheck, CompatibilityDecomposedOutOfUpperBounds)
{
	uint8_t r = quickcheck(0x30000, NormalizationForm_Compatibility_Decomposed);

	EXPECT_EQ(QuickCheckResult_Yes, r);
}