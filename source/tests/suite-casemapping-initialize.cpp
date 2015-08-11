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

TEST(CaseMappingInitialize, LocaleLithuanian)
{
	CaseMappingState state;
	const char* i = "Cuisine";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;

	EXPECT_STREQ("lt", setlocale(LC_ALL, "lt"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Uppercase));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_LITHUANIAN, state.locale);
	setlocale(LC_ALL, "C");

	EXPECT_STREQ("lt-LT", setlocale(LC_ALL, "lt-LT"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Uppercase));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_LITHUANIAN, state.locale);
	setlocale(LC_ALL, "C");

	EXPECT_STREQ("Lithuanian_Lithuania.1257", setlocale(LC_ALL, "lithuanian"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Uppercase));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_LITHUANIAN, state.locale);
	setlocale(LC_ALL, "C");

	EXPECT_STREQ("Lithuanian_Lithuania.1257", setlocale(LC_ALL, "Lithuanian_Lithuania.1257"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Uppercase));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_LITHUANIAN, state.locale);
	setlocale(LC_ALL, "C");

	EXPECT_STREQ("Lithuanian_Lithuania.1257", setlocale(LC_ALL, "Lithuanian_Lithuania.ACP"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Uppercase));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_LITHUANIAN, state.locale);
	setlocale(LC_ALL, "C");

	EXPECT_STREQ("Lithuanian_Lithuania.775", setlocale(LC_ALL, "Lithuanian_Lithuania.OCP"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Uppercase));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_LITHUANIAN, state.locale);
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
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_TURKISH_OR_AZERI_LATIN, state.locale);
	setlocale(LC_ALL, "C");

	EXPECT_STREQ("tr-TR", setlocale(LC_ALL, "tr-TR"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Uppercase));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_TURKISH_OR_AZERI_LATIN, state.locale);
	setlocale(LC_ALL, "C");

	EXPECT_STREQ("Turkish_Turkey.1254", setlocale(LC_ALL, "turkish"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Uppercase));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_TURKISH_OR_AZERI_LATIN, state.locale);
	setlocale(LC_ALL, "C");

	EXPECT_STREQ("Turkish_Turkey.1254", setlocale(LC_ALL, "Turkish_Turkey.1254"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Uppercase));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_TURKISH_OR_AZERI_LATIN, state.locale);
	setlocale(LC_ALL, "C");

	EXPECT_STREQ("Turkish_Turkey.1254", setlocale(LC_ALL, "Turkish_Turkey.ACP"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Uppercase));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_TURKISH_OR_AZERI_LATIN, state.locale);
	setlocale(LC_ALL, "C");

	EXPECT_STREQ("Turkish_Turkey.857", setlocale(LC_ALL, "Turkish_Turkey.OCP"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Uppercase));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_TURKISH_OR_AZERI_LATIN, state.locale);
	setlocale(LC_ALL, "C");
}

TEST(CaseMappingInitialize, LocaleAzeri)
{
	CaseMappingState state;
	const char* i = "Cuisine.";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;

	EXPECT_STREQ("az", setlocale(LC_ALL, "az"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Uppercase));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_TURKISH_OR_AZERI_LATIN, state.locale);
	setlocale(LC_ALL, "C");

	EXPECT_STREQ("az-Cyrl-AZ", setlocale(LC_ALL, "az-Cyrl-AZ"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Uppercase));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_DEFAULT, state.locale);
	setlocale(LC_ALL, "C");

	EXPECT_STREQ("az-Latn-AZ", setlocale(LC_ALL, "az-Latn-AZ"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Uppercase));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_TURKISH_OR_AZERI_LATIN, state.locale);
	setlocale(LC_ALL, "C");

	EXPECT_STREQ("Azeri_Azerbaijan.1254", setlocale(LC_ALL, "azeri"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Uppercase));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_TURKISH_OR_AZERI_LATIN, state.locale);
	setlocale(LC_ALL, "C");

	EXPECT_STREQ("Azeri_Azerbaijan.1254", setlocale(LC_ALL, "Azeri_Azerbaijan.1254"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Uppercase));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_TURKISH_OR_AZERI_LATIN, state.locale);
	setlocale(LC_ALL, "C");

	EXPECT_STREQ("Azeri_Azerbaijan.1254", setlocale(LC_ALL, "Azeri_Azerbaijan.ACP"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Uppercase));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_TURKISH_OR_AZERI_LATIN, state.locale);
	setlocale(LC_ALL, "C");

	EXPECT_STREQ("Azeri_Azerbaijan.857", setlocale(LC_ALL, "Azeri_Azerbaijan.857"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Uppercase));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_TURKISH_OR_AZERI_LATIN, state.locale);
	setlocale(LC_ALL, "C");

	EXPECT_STREQ("Azeri_Azerbaijan.857", setlocale(LC_ALL, "Azeri_Azerbaijan.OCP"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UnicodeProperty_Uppercase));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_TURKISH_OR_AZERI_LATIN, state.locale);
	setlocale(LC_ALL, "C");
}