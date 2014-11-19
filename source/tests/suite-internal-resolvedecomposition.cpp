#include "tests-base.hpp"

extern "C" {
	#include "../normalization.h"
}

TEST(ResolveDecomposition, Found)
{
	int32_t r = 0;
	const char* d = resolvedecomposition(0x0000000F, &r);

	EXPECT_STREQ("\x33\x00\x20\xcc\x81", d);
	EXPECT_EQ(FindResult_Found, r);
}

TEST(ResolveDecomposition, FoundFirst)
{
	int32_t r = 0;
	const char* d = resolvedecomposition(0x00000001, &r);

	EXPECT_STREQ("\x20", d);
	EXPECT_EQ(FindResult_Found, r);
}

TEST(ResolveDecomposition, FoundLast)
{
	int32_t r = 0;
	const char* d = resolvedecomposition(0x00004550, &r);

	EXPECT_STREQ("\xF0\xAA\x98\x80", d);
	EXPECT_EQ(FindResult_Found, r);
}

TEST(ResolveDecomposition, FoundWrongOffset)
{
	int32_t r = 0;
	const char* d = resolvedecomposition(0x0000000E, &r);

	EXPECT_STREQ("", d);
	EXPECT_EQ(FindResult_Found, r);
}

TEST(ResolveDecomposition, OffsetZero)
{
	int32_t r = 0;
	const char* d = resolvedecomposition(0x00000000, &r);

	EXPECT_STREQ("", d);
	EXPECT_EQ(FindResult_Found, r);
}

TEST(ResolveDecomposition, OffsetOutOfBounds)
{
	int32_t r = 0;
	const char* d = resolvedecomposition(0x00FFFFFF, &r);

	EXPECT_EQ(nullptr, d);
	EXPECT_EQ(FindResult_OutOfBounds, r);
}

TEST(ResolveDecomposition, NoResult)
{
	const char* d = resolvedecomposition(0x00000DDD, nullptr);

	EXPECT_EQ(nullptr, d);
}