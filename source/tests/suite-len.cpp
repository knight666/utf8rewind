#include "tests-base.hpp"

#include "utf8rewind.h"

TEST(Length, Valid)
{
	const char* c = "\xE0\xA4\x9C\xE0\xA4\xA1\xE0\xA4\xA4";

	EXPECT_EQ(3, utf8len(c));
}

TEST(Length, EndsInMiddle)
{
	const char* c = "Forest\0dweller";

	EXPECT_EQ(6, utf8len(c));
}

TEST(Length, OneByteCodepoint)
{
	const char* c = "\x30";

	EXPECT_EQ(1, utf8len(c));
}

TEST(Length, TwoByteCodepoint)
{
	const char* c = "\xC4\xB3";

	EXPECT_EQ(1, utf8len(c));
}

TEST(Length, TwoByteCodepointNotEnoughData)
{
	const char* c = "\xCC";

	EXPECT_EQ(SIZE_MAX, utf8len(c));
}

TEST(Length, ThreeByteCodepoint)
{
	const char* c = "\xE0\xAA\xBE";

	EXPECT_EQ(1, utf8len(c));
}

TEST(Length, ThreeByteCodepointNotEnoughData)
{
	const char* c = "\xE1\xB5";

	EXPECT_EQ(SIZE_MAX, utf8len(c));
}

TEST(Length, FourByteCodepoint)
{
	const char* c = "\xF0\x90\x86\x84";

	EXPECT_EQ(1, utf8len(c));
}

TEST(Length, FourByteCodepointNotEnoughData)
{
	const char* c = "\xF0\xDA";

	EXPECT_EQ(SIZE_MAX, utf8len(c));
}

TEST(Length, ZeroLength)
{
	const char* c = "";

	EXPECT_EQ(0, utf8len(c));
}

TEST(Length, NoData)
{
	EXPECT_EQ(0, utf8len(nullptr));
}