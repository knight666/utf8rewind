#include "tests-base.hpp"

extern "C" {
	#include "../normalization.h"
}

#include "helpers-strings.hpp"

TEST(FindDecomposition, NoResult)
{
	const char* r = finddecomposition(0x0002F9E5, DecompositionQuery_Decomposed, nullptr);

	EXPECT_EQ(nullptr, r);
}

TEST(FindDecomposition, InvalidQuery)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x0002F974, 33, &e);

	EXPECT_EQ(nullptr, r);
	EXPECT_EQ(FindResult_Invalid, e);
}

TEST(FindDecomposition, DecomposedFound)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x000004E4, DecompositionQuery_Decomposed, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_UTF8EQ("\xD0\x98\xCC\x88", r);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, DecomposedFoundFirst)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x000000C0, DecompositionQuery_Decomposed, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_UTF8EQ("A\xCC\x80", r);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, DecomposedFoundLast)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x0002FA1D, DecompositionQuery_Decomposed, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_UTF8EQ("\xF0\xAA\x98\x80", r);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, DecomposedFoundPivot)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x0000F91E, DecompositionQuery_Decomposed, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_UTF8EQ("\xE7\x88\x9B", r);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, DecomposedFoundPivotUp)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x00001E63, DecompositionQuery_Decomposed, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_UTF8EQ("s\xCC\xA3", r);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, DecomposedFoundPivotDown)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x0002F81A, DecompositionQuery_Decomposed, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_UTF8EQ("\xE5\x86\xAC", r);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, DecomposedFoundPivotDownDown)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x0000037E, DecompositionQuery_Decomposed, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_UTF8EQ(";", r);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, DecomposedFoundMaxDepth)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x0002F9DF, DecompositionQuery_Decomposed, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_UTF8EQ("\xE8\xBC\xB8", r);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, DecomposedMissing)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x0001FFFF, DecompositionQuery_Decomposed, &e);

	EXPECT_EQ(nullptr, r);
	EXPECT_EQ(FindResult_Missing, e);
}

TEST(FindDecomposition, DecomposedMissingOutOfLowerBounds)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x00000067, DecompositionQuery_Decomposed, &e);

	EXPECT_EQ(nullptr, r);
	EXPECT_EQ(FindResult_OutOfBounds, e);
}

TEST(FindDecomposition, DecomposedMissingOutOfUpperBounds)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x0011A26D, DecompositionQuery_Decomposed, &e);

	EXPECT_EQ(nullptr, r);
	EXPECT_EQ(FindResult_OutOfBounds, e);
}

TEST(FindDecomposition, CompatibilityDecomposedFound)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x000004E4, DecompositionQuery_Compatibility_Decomposed, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_UTF8EQ("\xD0\x98\xCC\x88", r);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, CompatibilityDecomposedFoundFirst)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x000000A0, DecompositionQuery_Compatibility_Decomposed, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_UTF8EQ(" ", r);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, CompatibilityDecomposedFoundLast)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x0002FA1D, DecompositionQuery_Compatibility_Decomposed, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_UTF8EQ("\xF0\xAA\x98\x80", r);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, CompatibilityDecomposedFoundPivot)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x0000FB4F, DecompositionQuery_Compatibility_Decomposed, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_UTF8EQ("\xD7\x90\xD7\x9C", r);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, CompatibilityDecomposedFoundPivotUp)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x000024E6, DecompositionQuery_Compatibility_Decomposed, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_UTF8EQ("w", r);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, CompatibilityDecomposedFoundPivotDown)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x0001D596, DecompositionQuery_Compatibility_Decomposed, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_UTF8EQ("q", r);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, CompatibilityDecomposedFoundPivotDownUp)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x00003342, DecompositionQuery_Compatibility_Decomposed, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_UTF8EQ("\xE3\x83\x9B\xE3\x83\xBC\xE3\x83\xB3", r);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, CompatibilityDecomposedFoundMaxDepth)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x0001D4CA, DecompositionQuery_Compatibility_Decomposed, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_UTF8EQ("u", r);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, CompatibilityDecomposedMissing)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x0001A2AF, DecompositionQuery_Compatibility_Decomposed, &e);

	EXPECT_EQ(nullptr, r);
	EXPECT_EQ(FindResult_Missing, e);
}

TEST(FindDecomposition, CompatibilityDecomposedMissingOutOfLowerBounds)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x00000023, DecompositionQuery_Compatibility_Decomposed, &e);

	EXPECT_EQ(nullptr, r);
	EXPECT_EQ(FindResult_OutOfBounds, e);
}

TEST(FindDecomposition, CompatibilityDecomposedMissingOutOfUpperBounds)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x00DD2A5D, DecompositionQuery_Compatibility_Decomposed, &e);

	EXPECT_EQ(nullptr, r);
	EXPECT_EQ(FindResult_OutOfBounds, e);
}

