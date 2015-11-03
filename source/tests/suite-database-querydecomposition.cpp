#include "tests-base.hpp"

extern "C" {
	#include "../internal/codepoint.h"
	#include "../internal/database.h"
}

#include "../helpers/helpers-strings.hpp"

// Decomposed

TEST(QueryDecompositionDecomposed, Found)
{
	uint8_t length = 0;
	EXPECT_UTF8LENGTHEQ("\xD0\x98\xCC\x88", database_querydecomposition(0x000004E4, NFDIndex1Ptr, NFDIndex2Ptr, NFDDataPtr, &length), length);
}

TEST(QueryDecompositionDecomposed, FoundFirst)
{
	uint8_t length = 0;
	EXPECT_UTF8LENGTHEQ("A\xCC\x80", database_querydecomposition(0x000000C0, NFDIndex1Ptr, NFDIndex2Ptr, NFDDataPtr, &length), length);
}

TEST(QueryDecompositionDecomposed, FoundLast)
{
	uint8_t length = 0;
	EXPECT_UTF8LENGTHEQ("\xF0\xAA\x98\x80", database_querydecomposition(0x0002FA1D, NFDIndex1Ptr, NFDIndex2Ptr, NFDDataPtr, &length), length);
}

TEST(QueryDecompositionDecomposed, FoundPivot)
{
	uint8_t length = 0;
	EXPECT_UTF8LENGTHEQ("\xE7\x88\x9B", database_querydecomposition(0x0000F91E, NFDIndex1Ptr, NFDIndex2Ptr, NFDDataPtr, &length), length);
}

TEST(QueryDecompositionDecomposed, FoundPivotUp)
{
	uint8_t length = 0;
	EXPECT_UTF8LENGTHEQ("s\xCC\xA3", database_querydecomposition(0x00001E63, NFDIndex1Ptr, NFDIndex2Ptr, NFDDataPtr, &length), length);
}

TEST(QueryDecompositionDecomposed, FoundPivotDown)
{
	uint8_t length = 0;
	EXPECT_UTF8LENGTHEQ("\xE5\x86\xAC", database_querydecomposition(0x0002F81A, NFDIndex1Ptr, NFDIndex2Ptr, NFDDataPtr, &length), length);
}

TEST(QueryDecompositionDecomposed, FoundPivotDownDown)
{
	uint8_t length = 0;
	EXPECT_UTF8LENGTHEQ(";", database_querydecomposition(0x0000037E, NFDIndex1Ptr, NFDIndex2Ptr, NFDDataPtr, &length), length);
}

TEST(QueryDecompositionDecomposed, FoundMaxDepth)
{
	uint8_t length = 0;
	EXPECT_UTF8LENGTHEQ("\xE8\xBC\xB8", database_querydecomposition(0x0002F9DF, NFDIndex1Ptr, NFDIndex2Ptr, NFDDataPtr, &length), length);
}

TEST(QueryDecompositionDecomposed, Missing)
{
	uint8_t length = 0;
	EXPECT_EQ(nullptr, database_querydecomposition(0x0001FFFF, NFDIndex1Ptr, NFDIndex2Ptr, NFDDataPtr, &length));
}

TEST(QueryDecompositionDecomposed, MissingOutOfLowerBounds)
{
	uint8_t length = 0;
	EXPECT_EQ(nullptr, database_querydecomposition(0x00000067, NFDIndex1Ptr, NFDIndex2Ptr, NFDDataPtr, &length));
}

TEST(QueryDecompositionDecomposed, MissingOutOfUpperBounds)
{
	uint8_t length = 0;
	EXPECT_EQ(nullptr, database_querydecomposition(0x0011A26D, NFDIndex1Ptr, NFDIndex2Ptr, NFDDataPtr, &length));
}

TEST(QueryDecompositionDecomposed, MissingIndexOutOfBounds)
{
	uint8_t length = 0;
	EXPECT_EQ(nullptr, database_querydecomposition(0xFFFFFFFF, NFDIndex1Ptr, NFDIndex2Ptr, NFDDataPtr, &length));
}

// Compatibility decomposed

TEST(QueryDecompositionCompatibilityDecomposed, Found)
{
	uint8_t length = 0;
	EXPECT_UTF8LENGTHEQ("\xD0\x98\xCC\x88", database_querydecomposition(0x000004E4, NFKDIndex1Ptr, NFKDIndex2Ptr, NFKDDataPtr, &length), length);
}

TEST(QueryDecompositionCompatibilityDecomposed, FoundFirst)
{
	uint8_t length = 0;
	EXPECT_UTF8LENGTHEQ(" ", database_querydecomposition(0x000000A0, NFKDIndex1Ptr, NFKDIndex2Ptr, NFKDDataPtr, &length), length);
}

