#include "helpers-casemapping.hpp"

#include "helpers-strings.hpp"

namespace helpers {

	std::string uppercase(unicode_t codepoint)
	{
		return uppercase(utf8(codepoint));
	}

	std::string uppercase(const std::string& text)
	{
		std::string converted;
		int32_t errors;

		size_t size_in_bytes = utf8toupper(text.c_str(), text.length(), nullptr, 0, &errors);
		if (size_in_bytes == 0 ||
			errors != UTF8_ERR_NONE)
		{
			return converted;
		}

		converted.resize(size_in_bytes);
		utf8toupper(text.c_str(), text.length(), &converted[0], size_in_bytes, nullptr);

		return converted;
	}

	std::string lowercase(unicode_t codepoint)
	{
		return lowercase(utf8(codepoint));
	}

	std::string lowercase(const std::string& text)
	{
		std::string converted;
		int32_t errors;

		size_t size_in_bytes = utf8tolower(text.c_str(), text.length(), nullptr, 0, &errors);
		if (size_in_bytes == 0 ||
			errors != UTF8_ERR_NONE)
		{
			return converted;
		}

		converted.resize(size_in_bytes);
		utf8tolower(text.c_str(), text.length(), &converted[0], size_in_bytes, nullptr);

		return converted;
	}

	std::string titlecase(unicode_t codepoint)
	{
		return titlecase(utf8(codepoint));
	}

	std::string titlecase(const std::string& text)
	{
		std::string converted;
		int32_t errors;

		size_t size_in_bytes = utf8totitle(text.c_str(), text.length(), nullptr, 0, &errors);
		if (size_in_bytes == 0 ||
			errors != UTF8_ERR_NONE)
		{
			return converted;
		}

		converted.resize(size_in_bytes);
		utf8totitle(text.c_str(), text.length(), &converted[0], size_in_bytes, nullptr);

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