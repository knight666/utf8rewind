#include "helpers-casemapping.hpp"

#include "helpers-strings.hpp"

extern "C" {
	#include "../internal/casemapping.h"
}

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

	std::string locale(uint32_t value)
	{
	#define LOCALE_CASE(_name) case CASEMAPPING_LOCALE_ ## _name: return "CASEMAPPING_LOCALE_" # _name;

		switch (value)
		{

		LOCALE_CASE(DEFAULT);
		LOCALE_CASE(LITHUANIAN);
		LOCALE_CASE(TURKISH);
		LOCALE_CASE(AZERI);
		
		default:
			std::stringstream ss;
			ss << "<invalid> (0x";
			ss << std::hex << std::setfill('0') << std::setw(2) << std::uppercase << error;
			ss << ")";
			return ss.str();

		}

	#undef ERROR_CASE
	}

	::testing::AssertionResult CompareCodepoint(
		const char* expressionExpected GTEST_ATTRIBUTE_UNUSED_, const char* expressionActual GTEST_ATTRIBUTE_UNUSED_,
		const CaseMappingEntry& entryExpected, const CaseMappingEntry& entryActual)
	{
		if (entryExpected.lowercase == entryActual.lowercase &&
			entryExpected.uppercase == entryActual.uppercase &&
			entryExpected.titlecase == entryActual.titlecase)
		{
			return ::testing::AssertionSuccess();
		}
		else
		{
			::testing::AssertionResult result = ::testing::AssertionFailure();

			result << entryExpected.name << " (" << identifiable(entryExpected.codepoint)  << ")" << std::endl;

			result << std::endl;

			if (entryExpected.lowercase != entryActual.lowercase)
			{
				result << "[Lowercase]" << std::endl;
				result << "    Actual:  \"" << printable(entryActual.lowercase) << "\" (" << identifiable(entryActual.lowercase) << ")" << std::endl;
				result << "  Expected:  \"" << printable(entryExpected.lowercase) << "\" (" << identifiable(entryExpected.lowercase) << ")" << std::endl;
			}
			else
			{
				result << "[Lowercase]  \"" << printable(entryExpected.lowercase) << "\" (" << identifiable(entryExpected.lowercase) << ")" << std::endl;
			}

			result << std::endl;

			if (entryExpected.uppercase != entryActual.uppercase)
			{
				result << "[Uppercase]" << std::endl;
				result << "    Actual:  \"" << printable(entryActual.uppercase) << "\" (" << identifiable(entryActual.uppercase) << ")" << std::endl;
				result << "  Expected:  \"" << printable(entryExpected.uppercase) << "\" (" << identifiable(entryExpected.uppercase) << ")" << std::endl;
			}
			else
			{
				result << "[Uppercase]  \"" << printable(entryActual.uppercase) << "\" (" << identifiable(entryActual.uppercase) << ")" << std::endl;
			}

			result << std::endl;

			if (entryExpected.titlecase != entryActual.titlecase)
			{
				result << "[Titlecase]" << std::endl;
				result << "    Actual:  \"" << printable(entryActual.titlecase) << "\" (" << identifiable(entryActual.titlecase) << ")" << std::endl;
				result << "  Expected:  \"" << printable(entryExpected.titlecase) << "\" (" << identifiable(entryExpected.titlecase) << ")" << std::endl;
			}
			else
			{
				result << "[Titlecase]  \"" << printable(entryExpected.titlecase) << "\" (" << identifiable(entryExpected.titlecase) << ")" << std::endl;
			}

			return result;
		}
	}

	::testing::AssertionResult CompareCaseMapping(
		const char* expressionExpected, const char* expressionActual,
		const CaseMappingEntry& entryExpected, const CaseMappingEntry& entryActual)
	{
		if (entryExpected.lowercase == entryActual.lowercase &&
			entryExpected.uppercase == entryActual.uppercase &&
			entryExpected.titlecase == entryActual.titlecase)
		{
			return ::testing::AssertionSuccess();
		}
		else
		{
			::testing::AssertionResult result = ::testing::AssertionFailure();

			result << "Input: \"" << printable(entryActual.input) << "\" (" << identifiable(entryActual.input) << ")" << std::endl;

			result << std::endl;

			if (entryExpected.lowercase != entryActual.lowercase)
			{
				result << "[Lowercase]" << std::endl;
				result << "    Actual:  \"" << printable(entryActual.lowercase) << "\" (" << identifiable(entryActual.lowercase) << ")" << std::endl;
				result << "  Expected:  \"" << printable(entryExpected.lowercase) << "\" (" << identifiable(entryExpected.lowercase) << ")" << std::endl;
			}
			else
			{
				result << "[Lowercase]  \"" << printable(entryExpected.lowercase) << "\" (" << identifiable(entryExpected.lowercase) << ")" << std::endl;
			}

			result << std::endl;

			if (entryExpected.uppercase != entryActual.uppercase)
			{
				result << "[Uppercase]" << std::endl;
				result << "    Actual:  \"" << printable(entryActual.uppercase) << "\" (" << identifiable(entryActual.uppercase) << ")" << std::endl;
				result << "  Expected:  \"" << printable(entryExpected.uppercase) << "\" (" << identifiable(entryExpected.uppercase) << ")" << std::endl;
			}
			else
			{
				result << "[Uppercase]  \"" << printable(entryActual.uppercase) << "\" (" << identifiable(entryActual.uppercase) << ")" << std::endl;
			}

			result << std::endl;

			if (entryExpected.titlecase != entryActual.titlecase)
			{
				result << "[Titlecase]" << std::endl;
				result << "    Actual:  \"" << printable(entryActual.titlecase) << "\" (" << identifiable(entryActual.titlecase) << ")" << std::endl;
				result << "  Expected:  \"" << printable(entryExpected.titlecase) << "\" (" << identifiable(entryExpected.titlecase) << ")" << std::endl;
			}
			else
			{
				result << "[Titlecase]  \"" << printable(entryExpected.titlecase) << "\" (" << identifiable(entryExpected.titlecase) << ")" << std::endl;
			}

			return result;
		}
	}

	::testing::AssertionResult CompareLocale(
		const char* expressionExpected GTEST_ATTRIBUTE_UNUSED_, const char* expressionActual,
		uint32_t localeExpected, uint32_t localeActual)
	{
		if (localeExpected == localeActual)
		{
			return ::testing::AssertionSuccess();
		}
		else
		{
			::testing::AssertionResult result = ::testing::AssertionFailure();

			result << "Value of: " << expressionActual << std::endl;

			result << "  Actual: " << locale(localeActual) << " (" << localeActual << ")" << std::endl;
			result << "Expected: " << locale(localeExpected) << " (" << localeExpected << ")";

			return result;
		}
	}

};