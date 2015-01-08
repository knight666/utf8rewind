#include "tests-base.hpp"

extern "C" {
	#include "../internal/casemapping.h"
	#include "../internal/database.h"
}

#include "helpers-strings.hpp"

TEST(CaseMappingExecute, NotEnoughSpaceBasicLatin)
{
	int32_t e = 0;
	size_t s = 0;
	char d[2] = { 0 };
	char* dp = d;
	uint8_t gc = database_queryproperty(0x00000073, UnicodeProperty_GeneralCategory);

	EXPECT_EQ(0, casemapping_execute(0x00000073, &dp, &s, gc, UnicodeProperty_Uppercase, &e));
	EXPECT_UTF8EQ("", d);
	EXPECT_EQ(d, dp);
	EXPECT_EQ(0, s);
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, e);
}

TEST(CaseMappingExecute, NotEnoughSpaceUnaffected)
{
	int32_t e = 0;
	size_t s = 1;
	char d[2] = { 0 };
	char* dp = d;
	uint8_t gc = database_queryproperty(0x0000211E, UnicodeProperty_GeneralCategory);

	EXPECT_EQ(0, casemapping_execute(0x0000211E, &dp, &s, gc, UnicodeProperty_Titlecase, &e));
	EXPECT_UTF8EQ("", d);
	EXPECT_EQ(d, dp);
	EXPECT_EQ(1, s);
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, e);
}

TEST(CaseMappingExecute, NotEnoughSpaceDecomposed)
{
	int32_t e = 0;
	size_t s = 1;
	char d[2] = { 0 };
	char* dp = d;
	uint8_t gc = database_queryproperty(0x00001F54, UnicodeProperty_GeneralCategory);

	EXPECT_EQ(0, casemapping_execute(0x00001F54, &dp, &s, gc, UnicodeProperty_Lowercase, &e));
	EXPECT_UTF8EQ("", d);
	EXPECT_EQ(d, dp);
	EXPECT_EQ(1, s);
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, e);
}

TEST(CaseMappingExecute, AmountOfBytesBasicLatin)
{
	int32_t e = 0;
	size_t s = 0;
	char* d = nullptr;
	uint8_t gc = database_queryproperty(0x00000042, UnicodeProperty_GeneralCategory);

	EXPECT_EQ(1, casemapping_execute(0x00000042, &d, &s, gc, UnicodeProperty_Titlecase, &e));
	EXPECT_EQ(0, e);
}

TEST(CaseMappingExecute, AmountOfBytesUnaffected)
{
	int32_t e = 0;
	size_t s = 0;
	char* d = nullptr;
	uint8_t gc = database_queryproperty(0x000000F7, UnicodeProperty_GeneralCategory);

	EXPECT_EQ(2, casemapping_execute(0x000000F7, &d, &s, gc, UnicodeProperty_Lowercase, &e));
	EXPECT_EQ(0, e);
}

TEST(CaseMappingExecute, AmountOfBytesDecomposed)
{
	int32_t e = 0;
	size_t s = 0;
	char* d = nullptr;
	uint8_t gc = database_queryproperty(0x000001F0, UnicodeProperty_GeneralCategory);

	EXPECT_EQ(2, casemapping_execute(0x000001F0, &d, &s, gc, UnicodeProperty_Lowercase, &e));
	EXPECT_EQ(0, e);
}

// Uppercase

TEST(CaseMappingExecuteUppercase, BasicLatinUppercase)
{
	int32_t e = 0;
	size_t s = 255;
	char d[256] = { 0 };
	char* dp = d;
	uint8_t gc = database_queryproperty(0x00000052, UnicodeProperty_GeneralCategory);

	EXPECT_EQ(1, casemapping_execute(0x00000052, &dp, &s, gc, UnicodeProperty_Uppercase, &e));
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
	uint8_t gc = database_queryproperty(0x00000065, UnicodeProperty_GeneralCategory);

	EXPECT_EQ(1, casemapping_execute(0x00000065, &dp, &s, gc, UnicodeProperty_Uppercase, &e));
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
	uint8_t gc = database_queryproperty(0x0000005D, UnicodeProperty_GeneralCategory);

	EXPECT_EQ(1, casemapping_execute(0x0000005D, &dp, &s, gc, UnicodeProperty_Uppercase, &e));
	EXPECT_UTF8EQ("]", d);
	EXPECT_EQ(d + 1, dp);
	EXPECT_EQ(254, s);
	EXPECT_EQ(0, e);
}

