#include "tests-base.hpp"

#include "utf8rewind.h"

TEST(ToUpper, Ascii)
{
	char* c = "Hello World!";
	const size_t s = 256;
	char d[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(12, utf8toupper(c, strlen(c), d, s, &errors));
	EXPECT_STREQ("HELLO WORLD!", d);
}