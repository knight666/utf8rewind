#include "tests-base.hpp"

#include "utf8rewind.h"

extern "C" {
	#include "../internal/database.h"
};

#include "helpers-strings.hpp"

TEST(Utf8IsNormalized, InvalidInput)
{
	uint8_t r = 0;

	EXPECT_EQ(0, utf8isnormalized(nullptr, 15, UTF8_NORMALIZE_DECOMPOSE, &r));
	EXPECT_EQ(UTF8_NORMALIZATION_RESULT_YES, r);
}

TEST(Utf8IsNormalized, InvalidLength)
{
	uint8_t r = 0;

	EXPECT_EQ(0, utf8isnormalized("text", 0, UTF8_NORMALIZE_DECOMPOSE | UTF8_NORMALIZE_COMPATIBILITY, &r));
	EXPECT_EQ(UTF8_NORMALIZATION_RESULT_YES, r);
}

TEST(Utf8IsNormalized, InvalidFlag)
{
	/*
		U+030E U+027A U+1FC2
		   230      0      0
	*/

	const char* i = "\xCC\x8E\xC9\xBA\xE1\xBF\x82";
	size_t is = strlen(i);
	uint8_t r = 0;

	EXPECT_EQ(0, utf8isnormalized(i, is, 0x88000110, &r));
	EXPECT_EQ(UTF8_NORMALIZATION_RESULT_YES, r);
}

TEST(Utf8IsNormalized, MissingResultParameter)
{
	/*
		U+1229 U+0D3E U+0F52
		     Y      M      N
		     0      0      0
	*/

	const char* i = "\xE1\x88\xA9\xE0\xB4\xBE\xE0\xBD\x92";
	size_t is = strlen(i);

	EXPECT_EQ(3, utf8isnormalized(i, is, UTF8_NORMALIZE_COMPOSE | UTF8_NORMALIZE_COMPATIBILITY, nullptr));
}