#include "tests-base.hpp"

#include "utf8rewind.h"

TEST(Seek, InvalidDirection)
{
	const char* t = "Interesting";

	const char* r = utf8seek(t, t, 2, 33);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("Interesting", r);
}