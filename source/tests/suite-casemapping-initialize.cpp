#include "tests-base.hpp"

extern "C" {
	#include "../internal/casemapping.h"
	#include "../internal/database.h"
}

#include "../helpers/helpers-casemapping.hpp"
#include "../helpers/helpers-locale.hpp"
#include "../helpers/helpers-strings.hpp"

TEST(CaseMappingInitialize, Initialize)
{
	CaseMappingState state;
	const char* i = "Greetings";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, TitlecaseIndex1Ptr, TitlecaseIndex2Ptr, TitlecaseDataPtr, UTF8_LOCALE_UNAFFECTED));
	EXPECT_EQ(i, state.src);
	EXPECT_EQ(is, state.src_size);
	EXPECT_EQ(o, state.dst);
	EXPECT_EQ(os, state.dst_size);
	EXPECT_EQ(TitlecaseIndex1Ptr, state.property_index1);
	EXPECT_EQ(TitlecaseIndex2Ptr, state.property_index2);
	EXPECT_EQ(TitlecaseDataPtr, state.property_data);
	EXPECT_LOCALE_EQ(UTF8_LOCALE_UNAFFECTED, state.locale);
}

TEST(CaseMappingInitialize, LocaleLithuanian)
{
	CaseMappingState state;
	const char* i = "Cuisine";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr, UTF8_LOCALE_LITHUANIAN));
	EXPECT_LOCALE_EQ(UTF8_LOCALE_LITHUANIAN, state.locale);
}

TEST(CaseMappingInitialize, LocaleTurkish)
{
	CaseMappingState state;
	const char* i = "I welcome our Turkish friends.";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr, UTF8_LOCALE_TURKISH));
	EXPECT_LOCALE_EQ(UTF8_LOCALE_TURKISH, state.locale);
}

TEST(CaseMappingInitialize, LocaleAzeriLatin)
{
	CaseMappingState state;
	const char* i = "Cuisine.";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr, UTF8_LOCALE_AZERI_LATIN));
	EXPECT_LOCALE_EQ(UTF8_LOCALE_AZERI_LATIN, state.locale);
}

TEST(CaseMappingInitialize, LocaleMaximum)
{
	CaseMappingState state;
	const char* i = "Light";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;

	EXPECT_FALSE(casemapping_initialize(&state, i, is, o, os, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr, UTF8_LOCALE_MAXIMUM));
	EXPECT_LOCALE_EQ(0, state.locale);
}

TEST(CaseMappingInitialize, LocaleInvalid)
{
	CaseMappingState state;
	const char* i = "Universal";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;

	EXPECT_FALSE(casemapping_initialize(&state, i, is, o, os, LowercaseIndex1Ptr, LowercaseIndex2Ptr, LowercaseDataPtr, 312));
	EXPECT_LOCALE_EQ(0, state.locale);
}