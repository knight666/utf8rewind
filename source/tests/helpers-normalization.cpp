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

		size_t length = utf8normalize(text.c_str(), text.size(), nullptr, 0, UTF8_NORMALIZE_DECOMPOSE, &errors);
		if (length == 0 ||
			errors != 0)
		{
			return "";
		}

		char* buffer = new char[length + 1];
		utf8normalize(text.c_str(), text.size(), buffer, length, UTF8_NORMALIZE_DECOMPOSE, &errors);
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

		size_t length = utf8normalize(text.c_str(), text.size(), nullptr, 0, UTF8_NORMALIZE_COMPOSE | UTF8_NORMALIZE_COMPATIBILITY, &errors);
		if (length == 0 ||
			errors != 0)
		{
			return "";
		}

		char* buffer = new char[length + 1];
		utf8normalize(text.c_str(), text.size(), buffer, length, UTF8_NORMALIZE_COMPOSE | UTF8_NORMALIZE_COMPATIBILITY, &errors);
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

		size_t length = utf8normalize(text.c_str(), text.size(), nullptr, 0, UTF8_NORMALIZE_DECOMPOSE | UTF8_NORMALIZE_COMPATIBILITY, &errors);
		if (length == 0 ||
			errors != 0)
		{
			return "";
		}

		char* buffer = new char[length + 1];
		utf8normalize(text.c_str(), text.size(), buffer, length, UTF8_NORMALIZE_DECOMPOSE | UTF8_NORMALIZE_COMPATIBILITY, &errors);
		buffer[length] = 0;

		std::string converted = buffer;

		delete [] buffer;

		return converted;
	}

	std::string PrintNormalizationString(const std::string& text)
	{
		std::stringstream ss;
		if (text.length() > 0)
		ss << "\t\"" << printable(text) << "\"" << std::endl;

		std::stringstream ss_id;
		ss_id << std::setfill(' ');
		std::stringstream ss_ccc;
		ss_ccc << std::setfill(' ');

		std::vector<unicode_t> codepoints = utf32(text);
		for (std::vector<unicode_t>::iterator it = codepoints.begin(); it != codepoints.end(); ++it)
		{
			if (it != codepoints.begin())
			{
				ss_id << " ";
				ss_ccc << " ";
			}
			std::string id = identifiable(*it);
			std::string ccc = canonicalCombiningClass(*it);
			size_t padding = std::max(id.length(), ccc.length());

			ss_id << std::setw(padding) << id;
			ss_ccc << std::setw(padding) << ccc;
		}

		ss << "\t\t" << ss_id.str() << std::endl;
		ss << "\t\t" << ss_ccc.str();

		return ss.str();
	}

	::testing::AssertionResult CompareNormalization(
		const char* expressionExpected GTEST_ATTRIBUTE_UNUSED_, const char* expressionActual GTEST_ATTRIBUTE_UNUSED_,
		const NormalizationEntry& entryExpected, const NormalizationEntry& entryActual)
	{
		if (entryExpected.decomposed == entryActual.decomposed &&
			entryExpected.composed == entryActual.composed &&
			entryExpected.decomposedCompatibility == entryActual.decomposedCompatibility &&
			entryExpected.composedCompatibility == entryActual.composedCompatibility)
		{
			return ::testing::AssertionSuccess();
		}
		else
		{
			::testing::AssertionResult result = ::testing::AssertionFailure();

			if (entryExpected.codepoint != 0)
			{
				result << entryExpected.name << " (" << identifiable(entryExpected.codepoint) << ")" << std::endl;
			}
			else
			{
				result << printable(entryExpected.sequence) << " (" << identifiable(entryExpected.sequence) << ")" << std::endl;
			}

			if (entryExpected.decomposed != entryActual.decomposed)
			{
				result << "[NFD] " << std::endl;
				result << "    Actual:  " << PrintNormalizationString(entryActual.decomposed) << std::endl;
				result << "  Expected:  " << PrintNormalizationString(entryExpected.decomposed) << std::endl;
			}
			else
			{
				result << "[NFD]        ";
				result << PrintNormalizationString(entryActual.decomposed) << std::endl;
			}

			if (entryExpected.composed != entryActual.composed)
			{
				result << "[NFC] " << std::endl;
				result << "    Actual:  " << PrintNormalizationString(entryActual.composed) << std::endl;
				result << "  Expected:  " << PrintNormalizationString(entryExpected.composed) << std::endl;
			}
			else
			{
				result << "[NFC]        ";
				result << PrintNormalizationString(entryActual.composed) << std::endl;
			}

			if (entryExpected.decomposedCompatibility != entryActual.decomposedCompatibility)
			{
				result << "[NFKD]" << std::endl;
				result << "    Actual:  " << PrintNormalizationString(entryActual.decomposedCompatibility) << std::endl;
				result << "  Expected:  " << PrintNormalizationString(entryExpected.decomposedCompatibility) << std::endl;
			}
			else
			{
				result << "[NFKD]       ";
				result << PrintNormalizationString(entryActual.decomposedCompatibility) << std::endl;
			}

			if (entryExpected.composedCompatibility != entryActual.composedCompatibility)
			{
				result << "[NFKC]" << std::endl;
				result << "    Actual:  " << PrintNormalizationString(entryActual.composedCompatibility) << std::endl;
				result << "  Expected:  " << PrintNormalizationString(entryExpected.composedCompatibility);
			}
			else
			{
				result << "[NFKC]       ";
				result << PrintNormalizationString(entryActual.composedCompatibility);
			}

			return result;
		}
	}

};