TEST(CaseMappingExecuteUppercase, MultiByteUppercase)
{
	int32_t e = 0;
	size_t s = 255;
	char d[256] = { 0 };
	char* dp = d;
	uint8_t gc = database_queryproperty(0x00010406, UnicodeProperty_GeneralCategory);

	EXPECT_EQ(4, casemapping_execute(0x00010406, &dp, &s, gc, UnicodeProperty_Uppercase, &e));
	EXPECT_UTF8EQ("\xF0\x90\x90\x86", d);
	EXPECT_EQ(d + 4, dp);
	EXPECT_EQ(251, s);
	EXPECT_EQ(0, e);
}

TEST(CaseMappingExecuteUppercase, MultiByteLowercase)
{
	int32_t e = 0;
	size_t s = 255;
	char d[256] = { 0 };
	char* dp = d;
	uint8_t gc = database_queryproperty(0x00001FD2, UnicodeProperty_GeneralCategory);

	EXPECT_EQ(6, casemapping_execute(0x00001FD2, &dp, &s, gc, UnicodeProperty_Uppercase, &e));
	EXPECT_UTF8EQ("\xCE\x99\xCC\x88\xCC\x80", d);
	EXPECT_EQ(d + 6, dp);
	EXPECT_EQ(249, s);
	EXPECT_EQ(0, e);
}

TEST(CaseMappingExecuteUppercase, MultiByteTitlecase)
{
	int32_t e = 0;
	size_t s = 255;
	char d[256] = { 0 };
	char* dp = d;
	uint8_t gc = database_queryproperty(0x000001C8, UnicodeProperty_GeneralCategory);

	EXPECT_EQ(2, casemapping_execute(0x000001C8, &dp, &s, gc, UnicodeProperty_Uppercase, &e));
	EXPECT_UTF8EQ("\xC7\x87", d);
	EXPECT_EQ(d + 2, dp);
	EXPECT_EQ(253, s);
	EXPECT_EQ(0, e);
}

TEST(CaseMappingExecuteUppercase, MultiByteUnaffected)
{
	int32_t e = 0;
	size_t s = 255;
	char d[256] = { 0 };
	char* dp = d;
	uint8_t gc = database_queryproperty(0x00000482, UnicodeProperty_GeneralCategory);

	EXPECT_EQ(2, casemapping_execute(0x00000482, &dp, &s, gc, UnicodeProperty_Uppercase, &e));
	EXPECT_UTF8EQ("\xD2\x82", d);
	EXPECT_EQ(d + 2, dp);
	EXPECT_EQ(253, s);
	EXPECT_EQ(0, e);
}

// Lowercase

TEST(CaseMappingExecuteLowercase, BasicLatinUppercase)
{
	int32_t e = 0;
	size_t s = 255;
	char d[256] = { 0 };
	char* dp = d;
	uint8_t gc = database_queryproperty(0x0000006D, UnicodeProperty_GeneralCategory);

	EXPECT_EQ(1, casemapping_execute(0x0000006D, &dp, &s, gc, UnicodeProperty_Lowercase, &e));
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
	uint8_t gc = database_queryproperty(0x00000054, UnicodeProperty_GeneralCategory);

	EXPECT_EQ(1, casemapping_execute(0x00000054, &dp, &s, gc, UnicodeProperty_Lowercase, &e));
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
	uint8_t gc = database_queryproperty(0x0000007D, UnicodeProperty_GeneralCategory);

	EXPECT_EQ(1, casemapping_execute(0x0000007D, &dp, &s, gc, UnicodeProperty_Lowercase, &e));
	EXPECT_UTF8EQ("}", d);
	EXPECT_EQ(d + 1, dp);
	EXPECT_EQ(254, s);
	EXPECT_EQ(0, e);
}

