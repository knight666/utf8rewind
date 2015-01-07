#include "tests-base.hpp"

extern "C" {
	#include "../internal/casemapping.h"
	#include "../internal/database.h"
}

#include "helpers-strings.hpp"

// Uppercase

TEST(CaseMappingExecuteUppercase, DecomposedNotEnoughSpace)
{
	int32_t e = 0;
	size_t s = 1;
	char d[2] = { 0 };
	char* dp = d;

	EXPECT_EQ(0, casemapping_execute(0x0000029E, &dp, &s, UnicodeProperty_Uppercase, &e));
	EXPECT_UTF8EQ("", d);
	EXPECT_EQ(d, dp);
	EXPECT_EQ(1, s);
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, e);
}

TEST(CaseMappingExecuteUppercase, UnaffectedNotEnoughSpace)
{
	int32_t e = 0;
	size_t s = 1;
	char d[2] = { 0 };
	char* dp = d;

	EXPECT_EQ(0, casemapping_execute(0x0000055E, &dp, &s, UnicodeProperty_Uppercase, &e));
	EXPECT_UTF8EQ("", d);
	EXPECT_EQ(d, dp);
	EXPECT_EQ(1, s);
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, e);
}

// Uppercase

TEST(CaseMappingExecuteUppercase, BasicLatinUppercase)
{
	int32_t e = 0;
	size_t s = 255;
	char d[256] = { 0 };
	char* dp = d;

	EXPECT_EQ(1, casemapping_execute(0x00000052, &dp, &s, UnicodeProperty_Uppercase, &e));
	EXPECT_UTF8EQ("R", d);
	EXPECT_EQ(d + 1, dp);
	EXPECT_EQ(254, s);
	EXPECT_EQ(0, e);
}

TEST(CaseMappingExecuteUppercase, BasicLatinLowercase)
{
	int32_t e = 0;
	size_t s = 255;
	char d[256] = { 0 };
	char* dp = d;

	EXPECT_EQ(1, casemapping_execute(0x00000065, &dp, &s, UnicodeProperty_Uppercase, &e));
	EXPECT_UTF8EQ("E", d);
	EXPECT_EQ(d + 1, dp);
	EXPECT_EQ(254, s);
	EXPECT_EQ(0, e);
}

TEST(CaseMappingExecuteUppercase, BasicLatinUnaffected)
{
	int32_t e = 0;
	size_t s = 255;
	char d[256] = { 0 };
	char* dp = d;

	EXPECT_EQ(1, casemapping_execute(0x0000005D, &dp, &s, UnicodeProperty_Uppercase, &e));
	EXPECT_UTF8EQ("]", d);
	EXPECT_EQ(d + 1, dp);
	EXPECT_EQ(254, s);
	EXPECT_EQ(0, e);
}

TEST(CaseMappingExecuteUppercase, TwoBytesUppercase)
{
	int32_t e = 0;
	size_t s = 255;
	char d[256] = { 0 };
	char* dp = d;

	EXPECT_EQ(2, casemapping_execute(0x00000546, &dp, &s, UnicodeProperty_Uppercase, &e));
	EXPECT_UTF8EQ("\xD5\x86", d);
	EXPECT_EQ(d + 2, dp);
	EXPECT_EQ(253, s);
	EXPECT_EQ(0, e);
}

TEST(CaseMappingExecuteUppercase, TwoBytesLowercase)
{
	int32_t e = 0;
	size_t s = 255;
	char d[256] = { 0 };
	char* dp = d;

	EXPECT_EQ(2, casemapping_execute(0x00000524, &dp, &s, UnicodeProperty_Uppercase, &e));
	EXPECT_UTF8EQ("\xD4\xA4", d);
	EXPECT_EQ(d + 2, dp);
	EXPECT_EQ(253, s);
	EXPECT_EQ(0, e);
}

TEST(CaseMappingExecuteUppercase, TwoBytesUnaffected)
{
	int32_t e = 0;
	size_t s = 255;
	char d[256] = { 0 };
	char* dp = d;

	EXPECT_EQ(2, casemapping_execute(0x00000482, &dp, &s, UnicodeProperty_Uppercase, &e));
	EXPECT_UTF8EQ("\xD2\x82", d);
	EXPECT_EQ(d + 2, dp);
	EXPECT_EQ(253, s);
	EXPECT_EQ(0, e);
}

