#include "helpers-normalization.hpp"

#include "helpers-strings.hpp"

extern "C" {
	#include "../internal/database.h"
};

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

		size_t length = utf8normalize(text.c_str(), text.size(), nullptr, 0, UTF8_NORMALIZE_COMPOSE, &errors);
		if (length == 0 ||
			errors != 0)
		{
			return "";
		}

		char* buffer = new char[length + 1];
		utf8normalize(text.c_str(), text.size(), buffer, length, UTF8_NORMALIZE_COMPOSE, &errors);
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

	std::string PrintCodepoint(const std::string& text)
	{
		std::stringstream ss;
		ss << "\t\"" << printable(text) << "\" (" << identifiable(text) << ")" << std::endl;
		return ss.str();
	}

	::testing::AssertionResult CompareNormalizationCodepoint(
		const char* expressionExpected, const char* expressionActual,
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

			result << entryExpected.name << " (" << identifiable(entryExpected.codepoint) << ")" << std::endl;

			if (entryExpected.decomposed != entryActual.decomposed)
			{
				result << "[NFD] " << std::endl;
				result << "    Actual:  " << PrintCodepoint(entryActual.decomposed) << std::endl;
				result << "  Expected:  " << PrintCodepoint(entryExpected.decomposed) << std::endl;
			}
			else
			{
				result << "[NFD]        ";
				result << PrintCodepoint(entryActual.decomposed) << std::endl;
			}

			if (entryExpected.composed != entryActual.composed)
			{
				result << "[NFC] " << std::endl;
				result << "    Actual:  " << PrintCodepoint(entryActual.composed) << std::endl;
				result << "  Expected:  " << PrintCodepoint(entryExpected.composed) << std::endl;
			}
			else
			{
				result << "[NFC]        ";
				result << PrintCodepoint(entryActual.composed) << std::endl;
			}

			if (entryExpected.decomposedCompatibility != entryActual.decomposedCompatibility)
			{
				result << "[NFKD]" << std::endl;
				result << "    Actual:  " << PrintCodepoint(entryActual.decomposedCompatibility) << std::endl;
				result << "  Expected:  " << PrintCodepoint(entryExpected.decomposedCompatibility) << std::endl;
			}
			else
			{
				result << "[NFKD]       ";
				result << PrintCodepoint(entryActual.decomposedCompatibility) << std::endl;
			}

			if (entryExpected.composedCompatibility != entryActual.composedCompatibility)
			{
				result << "[NFKC]" << std::endl;
				result << "    Actual:  " << PrintCodepoint(entryActual.composedCompatibility) << std::endl;
				result << "  Expected:  " << PrintCodepoint(entryExpected.composedCompatibility);
			}
			else
			{
				result << "[NFKC]       ";
				result << PrintCodepoint(entryActual.composedCompatibility);
			}

			return result;
		}
	}

	std::string PrintSequence(const std::string& text, uint8_t type)
	{
		std::stringstream ss;

		if (text.length() == 0)
		{
			return ss.str();
		}

		std::stringstream ss_id;
		ss_id << std::setfill(' ');
		std::stringstream ss_ccc;
		ss_ccc << std::setfill(' ');
		std::stringstream ss_qc;
		ss_qc << std::setfill(' ');

		std::vector<unicode_t> codepoints = utf32(text);
		for (std::vector<unicode_t>::iterator it = codepoints.begin(); it != codepoints.end(); ++it)
		{
			if (it != codepoints.begin())
			{
				ss_id << " ";
				ss_ccc << " ";
				ss_qc << " ";
			}

			std::string id = identifiable(*it);
			std::string ccc = canonicalCombiningClass(*it);

			size_t padding = std::max(id.length(), ccc.length());
			if (type != 0)
			{
				std::string qc = quickCheck(*it, type);
				padding = std::max(qc.length(), padding);
				ss_qc << std::setw(padding) << qc;
			}

			ss_id << std::setw(padding) << id;
			ss_ccc << std::setw(padding) << ccc;
		}

		ss << ss_id.str() << std::endl;
		ss << "             " << ss_ccc.str();
		if (type != 0)
		{
			ss << std::endl;
			ss << "             " << ss_qc.str();
		}

		return ss.str();
	}

	::testing::AssertionResult CompareNormalizationSequence(
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

			result << std::endl;

			result << "[Source]     " << PrintSequence(entryExpected.sequence, 0) << std::endl;

			if (entryExpected.decomposed != entryActual.decomposed)
			{
				result << "[NFD] " << std::endl;
				result << "    Actual:  " << PrintSequence(entryActual.decomposed, UnicodeProperty_Normalization_Decompose) << std::endl;
				result << "  Expected:  " << PrintSequence(entryExpected.decomposed, UnicodeProperty_Normalization_Decompose) << std::endl;
			}
			else
			{
				result << "[NFD]        ";
				result << PrintSequence(entryActual.decomposed, UnicodeProperty_Normalization_Decompose) << std::endl;
			}

			if (entryExpected.composed != entryActual.composed)
			{
				result << "[NFC] " << std::endl;
				result << "    Actual:  " << PrintSequence(entryActual.composed, UnicodeProperty_Normalization_Compose) << std::endl;
				result << "  Expected:  " << PrintSequence(entryExpected.composed, UnicodeProperty_Normalization_Compose) << std::endl;
			}
			else
			{
				result << "[NFC]        ";
				result << PrintSequence(entryActual.composed, UnicodeProperty_Normalization_Compose) << std::endl;
			}

			if (entryExpected.decomposedCompatibility != entryActual.decomposedCompatibility)
			{
				result << "[NFKD]" << std::endl;
				result << "    Actual:  " << PrintSequence(entryActual.decomposedCompatibility, UnicodeProperty_Normalization_Compatibility_Decompose) << std::endl;
				result << "  Expected:  " << PrintSequence(entryExpected.decomposedCompatibility, UnicodeProperty_Normalization_Compatibility_Decompose) << std::endl;
			}
			else
			{
				result << "[NFKD]       ";
				result << PrintSequence(entryActual.decomposedCompatibility, UnicodeProperty_Normalization_Compatibility_Decompose) << std::endl;
			}

			if (entryExpected.composedCompatibility != entryActual.composedCompatibility)
			{
				result << "[NFKC]" << std::endl;
				result << "    Actual:  " << PrintSequence(entryActual.composedCompatibility, UnicodeProperty_Normalization_Compatibility_Compose) << std::endl;
				result << "  Expected:  " << PrintSequence(entryExpected.composedCompatibility, UnicodeProperty_Normalization_Compatibility_Compose);
			}
			else
			{
				result << "[NFKC]       ";
				result << PrintSequence(entryActual.composedCompatibility, UnicodeProperty_Normalization_Compatibility_Compose);
			}

			return result;
		}
	}

};