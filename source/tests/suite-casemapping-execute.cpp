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
	int32_t errors = 0;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Titlecase, &errors));
	EXPECT_EQ(0, errors);
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
	int32_t errors = 0;

	EXPECT_FALSE(casemapping_initialize(&state, nullptr, 16, o, os, UnicodeProperty_Uppercase, &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(CaseMappingExecute, InitializeInvalidLength)
{
	CaseMappingState state;
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_FALSE(casemapping_initialize(&state, "Picnic", 0, o, os, UnicodeProperty_Lowercase, &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(CaseMappingExecute, InitializeOverlapFits)
{
	CaseMappingState state;
	int32_t errors = 0;

	char data[128] = { 0 };

	const char* i = data;
	size_t is = 64;
	char* o = data + 64;
	size_t os = 64;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Lowercase, &errors));
	EXPECT_EQ(0, errors);
}

TEST(CaseMappingExecute, InitializeOverlapStartsEqual)
{
	CaseMappingState state;
	int32_t errors = 0;

	char data[128] = { 0 };
	memset(data, 0xCD, 64);
	memset(data + 64, 0xAB, 64);

	const char* i = data;
	size_t is = 64;
	char* o = data;
	size_t os = 64;

	EXPECT_FALSE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Lowercase, &errors));
	EXPECT_EQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST(CaseMappingExecute, InitializeOverlapEndsEqual)
{
	CaseMappingState state;
	int32_t errors = 0;

	char data[128] = { 0 };
	memset(data, 0xCD, 64);
	memset(data + 64, 0xAB, 64);

	const char* i = data;
	size_t is = 32;
	char* o = data + 16;
	size_t os = 16;

	EXPECT_FALSE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Lowercase, &errors));
	EXPECT_EQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST(CaseMappingExecute, InitializeOverlapInputStartsInTarget)
{
	CaseMappingState state;
	int32_t errors = 0;

	char data[128] = { 0 };
	memset(data, 0xCD, 64);
	memset(data + 64, 0xAB, 64);

	const char* i = data + 32;
	size_t is = 64;
	char* o = data;
	size_t os = 64;

	EXPECT_FALSE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Lowercase, &errors));
	EXPECT_EQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST(CaseMappingExecute, InitializeOverlapInputEndsInTarget)
{
	CaseMappingState state;
	int32_t errors = 0;

	char data[128] = { 0 };
	memset(data, 0xCD, 64);
	memset(data + 64, 0xAB, 64);

	const char* i = data;
	size_t is = 64;
	char* o = data + 32;
	size_t os = 64;

	EXPECT_FALSE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Lowercase, &errors));
	EXPECT_EQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST(CaseMappingExecute, InitializeOverlapInputInsideTarget)
{
	CaseMappingState state;
	int32_t errors = 0;

	char data[128] = { 0 };
	memset(data, 0xCD, 64);
	memset(data + 64, 0xAB, 64);

	const char* i = data + 16;
	size_t is = 32;
	char* o = data;
	size_t os = 64;

	EXPECT_FALSE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Lowercase, &errors));
	EXPECT_EQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST(CaseMappingExecute, InitializeOverlapTargetStartsInInput)
{
	CaseMappingState state;
	int32_t errors = 0;

	char data[128] = { 0 };
	memset(data, 0xCD, 64);
	memset(data + 64, 0xAB, 64);

	const char* i = data + 32;
	size_t is = 64;
	char* o = data + 32;
	size_t os = 64;

	EXPECT_FALSE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Lowercase, &errors));
	EXPECT_EQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST(CaseMappingExecute, InitializeOverlapTargetEndsInInput)
{
	CaseMappingState state;
	int32_t errors = 0;

	char data[128] = { 0 };
	memset(data, 0xCD, 64);
	memset(data + 64, 0xAB, 64);

	const char* i = data + 64;
	size_t is = 64;
	char* o = data + 32;
	size_t os = 64;

	EXPECT_FALSE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Lowercase, &errors));
	EXPECT_EQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST(CaseMappingExecute, InitializeOverlapTargetInsideInput)
{
	CaseMappingState state;
	int32_t errors = 0;

	char data[128] = { 0 };
	memset(data, 0xCD, 64);
	memset(data + 64, 0xAB, 64);

	const char* i = data;
	size_t is = 64;
	char* o = data + 16;
	size_t os = 64;

	EXPECT_FALSE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Lowercase, &errors));
	EXPECT_EQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST(CaseMappingExecute, BasicLatinSingleLowercase)
{
	CaseMappingState state;
	const char* i = "R";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Lowercase, &errors));
	EXPECT_EQ(0, errors);

	EXPECT_EQ(1, casemapping_execute(&state));
	EXPECT_EQ(i + 1, state.src);
	EXPECT_EQ(is - 1, state.src_size);
	EXPECT_EQ(o + 1, state.dst);
	EXPECT_EQ(os - 1, state.dst_size);

	EXPECT_EQ(0, casemapping_execute(&state));

	EXPECT_UTF8EQ("r", o);
}

