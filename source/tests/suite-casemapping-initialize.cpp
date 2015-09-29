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

	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, TitlecaseIndex1Ptr, TitlecaseIndex2Ptr, TitlecaseDataPtr));
	EXPECT_EQ(i, state.src);
	EXPECT_EQ(is, state.src_size);
	EXPECT_EQ(o, state.dst);
	EXPECT_EQ(os, state.dst_size);
	EXPECT_EQ(TitlecaseIndex1Ptr, state.property_index1);
	EXPECT_EQ(TitlecaseIndex2Ptr, state.property_index2);
	EXPECT_EQ(TitlecaseDataPtr, state.property_data);
}

TEST(CaseMappingInitialize, LocaleEnglishUS)
{
	CaseMappingState state;
	const char* i = "Pancakes";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;

	SET_LOCALE_ENGLISH();
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, TitlecaseIndex1Ptr, TitlecaseIndex2Ptr, TitlecaseDataPtr));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_DEFAULT, state.locale);
	RESET_LOCALE();
}

TEST(CaseMappingInitialize, LocaleGreek)
{
	CaseMappingState state;
	const char* i = "Shocking revelations";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;

#if _WINDOWS
	EXPECT_STREQ("el", setlocale(LC_ALL, "el"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_GREEK, state.locale);
	RESET_LOCALE();

	EXPECT_STREQ("el-GR", setlocale(LC_ALL, "el-GR"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_GREEK, state.locale);
	RESET_LOCALE();

	EXPECT_STREQ("Greek_Greece.1253", setlocale(LC_ALL, "greek"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_GREEK, state.locale);
	RESET_LOCALE();

	EXPECT_STREQ("Greek_Greece.1253", setlocale(LC_ALL, "Greek_Greece.1253"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_GREEK, state.locale);
	RESET_LOCALE();

	EXPECT_STREQ("Greek_Greece.1253", setlocale(LC_ALL, "Greek_Greece.ACP"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_GREEK, state.locale);
	RESET_LOCALE();

	EXPECT_STREQ("Greek_Greece.737", setlocale(LC_ALL, "Greek_Greece.OCP"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_GREEK, state.locale);
	RESET_LOCALE();

	EXPECT_STREQ("Greek_Greece.737", setlocale(LC_ALL, "Greek_Greece.OCP"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_GREEK, state.locale);
	RESET_LOCALE();
#else
	EXPECT_STREQ("el_GR", setlocale(LC_ALL, "el_GR"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_GREEK, state.locale);
	RESET_LOCALE();

	EXPECT_STREQ("el_GR.iso88597", setlocale(LC_ALL, "el_GR.iso88597"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_GREEK, state.locale);
	RESET_LOCALE();

	EXPECT_STREQ("el_GR.utf8", setlocale(LC_ALL, "el_GR.utf8"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_GREEK, state.locale);
	RESET_LOCALE();
#endif
}

TEST(CaseMappingInitialize, LocaleLithuanian)
{
	CaseMappingState state;
	const char* i = "Cuisine";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;

#if _WINDOWS
	EXPECT_STREQ("lt", setlocale(LC_ALL, "lt"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_LITHUANIAN, state.locale);
	RESET_LOCALE();

	EXPECT_STREQ("lt-LT", setlocale(LC_ALL, "lt-LT"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_LITHUANIAN, state.locale);
	RESET_LOCALE();

	EXPECT_STREQ("Lithuanian_Lithuania.1257", setlocale(LC_ALL, "lithuanian"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_LITHUANIAN, state.locale);
	RESET_LOCALE();

	EXPECT_STREQ("Lithuanian_Lithuania.1257", setlocale(LC_ALL, "Lithuanian_Lithuania.1257"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_LITHUANIAN, state.locale);
	RESET_LOCALE();

	EXPECT_STREQ("Lithuanian_Lithuania.1257", setlocale(LC_ALL, "Lithuanian_Lithuania.ACP"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_LITHUANIAN, state.locale);
	RESET_LOCALE();

	EXPECT_STREQ("Lithuanian_Lithuania.775", setlocale(LC_ALL, "Lithuanian_Lithuania.775"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_LITHUANIAN, state.locale);
	RESET_LOCALE();

	EXPECT_STREQ("Lithuanian_Lithuania.775", setlocale(LC_ALL, "Lithuanian_Lithuania.OCP"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_LITHUANIAN, state.locale);
	RESET_LOCALE();
#else
	EXPECT_STREQ("lt_LT", setlocale(LC_ALL, "lt_LT"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_LITHUANIAN, state.locale);
	RESET_LOCALE();

	EXPECT_STREQ("lt_LT.iso885913", setlocale(LC_ALL, "lt_LT.iso885913"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_LITHUANIAN, state.locale);
	RESET_LOCALE();

	EXPECT_STREQ("lt_LT.utf8", setlocale(LC_ALL, "lt_LT.utf8"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_LITHUANIAN, state.locale);
	RESET_LOCALE();
#endif
}

TEST(CaseMappingInitialize, LocaleTurkish)
{
	CaseMappingState state;
	const char* i = "I welcome our Turkish friends.";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;

#if _WINDOWS
	EXPECT_STREQ("tr", setlocale(LC_ALL, "tr"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_TURKISH_OR_AZERI_LATIN, state.locale);
	RESET_LOCALE();

	EXPECT_STREQ("tr-TR", setlocale(LC_ALL, "tr-TR"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_TURKISH_OR_AZERI_LATIN, state.locale);
	RESET_LOCALE();

	EXPECT_STREQ("Turkish_Turkey.1254", setlocale(LC_ALL, "turkish"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_TURKISH_OR_AZERI_LATIN, state.locale);
	RESET_LOCALE();

	EXPECT_STREQ("Turkish_Turkey.1254", setlocale(LC_ALL, "Turkish_Turkey.1254"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_TURKISH_OR_AZERI_LATIN, state.locale);
	RESET_LOCALE();

	EXPECT_STREQ("Turkish_Turkey.1254", setlocale(LC_ALL, "Turkish_Turkey.ACP"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_TURKISH_OR_AZERI_LATIN, state.locale);
	RESET_LOCALE();

	EXPECT_STREQ("Turkish_Turkey.857", setlocale(LC_ALL, "Turkish_Turkey.857"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_TURKISH_OR_AZERI_LATIN, state.locale);
	RESET_LOCALE();

	EXPECT_STREQ("Turkish_Turkey.857", setlocale(LC_ALL, "Turkish_Turkey.OCP"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_TURKISH_OR_AZERI_LATIN, state.locale);
	RESET_LOCALE();
#else
	EXPECT_STREQ("tr_TR", setlocale(LC_ALL, "tr_TR"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_TURKISH_OR_AZERI_LATIN, state.locale);
	RESET_LOCALE();

	EXPECT_STREQ("tr_TR.iso88599", setlocale(LC_ALL, "tr_TR.iso88599"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_TURKISH_OR_AZERI_LATIN, state.locale);
	RESET_LOCALE();

	EXPECT_STREQ("tr_TR.utf8", setlocale(LC_ALL, "tr_TR.utf8"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_TURKISH_OR_AZERI_LATIN, state.locale);
	RESET_LOCALE();
#endif
}

TEST(CaseMappingInitialize, LocaleAzeri)
{
	CaseMappingState state;
	const char* i = "Cuisine.";
	size_t is = strlen(i);
	char o[256] = { 0 };
	size_t os = 255;

#if _WINDOWS
	EXPECT_STREQ("az", setlocale(LC_ALL, "az"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_TURKISH_OR_AZERI_LATIN, state.locale);
	RESET_LOCALE();

	EXPECT_STREQ("az-Cyrl-AZ", setlocale(LC_ALL, "az-Cyrl-AZ"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_DEFAULT, state.locale);
	RESET_LOCALE();

	EXPECT_STREQ("az-Latn-AZ", setlocale(LC_ALL, "az-Latn-AZ"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_TURKISH_OR_AZERI_LATIN, state.locale);
	RESET_LOCALE();

	EXPECT_STREQ("Azeri_Azerbaijan.1254", setlocale(LC_ALL, "azeri"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_TURKISH_OR_AZERI_LATIN, state.locale);
	RESET_LOCALE();

	EXPECT_STREQ("Azeri_Azerbaijan.1254", setlocale(LC_ALL, "Azeri_Azerbaijan.1254"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_TURKISH_OR_AZERI_LATIN, state.locale);
	RESET_LOCALE();

	EXPECT_STREQ("Azeri_Azerbaijan.1254", setlocale(LC_ALL, "Azeri_Azerbaijan.ACP"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_TURKISH_OR_AZERI_LATIN, state.locale);
	RESET_LOCALE();

	EXPECT_STREQ("Azeri_Azerbaijan.857", setlocale(LC_ALL, "Azeri_Azerbaijan.857"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_TURKISH_OR_AZERI_LATIN, state.locale);
	RESET_LOCALE();

	EXPECT_STREQ("Azeri_Azerbaijan.857", setlocale(LC_ALL, "Azeri_Azerbaijan.OCP"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_TURKISH_OR_AZERI_LATIN, state.locale);
	RESET_LOCALE();
#else
	EXPECT_STREQ("az_AZ", setlocale(LC_ALL, "az_AZ"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_TURKISH_OR_AZERI_LATIN, state.locale);
	RESET_LOCALE();

	EXPECT_STREQ("az_AZ.utf8", setlocale(LC_ALL, "az_AZ.utf8"));
	EXPECT_TRUE(casemapping_initialize(&state, i, is, o, os, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr));
	EXPECT_LOCALE_EQ(CASEMAPPING_LOCALE_TURKISH_OR_AZERI_LATIN, state.locale);
	RESET_LOCALE();
#endif
}