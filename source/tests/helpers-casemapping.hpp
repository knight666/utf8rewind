#pragma once

/*!
	\file
	\brief Case mapping helper functions.

	\cond INTERNAL
*/

#include "tests-base.hpp"

#include "utf8rewind.h"

#define CHECK_CASEMAPPING_NUL(_codepoint, _name) { \
	::helpers::CaseMappingEntry e; \
	e.codepoint = _codepoint; \
	e.uppercase = std::string(1, '\0'); \
	e.lowercase = std::string(1, '\0'); \
	e.titlecase = std::string(1, '\0'); \
	e.name = _name; \
	::helpers::CaseMappingEntry a; \
	a.uppercase = ::helpers::uppercase(_codepoint); \
	a.lowercase = ::helpers::lowercase(_codepoint); \
	a.titlecase = ::helpers::titlecase(_codepoint); \
	EXPECT_PRED_FORMAT2(::helpers::CompareCodepoint, e, a); \
}

#define CHECK_CASEMAPPING(_codepoint, _uppercase, _lowercase, _titlecase, _name) { \
	::helpers::CaseMappingEntry e; \
	e.codepoint = _codepoint; \
	e.uppercase = _uppercase; \
	e.lowercase = _lowercase; \
	e.titlecase = _titlecase; \
	e.name = _name; \
	::helpers::CaseMappingEntry a; \
	a.uppercase = ::helpers::uppercase(_codepoint); \
	a.lowercase = ::helpers::lowercase(_codepoint); \
	a.titlecase = ::helpers::titlecase(_codepoint); \
	EXPECT_PRED_FORMAT2(::helpers::CompareCodepoint, e, a); \
}

#define EXPECT_CASEMAPPING_EQ(_input, _lowercase, _uppercase, _titlecase) { \
	::helpers::CaseMappingEntry e; \
	e.lowercase = _lowercase; \
	e.uppercase = _uppercase; \
	e.titlecase = _titlecase; \
	::helpers::CaseMappingEntry a; \
	a.input = _input; \
	a.lowercase = ::helpers::lowercase(_input); \
	a.uppercase = ::helpers::uppercase(_input); \
	a.titlecase = ::helpers::titlecase(_input); \
	EXPECT_PRED_FORMAT2(::helpers::CompareCaseMapping, e, a); \
}

#define EXPECT_LOCALE_EQ(_expected, _actual) \
	EXPECT_PRED_FORMAT2(::helpers::CompareLocale, _expected, _actual); \

namespace helpers {

	std::string uppercase(unicode_t codepoint);
	std::string uppercase(const std::string& text);

	std::string lowercase(unicode_t codepoint);
	std::string lowercase(const std::string& text);

	std::string titlecase(unicode_t codepoint);
	std::string titlecase(const std::string& text);

	std::string locale(uint32_t value);

	struct CaseMappingEntry
	{
		CaseMappingEntry()
			: codepoint(0)
		{
		}

		unicode_t codepoint;
		std::string name;
		std::string input;
		std::string lowercase;
		std::string uppercase;
		std::string titlecase;
	};

	::testing::AssertionResult CompareCodepoint(
		const char* expressionExpected, const char* expressionActual,
		const CaseMappingEntry& entryExpected, const CaseMappingEntry& entryActual);

	::testing::AssertionResult CompareCaseMapping(
		const char* expressionExpected, const char* expressionActual,
		const CaseMappingEntry& entryExpected, const CaseMappingEntry& entryActual);

	::testing::AssertionResult CompareLocale(
		const char* expressionExpected, const char* expressionActual,
		uint32_t localeExpected, uint32_t localeActual);

};

/*! \endcond */