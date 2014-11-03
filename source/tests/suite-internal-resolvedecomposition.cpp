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
	const char* d = resolvedecomposition(0x00007FF5, &r);

	EXPECT_STREQ("\xE1\x84\x83\xE1\x85\xAE\xE1\x86\xAF", d);
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

TEST(ResolveDecomposition, NextPageFoundFirst)
{
	int32_t r = 0;
	const char* d = resolvedecomposition(0x01000001, &r);

	EXPECT_STREQ("\xe1\x84\x83\xe1\x85\xae\xe1\x86\xb0", d);
	EXPECT_EQ(FindResult_Found, r);
}

TEST(ResolveDecomposition, NextPageFoundLast)
{
	int32_t r = 0;
	const char* d = resolvedecomposition(0x01007FED, &r);

	EXPECT_STREQ("\xe1\x84\x89\xe1\x85\xa6\xe1\x86\xb9", d);
	EXPECT_EQ(FindResult_Found, r);
}

TEST(ResolveDecomposition, NetPageFoundWrongOffset)
{
	int32_t r = 0;
	const char* d = resolvedecomposition(0x02000014, &r);

	EXPECT_STREQ("", d);
	EXPECT_EQ(FindResult_Found, r);
}

TEST(ResolveDecomposition, NextPageOffsetZero)
{
	int32_t r = 0;
	const char* d = resolvedecomposition(0x02000000, &r);

	EXPECT_STREQ("", d);
	EXPECT_EQ(FindResult_Found, r);
}

TEST(ResolveDecomposition, NextPageOffsetOutOfBounds)
{
	int32_t r = 0;
	const char* d = resolvedecomposition(0x0118A722, &r);

	EXPECT_EQ(nullptr, d);
	EXPECT_EQ(FindResult_OutOfBounds, r);
}

TEST(ResolveDecomposition, LastPage)
{
	int32_t r = 0;
	const char* d = resolvedecomposition(0x03000001, &r);

	EXPECT_STREQ("\xe1\x84\x8e\xe1\x85\xb4\xe1\x86\xa9", d);
	EXPECT_EQ(FindResult_Found, r);
}

TEST(ResolveDecomposition, InvalidPage)
{
	int32_t r = 0;
	const char* d = resolvedecomposition(0x21000001, &r);

	EXPECT_STREQ(0, d);
	EXPECT_EQ(FindResult_Invalid, r);
}

TEST(ResolveDecomposition, NoResult)
{
	const char* d = resolvedecomposition(0x00000DDD, nullptr);

	EXPECT_EQ(nullptr, d);
}