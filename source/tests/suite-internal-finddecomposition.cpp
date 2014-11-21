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

TEST(FindDecomposition, UppercaseFound)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x00001E0B, DecompositionQuery_Uppercase, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_EQ(FindResult_Found, e);
	EXPECT_EQ(0x00001E0B, r->codepoint);
	EXPECT_EQ(0x00004FB8, r->offset);
}

TEST(FindDecomposition, UppercaseFoundFirst)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x000000B5, DecompositionQuery_Uppercase, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_EQ(FindResult_Found, e);
	EXPECT_EQ(0x000000B5, r->codepoint);
	EXPECT_EQ(0x00003BBC, r->offset);
}

TEST(FindDecomposition, UppercaseFoundLast)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x000118DF, DecompositionQuery_Uppercase, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_EQ(FindResult_Found, e);
	EXPECT_EQ(0x000118DF, r->codepoint);
	EXPECT_EQ(0x00006262, r->offset);
}

TEST(FindDecomposition, UppercaseFoundPivot)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x00001E89, DecompositionQuery_Uppercase, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_EQ(FindResult_Found, e);
	EXPECT_EQ(0x00001E89, r->codepoint);
	EXPECT_EQ(0x000051B0, r->offset);
}

TEST(FindDecomposition, UppercaseFoundPivotUp)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x000003E7, DecompositionQuery_Uppercase, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_EQ(FindResult_Found, e);
	EXPECT_EQ(0x000003E7, r->codepoint);
	EXPECT_EQ(0x00004A58, r->offset);
}

TEST(FindDecomposition, UppercaseFoundPivotDown)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x00002CA3, DecompositionQuery_Uppercase, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_EQ(FindResult_Found, e);
	EXPECT_EQ(0x00002CA3, r->codepoint);
	EXPECT_EQ(0x00005A0E, r->offset);
}

TEST(FindDecomposition, UppercaseFoundPivotDownUp)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x00001FA0, DecompositionQuery_Uppercase, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_EQ(FindResult_Found, e);
	EXPECT_EQ(0x00001FA0, r->codepoint);
	EXPECT_EQ(0x0000559F, r->offset);
}

TEST(FindDecomposition, UppercaseFoundMaxDepth)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x000024D7, DecompositionQuery_Uppercase, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_EQ(FindResult_Found, e);
	EXPECT_EQ(0x000024D7, r->codepoint);
	EXPECT_EQ(0x0000577B, r->offset);
}

TEST(FindDecomposition, UppercaseMissing)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x00002BAD, DecompositionQuery_Uppercase, &e);

	EXPECT_EQ(nullptr, r);
	EXPECT_EQ(FindResult_Missing, e);
}

TEST(FindDecomposition, UppercaseMissingOutOfLowerBounds)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x00000020, DecompositionQuery_Uppercase, &e);

	EXPECT_EQ(nullptr, r);
	EXPECT_EQ(FindResult_OutOfBounds, e);
}

TEST(FindDecomposition, UppercaseMissingOutOfUpperBounds)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x00101111, DecompositionQuery_Uppercase, &e);

	EXPECT_EQ(nullptr, r);
	EXPECT_EQ(FindResult_OutOfBounds, e);
}

TEST(FindDecomposition, LowercaseFound)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x00000393, DecompositionQuery_Lowercase, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_EQ(FindResult_Found, e);
	EXPECT_EQ(0x00000393, r->codepoint);
	EXPECT_EQ(0x00000973, r->offset);
}

TEST(FindDecomposition, LowercaseFoundFirst)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x000000C0, DecompositionQuery_Lowercase, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_EQ(FindResult_Found, e);
	EXPECT_EQ(0x000000C0, r->codepoint);
	EXPECT_EQ(0x00004555, r->offset);
}

TEST(FindDecomposition, LowercaseFoundLast)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x000118BF, DecompositionQuery_Lowercase, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_EQ(FindResult_Found, e);
	EXPECT_EQ(0x000118BF, r->codepoint);
	EXPECT_EQ(0x000061C2, r->offset);
}

TEST(FindDecomposition, LowercaseFoundPivot)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x00001E62, DecompositionQuery_Lowercase, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_EQ(FindResult_Found, e);
	EXPECT_EQ(0x00001E62, r->codepoint);
	EXPECT_EQ(0x00005114, r->offset);
}

TEST(FindDecomposition, LowercaseFoundPivotUp)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x00000409, DecompositionQuery_Lowercase, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_EQ(FindResult_Found, e);
	EXPECT_EQ(0x00000409, r->codepoint);
	EXPECT_EQ(0x00004AAC, r->offset);
}

