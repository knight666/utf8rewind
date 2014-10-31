#include "tests-base.hpp"

extern "C" {
	#include "../normalization.h"
}

TEST(FindComposition, NotFound)
{
	CompositionEntry* e = findcomposition(0x00000918d);

	EXPECT_EQ(nullptr, e);
}

TEST(FindComposition, NotFoundLowerBounds)
{
	CompositionEntry* e = findcomposition(0x00000001);

	EXPECT_EQ(nullptr, e);
}

TEST(FindComposition, NotFoundUpperBounds)
{
	CompositionEntry* e = findcomposition(0xFFFFFFFF);

	EXPECT_EQ(nullptr, e);
}