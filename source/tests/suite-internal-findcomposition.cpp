#include "tests-base.hpp"

extern "C" {
	#include "../normalization.h"
}

TEST(FindComposition, FoundFirst)
{
	int32_t r = 0;
	const CompositionEntry* e = findcomposition(0x000000A0, &r);

	ASSERT_NE(nullptr, e);
	EXPECT_EQ(FindResult_Found, r);
	EXPECT_EQ(0x000000A0, e->codepoint);
	EXPECT_EQ(0, e->offsetC);
	EXPECT_EQ(0, e->offsetD);
	EXPECT_EQ(0x1, e->offsetKC);
	EXPECT_EQ(0x1, e->offsetKD);
}

TEST(FindComposition, FoundLast)
{
	int32_t r = 0;
	const CompositionEntry* e = findcomposition(0x0002FA1D, &r);

	ASSERT_NE(nullptr, e);
	EXPECT_EQ(FindResult_Found, r);
	EXPECT_EQ(0x0002FA1D, e->codepoint);
	EXPECT_EQ(0x3007813, e->offsetC);
	EXPECT_EQ(0x3007813, e->offsetD);
	EXPECT_EQ(0x3007813, e->offsetKC);
	EXPECT_EQ(0x3007813, e->offsetKD);
}

TEST(FindComposition, FoundPivot)
{
	int32_t r = 0;
	const CompositionEntry* e = findcomposition(0x0000C3D6, &r);

	ASSERT_NE(nullptr, e);
	EXPECT_EQ(FindResult_Found, r);
	EXPECT_EQ(0x0000C3D6, e->codepoint);
	EXPECT_EQ(0, e->offsetC);
	EXPECT_EQ(0x0200192A, e->offsetD);
	EXPECT_EQ(0, e->offsetKC);
	EXPECT_EQ(0x0200192A, e->offsetKD);
}

TEST(FindComposition, NotFound)
{
	int32_t r = 0;
	const CompositionEntry* e = findcomposition(0x00000918d, &r);

	EXPECT_EQ(nullptr, e);
	EXPECT_EQ(FindResult_Missing, r);
}

TEST(FindComposition, NotFoundLowerBounds)
{
	int32_t r = 0;
	const CompositionEntry* e = findcomposition(0x00000001, &r);

	EXPECT_EQ(nullptr, e);
	EXPECT_EQ(FindResult_OutOfBounds, r);
}

TEST(FindComposition, NotFoundUpperBounds)
{
	int32_t r = 0;
	const CompositionEntry* e = findcomposition(0x0002FA1E, &r);

	EXPECT_EQ(nullptr, e);
	EXPECT_EQ(FindResult_OutOfBounds, r);
}

TEST(FindComposition, InvalidCodepoint)
{
	int32_t r = 0;
	const CompositionEntry* e = findcomposition(0x00110000, &r);

	EXPECT_EQ(nullptr, e);
	EXPECT_EQ(FindResult_InvalidCodepoint, r);
}

TEST(FindComposition, NoResult)
{
	const CompositionEntry* e = findcomposition(0x00000DDD, nullptr);

	EXPECT_EQ(nullptr, e);
}