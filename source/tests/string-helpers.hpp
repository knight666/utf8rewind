#pragma once

#include <utf8rewind.h>
#include <utf8string.hpp>

namespace testing {

	inline std::string GetUnicodeString(unicode_t unicode)
	{
		char formatted[128] = { 0 };
		_snprintf(formatted, 128, "U+%0X", unicode);
		return std::string(formatted);
	}

	inline ::testing::AssertionResult GetFailedIteratorResult(
		const char* a_ExpressionExpected, const char* a_ExpressionActual,
		unsigned int a_ValueExpected, const utf8rewind::Utf8String::iterator& a_ValueActual
	)
	{
		::testing::AssertionResult result = ::testing::AssertionFailure();

		result << "Value of: " << a_ExpressionActual << std::endl;
		result << "  Actual: " << GetUnicodeString(*a_ValueActual) << std::endl;
		result << "Expected: " << a_ExpressionExpected << std::endl;
		result << "Which is: " << GetUnicodeString(a_ValueExpected) << std::endl;

		return result;
	}

	inline ::testing::AssertionResult CompareUtf8(
		const char* a_ExpressionExpected, const char* a_ExpressionActual,
		unicode_t a_ValueExpected, const utf8rewind::Utf8String::iterator& a_ValueActual
		)
	{
		if (a_ValueExpected == *a_ValueActual)
		{
			return ::testing::AssertionSuccess();
		}
		else
		{
			return GetFailedIteratorResult(a_ExpressionExpected, a_ExpressionActual, a_ValueExpected, a_ValueActual);
		}
	}

};

#define EXPECT_UNICODE_EQ(_expected, _actual) EXPECT_PRED_FORMAT2(::testing::CompareUtf8, _expected, _actual)