TEST(CaseMappingExecuteUppercase, ThreeBytesUppercase)
{
	int32_t e = 0;
	size_t s = 255;
	char d[256] = { 0 };
	char* dp = d;

	EXPECT_EQ(3, casemapping_execute(0x000010C4, &dp, &s, UnicodeProperty_Uppercase, &e));
	EXPECT_UTF8EQ("\xE1\x83\x84", d);
	EXPECT_EQ(d + 3, dp);
	EXPECT_EQ(252, s);
	EXPECT_EQ(0, e);
}

TEST(CaseMappingExecuteUppercase, ThreeBytesLowercase)
{
	int32_t e = 0;
	size_t s = 255;
	char d[256] = { 0 };
	char* dp = d;

	EXPECT_EQ(3, casemapping_execute(0x00001E09, &dp, &s, UnicodeProperty_Uppercase, &e));
	EXPECT_UTF8EQ("\xE1\xB8\x88", d);
	EXPECT_EQ(d + 3, dp);
	EXPECT_EQ(252, s);
	EXPECT_EQ(0, e);
}

TEST(CaseMappingExecuteUppercase, ThreeBytesUnaffected)
{
	int32_t e = 0;
	size_t s = 255;
	char d[256] = { 0 };
	char* dp = d;

	EXPECT_EQ(3, casemapping_execute(0x00001FDD, &dp, &s, UnicodeProperty_Uppercase, &e));
	EXPECT_UTF8EQ("\xE1\xBF\x9D", d);
	EXPECT_EQ(d + 3, dp);
	EXPECT_EQ(252, s);
	EXPECT_EQ(0, e);
}

TEST(CaseMappingExecuteUppercase, FourBytesUppercase)
{
	int32_t e = 0;
	size_t s = 255;
	char d[256] = { 0 };
	char* dp = d;

	EXPECT_EQ(4, casemapping_execute(0x00010406, &dp, &s, UnicodeProperty_Uppercase, &e));
	EXPECT_UTF8EQ("\xF0\x90\x90\x86", d);
	EXPECT_EQ(d + 4, dp);
	EXPECT_EQ(251, s);
	EXPECT_EQ(0, e);
}

TEST(CaseMappingExecuteUppercase, FourBytesLowercase)
{
	int32_t e = 0;
	size_t s = 255;
	char d[256] = { 0 };
	char* dp = d;

	EXPECT_EQ(4, casemapping_execute(0x0001043C, &dp, &s, UnicodeProperty_Uppercase, &e));
	EXPECT_UTF8EQ("\xF0\x90\x90\x94", d);
	EXPECT_EQ(d + 4, dp);
	EXPECT_EQ(251, s);
	EXPECT_EQ(0, e);
}

TEST(CaseMappingExecuteUppercase, FourBytesUnaffected)
{
	int32_t e = 0;
	size_t s = 255;
	char d[256] = { 0 };
	char* dp = d;

	EXPECT_EQ(4, casemapping_execute(0x000118FF, &dp, &s, UnicodeProperty_Uppercase, &e));
	EXPECT_UTF8EQ("\xF0\x91\xA3\xBF", d);
	EXPECT_EQ(d + 4, dp);
	EXPECT_EQ(251, s);
	EXPECT_EQ(0, e);
}

TEST(CaseMappingExecuteLowercase, BasicLatinUppercase)
{
	int32_t e = 0;
	size_t s = 255;
	char d[256] = { 0 };
	char* dp = d;

	EXPECT_EQ(1, casemapping_execute(0x0000006D, &dp, &s, UnicodeProperty_Lowercase, &e));
	EXPECT_UTF8EQ("m", d);
	EXPECT_EQ(d + 1, dp);
	EXPECT_EQ(254, s);
	EXPECT_EQ(0, e);
}

TEST(CaseMappingExecuteLowercase, BasicLatinLowercase)
{
	int32_t e = 0;
	size_t s = 255;
	char d[256] = { 0 };
	char* dp = d;

	EXPECT_EQ(1, casemapping_execute(0x00000054, &dp, &s, UnicodeProperty_Lowercase, &e));
	EXPECT_UTF8EQ("t", d);
	EXPECT_EQ(d + 1, dp);
	EXPECT_EQ(254, s);
	EXPECT_EQ(0, e);
}

