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

TEST(Utf8IsNormalizedDecompose, MultiByteYesSingle)
{
	/*
		U+2205
		     Y
		     0
	*/

	const char* i = "\xE2\x88\x85";
	size_t is = strlen(i);
	size_t o = 0;

	EXPECT_EQ(UTF8_NORMALIZATION_RESULT_YES, utf8isnormalized(i, is, UTF8_NORMALIZE_DECOMPOSE, &o));
	EXPECT_EQ(3, o);
}

TEST(Utf8IsNormalizedDecompose, MultiByteYesMultiple)
{
	/*
		U+21A9 U+225D U+21F0 U+0338
		     Y      Y      Y      Y
		     0      0      0      0
	*/

	const char* i = "\xE2\x86\xA9\xE2\x89\x9D\xE2\x87\xB0\xCC\xB8";
	size_t is = strlen(i);
	size_t o = 0;

	EXPECT_EQ(UTF8_NORMALIZATION_RESULT_YES, utf8isnormalized(i, is, UTF8_NORMALIZE_DECOMPOSE, &o));
	EXPECT_EQ(11, o);
}

TEST(Utf8IsNormalizedDecompose, MultiByteYesUnordered)
{
	/*
		U+031A U+0F35 U+0328
		     Y      Y      Y
		   232    220    202
	*/

	const char* i = "\xCC\x9A\xE0\xBC\xB5\xCC\xA8";
	size_t is = strlen(i);
	size_t o = 0;

	EXPECT_EQ(UTF8_NORMALIZATION_RESULT_NO, utf8isnormalized(i, is, UTF8_NORMALIZE_DECOMPOSE, &o));
	EXPECT_EQ(7, o);
}

TEST(Utf8IsNormalizedDecompose, MultiByteYesCompatibility)
{
	/*
		U+027B U+05C0 U+07EB
		     Y      Y      Y
		     0      0    230
	*/

	const char* i = "\xC9\xBB\xD7\x80\xDF\xAB";
	size_t is = strlen(i);
	size_t o = 0;

	EXPECT_EQ(UTF8_NORMALIZATION_RESULT_YES, utf8isnormalized(i, is, UTF8_NORMALIZE_DECOMPOSE | UTF8_NORMALIZE_COMPATIBILITY, &o));
	EXPECT_EQ(6, o);
}