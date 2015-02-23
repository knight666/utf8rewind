#include "tests-base.hpp"

#include "utf8rewind.h"

extern "C" {
	#include "../internal/database.h"
};

#include "helpers-strings.hpp"

TEST(NormalizeDecomposition, BasicLatinSingle)
{
	/*
		U+007A
		     Y
		     0
	*/

	const char* i = "z";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_EQ(1, utf8normalize(i, is, o, os, UTF8_NORMALIZE_DECOMPOSE, &errors));
	EXPECT_UTF8EQ("z", o);
	EXPECT_EQ(0, errors);
}

TEST(NormalizeDecomposition, BasicLatinMultiple)
{
	/*
		U+0048 U+006F U+006D U+0065
		     Y      Y      Y      Y
		     0      0      0      0
	*/

	const char* i = "Home";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_EQ(4, utf8normalize(i, is, o, os, UTF8_NORMALIZE_DECOMPOSE, &errors));
	EXPECT_UTF8EQ("Home", o);
	EXPECT_EQ(0, errors);
}

TEST(NormalizeDecomposition, BasicLatinAmountOfBytes)
{
	/*
		U+0024 U+0020 U+0034 U+002E U+0032 U+0035 U+0020 U+0070 U+0065 U+0072 U+0020 U+0070 U+006F U+0075 U+006E U+0064
		     Y      Y      Y      Y      Y      Y      Y      Y      Y      Y      Y      Y      Y      Y      Y      Y
		     0      0      0      0      0      0      0      0      0      0      0      0      0      0      0      0
	*/

	const char* i = "$ 4.25 per pound";
	size_t is = strlen(i);
	int32_t errors = 0;

	EXPECT_EQ(16, utf8normalize(i, is, nullptr, 0, UTF8_NORMALIZE_DECOMPOSE, &errors));
	EXPECT_EQ(0, errors);
}