TEST(CaseMappingExecuteLowercase, BasicLatinUnaffected)
{
	int32_t e = 0;
	size_t s = 255;
	char d[256] = { 0 };
	char* dp = d;

	EXPECT_EQ(1, casemapping_execute(0x0000007D, &dp, &s, UnicodeProperty_Lowercase, &e));
	EXPECT_UTF8EQ("}", d);
	EXPECT_EQ(d + 1, dp);
	EXPECT_EQ(254, s);
	EXPECT_EQ(0, e);
}

TEST(CaseMappingExecuteLowercase, TwoBytesUppercase)
{
	int32_t e = 0;
	size_t s = 255;
	char d[256] = { 0 };
	char* dp = d;

	EXPECT_EQ(2, casemapping_execute(0x000000D8, &dp, &s, UnicodeProperty_Lowercase, &e));
	EXPECT_UTF8EQ("\xC3\xB8", d);
	EXPECT_EQ(d + 2, dp);
	EXPECT_EQ(253, s);
	EXPECT_EQ(0, e);
}

TEST(CaseMappingExecuteLowercase, TwoBytesLowercase)
{
	int32_t e = 0;
	size_t s = 255;
	char d[256] = { 0 };
	char* dp = d;

	EXPECT_EQ(2, casemapping_execute(0x000000EA, &dp, &s, UnicodeProperty_Lowercase, &e));
	EXPECT_UTF8EQ("\xC3\xAA", d);
	EXPECT_EQ(d + 2, dp);
	EXPECT_EQ(253, s);
	EXPECT_EQ(0, e);
}

TEST(CaseMappingExecuteLowercase, TwoBytesUnaffected)
{
	int32_t e = 0;
	size_t s = 255;
	char d[256] = { 0 };
	char* dp = d;

	EXPECT_EQ(2, casemapping_execute(0x000000D7, &dp, &s, UnicodeProperty_Lowercase, &e));
	EXPECT_UTF8EQ("\xC3\x97", d);
	EXPECT_EQ(d + 2, dp);
	EXPECT_EQ(253, s);
	EXPECT_EQ(0, e);
}

TEST(CaseMappingExecuteLowercase, ThreeBytesUppercase)
{
	int32_t e = 0;
	size_t s = 255;
	char d[256] = { 0 };
	char* dp = d;

	EXPECT_EQ(3, casemapping_execute(0x00001E94, &dp, &s, UnicodeProperty_Lowercase, &e));
	EXPECT_UTF8EQ("\xE1\xBA\x95", d);
	EXPECT_EQ(d + 3, dp);
	EXPECT_EQ(252, s);
	EXPECT_EQ(0, e);
}

TEST(CaseMappingExecuteLowercase, ThreeBytesLowercase)
{
	int32_t e = 0;
	size_t s = 255;
	char d[256] = { 0 };
	char* dp = d;

	EXPECT_EQ(3, casemapping_execute(0x00001E09, &dp, &s, UnicodeProperty_Lowercase, &e));
	EXPECT_UTF8EQ("\xE1\xB8\x89", d);
	EXPECT_EQ(d + 3, dp);
	EXPECT_EQ(252, s);
	EXPECT_EQ(0, e);
}

TEST(CaseMappingExecuteLowercase, ThreeBytesUnaffected)
{
	int32_t e = 0;
	size_t s = 255;
	char d[256] = { 0 };
	char* dp = d;

	EXPECT_EQ(3, casemapping_execute(0x00001FDE, &dp, &s, UnicodeProperty_Lowercase, &e));
	EXPECT_UTF8EQ("\xE1\xBF\x9E", d);
	EXPECT_EQ(d + 3, dp);
	EXPECT_EQ(252, s);
	EXPECT_EQ(0, e);
}

TEST(CaseMappingExecuteLowercase, FourBytesUppercase)
{
	int32_t e = 0;
	size_t s = 255;
	char d[256] = { 0 };
	char* dp = d;

	EXPECT_EQ(4, casemapping_execute(0x00010422, &dp, &s, UnicodeProperty_Lowercase, &e));
	EXPECT_UTF8EQ("\xF0\x90\x91\x8A", d);
	EXPECT_EQ(d + 4, dp);
	EXPECT_EQ(251, s);
	EXPECT_EQ(0, e);
}

TEST(CaseMappingExecuteLowercase, FourBytesLowercase)
{
	int32_t e = 0;
	size_t s = 255;
	char d[256] = { 0 };
	char* dp = d;

	EXPECT_EQ(4, casemapping_execute(0x0001044F, &dp, &s, UnicodeProperty_Lowercase, &e));
	EXPECT_UTF8EQ("\xF0\x90\x91\x8F", d);
	EXPECT_EQ(d + 4, dp);
	EXPECT_EQ(251, s);
	EXPECT_EQ(0, e);
}

