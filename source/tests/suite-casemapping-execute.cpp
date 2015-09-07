#include "tests-base.hpp"

extern "C" {
	#include "../internal/casemapping.h"
	#include "../internal/database.h"
}

#include "../helpers/helpers-strings.hpp"

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

TEST(CaseMappingExecute, BasicLatinSingleLowercase)
{
	CaseMappingState state;
	const char* i = "R";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Lowercase));

	EXPECT_EQ(1, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 1, state.src);
	EXPECT_EQ(is - 1, state.src_size);

	EXPECT_EQ(1, casemapping_write(&state));
	EXPECT_EQ(o + 1, state.dst);
	EXPECT_EQ(os - 1, state.dst_size);

	EXPECT_EQ(0, casemapping_readcodepoint(&state));

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

	EXPECT_EQ(1, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 1, state.src);
	EXPECT_EQ(is - 1, state.src_size);

	EXPECT_EQ(1, casemapping_write(&state));
	EXPECT_EQ(o + 1, state.dst);
	EXPECT_EQ(os - 1, state.dst_size);

	EXPECT_EQ(0, casemapping_readcodepoint(&state));

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

	EXPECT_EQ(1, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 1, state.src);
	EXPECT_EQ(is - 1, state.src_size);

	EXPECT_EQ(1, casemapping_write(&state));
	EXPECT_EQ(o + 1, state.dst);
	EXPECT_EQ(os - 1, state.dst_size);

	EXPECT_EQ(0, casemapping_readcodepoint(&state));

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

	EXPECT_EQ(1, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 1, state.src);
	EXPECT_EQ(is - 1, state.src_size);

	EXPECT_EQ(1, casemapping_write(&state));
	EXPECT_EQ(o + 1, state.dst);
	EXPECT_EQ(os - 1, state.dst_size);

	EXPECT_EQ(0, casemapping_readcodepoint(&state));

	EXPECT_UTF8EQ("`", o);
}

TEST(CaseMappingExecute, BasicLatinSingleAmountOfBytes)
{
	CaseMappingState state;
	const char* i = "!";
	size_t is = strlen(i);

	EXPECT_TRUE(casemapping_initialize(&state, i, is, nullptr, 0, UnicodeProperty_Uppercase));

	EXPECT_EQ(1, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 1, state.src);
	EXPECT_EQ(is - 1, state.src_size);

	EXPECT_EQ(1, casemapping_write(&state));
	EXPECT_EQ(nullptr, state.dst);
	EXPECT_EQ(0, state.dst_size);

	EXPECT_EQ(0, casemapping_readcodepoint(&state));
}

