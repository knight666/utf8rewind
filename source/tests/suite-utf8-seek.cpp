#include "tests-base.hpp"

#include "utf8rewind.h"

#include "helpers-strings.hpp"

TEST(Utf8Seek, InvalidDirection)
{
	const char* t = "Interesting";

	const char* r = utf8seek(t, t, 2, 33);

	EXPECT_EQ(t, r);
	EXPECT_UTF8EQ("Interesting", r);
}