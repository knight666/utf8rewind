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

			result << paramsActual.expression << std::endl;

			result << "    Actual: " << hex(paramsInput.text) << std::endl;

			size_t marker_actual = paramsActual.text - paramsInput.text;
			result << "            ";
			for (size_t i = 0; i < strlen(paramsInput.text); i++)
			{
				result << ((i == marker_actual) ? "^^^^" : "----");
			}
			result << std::endl;

			result << "  Expected: " << hex(paramsInput.text) << std::endl;

			result << "            ";
			for (size_t i = 0; i < strlen(paramsInput.text); i++)
			{
				result << ((i == paramsExpected.offset) ? "^^^^" : "----");
			}
			result << std::endl;

			return result;
		}
	}

};