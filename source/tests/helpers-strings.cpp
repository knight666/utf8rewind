#include "helpers-strings.hpp"

extern "C" {
	#include "../internal/database.h"
};

namespace helpers {

	void identifiable(std::stringstream& target, unicode_t codepoint)
	{
		target << "U+" << std::hex << std::setfill('0') << std::setw(4) << std::uppercase << codepoint;
	}

	std::string identifiable(unicode_t codepoint)
	{
		std::stringstream ss;
		identifiable(ss, codepoint);
		return ss.str();
	}

	std::string identifiable(unicode_t* codepoint, size_t codepointsSize)
	{
		std::stringstream ss;

		identifiable(ss, codepoint[0]);

		for (size_t i = 1; i < codepointsSize / sizeof(unicode_t); ++i)
		{
			ss << " ";
			identifiable(ss, codepoint[i]);
		}

		return ss.str();
	}

	std::string identifiable(const std::string& text)
	{
		std::vector<unicode_t> converted = utf32(text);
		if (converted.size() == 0)
		{
			return "";
		}

		std::stringstream ss;

		for (std::vector<unicode_t>::iterator it = converted.begin(); it != converted.end(); ++it)
		{
			if (it != converted.begin())
			{
				ss << " ";
			}
			identifiable(ss, *it);
		}

		return ss.str();
	}

	std::string utf8(unicode_t codepoint)
	{
		int32_t errors = 0;
		size_t size_in_bytes = utf32toutf8(&codepoint, sizeof(unicode_t), nullptr, 0, &errors);

		if (size_in_bytes == 0 ||
			errors != 0)
		{
			return "";
		}

		char buffer[32] = { 0 };
		utf32toutf8(&codepoint, sizeof(unicode_t), buffer, 31, &errors);
		return std::string(buffer);
	}

	std::string utf8(unicode_t* codepoints, size_t codepointsSize)
	{
		std::string converted;

		int32_t errors = 0;
		size_t size_in_bytes = utf32toutf8(codepoints, codepointsSize, nullptr, 0, &errors);

		if (size_in_bytes == 0 ||
			errors != 0)
		{
			return converted;
		}

		char* buffer = new char[size_in_bytes + 1];
		utf32toutf8(codepoints, codepointsSize, buffer, size_in_bytes, &errors);
		buffer[size_in_bytes] = 0;

		converted = buffer;

		delete [] buffer;

		return converted;
	}

	std::string utf8(const std::vector<unicode_t>& codepoints)
	{
		std::string converted;

		int32_t errors = 0;
		size_t size_in_bytes = utf32toutf8(&codepoints[0], codepoints.size() * sizeof(unicode_t), nullptr, 0, &errors);

		if (size_in_bytes == 0 ||
			errors != 0)
		{
			return converted;
		}

		char* buffer = new char[size_in_bytes + 1];
		utf32toutf8(&codepoints[0], codepoints.size() * sizeof(unicode_t), buffer, size_in_bytes, &errors);
		buffer[size_in_bytes] = 0;

		converted = buffer;

		delete [] buffer;

		return converted;
	}

	std::vector<unicode_t> utf32(const std::string& text)
	{
		std::vector<unicode_t> converted;

		if (text.length() == 0)
		{
			return converted;
		}

		int32_t errors = 0;

		size_t size_in_bytes = utf8toutf32(text.c_str(), text.size(), nullptr, 0, &errors);
		if (size_in_bytes == 0 ||
			errors != 0)
		{
			return converted;
		}

		converted.resize(size_in_bytes / sizeof(unicode_t));
		utf8toutf32(text.c_str(), text.size(), &converted[0], size_in_bytes, &errors);

		return converted;
	}

	void printable(std::stringstream& target, bool& wroteHex, uint8_t character)
	{
		if (character < 0x20)
		{
			wroteHex = false;

			switch (character)
			{

			case 0:
				break;

			case '\a':
				target << "\\a";
				break;

			case '\b':
				target << "\\b";
				break;

			case '\f':
				target << "\\f";
				break;

			case '\n':
				target << "\\n";
				break;

			case '\r':
				target << "\\r";
				break;

			case '\t':
				target << "\\t";
				break;

			case '\v':
				target << "\\v";
				break;

			default:
				target << "\\x" << std::uppercase << std::setfill('0') << std::hex << std::setw(2) << ((unicode_t)character & 0x000000FF);
				wroteHex = true;
				break;

			}
		}
		else if (character <= 0x7F)
		{
			if (wroteHex)
			{
				if ((character >= 'A' && character <= 'F') ||
					(character >= 'a' && character <= 'f') ||
					(character >= '0' && character <= '9'))
				{
					char head = *(--target.str().end());

					if ((head >= 'A' && head <= 'F') ||
						(head >= 'a' && head <= 'f') ||
						(head >= '0' && head <= '9'))
					{
						target << "\" \"";
					}
				}
			}

			target.put(character);

			wroteHex = false;
		}
		else
		{
			target << "\\x" << std::uppercase << std::setfill('0') << std::hex << std::setw(2) << ((unicode_t)character & 0x000000FF);

			wroteHex = true;
		}
	}

