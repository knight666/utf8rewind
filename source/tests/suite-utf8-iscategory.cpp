#include "tests-base.hpp"

#include "../helpers/helpers-strings.hpp"

TEST(Utf8IsCategory, BasicLatinSingle)
{
	const char* i = "L";
	size_t is = strlen(i);

	EXPECT_EQ(1, utf8iscategory(i, is, UTF8_CATEGORY_LETTER_UPPERCASE));
}

TEST(Utf8IsCategory, BasicLatinSingleMissing)
{
	const char* i = ")";
	size_t is = strlen(i);

	EXPECT_EQ(0, utf8iscategory(i, is, UTF8_CATEGORY_LETTER_LOWERCASE));
}

TEST(Utf8IsCategory, BasicLatinMultiple)
{
	const char* i = "123";
	size_t is = strlen(i);

	EXPECT_EQ(3, utf8iscategory(i, is, UTF8_CATEGORY_NUMBER_DECIMAL));
}

TEST(Utf8IsCategory, BasicLatinMultipleMissingAtStart)
{
	const char* i = "(good)";
	size_t is = strlen(i);

	EXPECT_EQ(0, utf8iscategory(i, is, UTF8_CATEGORY_LETTER_LOWERCASE));
}

TEST(Utf8IsCategory, BasicLatinMultipleMissingBeforeEnd)
{
	const char* i = "    %";
	size_t is = strlen(i);

	EXPECT_EQ(4, utf8iscategory(i, is, UTF8_CATEGORY_MARK_SPACING));
}

TEST(Utf8IsCategory, InvalidData)
{
	EXPECT_EQ(0, utf8iscategory(nullptr, 3, UTF8_CATEGORY_LETTER_UPPERCASE));
}

TEST(Utf8IsCategory, InvalidLength)
{
	EXPECT_EQ(0, utf8iscategory(nullptr, 3, UTF8_CATEGORY_LETTER_UPPERCASE));
}