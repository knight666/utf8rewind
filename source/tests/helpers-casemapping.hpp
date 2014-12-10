#pragma once

#include "tests-base.hpp"

#include "utf8rewind.h"

#define CHECK_UTF8_UPPERCASE(_codepoint, _name, _expected) { \
	::helpers::CaseMappingEntry e; \
	e.codepoint = _codepoint; \
	e.uppercase = _expected; \
	e.name = _name; \
	::helpers::CaseMappingEntry a; \
	a.codepoint = _codepoint; \
	a.uppercase = ::helpers::uppercase(_codepoint); \
	a.name = _name; \
	EXPECT_PRED_FORMAT2(::helpers::CompareUtf8Uppercase, e, a); \
}

#define CHECK_UTF8_LOWERCASE(_codepoint, _name, _expected) { \
	::helpers::CaseMappingEntry e; \
	e.codepoint = _codepoint; \
	e.lowercase = _expected; \
	e.name = _name; \
	::helpers::CaseMappingEntry a; \
	a.codepoint = _codepoint; \
	a.lowercase = ::helpers::lowercase(_codepoint); \
	a.name = _name; \
	EXPECT_PRED_FORMAT2(::helpers::CompareUtf8Lowercase, e, a); \
}

namespace helpers {

	std::string uppercase(unicode_t codepoint);
	std::string uppercase(const std::string& text);

	std::string lowercase(unicode_t codepoint);
	std::string lowercase(const std::string& text);

	struct CaseMappingEntry
	{
		CaseMappingEntry()
			: codepoint(0)
		{
		}

		unicode_t codepoint;
		std::string uppercase;
		std::string lowercase;
		std::string name;
	};

	::testing::AssertionResult CompareUtf8Uppercase(
		const char* expressionExpected, const char* expressionActual,
		const CaseMappingEntry& entryExpected, const CaseMappingEntry& entryActual);

	::testing::AssertionResult CompareUtf8Lowercase(
		const char* expressionLeft, const char* expressionRight,
		const CaseMappingEntry& entryExpected, const CaseMappingEntry& entryActual);

};