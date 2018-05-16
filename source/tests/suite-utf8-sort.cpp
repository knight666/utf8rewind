#include "tests-base.hpp"

TEST(Utf8Sort, Lesser)
{
	const char* l = "book";
	size_t ls = strlen(l);
	const char* r = "rack";
	size_t rs = strlen(r);
	int32_t errors = 1877;

	EXPECT_EQ(-1, utf8sort(l, ls, r, rs, UTF8_LOCALE_DEFAULT, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8Sort, Greater)
{
	const char* l = "medical";
	size_t ls = strlen(l);
	const char* r = "branch";
	size_t rs = strlen(r);
	int32_t errors = 33;

	EXPECT_EQ(1, utf8sort(l, ls, r, rs, UTF8_LOCALE_DEFAULT, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}