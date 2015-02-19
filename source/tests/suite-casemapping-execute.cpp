#include "tests-base.hpp"

extern "C" {
	#include "../internal/casemapping.h"
	#include "../internal/database.h"
}

#include "helpers-strings.hpp"

TEST(CaseMappingExecute, Initialize)
{
	CaseMappingState state;
	const char* i = "Greetings";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Titlecase));
	EXPECT_EQ(i, state.src);
	EXPECT_EQ(is, state.src_size);
	EXPECT_EQ(o, state.dst);
	EXPECT_EQ(os, state.dst_size);
	EXPECT_EQ(UnicodeProperty_Titlecase, state.property);
}

TEST(CaseMappingExecute, InitializeInvalidData)
{
	CaseMappingState state;
	char o[256] = { 0 };
	size_t os = 255;

	EXPECT_FALSE(casemapping_initialize(&state, nullptr, 16, o, os, UnicodeProperty_Uppercase));
}

TEST(CaseMappingExecute, InitializeInvalidLength)
{
	CaseMappingState state;
	char o[256] = { 0 };
	size_t os = 255;

	EXPECT_FALSE(casemapping_initialize(&state, "Picnic", 0, o, os, UnicodeProperty_Lowercase));
}

TEST(CaseMappingExecute, BasicLatinSingleLowercase)
{
	CaseMappingState state;
	const char* i = "R";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Lowercase));

	EXPECT_EQ(1, casemapping_execute2(&state));
	EXPECT_EQ(i + 1, state.src);
	EXPECT_EQ(is - 1, state.src_size);
	EXPECT_EQ(o + 1, state.dst);
	EXPECT_EQ(os - 1, state.dst_size);

	EXPECT_EQ(0, casemapping_execute2(&state));

	EXPECT_UTF8EQ("r", o);
}

TEST(CaseMappingExecute, BasicLatinSingleUppercase)
{
	CaseMappingState state;
	const char* i = "i";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Uppercase));

	EXPECT_EQ(1, casemapping_execute2(&state));
	EXPECT_EQ(i + 1, state.src);
	EXPECT_EQ(is - 1, state.src_size);
	EXPECT_EQ(o + 1, state.dst);
	EXPECT_EQ(os - 1, state.dst_size);

	EXPECT_EQ(0, casemapping_execute2(&state));

	EXPECT_UTF8EQ("I", o);
}

TEST(CaseMappingExecute, BasicLatinSingleTitlecase)
{
	CaseMappingState state;
	const char* i = "v";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Titlecase));

	EXPECT_EQ(1, casemapping_execute2(&state));
	EXPECT_EQ(i + 1, state.src);
	EXPECT_EQ(is - 1, state.src_size);
	EXPECT_EQ(o + 1, state.dst);
	EXPECT_EQ(os - 1, state.dst_size);

	EXPECT_EQ(0, casemapping_execute2(&state));

	EXPECT_UTF8EQ("V", o);
}

TEST(CaseMappingExecute, BasicLatinSingleUnaffected)
{
	CaseMappingState state;
	const char* i = "`";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Uppercase));

	EXPECT_EQ(1, casemapping_execute2(&state));
	EXPECT_EQ(i + 1, state.src);
	EXPECT_EQ(is - 1, state.src_size);
	EXPECT_EQ(o + 1, state.dst);
	EXPECT_EQ(os - 1, state.dst_size);

	EXPECT_EQ(0, casemapping_execute2(&state));

	EXPECT_UTF8EQ("`", o);
}

TEST(CaseMappingExecute, BasicLatinSingleAmountOfBytes)
{
	CaseMappingState state;
	const char* i = "!";
	size_t is = strlen(i);

	EXPECT_TRUE(casemapping_initialize(&state, i, is, nullptr, 0, UnicodeProperty_Uppercase));

	EXPECT_EQ(1, casemapping_execute2(&state));
	EXPECT_EQ(i + 1, state.src);
	EXPECT_EQ(is - 1, state.src_size);
	EXPECT_EQ(nullptr, state.dst);
	EXPECT_EQ(0, state.dst_size);

	EXPECT_EQ(0, casemapping_execute2(&state));
}

