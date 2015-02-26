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

TEST(NormalizeCompose, MultiByteUnaffectedSingle)
{
	/*
		U+20D7
		     Y
		     0
	*/

	const char* i = "\xE2\x83\x97";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_EQ(3, utf8normalize(i, is, o, os, UTF8_NORMALIZE_COMPOSE, &errors));
	EXPECT_UTF8EQ("\xE2\x83\x97", o);
	EXPECT_EQ(0, errors);
}

TEST(NormalizeCompose, MultiByteUnaffectedMultiple)
{
	/*
		U+206F U+1E9F U+202B
		     Y      Y      Y
		     0      0      0
	*/

	const char* i = "\xE2\x81\xAF\xE1\xBA\x9F\xE2\x80\xAB";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_EQ(9, utf8normalize(i, is, o, os, UTF8_NORMALIZE_COMPOSE, &errors));
	EXPECT_UTF8EQ("\xE2\x81\xAF\xE1\xBA\x9F\xE2\x80\xAB", o);
	EXPECT_EQ(0, errors);
}

TEST(NormalizeCompose, MultiByteUnaffectedCompatibility)
{
	/*
		U+20A0 U+20AC U+20B0 U+20AF
		     Y      Y      Y      Y
		     0      0      0      0
	*/

	const char* i = "\xE2\x82\xA0\xE2\x82\xAC\xE2\x82\xB0\xE2\x82\xAF";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_EQ(12, utf8normalize(i, is, o, os, UTF8_NORMALIZE_COMPOSE | UTF8_NORMALIZE_COMPATIBILITY, &errors));
	EXPECT_UTF8EQ("\xE2\x82\xA0\xE2\x82\xAC\xE2\x82\xB0\xE2\x82\xAF", o);
	EXPECT_EQ(0, errors);
}

TEST(NormalizeCompose, MultiByteUnaffectedAmountOfBytes)
{
	/*
		U+211C U+211F U+2122
		     Y      Y      Y
		     0      0      0
	*/

	const char* i = "\xE2\x84\x9C\xE2\x84\x9F\xE2\x84\xA2";
	size_t is = strlen(i);
	int32_t errors = 0;

	EXPECT_EQ(9, utf8normalize(i, is, nullptr, 0, UTF8_NORMALIZE_COMPOSE, &errors));
	EXPECT_EQ(0, errors);
}