TEST(FindDecomposition, UppercaseFound)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x00001E0B, DecompositionQuery_Uppercase, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_UTF8EQ("\xE1\xB8\x8A", r);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, UppercaseFoundFirst)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x000000B5, DecompositionQuery_Uppercase, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_UTF8EQ("\xCE\x9C", r);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, UppercaseFoundLast)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x000118DF, DecompositionQuery_Uppercase, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_UTF8EQ("\xF0\x91\xA2\xBF", r);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, UppercaseFoundPivot)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x00001EC5, DecompositionQuery_Uppercase, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_UTF8EQ("\xE1\xBB\x84", r);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, UppercaseFoundPivotUp)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x00000431, DecompositionQuery_Uppercase, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_UTF8EQ("\xD0\x91", r);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, UppercaseFoundPivotDown)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x00001FAF, DecompositionQuery_Uppercase, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_UTF8EQ("\xE1\xBD\xAF\xCE\x99", r);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, UppercaseFoundPivotDownUp)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x000024E2, DecompositionQuery_Uppercase, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_UTF8EQ("\xE2\x93\x88", r);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, UppercaseFoundMaxDepth)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x000024D7, DecompositionQuery_Uppercase, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_UTF8EQ("\xE2\x92\xBD", r);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, UppercaseMissing)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x00002BAD, DecompositionQuery_Uppercase, &e);

	EXPECT_EQ(nullptr, r);
	EXPECT_EQ(FindResult_Missing, e);
}

TEST(FindDecomposition, UppercaseMissingOutOfLowerBounds)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x00000020, DecompositionQuery_Uppercase, &e);

	EXPECT_EQ(nullptr, r);
	EXPECT_EQ(FindResult_OutOfBounds, e);
}

TEST(FindDecomposition, UppercaseMissingOutOfUpperBounds)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x00101111, DecompositionQuery_Uppercase, &e);

	EXPECT_EQ(nullptr, r);
	EXPECT_EQ(FindResult_OutOfBounds, e);
}

TEST(FindDecomposition, LowercaseFound)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x00000393, DecompositionQuery_Lowercase, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_UTF8EQ("\xCE\xB3", r);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, LowercaseFoundFirst)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x000000C0, DecompositionQuery_Lowercase, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_UTF8EQ("\xC3\xA0", r);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, LowercaseFoundLast)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x000118BF, DecompositionQuery_Lowercase, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_UTF8EQ("\xF0\x91\xA3\x9F", r);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, LowercaseFoundPivot)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x00001EA0, DecompositionQuery_Lowercase, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_UTF8EQ("\xE1\xBA\xA1", r);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, LowercaseFoundPivotUp)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x00000417, DecompositionQuery_Lowercase, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_UTF8EQ("\xD0\xB7", r);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, LowercaseFoundPivotDown)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x00002C2A, DecompositionQuery_Lowercase, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_UTF8EQ("\xE2\xB1\x9A", r);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, LowercaseFoundPivotDownUp)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x00001FAA, DecompositionQuery_Lowercase, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_UTF8EQ("\xE1\xBE\xA2", r);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, LowercaseFoundMaxDepth)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x0000A728, DecompositionQuery_Lowercase, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_UTF8EQ("\xEA\x9C\xA9", r);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, LowercaseMissing)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x0000F3AA, DecompositionQuery_Lowercase, &e);

	EXPECT_EQ(nullptr, r);
	EXPECT_EQ(FindResult_Missing, e);
}

TEST(FindDecomposition, LowercaseMissingOutOfLowerBounds)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x00000061, DecompositionQuery_Lowercase, &e);

	EXPECT_EQ(nullptr, r);
	EXPECT_EQ(FindResult_OutOfBounds, e);
}

TEST(FindDecomposition, LowercaseMissingOutOfUpperBounds)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x00012000, DecompositionQuery_Lowercase, &e);

	EXPECT_EQ(nullptr, r);
	EXPECT_EQ(FindResult_OutOfBounds, e);
}

TEST(FindDecomposition, TitlecaseFound)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x00000450, DecompositionQuery_Titlecase, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_UTF8EQ("\xD0\x80", r);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, TitlecaseFoundFirst)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x000000B5, DecompositionQuery_Titlecase, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_UTF8EQ("\xCE\x9C", r);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, TitlecaseFoundLast)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x000118DF, DecompositionQuery_Titlecase, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_UTF8EQ("\xF0\x91\xA2\xBF", r);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, TitlecaseFoundPivot)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x00001EC1, DecompositionQuery_Titlecase, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_UTF8EQ("\xE1\xBB\x80", r);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, TitlecaseFoundPivotUp)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x000003F8, DecompositionQuery_Titlecase, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_UTF8EQ("\xCF\xB7", r);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, TitlecaseFoundPivotDown)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x00002C91, DecompositionQuery_Titlecase, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_UTF8EQ("\xE2\xB2\x90", r);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, TitlecaseFoundPivotDownUp)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x00001FAE, DecompositionQuery_Titlecase, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_UTF8EQ("\xE1\xBE\xAE", r);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, TitlecaseFoundMaxDepth)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x00001F9F, DecompositionQuery_Titlecase, &e);

	ASSERT_NE(nullptr, r);
	EXPECT_UTF8EQ("\xE1\xBE\x9F", r);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(FindDecomposition, TitlecaseMissing)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x0000ABED, DecompositionQuery_Titlecase, &e);

	EXPECT_EQ(nullptr, r);
	EXPECT_EQ(FindResult_Missing, e);
}

TEST(FindDecomposition, TitlecaseMissingOutOfLowerBounds)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x00000072, DecompositionQuery_Titlecase, &e);

	EXPECT_EQ(nullptr, r);
	EXPECT_EQ(FindResult_OutOfBounds, e);
}

TEST(FindDecomposition, TitlecaseMissingOutOfUpperBounds)
{
	int32_t e = 0;
	const char* r = finddecomposition(0x0002112A, DecompositionQuery_Titlecase, &e);

	EXPECT_EQ(nullptr, r);
	EXPECT_EQ(FindResult_OutOfBounds, e);
}