TEST(FindDecomposition, LowercaseFoundPivotDown)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x00002C62, DecompositionQuery_Lowercase, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_EQ(FindResult_Found, e);
	EXPECT_EQ(0x00002C62, r->codepoint);
	EXPECT_EQ(0x00002A95, r->offset);
}

TEST(FindDecomposition, LowercaseFoundPivotDownUp)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x00001F9A, DecompositionQuery_Lowercase, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_EQ(FindResult_Found, e);
	EXPECT_EQ(0x00001F9A, r->codepoint);
	EXPECT_EQ(0x00005587, r->offset);
}

TEST(FindDecomposition, LowercaseFoundMaxDepth)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x0000A728, DecompositionQuery_Lowercase, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_EQ(FindResult_Found, e);
	EXPECT_EQ(0x0000A728, r->codepoint);
	EXPECT_EQ(0x00005D06, r->offset);
}

TEST(FindDecomposition, LowercaseMissing)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x0000F3AA, DecompositionQuery_Lowercase, &e);

	EXPECT_EQ(nullptr, r);
	EXPECT_EQ(FindResult_Missing, e);
}

TEST(FindDecomposition, LowercaseMissingOutOfLowerBounds)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x00000061, DecompositionQuery_Lowercase, &e);

	EXPECT_EQ(nullptr, r);
	EXPECT_EQ(FindResult_OutOfBounds, e);
}

TEST(FindDecomposition, LowercaseMissingOutOfUpperBounds)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x00012000, DecompositionQuery_Lowercase, &e);

	EXPECT_EQ(nullptr, r);
	EXPECT_EQ(FindResult_OutOfBounds, e);
}

TEST(FindDecomposition, TitlecaseFound)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x00001E3C, DecompositionQuery_Titlecase, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_EQ(FindResult_Found, e);
	EXPECT_EQ(0x00001E3C, r->codepoint);
	EXPECT_EQ(0x0000507C, r->offset);
}

TEST(FindDecomposition, TitlecaseFoundFirst)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x000000C0, DecompositionQuery_Titlecase, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_EQ(FindResult_Found, e);
	EXPECT_EQ(0x000000C0, r->codepoint);
	EXPECT_EQ(0x00004555, r->offset);
}

TEST(FindDecomposition, TitlecaseFoundLast)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x000118BF, DecompositionQuery_Titlecase, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_EQ(FindResult_Found, e);
	EXPECT_EQ(0x000118BF, r->codepoint);
	EXPECT_EQ(0x000061C2, r->offset);
}

TEST(FindDecomposition, TitlecaseFoundPivot)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x00001E62, DecompositionQuery_Titlecase, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_EQ(FindResult_Found, e);
	EXPECT_EQ(0x00001E62, r->codepoint);
	EXPECT_EQ(0x00005114, r->offset);
}

TEST(FindDecomposition, TitlecaseFoundPivotUp)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x00000409, DecompositionQuery_Titlecase, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_EQ(FindResult_Found, e);
	EXPECT_EQ(0x00000409, r->codepoint);
	EXPECT_EQ(0x00004AAC, r->offset);
}

TEST(FindDecomposition, TitlecaseFoundPivotDown)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x00002C62, DecompositionQuery_Titlecase, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_EQ(FindResult_Found, e);
	EXPECT_EQ(0x00002C62, r->codepoint);
	EXPECT_EQ(0x00002A95, r->offset);
}

TEST(FindDecomposition, TitlecaseFoundPivotDownUp)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x00001F9A, DecompositionQuery_Titlecase, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_EQ(FindResult_Found, e);
	EXPECT_EQ(0x00001F9A, r->codepoint);
	EXPECT_EQ(0x00005587, r->offset);
}

TEST(FindDecomposition, TitlecaseFoundMaxDepth)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x00001F9F, DecompositionQuery_Titlecase, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_EQ(FindResult_Found, e);
	EXPECT_EQ(0x00001F9F, r->codepoint);
	EXPECT_EQ(0x0000559B, r->offset);
}

TEST(FindDecomposition, TitlecaseMissing)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x0000ABED, DecompositionQuery_Titlecase, &e);

	EXPECT_EQ(nullptr, r);
	EXPECT_EQ(FindResult_Missing, e);
}

TEST(FindDecomposition, TitlecaseMissingOutOfLowerBounds)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x00000072, DecompositionQuery_Titlecase, &e);

	EXPECT_EQ(nullptr, r);
	EXPECT_EQ(FindResult_OutOfBounds, e);
}

TEST(FindDecomposition, TitlecaseMissingOutOfUpperBounds)
{
	int32_t e = 0;
	const DecompositionRecord* r = finddecomposition(0x0002112A, DecompositionQuery_Titlecase, &e);

	EXPECT_EQ(nullptr, r);
	EXPECT_EQ(FindResult_OutOfBounds, e);
}