TEST(QueryDecompositionCompatibilityDecomposed, FoundLast)
{
	uint8_t length = 0;
	EXPECT_UTF8LENGTHEQ("\xF0\xAA\x98\x80", database_querydecomposition(0x0002FA1D, NFKDIndex1Ptr, NFKDIndex2Ptr, NFKDDataPtr, &length), length);
}

TEST(QueryDecompositionCompatibilityDecomposed, FoundPivot)
{
	uint8_t length = 0;
	EXPECT_UTF8LENGTHEQ("\xD7\x90\xD7\x9C", database_querydecomposition(0x0000FB4F, NFKDIndex1Ptr, NFKDIndex2Ptr, NFKDDataPtr, &length), length);
}

TEST(QueryDecompositionCompatibilityDecomposed, FoundPivotUp)
{
	uint8_t length = 0;
	EXPECT_UTF8LENGTHEQ("w", database_querydecomposition(0x000024E6, NFKDIndex1Ptr, NFKDIndex2Ptr, NFKDDataPtr, &length), length);
}

TEST(QueryDecompositionCompatibilityDecomposed, FoundPivotDown)
{
	uint8_t length = 0;
	EXPECT_UTF8LENGTHEQ("q", database_querydecomposition(0x0001D596, NFKDIndex1Ptr, NFKDIndex2Ptr, NFKDDataPtr, &length), length);
}

TEST(QueryDecompositionCompatibilityDecomposed, FoundPivotDownUp)
{
	uint8_t length = 0;
	EXPECT_UTF8LENGTHEQ("\xE3\x83\x9B\xE3\x83\xBC\xE3\x83\xB3", database_querydecomposition(0x00003342, NFKDIndex1Ptr, NFKDIndex2Ptr, NFKDDataPtr, &length), length);
}

TEST(QueryDecompositionCompatibilityDecomposed, FoundMaxDepth)
{
	uint8_t length = 0;
	EXPECT_UTF8LENGTHEQ("u", database_querydecomposition(0x0001D4CA, NFKDIndex1Ptr, NFKDIndex2Ptr, NFKDDataPtr, &length), length);
}

TEST(QueryDecompositionCompatibilityDecomposed, Missing)
{
	uint8_t length = 0;
	EXPECT_EQ(nullptr, database_querydecomposition(0x0001A2AF, NFKDIndex1Ptr, NFKDIndex2Ptr, NFKDDataPtr, &length));
}

TEST(QueryDecompositionCompatibilityDecomposed, MissingOutOfLowerBounds)
{
	uint8_t length = 0;
	EXPECT_EQ(nullptr, database_querydecomposition(0x00000023, NFKDIndex1Ptr, NFKDIndex2Ptr, NFKDDataPtr, &length));
}

TEST(QueryDecompositionCompatibilityDecomposed, MissingOutOfUpperBounds)
{
	uint8_t length = 0;
	EXPECT_EQ(nullptr, database_querydecomposition(0x00DD2A5D, NFKDIndex1Ptr, NFKDIndex2Ptr, NFKDDataPtr, &length));
}

TEST(QueryDecompositionCompatibilityDecomposed, MissingIndexOutOfBounds)
{
	uint8_t length = 0;
	EXPECT_EQ(nullptr, database_querydecomposition(0xFFFFFFFF, NFKDIndex1Ptr, NFKDIndex2Ptr, NFKDDataPtr, &length));
}

// Uppercase

TEST(QueryDecompositionUppercase, Found)
{
	uint8_t length = 0;
	EXPECT_UTF8LENGTHEQ("\xE1\xB8\x8A", database_querydecomposition(0x00001E0B, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr, &length), length);
}

TEST(QueryDecompositionUppercase, FoundFirst)
{
	uint8_t length = 0;
	EXPECT_UTF8LENGTHEQ("\xCE\x9C", database_querydecomposition(0x000000B5, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr, &length), length);
}

TEST(QueryDecompositionUppercase, FoundLast)
{
	uint8_t length = 0;
	EXPECT_UTF8LENGTHEQ("\xF0\x91\xA2\xBF", database_querydecomposition(0x000118DF, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr, &length), length);
}

TEST(QueryDecompositionUppercase, FoundPivot)
{
	uint8_t length = 0;
	EXPECT_UTF8LENGTHEQ("\xE1\xBB\x84", database_querydecomposition(0x00001EC5, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr, &length), length);
}

TEST(QueryDecompositionUppercase, FoundPivotUp)
{
	uint8_t length = 0;
	EXPECT_UTF8LENGTHEQ("\xD0\x91", database_querydecomposition(0x00000431, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr, &length), length);
}

