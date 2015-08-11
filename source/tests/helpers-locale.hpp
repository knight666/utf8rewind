#pragma once

/*!
	\file
	\brief Locale helper functions.

	\cond INTERNAL
*/

#include "tests-base.hpp"

#define EXPECT_LOCALE_EQ(_expected, _actual) \
	EXPECT_PRED_FORMAT2(::helpers::CompareLocale, _expected, _actual); \

#define RESET_LOCALE()               setlocale(LC_ALL, "C")

#if _WINDOWS
	#define SET_LOCALE_ENGLISH()     EXPECT_STREQ("en-US", setlocale(LC_ALL, "en-US"))
	#define SET_LOCALE_GREEK()       EXPECT_STREQ("Greek_Greece.1253", setlocale(LC_ALL, "Greek_Greece.1253"))
	#define SET_LOCALE_LITHUANIAN()  EXPECT_STREQ("Lithuanian_Lithuania.1257", setlocale(LC_ALL, "Lithuanian_Lithuania.1257"))
	#define SET_LOCALE_TURKISH()     EXPECT_STREQ("Turkish_Turkey.1254", setlocale(LC_ALL, "Turkish_Turkey.1254"))
	#define SET_LOCALE_AZERI()       EXPECT_STREQ("Azeri_Azerbaijan.1254", setlocale(LC_ALL, "Azeri_Azerbaijan.1254"))
#else
	#define SET_LOCALE_ENGLISH()     EXPECT_STREQ("en_US.utf8", setlocale(LC_ALL, "en_US.utf8"))
	#define SET_LOCALE_GREEK()       EXPECT_STREQ("el_GR.utf8", setlocale(LC_ALL, "el_GR.utf8"))
	#define SET_LOCALE_LITHUANIAN()  EXPECT_STREQ("lt_LT.utf8", setlocale(LC_ALL, "lt_LT.utf8"))
	#define SET_LOCALE_TURKISH()     EXPECT_STREQ("tr_TR.utf8", setlocale(LC_ALL, "tr_TR.utf8"))
	#define SET_LOCALE_AZERI()       EXPECT_STREQ("az_AZ.utf8", setlocale(LC_ALL, "az_AZ.utf8"))
#endif

namespace helpers {

	std::string locale(uint32_t value);

	::testing::AssertionResult CompareLocale(
		const char* expressionExpected, const char* expressionActual,
		uint32_t localeExpected, uint32_t localeActual);

};

/*! \endcond */