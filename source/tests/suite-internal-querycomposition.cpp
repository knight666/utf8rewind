#include "tests-base.hpp"

extern "C" {
	#include "../internal/database.h"
}

TEST(QueryComposition, NoResult)
{
	unicode_t r = database_querycomposition(0x00000002F, 0x00001988, nullptr);

	EXPECT_EQ(0, r);
}

TEST(QueryComposition, Found)
{
	int32_t e = 0;
	unicode_t r = database_querycomposition(0x00001F7C, 0x00000345, &e);

	EXPECT_EQ(0x00001FF2, r);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(QueryComposition, FoundFirst)
{
	int32_t e = 0;
	unicode_t r = database_querycomposition(0x0000003C, 0x00000338, &e);

	EXPECT_EQ(0x0000226E, r);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(QueryComposition, FoundLast)
{
	int32_t e = 0;
	unicode_t r = database_querycomposition(0x000115B9, 0x000115AF, &e);

	EXPECT_EQ(0x000115BB, r);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(QueryComposition, FoundPivot)
{
	int32_t e = 0;
	unicode_t r = database_querycomposition(0x00000399, 0x00000314, &e);

	EXPECT_EQ(0x00001F39, r);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(QueryComposition, FoundPivotUp)
{
	int32_t e = 0;
	unicode_t r = database_querycomposition(0x00000069, 0x00000306, &e);

	EXPECT_EQ(0x0000012D, r);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(QueryComposition, FoundPivotDown)
{
	int32_t e = 0;
	unicode_t r = database_querycomposition(0x00001F00, 0x00000345, &e);

	EXPECT_EQ(0x00001F80, r);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(QueryComposition, FoundPivotDownDown)
{
	int32_t e = 0;
	unicode_t r = database_querycomposition(0x00001FFE, 0x00000301, &e);

	EXPECT_EQ(0x00001FDE, r);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(QueryComposition, FoundMaxDepth)
{
	int32_t e = 0;
	unicode_t r = database_querycomposition(0x00002282, 0x00000338, &e);

	EXPECT_EQ(0x00002284, r);
	EXPECT_EQ(FindResult_Found, e);
}

TEST(QueryComposition, Missing)
{
	int32_t e = 0;
	unicode_t r = database_querycomposition(0x00001F28, 0x00001D16, &e);

	EXPECT_EQ(0, r);
	EXPECT_EQ(FindResult_Missing, e);
}

TEST(QueryComposition, MissingOutOfLowerBounds)
{
	int32_t e = 0;
	unicode_t r = database_querycomposition(0x00000001, 0x0000011D, &e);

	EXPECT_EQ(0, r);
	EXPECT_EQ(FindResult_OutOfBounds, e);
}

TEST(QueryComposition, MissingOutOfUpperBounds)
{
	int32_t e = 0;
	unicode_t r = database_querycomposition(0xABABABAB, 0xDADADADA, &e);

	EXPECT_EQ(0, r);
	EXPECT_EQ(FindResult_OutOfBounds, e);
}