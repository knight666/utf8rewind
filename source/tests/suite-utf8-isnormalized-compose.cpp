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