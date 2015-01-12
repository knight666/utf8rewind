#pragma once

#include "tests-base.hpp"

#include "utf8rewind.h"

#define EXPECT_UTF8EQ(_expected, _actual) EXPECT_PRED_FORMAT2(::helpers::CompareUtf8Strings, _expected, _actual);

namespace helpers {

	std::string identifiable(unicode_t codepoint);
	std::string identifiable(unicode_t* codepoint, size_t codepointsSize);
	std::string identifiable(const std::string& text);

	std::string utf8(unicode_t codepoint);
	std::string utf8(unicode_t* codepoints, size_t codepointsSize);
	std::string utf8(const std::vector<unicode_t>& codepoints);

	std::string printable(unicode_t codepoint);
	std::string printable(unicode_t* codepoints, size_t codepointsSize);
	std::string printable(const std::string& text);

	::testing::AssertionResult CompareUtf8Strings(
		const char* expressionExpected, const char* expressionActual,
		const char* textExpected, const char* textActual
	);

};