TEST(CaseMappingExecute, BasicLatinSingleUppercase)
{
	CaseMappingState state;
	const char* i = "i";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Uppercase, &errors));
	EXPECT_EQ(0, errors);

	EXPECT_EQ(1, casemapping_execute(&state));
	EXPECT_EQ(i + 1, state.src);
	EXPECT_EQ(is - 1, state.src_size);
	EXPECT_EQ(o + 1, state.dst);
	EXPECT_EQ(os - 1, state.dst_size);

	EXPECT_EQ(0, casemapping_execute(&state));

	EXPECT_UTF8EQ("I", o);
}

TEST(CaseMappingExecute, BasicLatinSingleTitlecase)
{
	CaseMappingState state;
	const char* i = "v";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Titlecase, &errors));
	EXPECT_EQ(0, errors);

	EXPECT_EQ(1, casemapping_execute(&state));
	EXPECT_EQ(i + 1, state.src);
	EXPECT_EQ(is - 1, state.src_size);
	EXPECT_EQ(o + 1, state.dst);
	EXPECT_EQ(os - 1, state.dst_size);

	EXPECT_EQ(0, casemapping_execute(&state));

	EXPECT_UTF8EQ("V", o);
}

TEST(CaseMappingExecute, BasicLatinSingleUnaffected)
{
	CaseMappingState state;
	const char* i = "`";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Uppercase, &errors));
	EXPECT_EQ(0, errors);

	EXPECT_EQ(1, casemapping_execute(&state));
	EXPECT_EQ(i + 1, state.src);
	EXPECT_EQ(is - 1, state.src_size);
	EXPECT_EQ(o + 1, state.dst);
	EXPECT_EQ(os - 1, state.dst_size);

	EXPECT_EQ(0, casemapping_execute(&state));

	EXPECT_UTF8EQ("`", o);
}

TEST(CaseMappingExecute, BasicLatinSingleAmountOfBytes)
{
	CaseMappingState state;
	const char* i = "!";
	size_t is = strlen(i);
	int32_t errors = 0;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, nullptr, 0, UnicodeProperty_Uppercase, &errors));
	EXPECT_EQ(0, errors);

	EXPECT_EQ(1, casemapping_execute(&state));
	EXPECT_EQ(i + 1, state.src);
	EXPECT_EQ(is - 1, state.src_size);
	EXPECT_EQ(nullptr, state.dst);
	EXPECT_EQ(0, state.dst_size);

	EXPECT_EQ(0, casemapping_execute(&state));
}

TEST(CaseMappingExecute, BasicLatinSingleNotEnoughSpace)
{
	CaseMappingState state;
	const char* i = "^";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 0;
	int32_t errors = 0;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Uppercase, &errors));
	EXPECT_EQ(0, errors);

	EXPECT_EQ(0, casemapping_execute(&state));
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
	int32_t errors = 0;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Lowercase, &errors));
	EXPECT_EQ(0, errors);

	EXPECT_EQ(1, casemapping_execute(&state));
	EXPECT_EQ(i + 1, state.src);
	EXPECT_EQ(is - 1, state.src_size);
	EXPECT_EQ(o + 1, state.dst);
	EXPECT_EQ(os - 1, state.dst_size);

	EXPECT_EQ(1, casemapping_execute(&state));
	EXPECT_EQ(i + 2, state.src);
	EXPECT_EQ(is - 2, state.src_size);
	EXPECT_EQ(o + 2, state.dst);
	EXPECT_EQ(os - 2, state.dst_size);

	EXPECT_EQ(1, casemapping_execute(&state));
	EXPECT_EQ(i + 3, state.src);
	EXPECT_EQ(is - 3, state.src_size);
	EXPECT_EQ(o + 3, state.dst);
	EXPECT_EQ(os - 3, state.dst_size);

	EXPECT_EQ(0, casemapping_execute(&state));

	EXPECT_UTF8EQ("buy", o);
}

