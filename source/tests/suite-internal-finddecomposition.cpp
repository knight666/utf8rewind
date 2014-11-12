#include "tests-base.hpp"

extern "C" {
	#include "../normalization.h"
}

TEST(FindDecomposition, NoResult)
{
	const DecompositionRecord* r = finddecomposition(0x0002F9E5, NormalizationForm_Composed, nullptr);

	EXPECT_EQ(nullptr, r);
}

TEST(FindDecomposition, InvalidNormalizationForm)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x0002F974, 33, &e);

	EXPECT_EQ(nullptr, r);
	EXPECT_EQ(FindResult_Invalid, e);
}

TEST(FindDecomposition, UnhandledNormalizationFormComposed)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x00000F76, NormalizationForm_Composed, &e);

	EXPECT_EQ(nullptr, r);
	EXPECT_EQ(FindResult_Invalid, e);
}

TEST(FindDecomposition, UnhandledNormalizationFormCompatibilityComposed)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x00001EE8, NormalizationForm_Compatibility_Composed, &e);

	EXPECT_EQ(nullptr, r);
	EXPECT_EQ(FindResult_Invalid, e);
}

TEST(FindDecomposition, DecomposedMissing)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x0001FFFF, NormalizationForm_Decomposed, &e);

	EXPECT_EQ(nullptr, r);
	EXPECT_EQ(FindResult_Missing, e);
}

TEST(FindDecomposition, DecomposedMissingOutOfLowerBounds)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x00000067, NormalizationForm_Decomposed, &e);

	EXPECT_EQ(nullptr, r);
	EXPECT_EQ(FindResult_OutOfBounds, e);
}

TEST(FindDecomposition, DecomposedMissingOutOfUpperBounds)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x0011A26D, NormalizationForm_Decomposed, &e);

	EXPECT_EQ(nullptr, r);
	EXPECT_EQ(FindResult_OutOfBounds, e);
}

TEST(FindDecomposition, CompatibilityDecomposedMissing)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x0001A2AF, NormalizationForm_Compatibility_Decomposed, &e);

	EXPECT_EQ(nullptr, r);
	EXPECT_EQ(FindResult_Missing, e);
}

TEST(FindDecomposition, CompatibilityDecomposedMissingOutOfLowerBounds)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x00000023, NormalizationForm_Compatibility_Decomposed, &e);

	EXPECT_EQ(nullptr, r);
	EXPECT_EQ(FindResult_OutOfBounds, e);
}

TEST(FindDecomposition, CompatibilityDecomposedMissingOutOfUpperBounds)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x00DD2A5D, NormalizationForm_Compatibility_Decomposed, &e);

	EXPECT_EQ(nullptr, r);
	EXPECT_EQ(FindResult_OutOfBounds, e);
}

/*TEST(Decomposed, Found)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x000004E4, NormalizationForm_Decomposed, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_EQ(FindResult_Found, e);
	EXPECT_EQ(0x000004E4, r->codepoint);
	EXPECT_EQ(0x640, r->offset);
}

TEST(CompatibilityDecomposed, Found)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x000004E4, NormalizationForm_Compatibility_Decomposed, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_EQ(FindResult_Found, e);
	EXPECT_EQ(0x000004E4, r->codepoint);
	EXPECT_EQ(0x640, r->offset);
}*/