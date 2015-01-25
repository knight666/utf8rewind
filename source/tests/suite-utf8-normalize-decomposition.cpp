#include "tests-base.hpp"

#include "utf8rewind.h"

#include "helpers-strings.hpp"

TEST(NormalizeDecomposition, SingleBasicLatin)
{
	const char* i = "z";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_EQ(1, utf8normalize(i, is, o, os, UTF8_NORMALIZE_DECOMPOSE, &errors));
	EXPECT_UTF8EQ("z", o);
	EXPECT_EQ(0, errors);
}

TEST(NormalizeDecomposition, SingleUnaffected)
{
	const char* i = "\xC2\xB5";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_EQ(2, utf8normalize(i, is, o, os, UTF8_NORMALIZE_DECOMPOSE, &errors));
	EXPECT_UTF8EQ("\xC2\xB5", o);
	EXPECT_EQ(0, errors);
}

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

TEST(NormalizeDecomposition, SingleHangulUnaffected)
{
	const char* i = "\xE1\x86\xBD";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_EQ(3, utf8normalize(i, is, o, os, UTF8_NORMALIZE_DECOMPOSE, &errors));
	EXPECT_UTF8EQ("\xE1\x86\xBD", o);
	EXPECT_EQ(0, errors);
}

TEST(NormalizeDecomposition, SingleHangulDecomposeTwoCodepoints)
{
	const char* i = "\xEA\xB1\xB0";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_EQ(6, utf8normalize(i, is, o, os, UTF8_NORMALIZE_DECOMPOSE, &errors));
	EXPECT_UTF8EQ("\xE1\x84\x80\xE1\x85\xA5", o);
	EXPECT_EQ(0, errors);
}

TEST(NormalizeDecomposition, SingleHangulDecomposeThreeCodepoints)
{
	const char* i = "\xEA\xB6\x83";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_EQ(9, utf8normalize(i, is, o, os, UTF8_NORMALIZE_DECOMPOSE, &errors));
	EXPECT_UTF8EQ("\xE1\x84\x80\xE1\x85\xAE\xE1\x86\xBE", o);
	EXPECT_EQ(0, errors);
}

TEST(NormalizeDecomposition, SingleSequenceOrdered)
{
	const char* i = "\xC4\x88\xCC\x81";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_EQ(5, utf8normalize(i, is, o, os, UTF8_NORMALIZE_DECOMPOSE, &errors));
	EXPECT_UTF8EQ("C\xCC\x82\xCC\x81", o);
	EXPECT_EQ(0, errors);
}

TEST(NormalizeDecomposition, SingleSequenceUnordered)
{
	const char* i = "A\xCC\x84\xCC\x9D";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_EQ(5, utf8normalize(i, is, o, os, UTF8_NORMALIZE_DECOMPOSE, &errors));
	EXPECT_UTF8EQ("A\xCC\x9D\xCC\x84", o);
	EXPECT_EQ(0, errors);
}