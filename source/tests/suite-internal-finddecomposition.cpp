#include "tests-base.hpp"

extern "C" {
	#include "../normalization.h"
}

TEST(FindDecomposition, NoResult)
{
	const DecompositionRecord* r = finddecomposition(0x0002F9E5, DecompositionQuery_Decomposed, nullptr);

	EXPECT_EQ(nullptr, r);
}

TEST(FindDecomposition, InvalidQuery)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x0002F974, 33, &e);

	EXPECT_EQ(nullptr, r);
	EXPECT_EQ(FindResult_Invalid, e);
}

TEST(FindDecomposition, DecomposedFound)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x000004E4, DecompositionQuery_Decomposed, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_EQ(FindResult_Found, e);
	EXPECT_EQ(0x000004E4, r->codepoint);
	EXPECT_EQ(0x0000062E, r->offset);
}

TEST(FindDecomposition, DecomposedFoundFirst)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x000000C0, DecompositionQuery_Decomposed, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_EQ(0x000000C0, r->codepoint);
	EXPECT_EQ(0x00000032, r->offset);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, DecomposedFoundLast)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x0002FA1D, DecompositionQuery_Decomposed, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_EQ(0x0002FA1D, r->codepoint);
	EXPECT_EQ(0x00004550, r->offset);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, DecomposedFoundPivot)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x0000F91E, DecompositionQuery_Decomposed, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_EQ(0x0000F91E, r->codepoint);
	EXPECT_EQ(0x00002B04, r->offset);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, DecomposedFoundPivotUp)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x00001E63, DecompositionQuery_Decomposed, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_EQ(0x00001E63, r->codepoint);
	EXPECT_EQ(0x00000BA2, r->offset);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, DecomposedFoundPivotDown)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x0002F81A, DecompositionQuery_Decomposed, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_EQ(0x0002F81A, r->codepoint);
	EXPECT_EQ(0x00003E02, r->offset);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, DecomposedFoundPivotDownDown)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x0000037E, DecompositionQuery_Decomposed, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_EQ(0x0000037E, r->codepoint);
	EXPECT_EQ(0x000004E1, r->offset);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, DecomposedFoundMaxDepth)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x0002F9DF, DecompositionQuery_Decomposed, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_EQ(0x0002F9DF, r->codepoint);
	EXPECT_EQ(0x000030A9, r->offset);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, DecomposedMissing)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x0001FFFF, DecompositionQuery_Decomposed, &e);

	EXPECT_EQ(nullptr, r);
	EXPECT_EQ(FindResult_Missing, e);
}

TEST(FindDecomposition, DecomposedMissingOutOfLowerBounds)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x00000067, DecompositionQuery_Decomposed, &e);

	EXPECT_EQ(nullptr, r);
	EXPECT_EQ(FindResult_OutOfBounds, e);
}

TEST(FindDecomposition, DecomposedMissingOutOfUpperBounds)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x0011A26D, DecompositionQuery_Decomposed, &e);

	EXPECT_EQ(nullptr, r);
	EXPECT_EQ(FindResult_OutOfBounds, e);
}

TEST(FindDecomposition, CompatibilityDecomposedFound)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x000004E4, DecompositionQuery_Compatibility_Decomposed, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_EQ(FindResult_Found, e);
	EXPECT_EQ(0x000004E4, r->codepoint);
	EXPECT_EQ(0x0000062E, r->offset);
}

TEST(FindDecomposition, CompatibilityDecomposedFoundFirst)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x000000A0, DecompositionQuery_Compatibility_Decomposed, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_EQ(0x000000A0, r->codepoint);
	EXPECT_EQ(0x00000001, r->offset);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, CompatibilityDecomposedFoundLast)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x0002FA1D, DecompositionQuery_Compatibility_Decomposed, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_EQ(0x0002FA1D, r->codepoint);
	EXPECT_EQ(0x00004550, r->offset);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, CompatibilityDecomposedFoundPivot)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x0000FB4F, DecompositionQuery_Compatibility_Decomposed, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_EQ(0x0000FB4F, r->codepoint);
	EXPECT_EQ(0x000031EB, r->offset);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, CompatibilityDecomposedFoundPivotUp)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x000024E6, DecompositionQuery_Compatibility_Decomposed, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_EQ(0x000024E6, r->codepoint);
	EXPECT_EQ(0x000004A6, r->offset);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, CompatibilityDecomposedFoundPivotDown)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x0001D596, DecompositionQuery_Compatibility_Decomposed, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_EQ(0x0001D596, r->codepoint);
	EXPECT_EQ(0x000017FA, r->offset);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, CompatibilityDecomposedFoundPivotDownUp)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x00003342, DecompositionQuery_Compatibility_Decomposed, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_EQ(0x00003342, r->codepoint);
	EXPECT_EQ(0x00002688, r->offset);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, CompatibilityDecomposedFoundMaxDepth)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x0001D4CA, DecompositionQuery_Compatibility_Decomposed, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_EQ(0x0001D4CA, r->codepoint);
	EXPECT_EQ(0x00000964, r->offset);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, CompatibilityDecomposedMissing)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x0001A2AF, DecompositionQuery_Compatibility_Decomposed, &e);

	EXPECT_EQ(nullptr, r);
	EXPECT_EQ(FindResult_Missing, e);
}

TEST(FindDecomposition, CompatibilityDecomposedMissingOutOfLowerBounds)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x00000023, DecompositionQuery_Compatibility_Decomposed, &e);

	EXPECT_EQ(nullptr, r);
	EXPECT_EQ(FindResult_OutOfBounds, e);
}

TEST(FindDecomposition, CompatibilityDecomposedMissingOutOfUpperBounds)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x00DD2A5D, DecompositionQuery_Compatibility_Decomposed, &e);

	EXPECT_EQ(nullptr, r);
	EXPECT_EQ(FindResult_OutOfBounds, e);
}