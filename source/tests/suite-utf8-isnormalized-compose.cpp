#include "tests-base.hpp"

#include "utf8rewind.h"

extern "C" {
	#include "../internal/database.h"
};

#include "helpers-strings.hpp"

TEST(Utf8IsNormalizedCompose, BasicLatinSingle)
{
	/*
		U+0026
		     Y
		     0
	*/

	const char* i = "&";
	size_t is = strlen(i);
	size_t o = 0;

	EXPECT_EQ(UTF8_NORMALIZATION_RESULT_YES, utf8isnormalized(i, is, UTF8_NORMALIZE_COMPOSE, &o));
	EXPECT_EQ(1, o);
}

TEST(Utf8IsNormalizedCompose, BasicLatinMultiple)
{
	/*
		U+0073 U+0077 U+006F U+0072 U+0064
		     Y      Y      Y      Y      Y
		     0      0      0      0      0
	*/

	const char* i = "sword";
	size_t is = strlen(i);
	size_t o = 0;

	EXPECT_EQ(UTF8_NORMALIZATION_RESULT_YES, utf8isnormalized(i, is, UTF8_NORMALIZE_COMPOSE, &o));
	EXPECT_EQ(5, o);
}

TEST(Utf8IsNormalizedCompose, BasicLatinCompatibility)
{
	/*
		U+0023 U+0079 U+006F U+006C U+006F
		     Y      Y      Y      Y      Y
		     0      0      0      0      0
	*/

	const char* i = "#yolo";
	size_t is = strlen(i);
	size_t o = 0;

	EXPECT_EQ(UTF8_NORMALIZATION_RESULT_YES, utf8isnormalized(i, is, UTF8_NORMALIZE_COMPOSE | UTF8_NORMALIZE_COMPATIBILITY, &o));
	EXPECT_EQ(5, o);
}

TEST(Utf8IsNormalizedCompose, MultiByteYesSingle)
{
	/*
		U+2B6C
		     Y
		     0
	*/

	const char* i = "\xE2\xAD\xAC";
	size_t is = strlen(i);
	size_t o = 0;

	EXPECT_EQ(UTF8_NORMALIZATION_RESULT_YES, utf8isnormalized(i, is, UTF8_NORMALIZE_COMPOSE, &o));
	EXPECT_EQ(3, o);
}

TEST(Utf8IsNormalizedCompose, MultiByteYesMultiple)
{
	/*
		U+2A6E U+A99F U+2923 U+23B9
		     Y      Y      Y      Y
		     0      0      0      0
	*/

	const char* i = "\xE2\xA9\xAE\xEA\xA6\x9F\xE2\xA4\xA3\xE2\x8E\xB9";
	size_t is = strlen(i);
	size_t o = 0;

	EXPECT_EQ(UTF8_NORMALIZATION_RESULT_YES, utf8isnormalized(i, is, UTF8_NORMALIZE_COMPOSE, &o));
	EXPECT_EQ(12, o);
}

TEST(Utf8IsNormalizedCompose, MultiByteYesUnordered)
{
	/*
		U+1A18 U+0F39 U+0EB9
		     Y      Y      Y
		   220    216    118
	*/

	const char* i = "\xE1\xA8\x98\xE0\xBC\xB9\xE0\xBA\xB9";
	size_t is = strlen(i);
	size_t o = 0;

	EXPECT_EQ(UTF8_NORMALIZATION_RESULT_NO, utf8isnormalized(i, is, UTF8_NORMALIZE_COMPOSE, &o));
	EXPECT_EQ(3, o);
}

TEST(Utf8IsNormalizedCompose, MultiByteYesCompatibility)
{
	/*
		U+20E0 U+20B0 U+2108
		     Y      Y      Y
		     0      0      0
	*/

	const char* i = "\xE2\x83\xA0\xE2\x82\xB0\xE2\x84\x88";
	size_t is = strlen(i);
	size_t o = 0;

	EXPECT_EQ(UTF8_NORMALIZATION_RESULT_YES, utf8isnormalized(i, is, UTF8_NORMALIZE_COMPOSE | UTF8_NORMALIZE_COMPATIBILITY, &o));
	EXPECT_EQ(9, o);
}

TEST(Utf8IsNormalizedCompose, MultiByteMaybeSingle)
{
	/*
		U+0323
		     M
		   220
	*/

	const char* i = "\xCC\xA3";
	size_t is = strlen(i);
	size_t o = 0;

	EXPECT_EQ(UTF8_NORMALIZATION_RESULT_MAYBE, utf8isnormalized(i, is, UTF8_NORMALIZE_COMPOSE, &o));
	EXPECT_EQ(0, o);
}

TEST(Utf8IsNormalizedCompose, MultiByteMaybeMultiple)
{
	/*
		U+0B56 U+0CD6 U+031B U+0301
		     M      M      M      M
		     0      0    216    230
	*/

	const char* i = "\xE0\xAD\x96\xE0\xB3\x96\xCC\x9B\xCC\x81";
	size_t is = strlen(i);
	size_t o = 0;

	EXPECT_EQ(UTF8_NORMALIZATION_RESULT_MAYBE, utf8isnormalized(i, is, UTF8_NORMALIZE_COMPOSE, &o));
	EXPECT_EQ(0, o);
}