TEST(CaseMappingExecute, BasicLatinSingleNotEnoughSpace)
{
	CaseMappingState state;
	const char* i = "^";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 0;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Uppercase));

	EXPECT_EQ(1, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 1, state.src);
	EXPECT_EQ(is - 1, state.src_size);

	EXPECT_EQ(0, casemapping_write(&state));
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

	EXPECT_EQ(1, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 1, state.src);
	EXPECT_EQ(is - 1, state.src_size);

	EXPECT_EQ(1, casemapping_write(&state));
	EXPECT_EQ(o + 1, state.dst);
	EXPECT_EQ(os - 1, state.dst_size);

	EXPECT_EQ(1, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 2, state.src);
	EXPECT_EQ(is - 2, state.src_size);

	EXPECT_EQ(1, casemapping_write(&state));
	EXPECT_EQ(o + 2, state.dst);
	EXPECT_EQ(os - 2, state.dst_size);

	EXPECT_EQ(1, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 3, state.src);
	EXPECT_EQ(is - 3, state.src_size);

	EXPECT_EQ(1, casemapping_write(&state));
	EXPECT_EQ(o + 3, state.dst);
	EXPECT_EQ(os - 3, state.dst_size);

	EXPECT_EQ(0, casemapping_readcodepoint(&state));

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

	EXPECT_EQ(1, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 1, state.src);
	EXPECT_EQ(is - 1, state.src_size);

	EXPECT_EQ(1, casemapping_write(&state));
	EXPECT_EQ(o + 1, state.dst);
	EXPECT_EQ(os - 1, state.dst_size);

	EXPECT_EQ(1, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 2, state.src);
	EXPECT_EQ(is - 2, state.src_size);

	EXPECT_EQ(1, casemapping_write(&state));
	EXPECT_EQ(o + 2, state.dst);
	EXPECT_EQ(os - 2, state.dst_size);

	EXPECT_EQ(1, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 3, state.src);
	EXPECT_EQ(is - 3, state.src_size);

	EXPECT_EQ(1, casemapping_write(&state));
	EXPECT_EQ(o + 3, state.dst);
	EXPECT_EQ(os - 3, state.dst_size);

	EXPECT_EQ(1, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 4, state.src);
	EXPECT_EQ(is - 4, state.src_size);

	EXPECT_EQ(1, casemapping_write(&state));
	EXPECT_EQ(o + 4, state.dst);
	EXPECT_EQ(os - 4, state.dst_size);

	EXPECT_EQ(1, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 5, state.src);
	EXPECT_EQ(is - 5, state.src_size);

	EXPECT_EQ(1, casemapping_write(&state));
	EXPECT_EQ(o + 5, state.dst);
	EXPECT_EQ(os - 5, state.dst_size);

	EXPECT_EQ(0, casemapping_readcodepoint(&state));

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

	EXPECT_EQ(1, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 1, state.src);
	EXPECT_EQ(is - 1, state.src_size);

	EXPECT_EQ(1, casemapping_write(&state));
	EXPECT_EQ(o + 1, state.dst);
	EXPECT_EQ(os - 1, state.dst_size);

	EXPECT_EQ(1, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 2, state.src);
	EXPECT_EQ(is - 2, state.src_size);

	EXPECT_EQ(1, casemapping_write(&state));
	EXPECT_EQ(o + 2, state.dst);
	EXPECT_EQ(os - 2, state.dst_size);

	EXPECT_EQ(1, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 3, state.src);
	EXPECT_EQ(is - 3, state.src_size);

	EXPECT_EQ(1, casemapping_write(&state));
	EXPECT_EQ(o + 3, state.dst);
	EXPECT_EQ(os - 3, state.dst_size);

	EXPECT_EQ(1, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 4, state.src);
	EXPECT_EQ(is - 4, state.src_size);

	EXPECT_EQ(1, casemapping_write(&state));
	EXPECT_EQ(o + 4, state.dst);
	EXPECT_EQ(os - 4, state.dst_size);

	EXPECT_EQ(0, casemapping_readcodepoint(&state));

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

	EXPECT_EQ(1, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 1, state.src);
	EXPECT_EQ(is - 1, state.src_size);

	EXPECT_EQ(1, casemapping_write(&state));
	EXPECT_EQ(o + 1, state.dst);
	EXPECT_EQ(os - 1, state.dst_size);

	EXPECT_EQ(1, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 2, state.src);
	EXPECT_EQ(is - 2, state.src_size);

	EXPECT_EQ(1, casemapping_write(&state));
	EXPECT_EQ(o + 2, state.dst);
	EXPECT_EQ(os - 2, state.dst_size);

	EXPECT_EQ(1, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 3, state.src);
	EXPECT_EQ(is - 3, state.src_size);

	EXPECT_EQ(1, casemapping_write(&state));
	EXPECT_EQ(o + 3, state.dst);
	EXPECT_EQ(os - 3, state.dst_size);

	EXPECT_EQ(1, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 4, state.src);
	EXPECT_EQ(is - 4, state.src_size);

	EXPECT_EQ(1, casemapping_write(&state));
	EXPECT_EQ(o + 4, state.dst);
	EXPECT_EQ(os - 4, state.dst_size);

	EXPECT_EQ(0, casemapping_readcodepoint(&state));

	EXPECT_UTF8EQ("$5.-", o);
}

