#pragma once

#include "tests-base.hpp"

#include "utf8rewind.h"

#define CHECK_UTF8_CASEMAPPING(_codepoint, _uppercase, _lowercase, _titlecase, _name) { \
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
	EXPECT_PRED_FORMAT2(::helpers::CompareCasemapping, e, a); \
}

namespace helpers {

	std::string uppercase(unicode_t codepoint);
	std::string uppercase(const std::string& text);

	std::string lowercase(unicode_t codepoint);
	std::string lowercase(const std::string& text);

	std::string titlecase(unicode_t codepoint);
	std::string titlecase(const std::string& text);

	struct CaseMappingEntry
	{
		CaseMappingEntry()
			: codepoint(0)
		{
		}

		unicode_t codepoint;
		std::string uppercase;
		std::string lowercase;
		std::string titlecase;
		std::string name;
	};

	::testing::AssertionResult CompareCasemapping(
		const char* expressionExpected, const char* expressionActual,
		const CaseMappingEntry& entryExpected, const CaseMappingEntry& entryActual);

};