TEST(NormalizeDecomposition, BasicLatinNotEnoughSpace)
{
	/*
		U+0052 U+0061 U+0069 U+006E U+0079 U+0020 U+0064 U+0061 U+0079
		     Y      Y      Y      Y      Y      Y      Y      Y      Y
		     0      0      0      0      0      0      0      0      0
	*/

	const char* i = "Rainy day";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 4;
	int32_t errors = 0;

	EXPECT_EQ(4, utf8normalize(i, is, o, os, UTF8_NORMALIZE_DECOMPOSE, &errors));
	EXPECT_UTF8EQ("Rain", o);
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(NormalizeDecomposition, MultiByteUnaffectedSingle)
{
	/*
		U+00B5
		     Y
		     0
	*/

	const char* i = "\xC2\xB5";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_EQ(2, utf8normalize(i, is, o, os, UTF8_NORMALIZE_DECOMPOSE, &errors));
	EXPECT_UTF8EQ("\xC2\xB5", o);
	EXPECT_EQ(0, errors);
}

TEST(NormalizeDecomposition, MultiByteUnaffectedMultiple)
{
	/*
		U+1AA8 U+1A80 U+1A87
		     Y      Y      Y
		     0      0      0
	*/

	const char* i = "\xE1\xAA\xA8\xE1\xAA\x80\xE1\xAA\x87";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_EQ(9, utf8normalize(i, is, o, os, UTF8_NORMALIZE_DECOMPOSE, &errors));
	EXPECT_UTF8EQ("\xE1\xAA\xA8\xE1\xAA\x80\xE1\xAA\x87", o);
	EXPECT_EQ(0, errors);
}

TEST(NormalizeDecomposition, MultiByteUnaffectedAmountOfBytes)
{
	/*
		U+2181 U+2145 U+2086
		     Y      Y      Y
		     0      0      0
	*/

	const char* i = "\xE2\x86\x81\xE2\x85\x85\xE2\x82\x86";
	size_t is = strlen(i);
	int32_t errors = 0;

	EXPECT_EQ(9, utf8normalize(i, is, nullptr, 0, UTF8_NORMALIZE_DECOMPOSE, &errors));
	EXPECT_EQ(0, errors);
}

TEST(NormalizeDecomposition, MultiByteUnaffectedNotEnoughSpace)
{
	/*
		U+2035 U+2106 U+2090 U+2104
		     Y      Y      Y      Y
		     0      0      0      0
	*/

	const char* i = "\xE2\x80\xB5\xE2\x84\x86\xE2\x82\x90\xE2\x84\x84";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 8;
	int32_t errors = 0;

	EXPECT_EQ(6, utf8normalize(i, is, o, os, UTF8_NORMALIZE_DECOMPOSE, &errors));
	EXPECT_UTF8EQ("\xE2\x80\xB5\xE2\x84\x86", o);
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(NormalizeDecomposition, MultiByteDecomposeSingle)
{
	/*
		U+0958
		     N
		     0
	*/

	const char* i = "\xE0\xA5\x98";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_EQ(6, utf8normalize(i, is, o, os, UTF8_NORMALIZE_DECOMPOSE, &errors));
	EXPECT_UTF8EQ("\xE0\xA4\x95\xE0\xA4\xBC", o);
	EXPECT_EQ(0, errors);
}

TEST(NormalizeDecomposition, MultiByteDecomposeSingleAmountOfBytes)
{
	/*
		U+01FA
		     N
		     0
	*/

	const char* i = "\xC7\xBA";
	size_t is = strlen(i);
	int32_t errors = 0;

	EXPECT_EQ(5, utf8normalize(i, is, nullptr, 0, UTF8_NORMALIZE_DECOMPOSE, &errors));
	EXPECT_EQ(0, errors);
}

TEST(NormalizeDecomposition, MultiByteDecomposeSingleNotEnoughSpace)
{
	/*
		U+1E08
		     N
		     0
	*/

	const char* i = "\xE1\xB8\x88";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 4;
	int32_t errors = 0;

	EXPECT_EQ(3, utf8normalize(i, is, o, os, UTF8_NORMALIZE_DECOMPOSE, &errors));
	EXPECT_UTF8EQ("C\xCC\xA7", o);
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(NormalizeDecomposition, MultiByteDecomposeSequenceSingleOrdered)
{
	/*
		U+0108 U+0301
		     N      Y
		     0    230
	*/

	const char* i = "\xC4\x88\xCC\x81";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_EQ(5, utf8normalize(i, is, o, os, UTF8_NORMALIZE_DECOMPOSE, &errors));
	EXPECT_UTF8EQ("C\xCC\x82\xCC\x81", o);
	EXPECT_EQ(0, errors);
}

TEST(NormalizeDecomposition, MultiByteDecomposeSequenceSingleUnordered)
{
	/*
		U+0041 U+0304 U+031D
		     Y      Y      Y
		     0    230    220
	*/

	const char* i = "A\xCC\x84\xCC\x9D";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_EQ(5, utf8normalize(i, is, o, os, UTF8_NORMALIZE_DECOMPOSE, &errors));
	EXPECT_UTF8EQ("A\xCC\x9D\xCC\x84", o);
	EXPECT_EQ(0, errors);
}

TEST(NormalizeDecomposition, MultiByteDecomposeSequenceMultipleOrdered)
{
	/*
		U+0202 U+0315 U+038E U+0301
		     N      Y      N      Y
		     0    232      0    230
	*/

	const char* i = "\xC8\x82\xCC\x95\xCE\x8E\xCC\x81";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_EQ(11, utf8normalize(i, is, o, os, UTF8_NORMALIZE_DECOMPOSE, &errors));
	EXPECT_UTF8EQ("A\xCC\x91\xCC\x95\xCE\xA5\xCC\x81\xCC\x81", o);
	EXPECT_EQ(0, errors);
}

TEST(NormalizeDecomposition, MultiByteDecomposeSequenceMultipleUnordered)
{
	/*
		U+00CA U+0347 U+00C3 U+035C U+0348 U+00ED U+031B
		     N      Y      N      Y      Y      N      Y
		     0    220      0    233    220      0    216
	*/

	const char* i = "\xC3\x8A\xCD\x87\xC3\x83\xCD\x9C\xCD\x88\xC3\xAD\xCC\x9B";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_EQ(17, utf8normalize(i, is, o, os, UTF8_NORMALIZE_DECOMPOSE, &errors));
	EXPECT_UTF8EQ("E\xCD\x87\xCC\x82" "A\xCD\x88\xCC\x83\xCD\x9C" "i\xCC\x9B\xCC\x81", o);
	EXPECT_EQ(0, errors);
}

TEST(NormalizeDecomposition, MultiByteDecomposeSequenceAmountOfBytes)
{
	/*
		U+00C7 U+0301 U+0347
		     N      Y      Y
		     0    230    220
	*/

	const char* i = "\xC3\x87\xCC\x81\xCD\x87";
	size_t is = strlen(i);
	int32_t errors = 0;

	EXPECT_EQ(7, utf8normalize(i, is, nullptr, 0, UTF8_NORMALIZE_DECOMPOSE, &errors));
	EXPECT_EQ(0, errors);
}

TEST(NormalizeDecomposition, MultiByteDecomposeSequenceNotEnoughSpace)
{
	/*
		U+00C3 U+035C U+0348
		     N      Y      Y
		     0    233    220
	*/

	const char* i = "\xC3\x83\xCD\x9C\xCD\x88";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 6;
	int32_t errors = 0;

	EXPECT_EQ(5, utf8normalize(i, is, o, os, UTF8_NORMALIZE_DECOMPOSE, &errors));
	EXPECT_UTF8EQ("A\xCD\x88\xCC\x83", o);
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(NormalizeDecomposition, HangulUnaffectedSingle)
{
	/*
		U+11BD
		     Y
		     0
	*/

	const char* i = "\xE1\x86\xBD";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_EQ(3, utf8normalize(i, is, o, os, UTF8_NORMALIZE_DECOMPOSE, &errors));
	EXPECT_UTF8EQ("\xE1\x86\xBD", o);
	EXPECT_EQ(0, errors);
}

TEST(NormalizeDecomposition, HangulDecomposeSingleTwoCodepoints)
{
	/*
		U+AC70
		     N
		     0
	*/

	const char* i = "\xEA\xB1\xB0";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_EQ(6, utf8normalize(i, is, o, os, UTF8_NORMALIZE_DECOMPOSE, &errors));
	EXPECT_UTF8EQ("\xE1\x84\x80\xE1\x85\xA5", o);
	EXPECT_EQ(0, errors);
}

TEST(NormalizeDecomposition, HangulDecomposeSingleThreeCodepoints)
{
	/*
		U+AD83
		     N
		     0
	*/

	const char* i = "\xEA\xB6\x83";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_EQ(9, utf8normalize(i, is, o, os, UTF8_NORMALIZE_DECOMPOSE, &errors));
	EXPECT_UTF8EQ("\xE1\x84\x80\xE1\x85\xAE\xE1\x86\xBE", o);
	EXPECT_EQ(0, errors);
}

TEST(NormalizeDecomposition, HangulDecomposeSequenceMultiple)
{
	/*
		U+ACE0 U+1100 U+1169 U+11B0 U+ACFC
		     N      Y      Y      Y      N
		     0      0      0      0      0
	*/

	const char* i = "\xEA\xB3\xA0\xE1\x84\x80\xE1\x85\xA9\xE1\x86\xB0\xEA\xB3\xBC";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_EQ(21, utf8normalize(i, is, o, os, UTF8_NORMALIZE_DECOMPOSE, &errors));
	EXPECT_UTF8EQ("\xE1\x84\x80\xE1\x85\xA9\xE1\x84\x80\xE1\x85\xA9\xE1\x86\xB0\xE1\x84\x80\xE1\x85\xAA", o);
	EXPECT_EQ(0, errors);
}


TEST(NormalizeDecomposition, InvalidCodepointSingle)
{
	/*
		U+FFFD
		     Y
		     0
	*/

	const char* i = "\xF4";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_EQ(3, utf8normalize(i, is, o, os, UTF8_NORMALIZE_DECOMPOSE, &errors));
	EXPECT_UTF8EQ("\xEF\xBF\xBD", o);
	EXPECT_EQ(0, errors);
}

TEST(NormalizeDecomposition, InvalidCodepointMultiple)
{
	/*
		U+FFFD U+FFFD U+FFFD
		     Y      Y      Y
		     0      0      0
	*/

	const char* i = "\xEA\xF4\xC8";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_EQ(9, utf8normalize(i, is, o, os, UTF8_NORMALIZE_DECOMPOSE, &errors));
	EXPECT_UTF8EQ("\xEF\xBF\xBD\xEF\xBF\xBD\xEF\xBF\xBD", o);
	EXPECT_EQ(0, errors);
}


TEST(NormalizeDecomposition, MultipleDecompose)
{
	const char* i = "\xC5\x84\xC8\x8B\xC7\xBA";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_EQ(11, utf8normalize(i, is, o, os, UTF8_NORMALIZE_DECOMPOSE, &errors));
	EXPECT_UTF8EQ("n\xCC\x81" "i\xCC\x91" "A\xCC\x8A\xCC\x81", o);
	EXPECT_EQ(0, errors);
}

TEST(NormalizeDecomposition, MultipleNonStarter)
{
	const char* i = "\xCC\x97\xCC\x9B\xCC\x80";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_EQ(6, utf8normalize(i, is, o, os, UTF8_NORMALIZE_DECOMPOSE, &errors));
	EXPECT_UTF8EQ("\xCC\x9B\xCC\x97\xCC\x80", o);
	EXPECT_EQ(0, errors);
}

TEST(NormalizeDecomposition, AmountOfBytes)
{
	const char* i = "a\xD6\xAE\xDC\xB3\xCC\x80\xCC\x95" "b";
	size_t is = strlen(i);
	int32_t errors = 0;

	EXPECT_EQ(10, utf8normalize(i, is, nullptr, 0, UTF8_NORMALIZE_DECOMPOSE, &errors));
	EXPECT_EQ(0, errors);
}

TEST(NormalizeDecomposition, InvalidData)
{
	const char* i = nullptr;
	size_t is = 7;
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_EQ(0, utf8normalize(i, is, o, os, UTF8_NORMALIZE_DECOMPOSE, &errors));
	EXPECT_UTF8EQ("", o);
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(NormalizeDecomposition, NotEnoughSpace)
{
	const char* i = "a\xD6\xAE\xDC\xB3\xCC\x80\xCC\x95" "b";
	size_t is = strlen(i);
	char o[5] = { 0 };
	size_t os = 4;
	int32_t errors = 0;

	std::string seq = helpers::sequence(i, UnicodeProperty_Normalization_Decompose);

	EXPECT_EQ(3, utf8normalize(i, is, o, os, UTF8_NORMALIZE_DECOMPOSE, &errors));
	EXPECT_UTF8EQ("a\xD6\xAE", o);
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}