TEST(CaseMappingExecute, BasicLatinMultipleAmountOfBytes)
{
	CaseMappingState state;
	const char* i = "bar";
	size_t is = strlen(i);

	EXPECT_TRUE(casemapping_initialize(&state, i, is, nullptr, 0, UnicodeProperty_Lowercase));

	EXPECT_EQ(1, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 1, state.src);
	EXPECT_EQ(is - 1, state.src_size);

	EXPECT_EQ(1, casemapping_write(&state));
	EXPECT_EQ(nullptr, state.dst);
	EXPECT_EQ(0, state.dst_size);

	EXPECT_EQ(1, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 2, state.src);
	EXPECT_EQ(is - 2, state.src_size);

	EXPECT_EQ(1, casemapping_write(&state));
	EXPECT_EQ(nullptr, state.dst);
	EXPECT_EQ(0, state.dst_size);

	EXPECT_EQ(1, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 3, state.src);
	EXPECT_EQ(is - 3, state.src_size);

	EXPECT_EQ(1, casemapping_write(&state));
	EXPECT_EQ(nullptr, state.dst);
	EXPECT_EQ(0, state.dst_size);

	EXPECT_EQ(0, casemapping_readcodepoint(&state));
}

TEST(CaseMappingExecute, BasicLatinMultipleNotEnoughSpace)
{
	CaseMappingState state;
	const char* i = "disconnect";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 3;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Uppercase));

	EXPECT_EQ(1, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 1, state.src);
	EXPECT_EQ(is - 1, state.src_size);

	EXPECT_EQ(1, casemapping_write(&state));
	EXPECT_EQ(o + 1, state.dst);
	EXPECT_EQ(os - 1, state.dst_size);

	EXPECT_EQ(1, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 2, state.src);
	EXPECT_EQ(is - 2, state.src_size);

	EXPECT_EQ(1, casemapping_write(&state));
	EXPECT_EQ(o + 2, state.dst);
	EXPECT_EQ(os - 2, state.dst_size);

	EXPECT_EQ(1, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 3, state.src);
	EXPECT_EQ(is - 3, state.src_size);

	EXPECT_EQ(1, casemapping_write(&state));
	EXPECT_EQ(o + 3, state.dst);
	EXPECT_EQ(os - 3, state.dst_size);

	EXPECT_EQ(1, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 4, state.src);
	EXPECT_EQ(is - 4, state.src_size);

	EXPECT_EQ(0, casemapping_write(&state));

	EXPECT_UTF8EQ("DIS", o);
}

TEST(CaseMappingExecute, GeneralCategoryCaseMappedSingleLowercase)
{
	// LATIN CAPITAL LETTER O WITH STROKE

	CaseMappingState state;
	const char* i = "\xC3\x98";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Lowercase));

	EXPECT_EQ(2, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 2, state.src);
	EXPECT_EQ(is - 2, state.src_size);

	EXPECT_EQ(2, casemapping_write(&state));
	EXPECT_EQ(o + 2, state.dst);
	EXPECT_EQ(os - 2, state.dst_size);

	EXPECT_EQ(0, casemapping_readcodepoint(&state));

	EXPECT_UTF8EQ("\xC3\xB8", o);
}

TEST(CaseMappingExecute, GeneralCategoryCaseMappedSingleUppercase)
{
	// LATIN SMALL LETTER N PRECEDED BY APOSTROPHE

	CaseMappingState state;
	const char* i = "\xC5\x89";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Uppercase));

	EXPECT_EQ(2, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 2, state.src);
	EXPECT_EQ(is - 2, state.src_size);

	EXPECT_EQ(3, casemapping_write(&state));
	EXPECT_EQ(o + 3, state.dst);
	EXPECT_EQ(os - 3, state.dst_size);

	EXPECT_EQ(0, casemapping_readcodepoint(&state));

	EXPECT_UTF8EQ("\xCA\xBCN", o);
}

TEST(CaseMappingExecute, GeneralCategoryCaseMappedSingleTitlecase)
{
	// LATIN CAPITAL LETTER DZ WITH CARON

	CaseMappingState state;
	const char* i = "\xC7\x84";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Titlecase));

	EXPECT_EQ(2, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 2, state.src);
	EXPECT_EQ(is - 2, state.src_size);

	EXPECT_EQ(2, casemapping_write(&state));
	EXPECT_EQ(o + 2, state.dst);
	EXPECT_EQ(os - 2, state.dst_size);

	EXPECT_EQ(0, casemapping_readcodepoint(&state));

	EXPECT_UTF8EQ("\xC7\x85", o);
}

