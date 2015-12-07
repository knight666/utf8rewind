#include "tests-base.hpp"

#include "utf8rewind.h"

#include "../helpers/helpers-locale.hpp"
#include "../helpers/helpers-strings.hpp"

class Utf8ToCaseFolded
	: public ::testing::Test
{

protected:

	void SetUp()
	{
		SET_LOCALE_ENGLISH();
	}

	void TearDown()
	{
		RESET_LOCALE();
	}

};

TEST_F(Utf8ToCaseFolded, BasicLatinSingleUppercase)
{
	const char* c = "G";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(1, utf8tocasefolded(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("g", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}