TEST(CaseMappingExecuteLowercase, MultiByteUppercase)
{
	int32_t e = 0;
	size_t s = 255;
	char d[256] = { 0 };
	char* dp = d;
	uint8_t gc = database_queryproperty(0x000000D8, UnicodeProperty_GeneralCategory);

	EXPECT_EQ(2, casemapping_execute(0x000000D8, &dp, &s, gc, UnicodeProperty_Lowercase, &e));
	EXPECT_UTF8EQ("\xC3\xB8", d);
	EXPECT_EQ(d + 2, dp);
	EXPECT_EQ(253, s);
	EXPECT_EQ(0, e);
}

TEST(CaseMappingExecuteLowercase, MultiByteLowercase)
{
	int32_t e = 0;
	size_t s = 255;
	char d[256] = { 0 };
	char* dp = d;
	uint8_t gc = database_queryproperty(0x00001E09, UnicodeProperty_GeneralCategory);

	EXPECT_EQ(3, casemapping_execute(0x00001E09, &dp, &s, gc, UnicodeProperty_Lowercase, &e));
	EXPECT_UTF8EQ("\xE1\xB8\x89", d);
	EXPECT_EQ(d + 3, dp);
	EXPECT_EQ(252, s);
	EXPECT_EQ(0, e);
}

TEST(CaseMappingExecuteLowercase, MultiByteTitlecase)
{
	int32_t e = 0;
	size_t s = 255;
	char d[256] = { 0 };
	char* dp = d;
	uint8_t gc = database_queryproperty(0x00001FFC, UnicodeProperty_GeneralCategory);

	EXPECT_EQ(3, casemapping_execute(0x00001FFC, &dp, &s, gc, UnicodeProperty_Lowercase, &e));
	EXPECT_UTF8EQ("\xE1\xBF\xB3", d);
	EXPECT_EQ(d + 3, dp);
	EXPECT_EQ(252, s);
	EXPECT_EQ(0, e);
}

TEST(CaseMappingExecuteLowercase, MultiByteUnaffected)
{
	int32_t e = 0;
	size_t s = 255;
	char d[256] = { 0 };
	char* dp = d;
	uint8_t gc = database_queryproperty(0x000118E1, UnicodeProperty_GeneralCategory);

	EXPECT_EQ(4, casemapping_execute(0x000118E1, &dp, &s, gc, UnicodeProperty_Lowercase, &e));
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
	uint8_t gc = database_queryproperty(0x00000056, UnicodeProperty_GeneralCategory);

	EXPECT_EQ(1, casemapping_execute(0x00000056, &dp, &s, gc, UnicodeProperty_Titlecase, &e));
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
	uint8_t gc = database_queryproperty(0x0000006F, UnicodeProperty_GeneralCategory);

	EXPECT_EQ(1, casemapping_execute(0x0000006F, &dp, &s, gc, UnicodeProperty_Titlecase, &e));
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
	uint8_t gc = database_queryproperty(0x00000036, UnicodeProperty_GeneralCategory);

	EXPECT_EQ(1, casemapping_execute(0x00000036, &dp, &s, gc, UnicodeProperty_Titlecase, &e));
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
	uint8_t gc = database_queryproperty(0x000001F2, UnicodeProperty_GeneralCategory);

	EXPECT_EQ(2, casemapping_execute(0x000001F2, &dp, &s, gc, UnicodeProperty_Titlecase, &e));
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
	uint8_t gc = database_queryproperty(0x00000587, UnicodeProperty_GeneralCategory);

	EXPECT_EQ(4, casemapping_execute(0x00000587, &dp, &s, gc, UnicodeProperty_Titlecase, &e));
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
	uint8_t gc = database_queryproperty(0x000001C8, UnicodeProperty_GeneralCategory);

	EXPECT_EQ(2, casemapping_execute(0x000001C8, &dp, &s, gc, UnicodeProperty_Titlecase, &e));
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
	uint8_t gc = database_queryproperty(0x000000F7, UnicodeProperty_GeneralCategory);

	EXPECT_EQ(2, casemapping_execute(0x000000F7, &dp, &s, gc, UnicodeProperty_Titlecase, &e));
	EXPECT_UTF8EQ("\xC3\xB7", d);
	EXPECT_EQ(d + 2, dp);
	EXPECT_EQ(253, s);
	EXPECT_EQ(0, e);
}