TEST(CaseMappingExecute, GeneralCategoryCaseMappedSingleUnaffected)
{
	// OLD PERSONAL COMPUTER

	CaseMappingState state;
	const char* i = "\xF0\x9F\x96\xB3";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Lowercase));

	EXPECT_EQ(4, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 4, state.src);
	EXPECT_EQ(is - 4, state.src_size);

	EXPECT_EQ(4, casemapping_write(&state));
	EXPECT_EQ(o + 4, state.dst);
	EXPECT_EQ(os - 4, state.dst_size);

	EXPECT_EQ(0, casemapping_readcodepoint(&state));

	EXPECT_UTF8EQ("\xF0\x9F\x96\xB3", o);
}

TEST(CaseMappingExecute, GeneralCategoryCaseMappedSingleAmountOfBytes)
{
	// HARD DISK

	CaseMappingState state;
	const char* i = "\xF0\x9F\x96\xB4";
	size_t is = strlen(i);

	EXPECT_TRUE(casemapping_initialize(&state, i, is, nullptr, 0, UnicodeProperty_Lowercase));

	EXPECT_EQ(4, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 4, state.src);
	EXPECT_EQ(is - 4, state.src_size);

	EXPECT_EQ(4, casemapping_write(&state));
	EXPECT_EQ(nullptr, state.dst);
	EXPECT_EQ(0, state.dst_size);

	EXPECT_EQ(0, casemapping_readcodepoint(&state));
}

TEST(CaseMappingExecute, GeneralCategoryCaseMappedSingleNotEnoughSpace)
{
	// SWASH AMPERSAND ORNAMENT

	CaseMappingState state;
	const char* i = "\xF0\x9F\x99\xB5";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 3;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Uppercase));

	EXPECT_EQ(4, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 4, state.src);
	EXPECT_EQ(is - 4, state.src_size);

	EXPECT_EQ(0, casemapping_readcodepoint(&state));
	EXPECT_EQ(o, state.dst);
	EXPECT_EQ(os, state.dst_size);

	EXPECT_UTF8EQ("", o);
}

TEST(CaseMappingExecute, GeneralCategoryCaseMappedMultipleLowercase)
{
	// 01A4 017D 015E
	// 01A5 017E 015F

	CaseMappingState state;
	const char* i = "\xC6\xA4\xC5\xBD\xC5\x9E";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Lowercase));

	EXPECT_EQ(2, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 2, state.src);
	EXPECT_EQ(is - 2, state.src_size);

	EXPECT_EQ(2, casemapping_write(&state));
	EXPECT_EQ(o + 2, state.dst);
	EXPECT_EQ(os - 2, state.dst_size);

	EXPECT_EQ(2, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 4, state.src);
	EXPECT_EQ(is - 4, state.src_size);

	EXPECT_EQ(2, casemapping_write(&state));
	EXPECT_EQ(o + 4, state.dst);
	EXPECT_EQ(os - 4, state.dst_size);

	EXPECT_EQ(2, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 6, state.src);
	EXPECT_EQ(is - 6, state.src_size);

	EXPECT_EQ(2, casemapping_write(&state));
	EXPECT_EQ(o + 6, state.dst);
	EXPECT_EQ(os - 6, state.dst_size);

	EXPECT_EQ(0, casemapping_readcodepoint(&state));

	EXPECT_UTF8EQ("\xC6\xA5\xC5\xBE\xC5\x9F", o);
}

