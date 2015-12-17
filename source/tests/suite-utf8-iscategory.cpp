#include "tests-base.hpp"

#include "../helpers/helpers-strings.hpp"

TEST(Utf8IsCategory, BasicLatinSingleLetterUppercase)
{
	const char* i = "L";
	size_t is = strlen(i);

	EXPECT_EQ(1, utf8iscategory(i, is, UTF8_CATEGORY_LETTER_UPPERCASE));
}

TEST(Utf8IsCategory, BasicLatinSingleLetterUppercaseMissing)
{
	const char* i = "^";
	size_t is = strlen(i);

	EXPECT_EQ(0, utf8iscategory(i, is, UTF8_CATEGORY_LETTER_UPPERCASE));
}

TEST(Utf8IsCategory, BasicLatinMultipleLetterUppercase)
{
	const char* i = "KIP";
	size_t is = strlen(i);

	EXPECT_EQ(3, utf8iscategory(i, is, UTF8_CATEGORY_LETTER_UPPERCASE));
}

TEST(Utf8IsCategory, BasicLatinMultipleLetterUppercaseMissingAtStart)
{
	const char* i = "1048";
	size_t is = strlen(i);

	EXPECT_EQ(0, utf8iscategory(i, is, UTF8_CATEGORY_LETTER_UPPERCASE));
}

TEST(Utf8IsCategory, BasicLatinMultipleLetterUppercaseMissingBeforeEnd)
{
	const char* i = "DIn1";
	size_t is = strlen(i);

	EXPECT_EQ(2, utf8iscategory(i, is, UTF8_CATEGORY_LETTER_UPPERCASE));
}

TEST(Utf8IsCategory, InvalidData)
{
	EXPECT_EQ(0, utf8iscategory(nullptr, 3, UTF8_CATEGORY_LETTER_UPPERCASE));
}

TEST(Utf8IsCategory, InvalidLength)
{
	EXPECT_EQ(0, utf8iscategory(nullptr, 3, UTF8_CATEGORY_LETTER_UPPERCASE));
}