TEST(CaseMappingExecuteLowercase, FourBytesUnaffected)
{
	int32_t e = 0;
	size_t s = 255;
	char d[256] = { 0 };
	char* dp = d;

	EXPECT_EQ(4, casemapping_execute(0x000118E1, &dp, &s, UnicodeProperty_Lowercase, &e));
	EXPECT_UTF8EQ("\xF0\x91\xA3\xA1", d);
	EXPECT_EQ(d + 4, dp);
	EXPECT_EQ(251, s);
	EXPECT_EQ(0, e);
}

// Titlecase

TEST(CaseMappingExecuteTitlecase, BasicLatinUppercase)
{
	int32_t e = 0;
	size_t s = 255;
	char d[256] = { 0 };
	char* dp = d;

	EXPECT_EQ(1, casemapping_execute(0x00000056, &dp, &s, UnicodeProperty_Titlecase, &e));
	EXPECT_UTF8EQ("V", d);
	EXPECT_EQ(d + 1, dp);
	EXPECT_EQ(254, s);
	EXPECT_EQ(0, e);
}

TEST(CaseMappingExecuteTitlecase, BasicLatinLowercase)
{
	int32_t e = 0;
	size_t s = 255;
	char d[256] = { 0 };
	char* dp = d;

	EXPECT_EQ(1, casemapping_execute(0x0000006F, &dp, &s, UnicodeProperty_Titlecase, &e));
	EXPECT_UTF8EQ("O", d);
	EXPECT_EQ(d + 1, dp);
	EXPECT_EQ(254, s);
	EXPECT_EQ(0, e);
}

TEST(CaseMappingExecuteTitlecase, BasicLatinUnaffected)
{
	int32_t e = 0;
	size_t s = 255;
	char d[256] = { 0 };
	char* dp = d;

	EXPECT_EQ(1, casemapping_execute(0x00000036, &dp, &s, UnicodeProperty_Titlecase, &e));
	EXPECT_UTF8EQ("6", d);
	EXPECT_EQ(d + 1, dp);
	EXPECT_EQ(254, s);
	EXPECT_EQ(0, e);
}

TEST(CaseMappingExecuteTitlecase, MultiByteUppercase)
{
	int32_t e = 0;
	size_t s = 255;
	char d[256] = { 0 };
	char* dp = d;

	EXPECT_EQ(2, casemapping_execute(0x000001F2, &dp, &s, UnicodeProperty_Titlecase, &e));
	EXPECT_UTF8EQ("\xC7\xB2", d);
	EXPECT_EQ(d + 2, dp);
	EXPECT_EQ(253, s);
	EXPECT_EQ(0, e);
}

TEST(CaseMappingExecuteTitlecase, MultiByteLowercase)
{
	int32_t e = 0;
	size_t s = 255;
	char d[256] = { 0 };
	char* dp = d;

	EXPECT_EQ(4, casemapping_execute(0x00000587, &dp, &s, UnicodeProperty_Titlecase, &e));
	EXPECT_UTF8EQ("\xD4\xB5\xD6\x82", d);
	EXPECT_EQ(d + 4, dp);
	EXPECT_EQ(251, s);
	EXPECT_EQ(0, e);
}

TEST(CaseMappingExecuteTitlecase, MultiByteTitlecase)
{
	int32_t e = 0;
	size_t s = 255;
	char d[256] = { 0 };
	char* dp = d;

	EXPECT_EQ(2, casemapping_execute(0x000001C8, &dp, &s, UnicodeProperty_Titlecase, &e));
	EXPECT_UTF8EQ("\xC7\x88", d);
	EXPECT_EQ(d + 2, dp);
	EXPECT_EQ(253, s);
	EXPECT_EQ(0, e);
}

TEST(CaseMappingExecuteTitlecase, MultiByteUnaffected)
{
	int32_t e = 0;
	size_t s = 255;
	char d[256] = { 0 };
	char* dp = d;

	EXPECT_EQ(2, casemapping_execute(0x000000F7, &dp, &s, UnicodeProperty_Titlecase, &e));
	EXPECT_UTF8EQ("\xC3\xB7", d);
	EXPECT_EQ(d + 2, dp);
	EXPECT_EQ(253, s);
	EXPECT_EQ(0, e);
}