TEST(CaseMappingExecute, GeneralCategoryCaseMappedMultipleUppercase)
{
	// 0149 00DF
	// 02BC 004E 0053 0053

	CaseMappingState state;
	const char* i = "\xC5\x89\xC3\x9F";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Uppercase));

	EXPECT_EQ(2, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 2, state.src);
	EXPECT_EQ(is - 2, state.src_size);

	EXPECT_EQ(3, casemapping_write(&state));
	EXPECT_EQ(o + 3, state.dst);
	EXPECT_EQ(os - 3, state.dst_size);

	EXPECT_EQ(2, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 4, state.src);
	EXPECT_EQ(is - 4, state.src_size);

	EXPECT_EQ(2, casemapping_write(&state));
	EXPECT_EQ(o + 5, state.dst);
	EXPECT_EQ(os - 5, state.dst_size);

	EXPECT_EQ(0, casemapping_readcodepoint(&state));

	EXPECT_UTF8EQ("\xCA\xBCNSS", o);
}

TEST(CaseMappingExecute, GeneralCategoryCaseMappedMultipleTitlecase)
{
	// 01C7 01DC 01D0
	// 01C8 01DB 01CF

	CaseMappingState state;
	const char* i = "\xC7\x87\xC7\x9C\xC7\x90";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Titlecase));

	EXPECT_EQ(2, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 2, state.src);
	EXPECT_EQ(is - 2, state.src_size);

	EXPECT_EQ(2, casemapping_write(&state));
	EXPECT_EQ(o + 2, state.dst);
	EXPECT_EQ(os - 2, state.dst_size);

	EXPECT_EQ(2, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 4, state.src);
	EXPECT_EQ(is - 4, state.src_size);

	EXPECT_EQ(2, casemapping_write(&state));
	EXPECT_EQ(o + 4, state.dst);
	EXPECT_EQ(os - 4, state.dst_size);

	EXPECT_EQ(2, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 6, state.src);
	EXPECT_EQ(is - 6, state.src_size);

	EXPECT_EQ(2, casemapping_write(&state));
	EXPECT_EQ(o + 6, state.dst);
	EXPECT_EQ(os - 6, state.dst_size);

	EXPECT_EQ(0, casemapping_readcodepoint(&state));

	EXPECT_UTF8EQ("\xC7\x88\xC7\x9B\xC7\x8F", o);
}

TEST(CaseMappingExecute, GeneralCategoryCaseMappedMultipleUnaffected)
{
	// 1F5D8 1AA3 3010 1F64B
	// 1F5D8 1AA3 3010 1F64B

	CaseMappingState state;
	const char* i = "\xF0\x9F\x97\x98\xE1\xAA\xA3\xE3\x80\x90\xF0\x9F\x99\x8B";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Uppercase));

	EXPECT_EQ(4, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 4, state.src);
	EXPECT_EQ(is - 4, state.src_size);

	EXPECT_EQ(4, casemapping_write(&state));
	EXPECT_EQ(o + 4, state.dst);
	EXPECT_EQ(os - 4, state.dst_size);

	EXPECT_EQ(3, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 7, state.src);
	EXPECT_EQ(is - 7, state.src_size);

	EXPECT_EQ(3, casemapping_write(&state));
	EXPECT_EQ(o + 7, state.dst);
	EXPECT_EQ(os - 7, state.dst_size);

	EXPECT_EQ(3, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 10, state.src);
	EXPECT_EQ(is - 10, state.src_size);

	EXPECT_EQ(3, casemapping_write(&state));
	EXPECT_EQ(o + 10, state.dst);
	EXPECT_EQ(os - 10, state.dst_size);

	EXPECT_EQ(4, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 14, state.src);
	EXPECT_EQ(is - 14, state.src_size);

	EXPECT_EQ(4, casemapping_write(&state));
	EXPECT_EQ(o + 14, state.dst);
	EXPECT_EQ(os - 14, state.dst_size);

	EXPECT_EQ(0, casemapping_readcodepoint(&state));

	EXPECT_UTF8EQ("\xF0\x9F\x97\x98\xE1\xAA\xA3\xE3\x80\x90\xF0\x9F\x99\x8B", o);
}

