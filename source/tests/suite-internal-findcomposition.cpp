#include "tests-base.hpp"

extern "C" {
	#include "../normalization.h"
}

TEST(FindComposition, NotFound)
{
	int32_t r = 0;
	CompositionEntry* e = findcomposition(0x00000918d, &r);

	EXPECT_EQ(nullptr, e);
	EXPECT_EQ(FindResult_Missing, r);
}

TEST(FindComposition, NotFoundLowerBounds)
{
	int32_t r = 0;
	CompositionEntry* e = findcomposition(0x00000001, &r);

	EXPECT_EQ(nullptr, e);
	EXPECT_EQ(FindResult_OutOfBounds, r);
}

TEST(FindComposition, NotFoundUpperBounds)
{
	int32_t r = 0;
	CompositionEntry* e = findcomposition(0x0002FA1E, &r);

	EXPECT_EQ(nullptr, e);
	EXPECT_EQ(FindResult_OutOfBounds, r);
}

TEST(FindComposition, InvalidCodepoint)
{
	int32_t r = 0;
	CompositionEntry* e = findcomposition(0x00110000, &r);

	EXPECT_EQ(nullptr, e);
	EXPECT_EQ(FindResult_InvalidCodepoint, r);
}

TEST(FindComposition, NoResult)
{
	CompositionEntry* e = findcomposition(0x00000DDD, nullptr);

	EXPECT_EQ(nullptr, e);
}