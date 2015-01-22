#include "tests-base.hpp"

#include "utf8rewind.h"

#include "helpers-strings.hpp"

TEST(NormalizeDecomposition, SingleDecompose)
{
	const char* i = "\xE0\xA5\x98";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_EQ(6, utf8normalize(i, is, o, os, UTF8_NORMALIZE_DECOMPOSE, &errors));
	EXPECT_UTF8EQ("\xE0\xA4\x95\xE0\xA4\xBC", o);
	EXPECT_EQ(0, errors);
}