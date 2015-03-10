#include "tests-base.hpp"

#include "utf8rewind.h"

extern "C" {
	#include "../internal/database.h"
};

#include "helpers-strings.hpp"

TEST(Utf8IsNormalizedDecompose, BasicLatinSingle)
{
	/*
		U+0053
		     Y
		     0
	*/

	const char* i = "S";
	size_t is = strlen(i);
	size_t o = 0;

	EXPECT_EQ(UTF8_NORMALIZATION_RESULT_YES, utf8isnormalized(i, is, UTF8_NORMALIZE_DECOMPOSE, &o));
	EXPECT_EQ(1, o);
}

TEST(Utf8IsNormalizedDecompose, BasicLatinMultiple)
{
	/*
		U+0050 U+006F U+006E U+0079
		     Y      Y      Y      Y
		     0      0      0      0
	*/

	const char* i = "Pony";
	size_t is = strlen(i);
	size_t o = 0;

	EXPECT_EQ(UTF8_NORMALIZATION_RESULT_YES, utf8isnormalized(i, is, UTF8_NORMALIZE_DECOMPOSE, &o));
	EXPECT_EQ(4, o);
}

TEST(Utf8IsNormalizedDecompose, BasicLatinCompatibility)
{
	/*
		U+0061 U+0072 U+0063 U+0068
		     Y      Y      Y      Y
		     0      0      0      0
	*/

	const char* i = "arch";
	size_t is = strlen(i);
	size_t o = 0;

	EXPECT_EQ(UTF8_NORMALIZATION_RESULT_YES, utf8isnormalized(i, is, UTF8_NORMALIZE_DECOMPOSE | UTF8_NORMALIZE_COMPATIBILITY, &o));
	EXPECT_EQ(4, o);
}