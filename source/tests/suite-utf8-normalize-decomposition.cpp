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

TEST(NormalizeDecomposition, SingleInvalidCodepoint)
{
	const char* i = "\xF4";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_EQ(3, utf8normalize(i, is, o, os, UTF8_NORMALIZE_DECOMPOSE, &errors));
	EXPECT_UTF8EQ("\xEF\xBF\xBD", o);
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

TEST(NormalizeDecomposition, MultipleBasicLatin)
{
	const char* i = "Home";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_EQ(4, utf8normalize(i, is, o, os, UTF8_NORMALIZE_DECOMPOSE, &errors));
	EXPECT_UTF8EQ("Home", o);
	EXPECT_EQ(0, errors);
}

TEST(NormalizeDecomposition, MultipleUnaffected)
{
	const char* i = "\xE1\xAA\xA8\xE1\xAA\x80\xE1\xAA\x87";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_EQ(9, utf8normalize(i, is, o, os, UTF8_NORMALIZE_DECOMPOSE, &errors));
	EXPECT_UTF8EQ("\xE1\xAA\xA8\xE1\xAA\x80\xE1\xAA\x87", o);
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

TEST(NormalizeDecomposition, MultipleInvalidCodepoints)
{
	const char* i = "\xEA\xF4\xC8";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_EQ(9, utf8normalize(i, is, o, os, UTF8_NORMALIZE_DECOMPOSE, &errors));
	EXPECT_UTF8EQ("\xEF\xBF\xBD\xEF\xBF\xBD\xEF\xBF\xBD", o);
	EXPECT_EQ(0, errors);
}

TEST(NormalizeDecomposition, MultipleSequenceOrdered)
{
	const char* i = "\xC8\x82\xCC\x95\xCE\x8E\xCC\x81";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_EQ(11, utf8normalize(i, is, o, os, UTF8_NORMALIZE_DECOMPOSE, &errors));
	EXPECT_UTF8EQ("A\xCC\x91\xCC\x95\xCE\xA5\xCC\x81\xCC\x81", o);
	EXPECT_EQ(0, errors);
}

TEST(NormalizeDecomposition, MultipleSequenceUnordered)
{
	const char* i = "\xC3\x8A\xCD\x87\xC3\x83\xCD\x9C\xCD\x88\xC3\xAD\xCC\x9B";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_EQ(17, utf8normalize(i, is, o, os, UTF8_NORMALIZE_DECOMPOSE, &errors));
	EXPECT_UTF8EQ("E\xCD\x87\xCC\x82" "A\xCD\x88\xCC\x83\xCD\x9C" "i\xCC\x9B\xCC\x81", o);
	EXPECT_EQ(0, errors);
}

TEST(NormalizeDecomposition, MultipleSequenceHangul)
{
	const char* i = "\xEA\xB3\xA0\xE1\x84\x80\xE1\x85\xA9\xE1\x86\xB0\xEA\xB3\xBC";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_EQ(21, utf8normalize(i, is, o, os, UTF8_NORMALIZE_DECOMPOSE, &errors));
	EXPECT_UTF8EQ("\xE1\x84\x80\xE1\x85\xA9\xE1\x84\x80\xE1\x85\xA9\xE1\x86\xB0\xE1\x84\x80\xE1\x85\xAA", o);
	EXPECT_EQ(0, errors);
}