TEST(NormalizeCompose, MultiByteUnaffectedNotEnoughSpace)
{
	/*
		U+221A U+2215 U+2181 U+21D4
		     Y      Y      Y      Y
		     0      0      0      0
	*/

	const char* i = "\xE2\x88\x9A\xE2\x88\x95\xE2\x86\x81\xE2\x87\x94";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 6;
	int32_t errors = 0;

	EXPECT_EQ(6, utf8normalize(i, is, o, os, UTF8_NORMALIZE_COMPOSE, &errors));
	EXPECT_UTF8EQ("\xE2\x88\x9A\xE2\x88\x95", o);
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(NormalizeCompose, SequenceSingleComposeTwoCodepoints)
{
	/*
		U+0059 U+0301
		     Y      M
		     0    230
	*/

	const char* i = "Y\xCC\x81";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_EQ(2, utf8normalize(i, is, o, os, UTF8_NORMALIZE_COMPOSE, &errors));
	EXPECT_UTF8EQ("\xC3\x9D", o);
	EXPECT_EQ(0, errors);
}

TEST(NormalizeCompose, SequenceSingleComposeMultipleCodepoints)
{
	/*
		U+03C9 U+0313 U+0300 U+0345
		     Y      M      M      M
		     0    230    230    240
	*/

	const char* i = "\xCF\x89\xCC\x93\xCC\x80\xCD\x85";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_EQ(3, utf8normalize(i, is, o, os, UTF8_NORMALIZE_COMPOSE, &errors));
	EXPECT_UTF8EQ("\xE1\xBE\xA2", o);
	EXPECT_EQ(0, errors);
}

TEST(NormalizeCompose, SequenceSingleComposeUnordered)
{
	/*
		U+03A9 U+0313 U+0345 U+0651 U+0300
		     Y      M      M      Y      M
		     0    230    240     33    230
	*/

	const char* i = "\xCE\xA9\xCC\x93\xCD\x85\xD9\x91\xCC\x80";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_EQ(5, utf8normalize(i, is, o, os, UTF8_NORMALIZE_COMPOSE, &errors));
	EXPECT_UTF8EQ("\xE1\xBE\xAA\xD9\x91", o);
	EXPECT_EQ(0, errors);
}

TEST(NormalizeCompose, SequenceSingleDecomposeAndComposeOrdered)
{
	/*
		U+1F6A U+0345
		     Y      M
		     0    240
	*/

	const char* i = "\xE1\xBD\xAA\xCD\x85";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_EQ(3, utf8normalize(i, is, o, os, UTF8_NORMALIZE_COMPOSE, &errors));
	EXPECT_UTF8EQ("\xE1\xBE\xAA", o);
	EXPECT_EQ(0, errors);
}

TEST(NormalizeCompose, SequenceSingleDecomposeAndComposeUnordered)
{
	/*
		U+1E60 U+0323
		     Y      M
		     0    220
	*/

	const char* i = "\xE1\xB9\xA0\xCC\xA3";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_EQ(3, utf8normalize(i, is, o, os, UTF8_NORMALIZE_COMPOSE, &errors));
	EXPECT_UTF8EQ("\xE1\xB9\xA8", o);
	EXPECT_EQ(0, errors);
}

TEST(NormalizeCompose, SequenceSingleComposeCompatibility)
{
	/*
		U+0044 U+005A U+030C
		     Y      Y      M
		     0      0    230
	*/

	const char* i = "DZ\xCC\x8C";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_EQ(3, utf8normalize(i, is, o, os, UTF8_NORMALIZE_COMPOSE | UTF8_NORMALIZE_COMPATIBILITY, &errors));
	EXPECT_UTF8EQ("D\xC5\xBD", o);
	EXPECT_EQ(0, errors);
}

TEST(NormalizeCompose, SequenceSingleComposeAmountOfBytes)
{
	/*
		U+004C U+0323 U+0304
		     Y      M      M
		     0    220    230
	*/

	const char* i = "L\xCC\xA3\xCC\x84";
	size_t is = strlen(i);
	int32_t errors = 0;

	EXPECT_EQ(3, utf8normalize(i, is, nullptr, 0, UTF8_NORMALIZE_COMPOSE, &errors));
	EXPECT_EQ(0, errors);
}

TEST(NormalizeCompose, SequenceSingleComposeNotEnoughSpace)
{
	/*
		U+0073 U+030C U+0307
		     Y      M      M
		     0    230    230
	*/

	const char* i = "s\xCC\x8C\xCC\x87";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 2;
	int32_t errors = 0;

	EXPECT_EQ(0, utf8normalize(i, is, o, os, UTF8_NORMALIZE_COMPOSE, &errors));
	EXPECT_UTF8EQ("", o);
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(NormalizeCompose, SequenceMultipleComposeOrdered)
{
	/*
		U+03A9 U+0345 U+0397 U+0313 U+0300 U+0345 U+03B7 U+0313 U+0301
		     Y      M      Y      M      M      M      Y      M      M
		     0    240      0    230    230    240      0    230    230
	*/

	const char* i = "\xCE\xA9\xCD\x85\xCE\x97\xCC\x93\xCC\x80\xCD\x85\xCE\xB7\xCC\x93\xCC\x81";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_EQ(9, utf8normalize(i, is, o, os, UTF8_NORMALIZE_COMPOSE, &errors));
	EXPECT_UTF8EQ("\xE1\xBF\xBC\xE1\xBE\x9A\xE1\xBC\xA4", o);
	EXPECT_EQ(0, errors);
}

TEST(NormalizeCompose, SequenceMultipleComposeUnordered)
{
	/*
		U+00CA U+0323 U+1F80 U+0300 U+1FFC U+0314 U+0301
		     Y      M      Y      M      Y      M      M
		     0    220      0    230      0    230    230
	*/

	const char* i = "\xC3\x8A\xCC\xA3\xE1\xBE\x80\xCC\x80\xE1\xBF\xBC\xCC\x94\xCC\x81";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_EQ(9, utf8normalize(i, is, o, os, UTF8_NORMALIZE_COMPOSE, &errors));
	EXPECT_UTF8EQ("\xE1\xBB\x86\xE1\xBE\x82\xE1\xBE\xAD", o);
	EXPECT_EQ(0, errors);
}

TEST(NormalizeCompose, SequenceMultipleComposeCompatibility)
{
	/*
		U+110E U+1161 U+11B7 U+1100 U+1169 U+30C6 U+3099 U+0644 U+0627 U+0653
		     Y      M      M      Y      M      Y      M      Y      Y      M
		     0      0      0      0      0      0      8      0      0    230
	*/

	const char* i = "\xE1\x84\x8E\xE1\x85\xA1\xE1\x86\xB7\xE1\x84\x80\xE1\x85\xA9\xE3\x83\x86\xE3\x82\x99\xD9\x84\xD8\xA7\xD9\x93";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_EQ(13, utf8normalize(i, is, o, os, UTF8_NORMALIZE_COMPOSE | UTF8_NORMALIZE_COMPATIBILITY, &errors));
	EXPECT_UTF8EQ("\xEC\xB0\xB8\xEA\xB3\xA0\xE3\x83\x87\xD9\x84\xD8\xA2", o);
	EXPECT_EQ(0, errors);
}

TEST(NormalizeCompose, SequenceMultipleComposeAmountOfBytes)
{
	/*
		U+03B9 U+0304 U+03B1 U+0314 U+0345 U+0041 U+0302 U+0303
		     Y      M      Y      M      M      Y      M      M
		     0    230      0    230    240      0    230    230
	*/

	const char* i = "\xCE\xB9\xCC\x84\xCE\xB1\xCC\x94\xCD\x85" "A\xCC\x82\xCC\x83";
	size_t is = strlen(i);
	int32_t errors = 0;

	EXPECT_EQ(9, utf8normalize(i, is, nullptr, 0, UTF8_NORMALIZE_COMPOSE, &errors));
	EXPECT_EQ(0, errors);
}

TEST(NormalizeCompose, SequenceMultipleComposeNotEnoughSpace)
{
	/*
		U+006F U+0308 U+0304 U+0063 U+030C U+0055 U+031B U+0300
		     Y      M      M      Y      M      Y      M      M
		     0    230    230      0    230      0    216    230
	*/

	const char* i = "o\xCC\x88\xCC\x84" "c\xCC\x8CU\xCC\x9B\xCC\x80";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 3;
	int32_t errors = 0;

	EXPECT_EQ(2, utf8normalize(i, is, o, os, UTF8_NORMALIZE_COMPOSE, &errors));
	EXPECT_UTF8EQ("\xC8\xAB", o);
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}