TEST(CaseMappingExecute, BasicLatinMultipleUppercase)
{
	CaseMappingState state;
	const char* i = "mouse";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Uppercase, &errors));
	EXPECT_EQ(0, errors);

	EXPECT_EQ(1, casemapping_execute(&state));
	EXPECT_EQ(i + 1, state.src);
	EXPECT_EQ(is - 1, state.src_size);
	EXPECT_EQ(o + 1, state.dst);
	EXPECT_EQ(os - 1, state.dst_size);

	EXPECT_EQ(1, casemapping_execute(&state));
	EXPECT_EQ(i + 2, state.src);
	EXPECT_EQ(is - 2, state.src_size);
	EXPECT_EQ(o + 2, state.dst);
	EXPECT_EQ(os - 2, state.dst_size);

	EXPECT_EQ(1, casemapping_execute(&state));
	EXPECT_EQ(i + 3, state.src);
	EXPECT_EQ(is - 3, state.src_size);
	EXPECT_EQ(o + 3, state.dst);
	EXPECT_EQ(os - 3, state.dst_size);

	EXPECT_EQ(1, casemapping_execute(&state));
	EXPECT_EQ(i + 4, state.src);
	EXPECT_EQ(is - 4, state.src_size);
	EXPECT_EQ(o + 4, state.dst);
	EXPECT_EQ(os - 4, state.dst_size);

	EXPECT_EQ(1, casemapping_execute(&state));
	EXPECT_EQ(i + 5, state.src);
	EXPECT_EQ(is - 5, state.src_size);
	EXPECT_EQ(o + 5, state.dst);
	EXPECT_EQ(os - 5, state.dst_size);

	EXPECT_EQ(0, casemapping_execute(&state));

	EXPECT_UTF8EQ("MOUSE", o);
}

TEST(CaseMappingExecute, BasicLatinMultipleTitlecase)
{
	CaseMappingState state;
	const char* i = "Zing";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Titlecase, &errors));
	EXPECT_EQ(0, errors);

	EXPECT_EQ(1, casemapping_execute(&state));
	EXPECT_EQ(i + 1, state.src);
	EXPECT_EQ(is - 1, state.src_size);
	EXPECT_EQ(o + 1, state.dst);
	EXPECT_EQ(os - 1, state.dst_size);

	EXPECT_EQ(1, casemapping_execute(&state));
	EXPECT_EQ(i + 2, state.src);
	EXPECT_EQ(is - 2, state.src_size);
	EXPECT_EQ(o + 2, state.dst);
	EXPECT_EQ(os - 2, state.dst_size);

	EXPECT_EQ(1, casemapping_execute(&state));
	EXPECT_EQ(i + 3, state.src);
	EXPECT_EQ(is - 3, state.src_size);
	EXPECT_EQ(o + 3, state.dst);
	EXPECT_EQ(os - 3, state.dst_size);

	EXPECT_EQ(1, casemapping_execute(&state));
	EXPECT_EQ(i + 4, state.src);
	EXPECT_EQ(is - 4, state.src_size);
	EXPECT_EQ(o + 4, state.dst);
	EXPECT_EQ(os - 4, state.dst_size);

	EXPECT_EQ(0, casemapping_execute(&state));

	EXPECT_UTF8EQ("ZING", o);
}

TEST(CaseMappingExecute, BasicLatinMultipleUnaffected)
{
	CaseMappingState state;
	const char* i = "$5.-";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors = 0;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Titlecase, &errors));
	EXPECT_EQ(0, errors);

	EXPECT_EQ(1, casemapping_execute(&state));
	EXPECT_EQ(i + 1, state.src);
	EXPECT_EQ(is - 1, state.src_size);
	EXPECT_EQ(o + 1, state.dst);
	EXPECT_EQ(os - 1, state.dst_size);

	EXPECT_EQ(1, casemapping_execute(&state));
	EXPECT_EQ(i + 2, state.src);
	EXPECT_EQ(is - 2, state.src_size);
	EXPECT_EQ(o + 2, state.dst);
	EXPECT_EQ(os - 2, state.dst_size);

	EXPECT_EQ(1, casemapping_execute(&state));
	EXPECT_EQ(i + 3, state.src);
	EXPECT_EQ(is - 3, state.src_size);
	EXPECT_EQ(o + 3, state.dst);
	EXPECT_EQ(os - 3, state.dst_size);

	EXPECT_EQ(1, casemapping_execute(&state));
	EXPECT_EQ(i + 4, state.src);
	EXPECT_EQ(is - 4, state.src_size);
	EXPECT_EQ(o + 4, state.dst);
	EXPECT_EQ(os - 4, state.dst_size);

	EXPECT_EQ(0, casemapping_execute(&state));

	EXPECT_UTF8EQ("$5.-", o);
}