	std::string printable(unicode_t codepoint)
	{
		return printable(utf8(codepoint));
	}

	std::string printable(unicode_t* codepoints, size_t codepointsSize)
	{
		std::string converted;

		int32_t errors = 0;
		size_t size_in_bytes = utf32toutf8(codepoints, codepointsSize, nullptr, 0, &errors);

		if (size_in_bytes == 0 ||
			errors != 0)
		{
			return converted;
		}

		converted.resize(size_in_bytes + 1);
		utf32toutf8(codepoints, codepointsSize, &converted[0], size_in_bytes, &errors);

		return printable(converted);
	}

	std::string printable(const std::string& text)
	{
		std::stringstream ss;

		bool wrote_hex = false;

		for (std::string::const_iterator it = text.begin(); it != text.end(); ++it)
		{
			printable(ss, wrote_hex, (uint8_t)*it);
		}

		return ss.str();
	}

	void canonicalCombiningClass(std::stringstream& target, unicode_t codepoint)
	{
		target << (int)database_queryproperty(codepoint, UnicodeProperty_CanonicalCombiningClass);
	}

	std::string canonicalCombiningClass(unicode_t codepoint)
	{
		std::stringstream ss;
		canonicalCombiningClass(ss, codepoint);
		return ss.str();
	}

	std::string canonicalCombiningClass(unicode_t* codepoint, size_t codepointsSize)
	{
		std::stringstream ss;

		canonicalCombiningClass(ss, codepoint[0]);

		for (size_t i = 1; i < codepointsSize / sizeof(unicode_t); ++i)
		{
			ss << " ";
			canonicalCombiningClass(ss, codepoint[i]);
		}

		return ss.str();
	}

	std::string canonicalCombiningClass(const std::string& text)
	{
		std::vector<unicode_t> converted = utf32(text);
		if (converted.size() == 0)
		{
			return "";
		}

		std::stringstream ss;

		for (std::vector<unicode_t>::iterator it = converted.begin(); it != converted.end(); ++it)
		{
			if (it != converted.begin())
			{
				ss << " ";
			}
			canonicalCombiningClass(ss, *it);
		}

		return ss.str();
	}

	::testing::AssertionResult CompareUtf8Strings(
		const char* expressionExpected GTEST_ATTRIBUTE_UNUSED_, const char* expressionActual GTEST_ATTRIBUTE_UNUSED_,
		const char* textExpected, const char* textActual)
	{
		if (!strcmp(textActual, textExpected))
		{
			return ::testing::AssertionSuccess();
		}
		else
		{
			::testing::AssertionResult result = ::testing::AssertionFailure();

			result << "String mismatch" << std::endl;

			result << std::endl;

			result << "[UTF-8]" << std::endl;
			result << "    Actual: " << "\"" << printable(textActual) << "\"" << std::endl;
			result << "  Expected: " << "\"" << printable(textExpected) << "\"" << std::endl;

			result << std::endl;

			result << "[Codepoints]" << std::endl;
			result << "    Actual: " << "\"" << identifiable(textActual) << "\"" << std::endl;
			result << "  Expected: " << "\"" << identifiable(textExpected) << "\"" << std::endl;

			return result;
		}
	}

	::testing::AssertionResult CompareCodepoints(
		const char* expressionExpected, const char* expressionActual,
		unicode_t codepointExpected, unicode_t codepointActual)
	{
		if (codepointActual == codepointExpected)
		{
			return ::testing::AssertionSuccess();
		}
		else
		{
			::testing::AssertionResult result = ::testing::AssertionFailure();

			result << "Codepoint mismatch" << std::endl;

			result << "    Actual: " << identifiable(codepointActual) << " \"" << printable(codepointActual) << "\"" << std::endl;
			result << "  Expected: " << identifiable(codepointExpected) << " \"" << printable(codepointExpected) << "\"" << std::endl;

			return result;
		}
	}

};