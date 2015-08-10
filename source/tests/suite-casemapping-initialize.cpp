#include "tests-base.hpp"

extern "C" {
	#include "../internal/casemapping.h"
	#include "../internal/database.h"
}

#include "helpers-casemapping.hpp"
#include "helpers-strings.hpp"

TEST(CaseMappingInitialize, Initialize)
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

TEST(CaseMappingInitialize, LocaleEnglishUS)
{
	CaseMappingState state;
	const char* i = "Pancakes";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;

	EXPECT_STREQ("en-US", setlocale(LC_ALL, "en-US"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Titlecase));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_DEFAULT, state.locale);

	setlocale(LC_ALL, "C");
}

TEST(CaseMappingInitialize, LocaleTurkish)
{
	CaseMappingState state;
	const char* i = "I welcome our Turkish friends.";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;

	EXPECT_STREQ("tr", setlocale(LC_ALL, "tr"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Uppercase));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_TURKISH, state.locale);

	EXPECT_STREQ("tr-TR", setlocale(LC_ALL, "tr-TR"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Uppercase));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_TURKISH, state.locale);

	EXPECT_STREQ("Turkish_Turkey.1254", setlocale(LC_ALL, "turkish"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Uppercase));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_TURKISH, state.locale);

	EXPECT_STREQ("Turkish_Turkey.1254", setlocale(LC_ALL, "Turkish_Turkey.1254"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Uppercase));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_TURKISH, state.locale);

	EXPECT_STREQ("Turkish_Turkey.1254", setlocale(LC_ALL, "Turkish_Turkey.ACP"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Uppercase));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_TURKISH, state.locale);

	setlocale(LC_ALL, "C");
}