#pragma once

/*!
	\file
	\brief String helper functions.

	\cond INTERNAL
*/

#include "helpers-base.hpp"

#define EXPECT_UTF8EQ(_expected, _actual)                 EXPECT_PRED_FORMAT2(::helpers::CompareUtf8Strings, _expected, _actual)
#define EXPECT_UTF8LENGTHEQ(_expected, _actual, _length)  EXPECT_PRED_FORMAT3(::helpers::CompareUtf8LengthStrings, _expected, _actual, _length)
#define EXPECT_OFFSETEQ(_expected, _actual, _start)       EXPECT_PRED_FORMAT3(::helpers::CompareOffsets, _expected, _actual, _start)
#define EXPECT_MEMEQ(_expected, _actual, _size)           EXPECT_PRED_FORMAT3(::helpers::CompareMemory, _expected, _actual, _size)
#define EXPECT_CPEQ(_expected, _actual)                   EXPECT_PRED_FORMAT2(::helpers::CompareCodepoints, _expected, _actual)

namespace helpers {

	std::string identifiable(unicode_t codepoint);
	std::string identifiable(unicode_t* codepoint, size_t codepointsSize);
	std::string identifiable(const std::string& text);

	std::string utf8(unicode_t codepoint);
	std::string utf8(unicode_t* codepoints, size_t codepointsSize);
	std::string utf8(const std::vector<unicode_t>& codepoints);
	std::string utf8(const std::wstring& text);

	std::vector<utf16_t> utf16(const std::string& text);

	std::vector<unicode_t> utf32(const std::string& text);

	std::wstring wide(const std::string& text);

	std::string hex(unicode_t codepoint);
	std::string hex(unicode_t* codepoints, size_t codepointsSize);
	std::string hex(const std::string& text);

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

	::testing::AssertionResult CompareUtf8LengthStrings(
		const char* expressionExpected, const char* expressionActual, const char* expressionLength,
		const char* textExpected, const char* textActual, size_t length);

	::testing::AssertionResult CompareOffsets(
		const char* expressionExpected, const char* expressionActual, const char* expressionCount,
		const char* offsetExpected, const char* offsetActual, const char* offsetStart);

	::testing::AssertionResult CompareMemory(
		const char* expressionExpected, const char* expressionActual, const char* expressionCount,
		const char* memoryExpected, const char* memoryActual, size_t memorySize);

	::testing::AssertionResult CompareCodepoints(
		const char* expressionExpected, const char* expressionActual,
		unicode_t codepointExpected, unicode_t codepointActual);

};

/*! \endcond */