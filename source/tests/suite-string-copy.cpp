#include "tests-base.hpp"

#include "utf8string.hpp"

using namespace utf8rewind;

TEST(Copy, Ascii)
{
	Utf8String s("Stuff");

	Utf8String s2(s);

	EXPECT_STREQ("Stuff", s2.c_str());
	EXPECT_FALSE(s2.empty());
	EXPECT_EQ(6, s2.size());
	EXPECT_EQ(5, s2.length());
}

TEST(Copy, Utf8)
{
	Utf8String s("\xEF\xBC\xA1\xEF\xBC\xA1\xEF\xBC\xA8\xEF\xBC\xA6");

	Utf8String s2(s);

	EXPECT_STREQ("\xEF\xBC\xA1\xEF\xBC\xA1\xEF\xBC\xA8\xEF\xBC\xA6", s2.c_str());
	EXPECT_FALSE(s2.empty());
	EXPECT_EQ(13, s2.size());
	EXPECT_EQ(4, s2.length());
}

TEST(Copy, Utf8Invalid)
{
	Utf8String s("\xEF\xBC\xA1\xEF");

	Utf8String s2(s);

	EXPECT_STREQ("", s2.c_str());
	EXPECT_TRUE(s2.empty());
	EXPECT_EQ(1, s2.size());
	EXPECT_EQ(0, s2.length());
}

TEST(Copy, Utf16)
{
	Utf8String s(L"ﾛｿﾊ");

	Utf8String s2(s);

	EXPECT_STREQ("\xEF\xBE\x9B\xEF\xBD\xBF\xEF\xBE\x8A", s2.c_str());
	EXPECT_FALSE(s2.empty());
	EXPECT_EQ(10, s2.size());
	EXPECT_EQ(3, s2.length());
}

TEST(Copy, Utf16Invalid)
{
	Utf8String s(L"\xD800\x1100");

	Utf8String s2(s);

	EXPECT_STREQ("", s2.c_str());
	EXPECT_TRUE(s2.empty());
	EXPECT_EQ(1, s2.size());
	EXPECT_EQ(0, s2.length());
}

TEST(Copy, Empty)
{
	Utf8String s;

	Utf8String s2(s);

	EXPECT_STREQ("", s2.c_str());
	EXPECT_TRUE(s2.empty());
	EXPECT_EQ(1, s2.size());
	EXPECT_EQ(0, s2.length());
}