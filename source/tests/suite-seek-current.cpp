#include "tests-base.hpp"

#include "utf8rewind.h"

TEST(SeekCurrent, SwappedParameters)
{
	const char* t = "\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80";

	const char* r = utf8seek(t, t + strlen(t), 2, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x10480, o);
}

TEST(SeekCurrent, ZeroOffset)
{
	const char* t = "Banana";

	const char* r = utf8seek(t + 2, t, 0, SEEK_CUR);

	EXPECT_EQ(t + 2, r);
	EXPECT_STREQ("nana", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x6E, o);
}