TEST(CaseMappingExecute, BasicLatinMultipleAmountOfBytes)
{
	CaseMappingState state;
	const char* i = "bar";
	size_t is = strlen(i);
	int32_t errors = 0;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, nullptr, 0, UnicodeProperty_Lowercase, &errors));
	EXPECT_EQ(0, errors);

	EXPECT_EQ(1, casemapping_execute(&state));
	EXPECT_EQ(i + 1, state.src);
	EXPECT_EQ(is - 1, state.src_size);
	EXPECT_EQ(nullptr, state.dst);
	EXPECT_EQ(0, state.dst_size);

	EXPECT_EQ(1, casemapping_execute(&state));
	EXPECT_EQ(i + 2, state.src);
	EXPECT_EQ(is - 2, state.src_size);
	EXPECT_EQ(nullptr, state.dst);
	EXPECT_EQ(0, state.dst_size);

	EXPECT_EQ(1, casemapping_execute(&state));
	EXPECT_EQ(i + 3, state.src);
	EXPECT_EQ(is - 3, state.src_size);
	EXPECT_EQ(nullptr, state.dst);
	EXPECT_EQ(0, state.dst_size);

	EXPECT_EQ(0, casemapping_execute(&state));
}

TEST(CaseMappingExecute, BasicLatinMultipleNotEnoughSpace)
{
	CaseMappingState state;
	const char* i = "disconnect";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 3;
	int32_t errors = 0;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Uppercase, &errors));
	EXPECT_EQ(0, errors);

	EXPECT_EQ(1, casemapping_execute(&state));
	EXPECT_EQ(i + 1, state.src);
	EXPECT_EQ(is - 1, state.src_size);
	EXPECT_EQ(o + 1, state.dst);
	EXPECT_EQ(os - 1, state.dst_size);

	EXPECT_EQ(1, casemapping_execute(&state));
	EXPECT_EQ(i + 2, state.src);
	EXPECT_EQ(is - 2, state.src_size);
	EXPECT_EQ(o + 2, state.dst);
	EXPECT_EQ(os - 2, state.dst_size);

	EXPECT_EQ(1, casemapping_execute(&state));
	EXPECT_EQ(i + 3, state.src);
	EXPECT_EQ(is - 3, state.src_size);
	EXPECT_EQ(o + 3, state.dst);
	EXPECT_EQ(os - 3, state.dst_size);

	EXPECT_EQ(0, casemapping_execute(&state));

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
	int32_t errors = 0;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Lowercase, &errors));
	EXPECT_EQ(0, errors);

	EXPECT_EQ(2, casemapping_execute(&state));
	EXPECT_EQ(i + 2, state.src);
	EXPECT_EQ(is - 2, state.src_size);
	EXPECT_EQ(o + 2, state.dst);
	EXPECT_EQ(os - 2, state.dst_size);

	EXPECT_EQ(0, casemapping_execute(&state));

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
	int32_t errors = 0;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Uppercase, &errors));
	EXPECT_EQ(0, errors);

	EXPECT_EQ(3, casemapping_execute(&state));
	EXPECT_EQ(i + 2, state.src);
	EXPECT_EQ(is - 2, state.src_size);
	EXPECT_EQ(o + 3, state.dst);
	EXPECT_EQ(os - 3, state.dst_size);

	EXPECT_EQ(0, casemapping_execute(&state));

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
	int32_t errors = 0;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Titlecase, &errors));
	EXPECT_EQ(0, errors);

	EXPECT_EQ(2, casemapping_execute(&state));
	EXPECT_EQ(i + 2, state.src);
	EXPECT_EQ(is - 2, state.src_size);
	EXPECT_EQ(o + 2, state.dst);
	EXPECT_EQ(os - 2, state.dst_size);

	EXPECT_EQ(0, casemapping_execute(&state));

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
	int32_t errors = 0;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Lowercase, &errors));
	EXPECT_EQ(0, errors);

	EXPECT_EQ(4, casemapping_execute(&state));
	EXPECT_EQ(i + 4, state.src);
	EXPECT_EQ(is - 4, state.src_size);
	EXPECT_EQ(o + 4, state.dst);
	EXPECT_EQ(os - 4, state.dst_size);

	EXPECT_EQ(0, casemapping_execute(&state));

	EXPECT_UTF8EQ("\xF0\x9F\x96\xB3", o);
}

