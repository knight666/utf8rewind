#include "tests-base.hpp"
#include "string-helpers.hpp"

#include "utf8string.hpp"

using namespace utf8rewind;

TEST(IteratorConstruct, Ascii)
{
	Utf8String s("Magnificent");

	Utf8String::iterator b = s.begin();
	Utf8String::iterator e = s.end();

	EXPECT_UNICODE_EQ('M', b);
	EXPECT_UNICODE_EQ(0, e);
}

TEST(IteratorConstruct, Utf8)
{
	Utf8String s("\xE3\x84\x88\xE3\x90\x8E\xE3\x87\xBF");

	Utf8String::iterator b = s.begin();
	Utf8String::iterator e = s.end();

	EXPECT_UNICODE_EQ(0x3108, b);
	EXPECT_UNICODE_EQ(0, e);
}

TEST(IteratorConstruct, Utf8Invalid)
{
	Utf8String s("\xEF\xBC\xA1\xEF");

	Utf8String::iterator b = s.begin();
	Utf8String::iterator e = s.end();

	EXPECT_UNICODE_EQ(0, b);
	EXPECT_UNICODE_EQ(0, e);
}