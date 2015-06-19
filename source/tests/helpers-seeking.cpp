#include "helpers-seeking.hpp"

#include "helpers-strings.hpp"

namespace helpers {

	::testing::AssertionResult CompareSeeking(
		const char* expressionExpected, const char* expressionActual, const char* expressionInput,
		const SeekingParameters& paramsExpected, const SeekingParameters& paramsActual, const SeekingParameters& paramsInput)
	{
		if (!strcmp(paramsExpected.text, paramsActual.text))
		{
			return ::testing::AssertionSuccess();
		}
		else
		{
			::testing::AssertionResult result = ::testing::AssertionFailure();

			result << "Offset mismatch" << std::endl;

			result << std::endl;

			result << paramsActual.expression << std::endl;

			result << std::endl;

			result << "    Actual: " << "\"" << printable(paramsActual.text) << "\" (offset " << (ptrdiff_t)(paramsActual.text - paramsInput.text) << ")" << std::endl;
			result << "  Expected: " << "\"" << printable(paramsExpected.text) << "\" (offset " << paramsExpected.offset << ")" << std::endl;

			result << std::endl;

			return result;
		}
	}

};