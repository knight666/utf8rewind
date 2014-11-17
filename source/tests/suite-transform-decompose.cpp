#include "tests-base.hpp"

#include "utf8rewind.h"

TEST(TransformDecompose, Decompose)
{
	const char* c = "Bj\xC3\xB6rn Zonderland";
	const size_t s = 512;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(18, utf8transform(c, strlen(c), b, s, UTF8_TRANSFORM_DECOMPOSED, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("Bjo\xCC\x88rn Zonderland", b);
}

TEST(TransformDecompose, DecomposeNotEnoughSpace)
{
	const char* c = "Am\xC3\x87zing";
	const size_t s = 7;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(6, utf8transform(c, strlen(c), b, s - 1, UTF8_TRANSFORM_DECOMPOSED, &errors));
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
	EXPECT_STREQ("AmC\xCC\xA7z", b);
}

TEST(TransformDecompose, DecomposeNotEnoughSpaceAtEnd)
{
	const char* c = "Brill\xC3\x95";
	const size_t s = 7;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(5, utf8transform(c, strlen(c), b, s - 1, UTF8_TRANSFORM_DECOMPOSED, &errors));
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
	EXPECT_STREQ("Brill", b);
}

TEST(TransformDecompose, DecomposeNotEnoughSpaceAtStart)
{
	const char* c = "\xE1\xB8\xAE";
	const size_t s = 4;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf8transform(c, strlen(c), b, s - 1, UTF8_TRANSFORM_DECOMPOSED, &errors));
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
	EXPECT_STREQ("", b);
}