TEST(CaseMappingExecute, BasicLatinSingleNotEnoughSpace)
{
	CaseMappingState state;
	const char* i = "^";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 0;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Uppercase));

	EXPECT_EQ(0, casemapping_execute2(&state));
	EXPECT_EQ(i, state.src);
	EXPECT_EQ(is - 1, state.src_size);
	EXPECT_EQ(o, state.dst);
	EXPECT_EQ(os, state.dst_size);

	EXPECT_UTF8EQ("", o);
}

TEST(CaseMappingExecute, BasicLatinMultipleLowercase)
{
	CaseMappingState state;
	const char* i = "BUY";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Lowercase));

	EXPECT_EQ(1, casemapping_execute2(&state));
	EXPECT_EQ(i + 1, state.src);
	EXPECT_EQ(is - 1, state.src_size);
	EXPECT_EQ(o + 1, state.dst);
	EXPECT_EQ(os - 1, state.dst_size);

	EXPECT_EQ(1, casemapping_execute2(&state));
	EXPECT_EQ(i + 2, state.src);
	EXPECT_EQ(is - 2, state.src_size);
	EXPECT_EQ(o + 2, state.dst);
	EXPECT_EQ(os - 2, state.dst_size);

	EXPECT_EQ(1, casemapping_execute2(&state));
	EXPECT_EQ(i + 3, state.src);
	EXPECT_EQ(is - 3, state.src_size);
	EXPECT_EQ(o + 3, state.dst);
	EXPECT_EQ(os - 3, state.dst_size);

	EXPECT_EQ(0, casemapping_execute2(&state));

	EXPECT_UTF8EQ("buy", o);
}

TEST(CaseMappingExecute, BasicLatinMultipleUppercase)
{
	CaseMappingState state;
	const char* i = "mouse";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Uppercase));

	EXPECT_EQ(1, casemapping_execute2(&state));
	EXPECT_EQ(i + 1, state.src);
	EXPECT_EQ(is - 1, state.src_size);
	EXPECT_EQ(o + 1, state.dst);
	EXPECT_EQ(os - 1, state.dst_size);

	EXPECT_EQ(1, casemapping_execute2(&state));
	EXPECT_EQ(i + 2, state.src);
	EXPECT_EQ(is - 2, state.src_size);
	EXPECT_EQ(o + 2, state.dst);
	EXPECT_EQ(os - 2, state.dst_size);

	EXPECT_EQ(1, casemapping_execute2(&state));
	EXPECT_EQ(i + 3, state.src);
	EXPECT_EQ(is - 3, state.src_size);
	EXPECT_EQ(o + 3, state.dst);
	EXPECT_EQ(os - 3, state.dst_size);

	EXPECT_EQ(1, casemapping_execute2(&state));
	EXPECT_EQ(i + 4, state.src);
	EXPECT_EQ(is - 4, state.src_size);
	EXPECT_EQ(o + 4, state.dst);
	EXPECT_EQ(os - 4, state.dst_size);

	EXPECT_EQ(1, casemapping_execute2(&state));
	EXPECT_EQ(i + 5, state.src);
	EXPECT_EQ(is - 5, state.src_size);
	EXPECT_EQ(o + 5, state.dst);
	EXPECT_EQ(os - 5, state.dst_size);

	EXPECT_EQ(0, casemapping_execute2(&state));

	EXPECT_UTF8EQ("MOUSE", o);
}

TEST(CaseMappingExecute, BasicLatinMultipleTitlecase)
{
	CaseMappingState state;
	const char* i = "Zing";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Titlecase));

	EXPECT_EQ(1, casemapping_execute2(&state));
	EXPECT_EQ(i + 1, state.src);
	EXPECT_EQ(is - 1, state.src_size);
	EXPECT_EQ(o + 1, state.dst);
	EXPECT_EQ(os - 1, state.dst_size);

	EXPECT_EQ(1, casemapping_execute2(&state));
	EXPECT_EQ(i + 2, state.src);
	EXPECT_EQ(is - 2, state.src_size);
	EXPECT_EQ(o + 2, state.dst);
	EXPECT_EQ(os - 2, state.dst_size);

	EXPECT_EQ(1, casemapping_execute2(&state));
	EXPECT_EQ(i + 3, state.src);
	EXPECT_EQ(is - 3, state.src_size);
	EXPECT_EQ(o + 3, state.dst);
	EXPECT_EQ(os - 3, state.dst_size);

	EXPECT_EQ(1, casemapping_execute2(&state));
	EXPECT_EQ(i + 4, state.src);
	EXPECT_EQ(is - 4, state.src_size);
	EXPECT_EQ(o + 4, state.dst);
	EXPECT_EQ(os - 4, state.dst_size);

	EXPECT_EQ(0, casemapping_execute2(&state));

	EXPECT_UTF8EQ("ZING", o);
}

