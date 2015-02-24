#include "tests-base.hpp"

#include "utf8rewind.h"

extern "C" {
	#include "../internal/database.h"
};

#include "helpers-strings.hpp"

TEST(NormalizeCompose, BasicLatinSingle)
{
	/*
		U+004A
		     Y
		     0
	*/

	const char* i = "J";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_EQ(1, utf8normalize(i, is, o, os, UTF8_NORMALIZE_COMPOSE, &errors));
	EXPECT_UTF8EQ("J", o);
	EXPECT_EQ(0, errors);
}

TEST(NormalizeCompose, BasicLatinMultiple)
{
	/*
		U+0053 U+0063 U+0072 U+0065 U+0065 U+006E U+0073 U+0068 U+006F U+0074
		     Y      Y      Y      Y      Y      Y      Y      Y      Y      Y
		     0      0      0      0      0      0      0      0      0      0
	*/

	const char* i = "Screenshot";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_EQ(10, utf8normalize(i, is, o, os, UTF8_NORMALIZE_COMPOSE, &errors));
	EXPECT_UTF8EQ("Screenshot", o);
	EXPECT_EQ(0, errors);
}

TEST(NormalizeCompose, BasicLatinCompatibility)
{
	/*
		U+0061 U+0058 U+0058 U+006F
		     Y      Y      Y      Y
		     0      0      0      0
	*/

	const char* i = "aXXo";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_EQ(4, utf8normalize(i, is, o, os, UTF8_NORMALIZE_COMPOSE | UTF8_NORMALIZE_COMPATIBILITY, &errors));
	EXPECT_UTF8EQ("aXXo", o);
	EXPECT_EQ(0, errors);
}

TEST(NormalizeCompose, BasicLatinAmountOfBytes)
{
	/*
		U+0048 U+0061 U+006E U+0064 U+0073 U+006F U+006D U+0065
		     Y      Y      Y      Y      Y      Y      Y      Y
		     0      0      0      0      0      0      0      0
	*/

	const char* i = "Handsome";
	size_t is = strlen(i);
	int32_t errors = 0;

	EXPECT_EQ(8, utf8normalize(i, is, nullptr, 0, UTF8_NORMALIZE_COMPOSE | UTF8_NORMALIZE_COMPATIBILITY, &errors));
	EXPECT_EQ(0, errors);
}

TEST(NormalizeCompose, BasicLatinNotEnoughSpace)
{
	/*
		U+0041 U+0070 U+0070 U+006C U+0065
		     Y      Y      Y      Y      Y
		     0      0      0      0      0
	*/

	const char* i = "Apple";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 3;
	int32_t errors = 0;

	EXPECT_EQ(3, utf8normalize(i, is, o, os, UTF8_NORMALIZE_COMPOSE | UTF8_NORMALIZE_COMPATIBILITY, &errors));
	EXPECT_UTF8EQ("App", o);
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}