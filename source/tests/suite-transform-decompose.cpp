#include "tests-base.hpp"

#include "utf8rewind.h"

TEST(TransformDecompose, Decompose)
{
	const char* input = "Bj\xC3\xB6rn Zonderland";
	size_t input_size = strlen(input);
	const size_t output_size = 512;
	char output[output_size] = { 0 };

	int32_t errors = 0;

	EXPECT_EQ(18, utf8transform(input, input_size, output, output_size, UTF8_TRANSFORM_DECOMPOSED, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("Bjo\xCC\x88rn Zonderland", output);
}