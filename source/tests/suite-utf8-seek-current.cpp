#include "tests-base.hpp"

#include "utf8rewind.h"

#include "helpers-seeking.hpp"

TEST(Utf8SeekCurrent, SwappedParameters)
{
	const char* t = "\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80";

	EXPECT_SEEKEQ(t, 0, 0, strlen(t), 0, SEEK_CUR);
}

TEST(Utf8SeekCurrent, ZeroOffset)
{
	const char* t = "Banana";

	EXPECT_SEEKEQ(t, 0, 0, 0, 0, SEEK_CUR);
}