TEST(CaseMappingExecute, BasicLatinMultipleUnaffected)
{
	CaseMappingState state;
	const char* i = "$5.-";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Titlecase));

	EXPECT_EQ(1, casemapping_execute2(&state));
	EXPECT_EQ(i + 1, state.src);
	EXPECT_EQ(is - 1, state.src_size);
	EXPECT_EQ(o + 1, state.dst);
	EXPECT_EQ(os - 1, state.dst_size);

	EXPECT_EQ(1, casemapping_execute2(&state));
	EXPECT_EQ(i + 2, state.src);
	EXPECT_EQ(is - 2, state.src_size);
	EXPECT_EQ(o + 2, state.dst);
	EXPECT_EQ(os - 2, state.dst_size);

	EXPECT_EQ(1, casemapping_execute2(&state));
	EXPECT_EQ(i + 3, state.src);
	EXPECT_EQ(is - 3, state.src_size);
	EXPECT_EQ(o + 3, state.dst);
	EXPECT_EQ(os - 3, state.dst_size);

	EXPECT_EQ(1, casemapping_execute2(&state));
	EXPECT_EQ(i + 4, state.src);
	EXPECT_EQ(is - 4, state.src_size);
	EXPECT_EQ(o + 4, state.dst);
	EXPECT_EQ(os - 4, state.dst_size);

	EXPECT_EQ(0, casemapping_execute2(&state));

	EXPECT_UTF8EQ("$5.-", o);
}

TEST(CaseMappingExecute, BasicLatinMultipleAmountOfBytes)
{
	CaseMappingState state;
	const char* i = "bar";
	size_t is = strlen(i);

	EXPECT_TRUE(casemapping_initialize(&state, i, is, nullptr, 0, UnicodeProperty_Lowercase));

	EXPECT_EQ(1, casemapping_execute2(&state));
	EXPECT_EQ(i + 1, state.src);
	EXPECT_EQ(is - 1, state.src_size);
	EXPECT_EQ(nullptr, state.dst);
	EXPECT_EQ(0, state.dst_size);

	EXPECT_EQ(1, casemapping_execute2(&state));
	EXPECT_EQ(i + 2, state.src);
	EXPECT_EQ(is - 2, state.src_size);
	EXPECT_EQ(nullptr, state.dst);
	EXPECT_EQ(0, state.dst_size);

	EXPECT_EQ(1, casemapping_execute2(&state));
	EXPECT_EQ(i + 3, state.src);
	EXPECT_EQ(is - 3, state.src_size);
	EXPECT_EQ(nullptr, state.dst);
	EXPECT_EQ(0, state.dst_size);

	EXPECT_EQ(0, casemapping_execute2(&state));
}

TEST(CaseMappingExecute, BasicLatinMultipleNotEnoughSpace)
{
	CaseMappingState state;
	const char* i = "disconnect";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 3;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Uppercase));

	EXPECT_EQ(1, casemapping_execute2(&state));
	EXPECT_EQ(i + 1, state.src);
	EXPECT_EQ(is - 1, state.src_size);
	EXPECT_EQ(o + 1, state.dst);
	EXPECT_EQ(os - 1, state.dst_size);

	EXPECT_EQ(1, casemapping_execute2(&state));
	EXPECT_EQ(i + 2, state.src);
	EXPECT_EQ(is - 2, state.src_size);
	EXPECT_EQ(o + 2, state.dst);
	EXPECT_EQ(os - 2, state.dst_size);

	EXPECT_EQ(1, casemapping_execute2(&state));
	EXPECT_EQ(i + 3, state.src);
	EXPECT_EQ(is - 3, state.src_size);
	EXPECT_EQ(o + 3, state.dst);
	EXPECT_EQ(os - 3, state.dst_size);

	EXPECT_EQ(0, casemapping_execute2(&state));

	EXPECT_UTF8EQ("DIS", o);
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