#include "tests-base.hpp"

extern "C" {
	#include "../normalization.h"
}

TEST(ResolveDecomposition, Found)
{
	int32_t r = 0;
	const char* d = resolvedecomposition(0xf, &r);

	EXPECT_STREQ("\x33\x00\x20\xcc\x81", d);
	EXPECT_EQ(FindResult_Found, r);
}

TEST(ResolveDecomposition, OffsetZero)
{
	int32_t r = 0;
	const char* d = resolvedecomposition(0, &r);

	EXPECT_EQ(nullptr, d);
	EXPECT_EQ(FindResult_Invalid, r);
}

TEST(ResolveDecomposition, OffsetOutOfBounds)
{
	int32_t r = 0;
	const char* d = resolvedecomposition(0, &r);

	EXPECT_EQ(nullptr, d);
	EXPECT_EQ(FindResult_OutOfBounds, r);
}