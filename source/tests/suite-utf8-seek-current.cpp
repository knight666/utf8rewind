#include "tests-base.hpp"

#include "utf8rewind.h"

#include "helpers-strings.hpp"

TEST(Utf8SeekCurrent, SwappedParameters)
{
	const char* t = "\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80";

	const char* r = utf8seek(t, t + strlen(t), 2, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_UTF8EQ("\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80", r);
}

TEST(Utf8SeekCurrent, ZeroOffset)
{
	const char* t = "Banana";

	const char* r = utf8seek(t + 2, t, 0, SEEK_CUR);

	EXPECT_EQ(t + 2, r);
	EXPECT_UTF8EQ("nana", r);
}