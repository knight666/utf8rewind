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

	::testing::AssertionResult CompareSeeking2(
		const char* expressionExpected, const char* expressionActual,
		const SeekingParameters& paramsExpected, const SeekingParameters& paramsActual)
	{
		if (!strcmp(paramsExpected.text, paramsActual.text))
		{
			return ::testing::AssertionSuccess();
		}
		else
		{
			const char* input = paramsActual.text - paramsActual.offset;

			::testing::AssertionResult result = ::testing::AssertionFailure();

			result << paramsActual.expression << std::endl;

			result << std::endl;

			result << "[Offset]" << std::endl;
			result << "    Actual: " << paramsActual.offset << std::endl;
			result << "  Expected: " << paramsExpected.offset << std::endl;

			result << std::endl;

			result << "[Text]" << std::endl;
			result << "    Actual: " << hex(input) << std::endl;

			result << "            ";
			for (size_t i = 0; i < strlen(input); i++)
			{
				result << ((i == paramsActual.offset) ? "^^^^" : "----");
			}
			result << std::endl;

			result << "  Expected: " << hex(input) << std::endl;

			result << "            ";
			for (size_t i = 0; i < strlen(input); i++)
			{
				result << ((i == paramsExpected.offset) ? "^^^^" : "----");
			}
			result << std::endl;

			return result;
		}
	}

};