TEST(CaseMappingExecute, GeneralCategoryCaseMappedSingleAmountOfBytes)
{
	// HARD DISK

	CaseMappingState state;
	const char* i = "\xF0\x9F\x96\xB4";
	size_t is = strlen(i);
	int32_t errors = 0;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, nullptr, 0, UnicodeProperty_Lowercase, &errors));
	EXPECT_EQ(0, errors);

	EXPECT_EQ(4, casemapping_execute(&state));
	EXPECT_EQ(i + 4, state.src);
	EXPECT_EQ(is - 4, state.src_size);
	EXPECT_EQ(nullptr, state.dst);
	EXPECT_EQ(0, state.dst_size);

	EXPECT_EQ(0, casemapping_execute(&state));
}

TEST(CaseMappingExecute, GeneralCategoryCaseMappedSingleNotEnoughSpace)
{
	// SWASH AMPERSAND ORNAMENT

	CaseMappingState state;
	const char* i = "\xF0\x9F\x99\xB5";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 3;
	int32_t errors = 0;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Uppercase, &errors));
	EXPECT_EQ(0, errors);

	EXPECT_EQ(0, casemapping_execute(&state));
	EXPECT_EQ(i, state.src);
	EXPECT_EQ(is - 4, state.src_size);
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
	int32_t errors = 0;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Lowercase, &errors));
	EXPECT_EQ(0, errors);

	EXPECT_EQ(2, casemapping_execute(&state));
	EXPECT_EQ(i + 2, state.src);
	EXPECT_EQ(is - 2, state.src_size);
	EXPECT_EQ(o + 2, state.dst);
	EXPECT_EQ(os - 2, state.dst_size);

	EXPECT_EQ(2, casemapping_execute(&state));
	EXPECT_EQ(i + 4, state.src);
	EXPECT_EQ(is - 4, state.src_size);
	EXPECT_EQ(o + 4, state.dst);
	EXPECT_EQ(os - 4, state.dst_size);

	EXPECT_EQ(2, casemapping_execute(&state));
	EXPECT_EQ(i + 6, state.src);
	EXPECT_EQ(is - 6, state.src_size);
	EXPECT_EQ(o + 6, state.dst);
	EXPECT_EQ(os - 6, state.dst_size);

	EXPECT_EQ(0, casemapping_execute(&state));

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
	int32_t errors = 0;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Uppercase, &errors));
	EXPECT_EQ(0, errors);

	EXPECT_EQ(3, casemapping_execute(&state));
	EXPECT_EQ(i + 2, state.src);
	EXPECT_EQ(is - 2, state.src_size);
	EXPECT_EQ(o + 3, state.dst);
	EXPECT_EQ(os - 3, state.dst_size);

	EXPECT_EQ(2, casemapping_execute(&state));
	EXPECT_EQ(i + 4, state.src);
	EXPECT_EQ(is - 4, state.src_size);
	EXPECT_EQ(o + 5, state.dst);
	EXPECT_EQ(os - 5, state.dst_size);

	EXPECT_EQ(0, casemapping_execute(&state));

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
	int32_t errors = 0;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Titlecase, &errors));
	EXPECT_EQ(0, errors);

	EXPECT_EQ(2, casemapping_execute(&state));
	EXPECT_EQ(i + 2, state.src);
	EXPECT_EQ(is - 2, state.src_size);
	EXPECT_EQ(o + 2, state.dst);
	EXPECT_EQ(os - 2, state.dst_size);

	EXPECT_EQ(2, casemapping_execute(&state));
	EXPECT_EQ(i + 4, state.src);
	EXPECT_EQ(is - 4, state.src_size);
	EXPECT_EQ(o + 4, state.dst);
	EXPECT_EQ(os - 4, state.dst_size);

	EXPECT_EQ(2, casemapping_execute(&state));
	EXPECT_EQ(i + 6, state.src);
	EXPECT_EQ(is - 6, state.src_size);
	EXPECT_EQ(o + 6, state.dst);
	EXPECT_EQ(os - 6, state.dst_size);

	EXPECT_EQ(0, casemapping_execute(&state));

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
	int32_t errors = 0;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Uppercase, &errors));
	EXPECT_EQ(0, errors);

	EXPECT_EQ(4, casemapping_execute(&state));
	EXPECT_EQ(i + 4, state.src);
	EXPECT_EQ(is - 4, state.src_size);
	EXPECT_EQ(o + 4, state.dst);
	EXPECT_EQ(os - 4, state.dst_size);

	EXPECT_EQ(3, casemapping_execute(&state));
	EXPECT_EQ(i + 7, state.src);
	EXPECT_EQ(is - 7, state.src_size);
	EXPECT_EQ(o + 7, state.dst);
	EXPECT_EQ(os - 7, state.dst_size);

	EXPECT_EQ(3, casemapping_execute(&state));
	EXPECT_EQ(i + 10, state.src);
	EXPECT_EQ(is - 10, state.src_size);
	EXPECT_EQ(o + 10, state.dst);
	EXPECT_EQ(os - 10, state.dst_size);

	EXPECT_EQ(4, casemapping_execute(&state));
	EXPECT_EQ(i + 14, state.src);
	EXPECT_EQ(is - 14, state.src_size);
	EXPECT_EQ(o + 14, state.dst);
	EXPECT_EQ(os - 14, state.dst_size);

	EXPECT_EQ(0, casemapping_execute(&state));

	EXPECT_UTF8EQ("\xF0\x9F\x97\x98\xE1\xAA\xA3\xE3\x80\x90\xF0\x9F\x99\x8B", o);
}