TEST(Utf8IsNormalizedCompose, MultiByteMaybeSequenceStart)
{
	/*
		U+0342 U+0024 U+01D4
		     M      Y      Y
		   230      0      0
	*/

	const char* i = "\xCD\x82$\xC7\x94";
	size_t is = strlen(i);
	size_t o = 0;

	EXPECT_EQ(UTF8_NORMALIZATION_RESULT_MAYBE, utf8isnormalized(i, is, UTF8_NORMALIZE_COMPOSE, &o));
	EXPECT_EQ(0, o);
}

TEST(Utf8IsNormalizedCompose, MultiByteMaybeSequenceEnd)
{
	/*
		U+0157 U+0301
		     Y      M
		     0    230
	*/

	const char* i = "\xC5\x97\xCC\x81";
	size_t is = strlen(i);
	size_t o = 0;

	EXPECT_EQ(UTF8_NORMALIZATION_RESULT_MAYBE, utf8isnormalized(i, is, UTF8_NORMALIZE_COMPOSE, &o));
	EXPECT_EQ(2, o);
}

TEST(Utf8IsNormalizedCompose, MultiByteMaybeUnordered)
{
	/*
		U+0301 U+031B U+0328
		     M      M      M
		   230    216    202
	*/

	const char* i = "\xCC\x81\xCC\x9B\xCC\xA8";
	size_t is = strlen(i);
	size_t o = 0;

	EXPECT_EQ(UTF8_NORMALIZATION_RESULT_NO, utf8isnormalized(i, is, UTF8_NORMALIZE_COMPOSE, &o));
	EXPECT_EQ(0, o);
}

TEST(Utf8IsNormalizedCompose, MultiByteMaybeCompatibility)
{
	/*
		U+093C U+0C56 U+0328
		     M      M      M
		     7     91    202
	*/

	const char* i = "\xE0\xA4\xBC\xE0\xB1\x96\xCC\xA8";
	size_t is = strlen(i);
	size_t o = 0;

	EXPECT_EQ(UTF8_NORMALIZATION_RESULT_MAYBE, utf8isnormalized(i, is, UTF8_NORMALIZE_COMPOSE | UTF8_NORMALIZE_COMPATIBILITY, &o));
	EXPECT_EQ(0, o);
}

TEST(Utf8IsNormalizedCompose, MultiByteNoSingle)
{
	/*
		U+0F4D
		     N
		     0
	*/

	const char* i = "\xE0\xBD\x8D";
	size_t is = strlen(i);
	size_t o = 0;

	EXPECT_EQ(UTF8_NORMALIZATION_RESULT_NO, utf8isnormalized(i, is, UTF8_NORMALIZE_COMPOSE, &o));
	EXPECT_EQ(0, o);
}

TEST(Utf8IsNormalizedCompose, MultiByteNoMultiple)
{
	/*
		U+0374 U+FA26 U+FB34 U+0F76
		     N      N      N      N
		     0      0      0      0
	*/

	const char* i = "\xCD\xB4\xEF\xA8\xA6\xEF\xAC\xB4\xE0\xBD\xB6";
	size_t is = strlen(i);
	size_t o = 0;

	EXPECT_EQ(UTF8_NORMALIZATION_RESULT_NO, utf8isnormalized(i, is, UTF8_NORMALIZE_COMPOSE, &o));
	EXPECT_EQ(0, o);
}

TEST(Utf8IsNormalizedCompose, MultiByteNoSequenceStart)
{
	/*
		U+0F93 U+0F8F U+0F12
		     N      Y      Y
		     0      0      0
	*/

	const char* i = "\xE0\xBE\x93\xE0\xBE\x8F\xE0\xBC\x92";
	size_t is = strlen(i);
	size_t o = 0;

	EXPECT_EQ(UTF8_NORMALIZATION_RESULT_NO, utf8isnormalized(i, is, UTF8_NORMALIZE_COMPOSE, &o));
	EXPECT_EQ(0, o);
}

TEST(Utf8IsNormalizedCompose, MultiByteNoSequenceEnd)
{
	/*
		U+025C U+02D9 U+0341
		     Y      Y      N
		     0      0    230
	*/

	const char* i = "\xC9\x9C\xCB\x99\xCD\x81";
	size_t is = strlen(i);
	size_t o = 0;

	EXPECT_EQ(UTF8_NORMALIZATION_RESULT_NO, utf8isnormalized(i, is, UTF8_NORMALIZE_COMPOSE, &o));
	EXPECT_EQ(4, o);
}

TEST(Utf8IsNormalizedCompose, MultiByteNoUnordered)
{
	/*
		U+031A U+0315 U+0341
		     Y      Y      N
		   232    232    230
	*/

	const char* i = "\xCC\x9A\xCC\x95\xCD\x81";
	size_t is = strlen(i);
	size_t o = 0;

	EXPECT_EQ(UTF8_NORMALIZATION_RESULT_NO, utf8isnormalized(i, is, UTF8_NORMALIZE_COMPOSE, &o));
	EXPECT_EQ(4, o);
}

TEST(Utf8IsNormalizedCompose, MultiByteNoCompatibility)
{
	/*
		U+1D400 U+FE3F U+FF3E
		      N      N      N
		      0      0      0
	*/

	const char* i = "\xF0\x9D\x90\x80\xEF\xB8\xBF\xEF\xBC\xBE";
	size_t is = strlen(i);
	size_t o = 0;

	EXPECT_EQ(UTF8_NORMALIZATION_RESULT_NO, utf8isnormalized(i, is, UTF8_NORMALIZE_COMPOSE | UTF8_NORMALIZE_COMPATIBILITY, &o));
	EXPECT_EQ(0, o);
}