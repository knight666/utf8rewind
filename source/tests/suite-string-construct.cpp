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
	Utf8String s("STARG\x03\x9B\x03\x0ATE SG-1");
	EXPECT_STREQ("STARG\x03\x9B\x03\x0ATE SG-1", s.c_str());
	EXPECT_FALSE(s.empty());
	EXPECT_EQ(17, s.size());
	EXPECT_EQ(16, s.length());
}

TEST(Construct, NoData)
{
	Utf8String s("");
	EXPECT_STREQ("", s.c_str());
	EXPECT_TRUE(s.empty());
	EXPECT_EQ(1, s.size());
	EXPECT_EQ(0, s.length());
}