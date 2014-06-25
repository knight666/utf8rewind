#include "tests-base.hpp"

#include "utf8rewind.h"

TEST(DecodeUtf16, Character)
{
	const char* i = "\xE0\xA4\x9C";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8decodeutf16(i, strlen(i), o, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x091C, o[0]);
}

TEST(DecodeUtf16, String)
{
	const char* i = "\xE0\xA4\x9C\xE0\xA4\xA1\xE0\xA4\xA4";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(6, utf8decodeutf16(i, strlen(i), o, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x091C, o[0]);
	EXPECT_EQ(0x0921, o[1]);
	EXPECT_EQ(0x0924, o[2]);
}

TEST(DecodeUtf16, SurrogatePair)
{
	const char* i = "\xF0\x9F\x98\xA4";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8decodeutf16(i, strlen(i), o, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0xD83D, o[0]);
	EXPECT_EQ(0xDE24, o[1]);
}