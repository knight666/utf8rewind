#include "helpers-normalization.hpp"

#include "helpers-strings.hpp"

namespace helpers {

	std::string nfc(unicode_t codepointLeft, unicode_t codepointRight)
	{
		std::string converted;
		converted += utf8(codepointLeft);
		converted += utf8(codepointRight);

		return nfc(converted);
	}

	std::string nfc(const std::string& text)
	{
		int32_t errors = 0;

		size_t length = utf8transform(text.c_str(), text.size(), nullptr, 0, UTF8_TRANSFORM_COMPOSED, &errors);
		if (length == 0 ||
			errors != 0)
		{
			return "";
		}

		char* buffer = new char[length + 1];
		utf8transform(text.c_str(), text.size(), buffer, length, UTF8_TRANSFORM_COMPOSED, &errors);
		buffer[length] = 0;

		std::string converted = buffer;

		delete [] buffer;

		return converted;
	}

	std::string nfd(unicode_t codepoint)
	{
		return nfd(utf8(codepoint));
	}

	std::string nfd(const std::string& text)
	{
		int32_t errors = 0;

		size_t length = utf8transform(text.c_str(), text.size(), nullptr, 0, UTF8_TRANSFORM_DECOMPOSED, &errors);
		if (length == 0 ||
			errors != 0)
		{
			return "";
		}

		char* buffer = new char[length + 1];
		utf8transform(text.c_str(), text.size(), buffer, length, UTF8_TRANSFORM_DECOMPOSED, &errors);
		buffer[length] = 0;

		std::string converted = buffer;

		delete [] buffer;

		return converted;
	}

	std::string nfkc(unicode_t codepointLeft, unicode_t codepointRight)
	{
		std::string converted;
		converted += utf8(codepointLeft);
		converted += utf8(codepointRight);

		return nfkc(converted);
	}

	std::string nfkc(const std::string& text)
	{
		int32_t errors = 0;

		size_t length = utf8transform(text.c_str(), text.size(), nullptr, 0, UTF8_TRANSFORM_COMPATIBILITY_COMPOSED, &errors);
		if (length == 0 ||
			errors != 0)
		{
			return "";
		}

		char* buffer = new char[length + 1];
		utf8transform(text.c_str(), text.size(), buffer, length, UTF8_TRANSFORM_COMPATIBILITY_COMPOSED, &errors);
		buffer[length] = 0;

		std::string converted = buffer;

		delete [] buffer;

		return converted;
	}

	std::string nfkd(unicode_t codepoint)
	{
		return nfkd(utf8(codepoint));
	}

	std::string nfkd(const std::string& text)
	{
		int32_t errors = 0;

		size_t length = utf8transform(text.c_str(), text.size(), nullptr, 0, UTF8_TRANSFORM_COMPATIBILITY_DECOMPOSED, &errors);
		if (length == 0 ||
			errors != 0)
		{
			return "";
		}

		char* buffer = new char[length + 1];
		utf8transform(text.c_str(), text.size(), buffer, length, UTF8_TRANSFORM_COMPATIBILITY_DECOMPOSED, &errors);
		buffer[length] = 0;

		std::string converted = buffer;

		delete [] buffer;

		return converted;
	}

	::testing::AssertionResult CompareNormalization(
		const char* expressionExpected, const char* expressionActual,
		const NormalizationEntry& entryExpected, const NormalizationEntry& entryActual)
	{
		if (entryExpected.decomposed == entryActual.decomposed &&
			entryExpected.composed == entryActual.composed)
		{
			return ::testing::AssertionSuccess();
		}
		else
		{
			::testing::AssertionResult result = ::testing::AssertionFailure();

			result << "Normalization failed" << std::endl;
			result << identifiable(entryExpected.codepoint) << " (" << entryExpected.name << ")" << std::endl;
			result << std::endl;

			if (entryExpected.decomposed != entryActual.decomposed)
			{
				result << std::endl;
				result << (entryExpected.compatibility ? "[NFKD]" : "[NFD] ") << std::endl;
				result << "    Actual:   " << printable(entryActual.decomposed) << " (" << identifiable(entryActual.decomposed) << ")" << std::endl;
				result << "  Expected:   " << printable(entryExpected.decomposed) << " (" << identifiable(entryExpected.decomposed) << ")" << std::endl;
				result << std::endl;
			}
			else
			{
				result << (entryExpected.compatibility ? "[NFKD]" : "[NFD] ") << "        ";
				result << printable(entryActual.decomposed) << " (" << identifiable(entryActual.decomposed) << ")" << std::endl;
			}

			if (entryExpected.composed != entryActual.composed)
			{
				result << std::endl;
				result << (entryExpected.compatibility ? "[NFKC]" : "[NFC] ") << std::endl;
				result << "    Actual:   " << printable(entryActual.composed) << " (" << identifiable(entryActual.composed) << ")" << std::endl;
				result << "  Expected:   " << printable(entryExpected.composed) << " (" << identifiable(entryExpected.composed) << ")" << std::endl;
				result << std::endl;
			}
			else
			{
				result << (entryExpected.compatibility ? "[NFKC]" : "[NFC] ") << "        ";
				result << printable(entryActual.composed) << " (" << identifiable(entryActual.composed) << ")" << std::endl;
			}

			return result;
		}
	}

};