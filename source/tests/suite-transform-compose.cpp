#include "tests-base.hpp"

#include "utf8rewind.h"

#include "helpers-strings.hpp"

TEST(TransformCompose, Found)
{
	const char* c = "D\xCC\x87";
	const size_t s = 512;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8transform(c, strlen(c), b, s, UTF8_TRANSFORM_COMPOSED, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xE1\xB8\x8A", b);
}