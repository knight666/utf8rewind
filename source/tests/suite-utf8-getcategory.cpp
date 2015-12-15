#include "tests-base.hpp"

#include "../helpers/helpers-strings.hpp"

TEST(Utf8GetCategory, BasicLatinSingle)
{
	const char* i = "L";
	size_t is = strlen(i);

	EXPECT_TRUE(utf8iscategory(i, is, UTF8_CATEGORY_LETTER_UPPERCASE));
}