TEST(CaseMappingExecute, GeneralCategoryCaseMappedMultipleAmountOfBytes)
{
	// 0130 0390 041A
	// 0069 0307 0390 043A

	CaseMappingState state;
	const char* i = "\xC4\xB0\xCE\x90\xD0\x9A";
	size_t is = strlen(i);

	EXPECT_TRUE(casemapping_initialize(&state, i, is, nullptr, 0, UnicodeProperty_Lowercase));

	EXPECT_EQ(2, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 2, state.src);
	EXPECT_EQ(is - 2, state.src_size);

	EXPECT_EQ(3, casemapping_write(&state));
	EXPECT_EQ(nullptr, state.dst);
	EXPECT_EQ(0, state.dst_size);

	EXPECT_EQ(2, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 4, state.src);
	EXPECT_EQ(is - 4, state.src_size);

	EXPECT_EQ(2, casemapping_write(&state));
	EXPECT_EQ(nullptr, state.dst);
	EXPECT_EQ(0, state.dst_size);

	EXPECT_EQ(2, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 6, state.src);
	EXPECT_EQ(is - 6, state.src_size);

	EXPECT_EQ(2, casemapping_write(&state));
	EXPECT_EQ(nullptr, state.dst);
	EXPECT_EQ(0, state.dst_size);

	EXPECT_EQ(0, casemapping_readcodepoint(&state));
}

TEST(CaseMappingExecute, GeneralCategoryCaseMappedMultipleNotEnoughSpace)
{
	// 03E4 03B0 0390
	// 03E4 03A5 0308 0301 0399 0308 0301

	CaseMappingState state;
	const char* i = "\xCF\xA4\xCE\xB0\xCE\x90";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 9;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Uppercase));

	EXPECT_EQ(2, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 2, state.src);
	EXPECT_EQ(is - 2, state.src_size);

	EXPECT_EQ(2, casemapping_write(&state));
	EXPECT_EQ(o + 2, state.dst);
	EXPECT_EQ(os - 2, state.dst_size);

	EXPECT_EQ(2, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 4, state.src);
	EXPECT_EQ(is - 4, state.src_size);

	EXPECT_EQ(6, casemapping_write(&state));
	EXPECT_EQ(o + 8, state.dst);
	EXPECT_EQ(os - 8, state.dst_size);

	EXPECT_EQ(2, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 6, state.src);
	EXPECT_EQ(is - 6, state.src_size);

	EXPECT_EQ(0, casemapping_readcodepoint(&state));

	EXPECT_UTF8EQ("\xCF\xA4\xCE\xA5\xCC\x88\xCC\x81", o);
}

TEST(CaseMappingExecute, InvalidCodepointSingle)
{
	CaseMappingState state;
	const char* i = "\xDA";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Lowercase));

	EXPECT_EQ(1, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 1, state.src);
	EXPECT_EQ(is - 1, state.src_size);

	EXPECT_EQ(3, casemapping_write(&state));
	EXPECT_EQ(o + 3, state.dst);
	EXPECT_EQ(os - 3, state.dst_size);

	EXPECT_EQ(0, casemapping_readcodepoint(&state));

	EXPECT_UTF8EQ("\xEF\xBF\xBD", o);
}

TEST(CaseMappingExecute, InvalidCodepointSingleAmountOfBytes)
{
	CaseMappingState state;
	const char* i = "\xF4\x89";
	size_t is = strlen(i);

	EXPECT_TRUE(casemapping_initialize(&state, i, is, nullptr, 0, UnicodeProperty_Titlecase));

	EXPECT_EQ(2, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 2, state.src);
	EXPECT_EQ(is - 2, state.src_size);

	EXPECT_EQ(3, casemapping_write(&state));
	EXPECT_EQ(nullptr, state.dst);
	EXPECT_EQ(0, state.dst_size);

	EXPECT_EQ(0, casemapping_readcodepoint(&state));
}

TEST(CaseMappingExecute, InvalidCodepointSingleNotEnoughSpace)
{
	CaseMappingState state;
	const char* i = "\xC4";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 2;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Uppercase));

	EXPECT_EQ(1, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 1, state.src);
	EXPECT_EQ(is - 1, state.src_size);

	EXPECT_EQ(0, casemapping_write(&state));
	EXPECT_EQ(o, state.dst);
	EXPECT_EQ(os, state.dst_size);

	EXPECT_EQ(0, casemapping_readcodepoint(&state));

	EXPECT_UTF8EQ("", o);
}

