#include "tests-base.hpp"

#include "utf8string.hpp"

using namespace utf8rewind;

TEST(Clear, Filled)
{
	Utf8String s("whatsit");

	s.clear();

	EXPECT_TRUE(s.empty());
	EXPECT_EQ(1, s.size());
	EXPECT_EQ(0, s.length());
}

TEST(Clear, Empty)
{
	Utf8String s;

	s.clear();

	EXPECT_TRUE(s.empty());
	EXPECT_EQ(1, s.size());
	EXPECT_EQ(0, s.length());
}