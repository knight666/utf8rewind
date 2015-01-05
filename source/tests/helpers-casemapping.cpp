#include "helpers-casemapping.hpp"

#include "helpers-strings.hpp"

namespace helpers {

	std::string uppercase(unicode_t codepoint)
	{
		int32_t errors = 0;

		char input_utf8[16] = { 0 };
		utf32toutf8(&codepoint, sizeof(unicode_t), input_utf8, 16, &errors);
		if (errors != 0)
		{
			return "";
		}

		char result_upper[256] = { 0 };
		utf8toupper(input_utf8, strlen(input_utf8), result_upper, 256, 0, &errors);
		if (errors != 0)
		{
			return "";
		}

		return result_upper;
	}

	std::string uppercase(const std::string& value)
	{
		int32_t errors = 0;

		std::string converted;

		size_t l = utf8toupper(value.c_str(), value.size() - 1, nullptr, 0, 0, &errors);
		if (l == 0 ||
			errors != 0)
		{
			return converted;
		}

		converted.resize(l + 1);
		utf8toupper(value.c_str(), value.size() - 1, &converted[0], l, 0, &errors);

		return converted;
	}

	std::string lowercase(unicode_t codepoint)
	{
		int32_t errors = 0;

		char input_utf8[16] = { 0 };
		utf32toutf8(&codepoint, sizeof(unicode_t), input_utf8, 16, &errors);
		if (errors != 0)
		{
			return "";
		}

		char result_lower[256] = { 0 };
		utf8tolower(input_utf8, strlen(input_utf8), result_lower, 256, 0, &errors);
		if (errors != 0)
		{
			return "";
		}

		return result_lower;
	}

	std::string lowercase(const std::string& value)
	{
		int32_t errors = 0;

		std::string converted;

		size_t l = utf8tolower(value.c_str(), value.size() - 1, nullptr, 0, 0, &errors);
		if (l == 0 ||
			errors != 0)
		{
			return converted;
		}

		converted.resize(l + 1);
		utf8tolower(value.c_str(), value.size() - 1, &converted[0], l, 0, &errors);

		return converted;
	}

	::testing::AssertionResult CompareCasemapping(
		const char* expressionExpected GTEST_ATTRIBUTE_UNUSED_, const char* expressionActual GTEST_ATTRIBUTE_UNUSED_,
		const CaseMappingEntry& entryExpected, const CaseMappingEntry& entryActual)
	{
		if (entryExpected.uppercase == entryActual.uppercase &&
			entryExpected.lowercase == entryActual.lowercase)
		{
			return ::testing::AssertionSuccess();
		}
		else
		{
			::testing::AssertionResult result = ::testing::AssertionFailure();

			result <<  entryExpected.name << " (" << helpers::identifiable(entryExpected.codepoint)  << ")" << std::endl;

			result << std::endl;

			if (entryExpected.uppercase != entryActual.uppercase)
			{
				result << "[Uppercase]" << std::endl;
				result << "    Actual:  \"" << helpers::printable(entryActual.uppercase) << "\" (" << helpers::identifiable(entryActual.uppercase) << ")" << std::endl;
				result << "  Expected:  \"" << helpers::printable(entryExpected.uppercase) << "\" (" << helpers::identifiable(entryExpected.uppercase) << ")" << std::endl;
			}
			else
			{
				result << "[Uppercase]  \"" << helpers::printable(entryActual.uppercase) << "\" (" << helpers::identifiable(entryActual.uppercase) << ")" << std::endl;
			}

			result << std::endl;

			if (entryExpected.lowercase != entryActual.lowercase)
			{
				result << "[Lowercase]" << std::endl;
				result << "    Actual:  \"" << helpers::printable(entryActual.lowercase) << "\" (" << helpers::identifiable(entryActual.lowercase) << ")" << std::endl;
				result << "  Expected:  \"" << helpers::printable(entryExpected.lowercase) << "\" (" << helpers::identifiable(entryExpected.lowercase) << ")" << std::endl;
			}
			else
			{
				result << "[Lowercase]  \"" << helpers::printable(entryExpected.lowercase) << "\" (" << helpers::identifiable(entryExpected.lowercase) << ")" << std::endl;
			}

			return result;
		}
	}

};