#include "helpers-strings.hpp"

namespace helpers {

	void identifiable(std::stringstream& target, unicode_t codepoint)
	{
		if (codepoint < 0x20)
		{
			switch (codepoint)
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
				target << "\\x" << std::hex << std::setfill('0') << std::setw(2) << std::uppercase << codepoint << "";
				break;

			}
		}
		else if (codepoint <= 0x7F)
		{
			target.put((char)codepoint);
		}
		else
		{
			target << "\\u" << std::hex << std::uppercase << codepoint << "";
		}
	}

	std::string identifiable(unicode_t codepoint)
	{
		std::stringstream ss;
		identifiable(ss, codepoint);
		return ss.str();
	}

	std::string identifiable(const std::string& text)
	{
		if (text == "")
		{
			return "";
		}

		int32_t errors = 0;
		size_t size_in_bytes = utf8toutf32(text.c_str(), text.size(), nullptr, 0, &errors);
		if (size_in_bytes == 0 ||
			errors != 0)
		{
			return "";
		}

		std::vector<unicode_t> converted;
		converted.resize(size_in_bytes / sizeof(unicode_t));

		utf8toutf32(text.c_str(), text.size(), &converted[0], size_in_bytes, &errors);

		std::stringstream ss;

		for (std::vector<unicode_t>::iterator it = converted.begin(); it != converted.end(); ++it)
		{
			identifiable(ss, *it);
		}

		return ss.str();
	}

	std::string utf8(unicode_t codepoint)
	{
		std::string converted;

		int32_t errors = 0;
		size_t size_in_bytes = utf32toutf8(&codepoint, sizeof(unicode_t), nullptr, 0, &errors);

		if (size_in_bytes == 0 ||
			errors != 0)
		{
			return converted;
		}

		converted.resize(size_in_bytes);
		utf32toutf8(&codepoint, sizeof(unicode_t), &converted[0], size_in_bytes, &errors);

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

		converted.resize(size_in_bytes);
		utf32toutf8(&codepoints[0], codepoints.size() * sizeof(unicode_t), &converted[0], size_in_bytes, &errors);

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

	std::string printable(const std::string& text)
	{
		std::stringstream ss;

		ss << "\"";

		bool wrote_hex = false;

		for (std::string::const_iterator it = text.begin(); it != text.end(); ++it)
		{
			printable(ss, wrote_hex, (uint8_t)*it);
		}

		ss << "\"";

		return ss.str();
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

		converted.resize(size_in_bytes);
		utf32toutf8(codepoints, codepointsSize, &converted[0], size_in_bytes, &errors);

		return printable(converted);
	}

	::testing::AssertionResult CompareUtf8Strings(
		const char* expressionLeft, const char* expressionRight,
		const std::string& textLeft, const std::string& textRight
	)
	{
		if (!strcmp(textLeft.c_str(), textRight.c_str()))
		{
			return ::testing::AssertionSuccess();
		}
		else
		{
			::testing::AssertionResult result = ::testing::AssertionFailure();

			result << "String mismatch" << std::endl;
			result << "Expected:" << std::endl;
			result << helpers::identifiable(textLeft) << std::endl;
			result << "  Actual:" << std::endl;
			result << helpers::identifiable(textRight) << std::endl;

			return result;
		}
	}

};