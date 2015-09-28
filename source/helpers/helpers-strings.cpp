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
		std::string converted;

		int32_t errors;
		size_t size_in_bytes = utf32toutf8(
			&codepoint, sizeof(unicode_t),
			nullptr, 0,
			&errors);

		if (size_in_bytes == 0 ||
			errors != UTF8_ERR_NONE)
		{
			return converted;
		}

		converted.resize(size_in_bytes);

		utf32toutf8(
			&codepoint, sizeof(unicode_t),
			&converted[0], size_in_bytes,
			nullptr);

		return converted;
	}

	std::string utf8(unicode_t* codepoints, size_t codepointsSize)
	{
		std::string converted;

		int32_t errors;
		size_t size_in_bytes = utf32toutf8(
			codepoints, codepointsSize,
			nullptr, 0,
			&errors);

		if (size_in_bytes == 0 ||
			errors != UTF8_ERR_NONE)
		{
			return converted;
		}

		converted.resize(size_in_bytes);

		utf32toutf8(
			codepoints, codepointsSize,
			&converted[0], size_in_bytes,
			nullptr);

		return converted;
	}

	std::string utf8(const std::vector<unicode_t>& codepoints)
	{
		std::string converted;

		int32_t errors;
		size_t size_in_bytes = utf32toutf8(
			&codepoints[0], codepoints.size() * sizeof(unicode_t),
			nullptr, 0,
			&errors);

		if (size_in_bytes == 0 ||
			errors != UTF8_ERR_NONE)
		{
			return converted;
		}

		converted.resize(size_in_bytes);

		utf32toutf8(
			&codepoints[0], codepoints.size() * sizeof(unicode_t),
			&converted[0], size_in_bytes,
			nullptr);

		return converted;
	}

	std::string utf8(const std::wstring& text)
	{
		std::string converted;

		int32_t errors;
		size_t size_in_bytes = widetoutf8(
			text.c_str(), text.size(),
			nullptr, 0,
			&errors);

		if (size_in_bytes == 0 ||
			errors != UTF8_ERR_NONE)
		{
			return converted;
		}

		converted.resize(size_in_bytes);

		widetoutf8(
			text.c_str(), text.size(),
			&converted[0], size_in_bytes,
			&errors);

		return converted;
	}

	std::vector<utf16_t> utf16(const std::string& text)
	{
		std::vector<utf16_t> converted;

		int32_t errors;
		size_t size_in_bytes = utf8toutf16(
			text.c_str(), text.size(),
			nullptr, 0,
			&errors);

		if (size_in_bytes == 0 ||
			errors != UTF8_ERR_NONE)
		{
			return converted;
		}

		converted.resize(size_in_bytes / sizeof(utf16_t));

		utf8toutf16(
			text.c_str(), text.size(),
			&converted[0], size_in_bytes,
			&errors);

		return converted;
	}

	std::vector<unicode_t> utf32(const std::string& text)
	{
		std::vector<unicode_t> converted;

		int32_t errors;
		size_t size_in_bytes = utf8toutf32(
			text.c_str(), text.size(),
			nullptr, 0,
			&errors);

		if (size_in_bytes == 0 ||
			errors != UTF8_ERR_NONE)
		{
			return converted;
		}

		converted.resize(size_in_bytes / sizeof(unicode_t));

		utf8toutf32(
			text.c_str(), text.size(),
			&converted[0], size_in_bytes,
			&errors);

		return converted;
	}

	std::wstring wide(const std::string& text)
	{
		std::wstring converted;

		int32_t errors;
		size_t size_in_bytes = utf8towide(
			text.c_str(), text.size(),
			nullptr, 0,
			&errors);

		if (size_in_bytes == 0 ||
			errors != UTF8_ERR_NONE)
		{
			return converted;
		}

		converted.resize(size_in_bytes / UTF8_WCHAR_SIZE);

		utf8towide(
			text.c_str(), text.size(),
			&converted[0], size_in_bytes,
			&errors);

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

	std::string hex(unicode_t codepoint)
	{
		return hex(utf8(codepoint));
	}

	std::string hex(unicode_t* codepoints, size_t codepointsSize)
	{
		std::string converted;

		int32_t errors;
		size_t size_in_bytes = utf32toutf8(codepoints, codepointsSize, nullptr, 0, &errors);

		if (size_in_bytes == 0 ||
			errors != UTF8_ERR_NONE)
		{
			return converted;
		}

		converted.resize(size_in_bytes);
		utf32toutf8(codepoints, codepointsSize, &converted[0], size_in_bytes, nullptr);

		return hex(converted);
	}

	std::string hex(const std::string& text)
	{
		std::stringstream ss;

		for (std::string::const_iterator it = text.begin(); it != text.end(); ++it)
		{
			ss << "\\x" << std::uppercase << std::setfill('0') << std::hex << std::setw(2) << ((unicode_t)*it & 0x000000FF);
		}

		return ss.str();
	}

	std::string printable(unicode_t codepoint)
	{
		return printable(utf8(codepoint));
	}

	std::string printable(unicode_t* codepoints, size_t codepointsSize)
	{
		std::string converted;

		int32_t errors;
		size_t size_in_bytes = utf32toutf8(codepoints, codepointsSize, nullptr, 0, &errors);

		if (size_in_bytes == 0 ||
			errors != UTF8_ERR_NONE)
		{
			return converted;
		}

		converted.resize(size_in_bytes);
		utf32toutf8(codepoints, codepointsSize, &converted[0], size_in_bytes, nullptr);

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

	void sequence(std::stringstream& targetID, std::stringstream& targetCCC, std::stringstream& targetQC, unicode_t codepoint, uint8_t type)
	{
		std::string id = identifiable(codepoint);
		std::string ccc = canonicalCombiningClass(codepoint);

		size_t padding = std::max(id.length(), ccc.length());
		if (type != 0)
		{
			std::string qc = quickCheck(codepoint, type);
			padding = std::max(qc.length(), padding);
			targetQC << std::setw(padding) << qc;
		}

		targetID << std::setw(padding) << identifiable(codepoint);
		targetCCC << std::setw(padding) << canonicalCombiningClass(codepoint);
	}

	std::string sequence(unicode_t codepoint, uint8_t type)
	{
		std::stringstream ss_id;
		ss_id << std::setfill(' ');
		std::stringstream ss_ccc;
		ss_ccc << std::setfill(' ');
		std::stringstream ss_qc;
		ss_qc << std::setfill(' ');

		sequence(ss_id, ss_ccc, ss_qc, codepoint, type);

		std::stringstream ss;
		ss << ss_id.str() << std::endl;
		ss << ss_qc.str() << std::endl;
		ss << ss_ccc.str();

		return ss.str();
	}

	std::string sequence(unicode_t* codepoint, size_t codepointsSize, uint8_t type)
	{
		std::stringstream ss_id;
		ss_id << std::setfill(' ');
		std::stringstream ss_ccc;
		ss_ccc << std::setfill(' ');
		std::stringstream ss_qc;
		ss_qc << std::setfill(' ');

		sequence(ss_id, ss_ccc, ss_qc, codepoint[0], type);

		for (size_t i = 1; i < codepointsSize / sizeof(unicode_t); ++i)
		{
			ss_id << " ";
			ss_ccc << " ";
			ss_qc << " ";

			sequence(ss_id, ss_ccc, ss_qc, codepoint[i], type);
		}

		std::stringstream ss;
		ss << ss_id.str() << std::endl;
		ss << ss_qc.str() << std::endl;
		ss << ss_ccc.str();

		return ss.str();
	}

	std::string sequence(const std::string& text, uint8_t type)
	{
		std::vector<unicode_t> converted = utf32(text);
		if (converted.size() == 0)
		{
			return "";
		}

		return sequence(&converted[0], converted.size() * sizeof(unicode_t), type);
	}

	void canonicalCombiningClass(std::stringstream& target, unicode_t codepoint)
	{
		target << (int)PROPERTY_GET_CCC(codepoint);
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

	void quickCheck(std::stringstream& target, unicode_t codepoint, uint8_t type)
	{
		uint8_t qc;

		switch (type)
		{

		case UnicodeProperty_Normalization_Compose:
			qc = PROPERTY_GET_NFC(codepoint);
			break;

		case UnicodeProperty_Normalization_Decompose:
			qc = PROPERTY_GET_NFD(codepoint);
			break;

		case UnicodeProperty_Normalization_Compatibility_Compose:
			qc = PROPERTY_GET_NFKC(codepoint);
			break;

		case UnicodeProperty_Normalization_Compatibility_Decompose:
			qc = PROPERTY_GET_NFKD(codepoint);
			break;

		default:
			qc = QuickCheckResult_Yes;
			break;

		}

		switch (qc)
		{

		case QuickCheckResult_Yes:
			target << "Y";
			break;

		case QuickCheckResult_Maybe:
			target << "M";
			break;

		case QuickCheckResult_No:
			target << "N";
			break;

		default:
			target << "<invalid> (0x";
			target << std::hex << std::setfill('0') << std::setw(2) << std::uppercase << (int)qc;
			target << ")";
			break;

		}
	}

	std::string quickCheck(unicode_t codepoint, uint8_t type)
	{
		std::stringstream ss;
		quickCheck(ss, codepoint, type);
		return ss.str();
	}

	std::string quickCheck(unicode_t* codepoint, size_t codepointsSize, uint8_t type)
	{
		std::stringstream ss;

		quickCheck(ss, codepoint[0], type);

		for (size_t i = 1; i < codepointsSize / sizeof(unicode_t); ++i)
		{
			ss << " ";
			quickCheck(ss, codepoint[i], type);
		}

		return ss.str();
	}

	std::string quickCheck(const std::string& text, uint8_t type)
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
			quickCheck(ss, *it, type);
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

	::testing::AssertionResult CompareUtf8LengthStrings(
		const char* expressionExpected GTEST_ATTRIBUTE_UNUSED_, const char* expressionActual GTEST_ATTRIBUTE_UNUSED_, const char* expressionLength GTEST_ATTRIBUTE_UNUSED_,
		const char* textExpected, const char* textActual, size_t length)
	{
		if (!strncmp(textActual, textExpected, length))
		{
			return ::testing::AssertionSuccess();
		}
		else
		{
			::testing::AssertionResult result = ::testing::AssertionFailure();

			result << "String mismatch" << std::endl;

			result << std::endl;

			result << "[UTF-8]" << std::endl;
			result << "    Actual: " << "\"" << printable(std::string(textActual, length)) << "\"" << std::endl;
			result << "  Expected: " << "\"" << printable(textExpected) << "\"" << std::endl;

			result << std::endl;

			result << "[Codepoints]" << std::endl;
			result << "    Actual: " << "\"" << identifiable(std::string(textActual, length)) << "\"" << std::endl;
			result << "  Expected: " << "\"" << identifiable(textExpected) << "\"" << std::endl;

			return result;
		}
	}

	::testing::AssertionResult CompareOffsets(
		const char* expressionExpected GTEST_ATTRIBUTE_UNUSED_, const char* expressionActual GTEST_ATTRIBUTE_UNUSED_, const char* expressionCount GTEST_ATTRIBUTE_UNUSED_,
		const char* offsetExpected, const char* offsetActual, const char* offsetStart)
	{
		if (offsetExpected == offsetActual)
		{
			return ::testing::AssertionSuccess();
		}
		else
		{
			::testing::AssertionResult result = ::testing::AssertionFailure();

			result << "Offset mismatch." << std::endl;

			result << std::endl;

			result
				<< "[Offset]" << std::endl
				<< "    Actual: " << (ptrdiff_t)(offsetActual - offsetStart) << std::endl
				<< "  Expected: " << (ptrdiff_t)(offsetExpected - offsetStart) << std::endl;

			result << std::endl;

			result
				<< "[Text]" << std::endl
				<< "    Actual: \"" << printable(offsetActual) << "\"" << std::endl
				<< "  Expected: \"" << printable(offsetExpected) << "\"" << std::endl;

			return result;
		}
	}

	::testing::AssertionResult CompareMemory(
		const char* expressionExpected GTEST_ATTRIBUTE_UNUSED_, const char* expressionActual GTEST_ATTRIBUTE_UNUSED_, const char* expressionCount GTEST_ATTRIBUTE_UNUSED_,
		const char* memoryExpected, const char* memoryActual, size_t memorySize)
	{
		if (!memcmp(memoryExpected, memoryActual, memorySize))
		{
			return ::testing::AssertionSuccess();
		}
		else
		{
			::testing::AssertionResult result = ::testing::AssertionFailure();

			result << "Memory mismatch" << std::endl;

			const char* src_a = memoryActual;
			const char* src_e = memoryExpected;

			for (size_t i = 0; i < memorySize; ++i)
			{
				if (*src_a != *src_e)
				{
					result << "[Index " << i << "]" << std::endl;

					std::stringstream ssa;
					ssa << "0x" << std::uppercase << std::setfill('0') << std::hex << std::setw(2) << ((uint32_t)*src_a & 0x000000FF);
					result << "    Actual: " << ssa.str() << std::endl;

					std::stringstream sse;
					sse << "0x" << std::uppercase << std::setfill('0') << std::hex << std::setw(2) << ((uint32_t)*src_e & 0x000000FF);
					result << "  Expected: " << sse.str() << std::endl;
				}

				++src_a;
				++src_e;
			}

			return result;
		}
	}

	::testing::AssertionResult CompareCodepoints(
		const char* expressionExpected GTEST_ATTRIBUTE_UNUSED_, const char* expressionActual GTEST_ATTRIBUTE_UNUSED_,
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