TEST(CaseMappingExecute, InvalidCodepointMultiple)
{
	CaseMappingState state;
	const char* i = "\xCC\xCD\xD9";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Lowercase));

	EXPECT_EQ(1, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 1, state.src);
	EXPECT_EQ(is - 1, state.src_size);

	EXPECT_EQ(3, casemapping_write(&state));
	EXPECT_EQ(o + 3, state.dst);
	EXPECT_EQ(os - 3, state.dst_size);

	EXPECT_EQ(1, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 2, state.src);
	EXPECT_EQ(is - 2, state.src_size);

	EXPECT_EQ(3, casemapping_write(&state));
	EXPECT_EQ(o + 6, state.dst);
	EXPECT_EQ(os - 6, state.dst_size);

	EXPECT_EQ(1, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 3, state.src);
	EXPECT_EQ(is - 3, state.src_size);

	EXPECT_EQ(3, casemapping_write(&state));
	EXPECT_EQ(o + 9, state.dst);
	EXPECT_EQ(os - 9, state.dst_size);

	EXPECT_EQ(0, casemapping_readcodepoint(&state));

	EXPECT_UTF8EQ("\xEF\xBF\xBD\xEF\xBF\xBD\xEF\xBF\xBD", o);
}

TEST(CaseMappingExecute, InvalidCodepointMultipleAmountOfBytes)
{
	CaseMappingState state;
	const char* i = "\xDA\xE0\x88\xDE\xCB";
	size_t is = strlen(i);

	EXPECT_TRUE(casemapping_initialize(&state, i, is, nullptr, 0, UnicodeProperty_Lowercase));

	EXPECT_EQ(1, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 1, state.src);
	EXPECT_EQ(is - 1, state.src_size);

	EXPECT_EQ(3, casemapping_write(&state));
	EXPECT_EQ(nullptr, state.dst);
	EXPECT_EQ(0, state.dst_size);

	EXPECT_EQ(2, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 3, state.src);
	EXPECT_EQ(is - 3, state.src_size);

	EXPECT_EQ(3, casemapping_write(&state));
	EXPECT_EQ(nullptr, state.dst);
	EXPECT_EQ(0, state.dst_size);

	EXPECT_EQ(1, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 4, state.src);
	EXPECT_EQ(is - 4, state.src_size);

	EXPECT_EQ(3, casemapping_write(&state));
	EXPECT_EQ(nullptr, state.dst);
	EXPECT_EQ(0, state.dst_size);

	EXPECT_EQ(1, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 5, state.src);
	EXPECT_EQ(is - 5, state.src_size);

	EXPECT_EQ(3, casemapping_write(&state));
	EXPECT_EQ(nullptr, state.dst);
	EXPECT_EQ(0, state.dst_size);

	EXPECT_EQ(0, casemapping_readcodepoint(&state));
}

TEST(CaseMappingExecute, InvalidCodepointMultipleNotEnoughSpace)
{
	CaseMappingState state;
	const char* i = "\xDE\xCD\xCA\xDB";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 7;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Lowercase));

	EXPECT_EQ(1, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 1, state.src);
	EXPECT_EQ(is - 1, state.src_size);

	EXPECT_EQ(3, casemapping_write(&state));
	EXPECT_EQ(o + 3, state.dst);
	EXPECT_EQ(os - 3, state.dst_size);

	EXPECT_EQ(1, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 2, state.src);
	EXPECT_EQ(is - 2, state.src_size);

	EXPECT_EQ(3, casemapping_write(&state));
	EXPECT_EQ(o + 6, state.dst);
	EXPECT_EQ(os - 6, state.dst_size);

	EXPECT_EQ(1, casemapping_readcodepoint(&state));
	EXPECT_EQ(i + 3, state.src);
	EXPECT_EQ(is - 3, state.src_size);

	EXPECT_EQ(0, casemapping_write(&state));
}