TEST(CaseMappingExecute, GeneralCategoryCaseMappedMultipleAmountOfBytes)
{
	// 0130 0390 041A
	// 0069 0307 0390 043A

	CaseMappingState state;
	const char* i = "\xC4\xB0\xCE\x90\xD0\x9A";
	size_t is = strlen(i);
	int32_t errors = 0;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, nullptr, 0, UnicodeProperty_Lowercase, &errors));
	EXPECT_EQ(0, errors);

	EXPECT_EQ(3, casemapping_execute(&state));
	EXPECT_EQ(i + 2, state.src);
	EXPECT_EQ(is - 2, state.src_size);
	EXPECT_EQ(nullptr, state.dst);
	EXPECT_EQ(0, state.dst_size);

	EXPECT_EQ(2, casemapping_execute(&state));
	EXPECT_EQ(i + 4, state.src);
	EXPECT_EQ(is - 4, state.src_size);
	EXPECT_EQ(nullptr, state.dst);
	EXPECT_EQ(0, state.dst_size);

	EXPECT_EQ(2, casemapping_execute(&state));
	EXPECT_EQ(i + 6, state.src);
	EXPECT_EQ(is - 6, state.src_size);
	EXPECT_EQ(nullptr, state.dst);
	EXPECT_EQ(0, state.dst_size);

	EXPECT_EQ(0, casemapping_execute(&state));
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
	int32_t errors = 0;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Uppercase, &errors));
	EXPECT_EQ(0, errors);

	EXPECT_EQ(2, casemapping_execute(&state));
	EXPECT_EQ(i + 2, state.src);
	EXPECT_EQ(is - 2, state.src_size);
	EXPECT_EQ(o + 2, state.dst);
	EXPECT_EQ(os - 2, state.dst_size);

	EXPECT_EQ(6, casemapping_execute(&state));
	EXPECT_EQ(i + 4, state.src);
	EXPECT_EQ(is - 4, state.src_size);
	EXPECT_EQ(o + 8, state.dst);
	EXPECT_EQ(os - 8, state.dst_size);

	EXPECT_EQ(0, casemapping_execute(&state));
	EXPECT_EQ(i + 4, state.src);
	EXPECT_EQ(is - 6, state.src_size);
	EXPECT_EQ(o + 8, state.dst);
	EXPECT_EQ(os - 8, state.dst_size);

	EXPECT_UTF8EQ("\xCF\xA4\xCE\xA5\xCC\x88\xCC\x81", o);
}