TEST(QueryDecompositionUppercase, FoundPivotDown)
{
	uint8_t length = 0;
	EXPECT_UTF8LENGTHEQ("\xE1\xBD\xAF\xCE\x99", database_querydecomposition(0x00001FAF, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr, &length), length);
}

TEST(QueryDecompositionUppercase, FoundPivotDownUp)
{
	uint8_t length = 0;
	EXPECT_UTF8LENGTHEQ("\xE2\x93\x88", database_querydecomposition(0x000024E2, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr, &length), length);
}

TEST(QueryDecompositionUppercase, FoundMaxDepth)
{
	uint8_t length = 0;
	EXPECT_UTF8LENGTHEQ("\xE2\x92\xBD", database_querydecomposition(0x000024D7, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr, &length), length);
}

TEST(QueryDecompositionUppercase, Missing)
{
	uint8_t length = 0;
	EXPECT_EQ(nullptr, database_querydecomposition(0x00002BAD, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr, &length));
}

TEST(QueryDecompositionUppercase, MissingOutOfLowerBounds)
{
	uint8_t length = 0;
	EXPECT_EQ(nullptr, database_querydecomposition(0x00000020, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr, &length));
}

TEST(QueryDecompositionUppercase, MissingOutOfUpperBounds)
{
	uint8_t length = 0;
	EXPECT_EQ(nullptr, database_querydecomposition(0x00101111, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr, &length));
}

TEST(QueryDecompositionUppercase, MissingIndexOutOfBounds)
{
	uint8_t length = 0;
	EXPECT_EQ(nullptr, database_querydecomposition(0xFFFFFFFF, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr, &length));
}

// Lowercase

TEST(QueryDecompositionLowercase, Found)
{
	uint8_t length = 0;
	EXPECT_UTF8LENGTHEQ("\xCE\xB3", database_querydecomposition(0x00000393, LowercaseIndex1Ptr, LowercaseIndex2Ptr, LowercaseDataPtr, &length), length);
}

TEST(QueryDecompositionLowercase, FoundFirst)
{
	uint8_t length = 0;
	EXPECT_UTF8LENGTHEQ("\xC3\xA0", database_querydecomposition(0x000000C0, LowercaseIndex1Ptr, LowercaseIndex2Ptr, LowercaseDataPtr, &length), length);
}

TEST(QueryDecompositionLowercase, FoundLast)
{
	uint8_t length = 0;
	EXPECT_UTF8LENGTHEQ("\xF0\x91\xA3\x9F", database_querydecomposition(0x000118BF, LowercaseIndex1Ptr, LowercaseIndex2Ptr, LowercaseDataPtr, &length), length);
}

TEST(QueryDecompositionLowercase, FoundPivot)
{
	uint8_t length = 0;
	EXPECT_UTF8LENGTHEQ("\xE1\xBA\xA1", database_querydecomposition(0x00001EA0, LowercaseIndex1Ptr, LowercaseIndex2Ptr, LowercaseDataPtr, &length), length);
}

TEST(QueryDecompositionLowercase, FoundPivotUp)
{
	uint8_t length = 0;
	EXPECT_UTF8LENGTHEQ("\xD0\xB7", database_querydecomposition(0x00000417, LowercaseIndex1Ptr, LowercaseIndex2Ptr, LowercaseDataPtr, &length), length);
}

TEST(QueryDecompositionLowercase, FoundPivotDown)
{
	uint8_t length = 0;
	EXPECT_UTF8LENGTHEQ("\xE2\xB1\x9A", database_querydecomposition(0x00002C2A, LowercaseIndex1Ptr, LowercaseIndex2Ptr, LowercaseDataPtr, &length), length);
}

TEST(QueryDecompositionLowercase, FoundPivotDownUp)
{
	uint8_t length = 0;
	EXPECT_UTF8LENGTHEQ("\xE1\xBE\xA2", database_querydecomposition(0x00001FAA, LowercaseIndex1Ptr, LowercaseIndex2Ptr, LowercaseDataPtr, &length), length);
}

TEST(QueryDecompositionLowercase, FoundMaxDepth)
{
	uint8_t length = 0;
	EXPECT_UTF8LENGTHEQ("\xEA\x9C\xA9", database_querydecomposition(0x0000A728, LowercaseIndex1Ptr, LowercaseIndex2Ptr, LowercaseDataPtr, &length), length);
}

TEST(QueryDecompositionLowercase, Missing)
{
	uint8_t length = 0;
	EXPECT_EQ(nullptr, database_querydecomposition(0x0000F3AA, LowercaseIndex1Ptr, LowercaseIndex2Ptr, LowercaseDataPtr, &length));
}

TEST(QueryDecompositionLowercase, MissingOutOfLowerBounds)
{
	uint8_t length = 0;
	EXPECT_EQ(nullptr, database_querydecomposition(0x00000021, LowercaseIndex1Ptr, LowercaseIndex2Ptr, LowercaseDataPtr, &length));
}

