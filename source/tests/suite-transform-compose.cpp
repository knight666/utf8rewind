#include "tests-base.hpp"

#include "utf8rewind.h"

#include "helpers-strings.hpp"

TEST(TransformCompose, UnaffectedSingleCodepoint)
{
	// 0375

	const char* c = "\xCD\xB5";
	const size_t s = 512;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8transform(c, strlen(c), b, s - 1, UTF8_TRANSFORM_COMPOSED, &errors));
	EXPECT_UTF8EQ("\xCD\xB5", b);
	EXPECT_EQ(0, errors);
}

TEST(TransformCompose, UnaffectedMultipleCodepoints)
{
	// 0375

	const char* c = "\xCD\xB5";
	const size_t s = 512;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8transform(c, strlen(c), b, s - 1, UTF8_TRANSFORM_COMPOSED, &errors));
	EXPECT_UTF8EQ("\xCD\xB5", b);
	EXPECT_EQ(0, errors);
}

TEST(TransformCompose, InvalidSingleCodepoint)
{
	const char* c = "\xCC";
	const size_t s = 512;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8transform(c, strlen(c), b, s - 1, UTF8_TRANSFORM_COMPOSED, &errors));
	EXPECT_UTF8EQ("\xEF\xBF\xBD", b);
	EXPECT_EQ(0, errors);
}

TEST(TransformCompose, InvalidMultipleCodepoints)
{
	const char* c = "\xCD\xF0\x87\xC2\xFE";
	const size_t s = 512;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(12, utf8transform(c, strlen(c), b, s - 1, UTF8_TRANSFORM_COMPOSED, &errors));
	EXPECT_UTF8EQ("\xEF\xBF\xBD\xEF\xBF\xBD\xEF\xBF\xBD\xEF\xBF\xBD", b);
	EXPECT_EQ(0, errors);
}

TEST(TransformCompose, DecomposedSingleCodepoint)
{
	// 0151
	// 006F 030B
	// 0151

	const char* c = "\xC5\x91";
	const size_t s = 512;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8transform(c, strlen(c), b, s - 1, UTF8_TRANSFORM_COMPOSED, &errors));
	EXPECT_UTF8EQ("\xC5\x91", b);
	EXPECT_EQ(0, errors);
}

TEST(TransformCompose, DecomposedMultpleCodepoints)
{
	// 0121 016A 0390
	// 0067 0307 0055 0304 03B9 0308 0301
	// 0121 016A 0390

	const char* c = "\xC4\xA1\xC5\xAA\xCE\x90";
	const size_t s = 512;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(6, utf8transform(c, strlen(c), b, s - 1, UTF8_TRANSFORM_COMPOSED, &errors));
	EXPECT_UTF8EQ("\xC4\xA1\xC5\xAA\xCE\x90", b);
	EXPECT_EQ(0, errors);
}

TEST(TransformCompose, SequenceUnaffected)
{
	const char* c = "Boo";
	const size_t s = 512;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8transform(c, strlen(c), b, s - 1, UTF8_TRANSFORM_COMPOSED, &errors));
	EXPECT_UTF8EQ("Boo", b);
	EXPECT_EQ(0, errors);
}

TEST(TransformCompose, SequenceSingleComposedSingleCodepoint)
{
	// 00C6 0301
	// 01FC

	const char* c = "\xC3\x86\xCC\x81";
	const size_t s = 512;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8transform(c, strlen(c), b, s - 1, UTF8_TRANSFORM_COMPOSED, &errors));
	EXPECT_UTF8EQ("\xC7\xBC", b);
	EXPECT_EQ(0, errors);
}

TEST(TransformCompose, SequenceSingleComposedMultipleCodepoints)
{
	// 004C 0323 0304
	// 1E38

	const char* c = "L\xCC\xA3\xCC\x84";
	const size_t s = 512;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8transform(c, strlen(c), b, s - 1, UTF8_TRANSFORM_COMPOSED, &errors));
	EXPECT_UTF8EQ("\xE1\xB8\xB8", b);
	EXPECT_EQ(0, errors);
}

TEST(TransformCompose, SequenceSingleDecomposed)
{
	// 0228 0306
	// 0045 0327 0306
	// 1E1C

	const char* c = "\xC8\xA8\xCC\x86";
	const size_t s = 512;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8transform(c, strlen(c), b, s - 1, UTF8_TRANSFORM_COMPOSED, &errors));
	EXPECT_UTF8EQ("\xE1\xB8\x9C", b);
	EXPECT_EQ(0, errors);
}

TEST(TransformCompose, SequenceMultipleDecomposed)
{
	// 2244 1FB6 0345 1F68 0345
	// 2243 0338 03B1 0342 0345 03A9 0313 0345
	// 2244 1FB7 1FA8

	const char* c = "\xE2\x89\x84\xE1\xBE\xB6\xCD\x85\xE1\xBD\xA8\xCD\x85";
	const size_t s = 512;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(9, utf8transform(c, strlen(c), b, s - 1, UTF8_TRANSFORM_COMPOSED, &errors));
	EXPECT_UTF8EQ("\xE2\x89\x84\xE1\xBE\xB7\xE1\xBE\xA8", b);
	EXPECT_EQ(0, errors);
}