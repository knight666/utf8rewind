#include "tests-base.hpp"

#include "utf8string.hpp"

using namespace utf8rewind;

TEST(Construct, Empty)
{
	Utf8String s;
	EXPECT_TRUE(s.empty());
	EXPECT_EQ(1, s.size());
	EXPECT_EQ(0, s.length());
}

TEST(Construct, Ascii)
{
	Utf8String s("Hello World!");
	EXPECT_STREQ("Hello World!", s.c_str());
	EXPECT_FALSE(s.empty());
	EXPECT_EQ(13, s.size());
	EXPECT_EQ(12, s.length());
}

TEST(Construct, Utf8)
{
	Utf8String s("STARG\xCC\x12TE SG-1");
	EXPECT_STREQ("STARG\xCC\x12TE SG-1", s.c_str());
	EXPECT_FALSE(s.empty());
	EXPECT_EQ(15, s.size());
	EXPECT_EQ(13, s.length());
}

TEST(Construct, Utf8InvalidCharacter)
{
	Utf8String s("K-\x03\x9B\x03\x0A-ARATE");
	EXPECT_STREQ("", s.c_str());
	EXPECT_TRUE(s.empty());
	EXPECT_EQ(1, s.size());
	EXPECT_EQ(0, s.length());
}

TEST(Construct, Utf16)
{
	Utf8String s(L"㒱㒳");
	EXPECT_STREQ("\xE3\x92\xB1\xE3\x92\xB3", s.c_str());
	EXPECT_FALSE(s.empty());
	EXPECT_EQ(7, s.size());
	EXPECT_EQ(2, s.length());
}

TEST(Construct, Utf16UnmatchedSurrogatePair)
{
	Utf8String s(L"\xD800\x1100");
	EXPECT_STREQ("", s.c_str());
	EXPECT_TRUE(s.empty());
	EXPECT_EQ(1, s.size());
	EXPECT_EQ(0, s.length());
}

TEST(Construct, Utf16NoData)
{
	Utf8String s(L"");
	EXPECT_STREQ("", s.c_str());
	EXPECT_TRUE(s.empty());
	EXPECT_EQ(1, s.size());
	EXPECT_EQ(0, s.length());
}

TEST(Construct, NoData)
{
	Utf8String s("");
	EXPECT_STREQ("", s.c_str());
	EXPECT_TRUE(s.empty());
	EXPECT_EQ(1, s.size());
	EXPECT_EQ(0, s.length());
}