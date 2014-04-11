#include "tests-base.hpp"

#include "utf8rewind.h"

TEST(ConvertUcs2, ZeroLength)
{
	ucs2_t c = 0x8812;
	const size_t s = 256;
	char b[s] = { 0 };

	EXPECT_EQ(0, utf8convertucs2(c, b, 0));
	EXPECT_STREQ("", b);
}

TEST(ConvertUcs2, ZeroBuffer)
{
	ucs2_t c = 'T';
	const size_t s = 256;

	EXPECT_EQ(0, utf8convertucs2(c, nullptr, s));
}

TEST(ConvertUcs2, OneByte)
{
	ucs2_t c = L'A';
	const size_t s = 256;
	char b[s] = { 0 };

	EXPECT_EQ(1, utf8convertucs2(c, b, s));
	EXPECT_STREQ("A", b);
}

TEST(ConvertUcs2, OneByteMinimum)
{
	ucs2_t c = 0;
	const size_t s = 256;
	char b[s] = { 0 };

	EXPECT_EQ(1, utf8convertucs2(c, b, s));
	EXPECT_STREQ("", b);
}

TEST(ConvertUcs2, OneByteMaximum)
{
	ucs2_t c = 0x7F;
	const size_t s = 256;
	char b[s] = { 0 };

	EXPECT_EQ(1, utf8convertucs2(c, b, s));
	EXPECT_STREQ("\x7F", b);
}

TEST(ConvertUcs2, TwoBytes)
{
	ucs2_t c = 0x80;
	const size_t s = 256;
	char b[s] = { 0 };

	EXPECT_EQ(2, utf8convertucs2(c, b, s));
	EXPECT_STREQ("\xC2\x80", b);
}

TEST(ConvertUcs2, TwoBytesMinimum)
{
	ucs2_t c = 0x80;
	const size_t s = 256;
	char b[s] = { 0 };

	EXPECT_EQ(2, utf8convertucs2(c, b, s));
	EXPECT_STREQ("\xC2\x80", b);
}

TEST(ConvertUcs2, TwoBytesMaximum)
{
	ucs2_t c = 0x7FF;
	const size_t s = 256;
	char b[s] = { 0 };

	EXPECT_EQ(2, utf8convertucs2(c, b, s));
	EXPECT_STREQ("\xDF\xBF", b);
}

TEST(ConvertUcs2, ThreeByteString)
{
	const size_t s = 256;
	char b[s] = { 0 };
	char* d = b;

	{
		ucs2_t c = L'㒱';
		size_t l = utf8convertucs2(c, d, s);
		EXPECT_EQ(3, l);
		d += l;
	}

	{
		ucs2_t c = L'㒳';
		size_t l = utf8convertucs2(c, d, s);
		EXPECT_EQ(3, l);
		d += l;
	}

	EXPECT_STREQ("\xE3\x92\xB1\xE3\x92\xB3", b);
}

TEST(ConvertUcs2, ThreeBytesMinimum)
{
	ucs2_t c = 0x800;
	const size_t s = 256;
	char b[s] = { 0 };

	EXPECT_EQ(3, utf8convertucs2(c, b, s));
	EXPECT_STREQ("\xE0\xA0\x80", b);
}

TEST(ConvertUcs2, ThreeBytesBeforeSurrogatePairs)
{
	ucs2_t c = 0xD7FF;
	const size_t s = 256;
	char b[s] = { 0 };

	EXPECT_EQ(3, utf8convertucs2(c, b, s));
	EXPECT_STREQ("\xED\x9F\xBF", b);
}

TEST(ConvertUcs2, ThreeBytesAfterSurrogatePairs)
{
	ucs2_t c = 0xE000;
	const size_t s = 256;
	char b[s] = { 0 };

	EXPECT_EQ(3, utf8convertucs2(c, b, s));
	EXPECT_STREQ("\xEE\x80\x80", b);
}

TEST(ConvertUcs2, ThreeBytesMaximum)
{
	ucs2_t c = 0xFFFF;
	const size_t s = 256;
	char b[s] = { 0 };

	EXPECT_EQ(3, utf8convertucs2(c, b, s));
	EXPECT_STREQ("\xEF\xBF\xBF", b);
}

TEST(ConvertUcs2, SurrogateStart)
{
	ucs2_t c = 0xD800;
	const size_t s = 256;
	char b[s] = { 0 };

	EXPECT_EQ(0, utf8convertucs2(c, b, s));
	EXPECT_STREQ("", b);
}

TEST(ConvertUcs2, SurrogateEnd)
{
	ucs2_t c = 0xDFFF;
	const size_t s = 256;
	char b[s] = { 0 };

	EXPECT_EQ(0, utf8convertucs2(c, b, s));
	EXPECT_STREQ("", b);
}