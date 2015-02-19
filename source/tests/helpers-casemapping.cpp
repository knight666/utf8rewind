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
		utf8toupper(input_utf8, strlen(input_utf8), result_upper, 256, &errors);
		if (errors != 0)
		{
			return "";
		}

		return result_upper;
	}

	std::string uppercase(const std::string& text)
	{
		int32_t errors = 0;

		std::string converted;

		size_t l = utf8toupper(text.c_str(), text.size() - 1, nullptr, 0, &errors);
		if (l == 0 ||
			errors != 0)
		{
			return converted;
		}

		converted.resize(l + 1);
		utf8toupper(text.c_str(), text.size() - 1, &converted[0], l, &errors);

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
		utf8tolower(input_utf8, strlen(input_utf8), result_lower, 256, &errors);
		if (errors != 0)
		{
			return "";
		}

		return result_lower;
	}

	std::string lowercase(const std::string& text)
	{
		int32_t errors = 0;

		std::string converted;

		size_t l = utf8tolower(text.c_str(), text.size() - 1, nullptr, 0, &errors);
		if (l == 0 ||
			errors != 0)
		{
			return converted;
		}

		converted.resize(l + 1);
		utf8tolower(text.c_str(), text.size() - 1, &converted[0], l, &errors);

		return converted;
	}

	std::string titlecase(unicode_t codepoint)
	{
		int32_t errors = 0;

		char input_utf8[16] = { 0 };
		utf32toutf8(&codepoint, sizeof(unicode_t), input_utf8, 16, &errors);
		if (errors != 0)
		{
			return "";
		}

		char result_title[256] = { 0 };
		utf8totitle(input_utf8, strlen(input_utf8), result_title, 256, &errors);
		if (errors != 0)
		{
			return "";
		}

		return result_title;
	}

	std::string titlecase(const std::string& text)
	{
		int32_t errors = 0;

		std::string converted;

		size_t l = utf8totitle(text.c_str(), text.size() - 1, nullptr, 0, &errors);
		if (l == 0 ||
			errors != 0)
		{
			return converted;
		}

		converted.resize(l + 1);
		utf8totitle(text.c_str(), text.size() - 1, &converted[0], l, &errors);

		return converted;
	}

	::testing::AssertionResult CompareCasemapping(
		const char* expressionExpected GTEST_ATTRIBUTE_UNUSED_, const char* expressionActual GTEST_ATTRIBUTE_UNUSED_,
		const CaseMappingEntry& entryExpected, const CaseMappingEntry& entryActual)
	{
		if (entryExpected.uppercase == entryActual.uppercase &&
			entryExpected.lowercase == entryActual.lowercase &&
			entryExpected.titlecase == entryActual.titlecase)
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

			result << std::endl;

			if (entryExpected.titlecase != entryActual.titlecase)
			{
				result << "[Titlecase]" << std::endl;
				result << "    Actual:  \"" << helpers::printable(entryActual.titlecase) << "\" (" << helpers::identifiable(entryActual.titlecase) << ")" << std::endl;
				result << "  Expected:  \"" << helpers::printable(entryExpected.titlecase) << "\" (" << helpers::identifiable(entryExpected.titlecase) << ")" << std::endl;
			}
			else
			{
				result << "[Titlecase]  \"" << helpers::printable(entryExpected.titlecase) << "\" (" << helpers::identifiable(entryExpected.titlecase) << ")" << std::endl;
			}

			return result;
		}
	}

};