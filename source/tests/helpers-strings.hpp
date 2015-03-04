#pragma once

/*!
	\cond INTERNAL

	\file helpers-strings.hpp
	\brief Functions for working with strings in tests.
*/

#include "tests-base.hpp"

#include "utf8rewind.h"

#define EXPECT_UTF8EQ(_expected, _actual) EXPECT_PRED_FORMAT2(::helpers::CompareUtf8Strings, _expected, _actual);
#define EXPECT_CPEQ(_expected, _actual) EXPECT_PRED_FORMAT2(::helpers::CompareCodepoints, _expected, _actual);

namespace helpers {

	std::string identifiable(unicode_t codepoint);
	std::string identifiable(unicode_t* codepoint, size_t codepointsSize);
	std::string identifiable(const std::string& text);

	std::string utf8(unicode_t codepoint);
	std::string utf8(unicode_t* codepoints, size_t codepointsSize);
	std::string utf8(const std::vector<unicode_t>& codepoints);

	std::vector<unicode_t> utf32(const std::string& text);

	std::string printable(unicode_t codepoint);
	std::string printable(unicode_t* codepoints, size_t codepointsSize);
	std::string printable(const std::string& text);

	std::string sequence(unicode_t codepoint, uint8_t type);
	std::string sequence(unicode_t* codepoint, size_t codepointsSize, uint8_t type);
	std::string sequence(const std::string& text, uint8_t type);

	std::string canonicalCombiningClass(unicode_t codepoint);
	std::string canonicalCombiningClass(unicode_t* codepoint, size_t codepointsSize);
	std::string canonicalCombiningClass(const std::string& text);

	std::string quickCheck(unicode_t codepoint, uint8_t type);
	std::string quickCheck(unicode_t* codepoint, size_t codepointsSize, uint8_t type);
	std::string quickCheck(const std::string& text, uint8_t type);

	::testing::AssertionResult CompareUtf8Strings(
		const char* expressionExpected, const char* expressionActual,
		const char* textExpected, const char* textActual);

	::testing::AssertionResult CompareCodepoints(
		const char* expressionExpected, const char* expressionActual,
		unicode_t codepointExpected, unicode_t codepointActual);

};

/*! \endcond */