TEST(QueryDecompositionLowercase, MissingOutOfUpperBounds)
{
	uint8_t length = 0;
	EXPECT_EQ(nullptr, database_querydecomposition(0x00012000, LowercaseIndex1Ptr, LowercaseIndex2Ptr, LowercaseDataPtr, &length));
}

TEST(QueryDecompositionLowercase, MissingIndexOutOfBounds)
{
	uint8_t length = 0;
	EXPECT_EQ(nullptr, database_querydecomposition(0xFFFFFFFF, LowercaseIndex1Ptr, LowercaseIndex2Ptr, LowercaseDataPtr, &length));
}

// Titlecase

TEST(QueryDecompositionTitlecase, Found)
{
	uint8_t length = 0;
	EXPECT_UTF8LENGTHEQ("\xD0\x80", database_querydecomposition(0x00000450, TitlecaseIndex1Ptr, TitlecaseIndex2Ptr, TitlecaseDataPtr, &length), length);
}

TEST(QueryDecompositionTitlecase, FoundFirst)
{
	uint8_t length = 0;
	EXPECT_UTF8LENGTHEQ("\xCE\x9C", database_querydecomposition(0x000000B5, TitlecaseIndex1Ptr, TitlecaseIndex2Ptr, TitlecaseDataPtr, &length), length);
}

TEST(QueryDecompositionTitlecase, FoundLast)
{
	uint8_t length = 0;
	EXPECT_UTF8LENGTHEQ("\xF0\x91\xA2\xBF", database_querydecomposition(0x000118DF, TitlecaseIndex1Ptr, TitlecaseIndex2Ptr, TitlecaseDataPtr, &length), length);
}

TEST(QueryDecompositionTitlecase, FoundPivot)
{
	uint8_t length = 0;
	EXPECT_UTF8LENGTHEQ("\xE1\xBB\x80", database_querydecomposition(0x00001EC1, TitlecaseIndex1Ptr, TitlecaseIndex2Ptr, TitlecaseDataPtr, &length), length);
}

TEST(QueryDecompositionTitlecase, FoundPivotUp)
{
	uint8_t length = 0;
	EXPECT_UTF8LENGTHEQ("\xCF\xB7", database_querydecomposition(0x000003F8, TitlecaseIndex1Ptr, TitlecaseIndex2Ptr, TitlecaseDataPtr, &length), length);
}

TEST(QueryDecompositionTitlecase, FoundPivotDown)
{
	uint8_t length = 0;
	EXPECT_UTF8LENGTHEQ("\xE2\xB2\x90", database_querydecomposition(0x00002C91, TitlecaseIndex1Ptr, TitlecaseIndex2Ptr, TitlecaseDataPtr, &length), length);
}

TEST(QueryDecompositionTitlecase, FoundPivotDownUp)
{
	uint8_t length = 0;
	EXPECT_UTF8LENGTHEQ("\xE1\xBE\xAE", database_querydecomposition(0x00001FAE, TitlecaseIndex1Ptr, TitlecaseIndex2Ptr, TitlecaseDataPtr, &length), length);
}

TEST(QueryDecompositionTitlecase, FoundMaxDepth)
{
	uint8_t length = 0;
	EXPECT_UTF8LENGTHEQ("\xE1\xBE\x9F", database_querydecomposition(0x00001F9F, TitlecaseIndex1Ptr, TitlecaseIndex2Ptr, TitlecaseDataPtr, &length), length);
}

TEST(QueryDecompositionTitlecase, Missing)
{
	uint8_t length = 0;
	EXPECT_EQ(nullptr, database_querydecomposition(0x0000ABED, TitlecaseIndex1Ptr, TitlecaseIndex2Ptr, TitlecaseDataPtr, &length));
}

TEST(QueryDecompositionTitlecase, MissingOutOfLowerBounds)
{
	uint8_t length = 0;
	EXPECT_EQ(nullptr, database_querydecomposition(0x0000001A, TitlecaseIndex1Ptr, TitlecaseIndex2Ptr, TitlecaseDataPtr, &length));
}

TEST(QueryDecompositionTitlecase, MissingOutOfUpperBounds)
{
	uint8_t length = 0;
	EXPECT_EQ(nullptr, database_querydecomposition(0x0002112A, TitlecaseIndex1Ptr, TitlecaseIndex2Ptr, TitlecaseDataPtr, &length));
}

TEST(QueryDecompositionTitlecase, MissingIndexOutOfBounds)
{
	uint8_t length = 0;
	EXPECT_EQ(nullptr, database_querydecomposition(0xFFFFFFFF, TitlecaseIndex1Ptr, TitlecaseIndex2Ptr, TitlecaseDataPtr, &length));
}