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

	EXPECT_EQ(UTF8_NORMALIZATION_RESULT_YES, utf8isnormalized(i, is, UTF8_NORMALIZE_COMPOSE));
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

	EXPECT_EQ(UTF8_NORMALIZATION_RESULT_YES, utf8isnormalized(i, is, UTF8_NORMALIZE_COMPOSE));
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

	EXPECT_EQ(UTF8_NORMALIZATION_RESULT_YES, utf8isnormalized(i, is, UTF8_NORMALIZE_COMPOSE | UTF8_NORMALIZE_COMPATIBILITY));
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

	EXPECT_EQ(UTF8_NORMALIZATION_RESULT_YES, utf8isnormalized(i, is, UTF8_NORMALIZE_COMPOSE));
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

	EXPECT_EQ(UTF8_NORMALIZATION_RESULT_YES, utf8isnormalized(i, is, UTF8_NORMALIZE_COMPOSE));
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

	EXPECT_EQ(UTF8_NORMALIZATION_RESULT_NO, utf8isnormalized(i, is, UTF8_NORMALIZE_COMPOSE));
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

	EXPECT_EQ(UTF8_NORMALIZATION_RESULT_YES, utf8isnormalized(i, is, UTF8_NORMALIZE_COMPOSE | UTF8_NORMALIZE_COMPATIBILITY));
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

	EXPECT_EQ(UTF8_NORMALIZATION_RESULT_MAYBE, utf8isnormalized(i, is, UTF8_NORMALIZE_COMPOSE));
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

	EXPECT_EQ(UTF8_NORMALIZATION_RESULT_MAYBE, utf8isnormalized(i, is, UTF8_NORMALIZE_COMPOSE));
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

	EXPECT_EQ(UTF8_NORMALIZATION_RESULT_MAYBE, utf8isnormalized(i, is, UTF8_NORMALIZE_COMPOSE));
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

	EXPECT_EQ(UTF8_NORMALIZATION_RESULT_MAYBE, utf8isnormalized(i, is, UTF8_NORMALIZE_COMPOSE));
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

	EXPECT_EQ(UTF8_NORMALIZATION_RESULT_NO, utf8isnormalized(i, is, UTF8_NORMALIZE_COMPOSE));
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

	EXPECT_EQ(UTF8_NORMALIZATION_RESULT_MAYBE, utf8isnormalized(i, is, UTF8_NORMALIZE_COMPOSE | UTF8_NORMALIZE_COMPATIBILITY));
}