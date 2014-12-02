#include "helpers-strings.hpp"

#define TEST_ENABLE_UPPERCASE (1)
#define TEST_ENABLE_LOWERCASE (1)
#define TEST_ENABLE_TITLECASE (0)

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

	std::string uppercase(unicode_t codepoint)
	{
#if TEST_ENABLE_UPPERCASE
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
#else
		CaseMappingEntry* entry = CaseMappingDatabase::Get().Find(codepoint);
		if (entry != nullptr)
		{
			return entry->uppercase;
		}

		return "";
#endif
	}

	std::string uppercase(const std::string& value)
	{
		int32_t errors = 0;

		std::string converted;

		size_t l = utf8toupper(value.c_str(), value.size() - 1, nullptr, 0, &errors);
		if (l == 0 ||
			errors != 0)
		{
			return converted;
		}

		converted.resize(l + 1);
		utf8toupper(value.c_str(), value.size() - 1, &converted[0], l, &errors);

		return converted;
	}

	std::string lowercase(unicode_t codepoint)
	{
#if TEST_ENABLE_LOWERCASE
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
#else
		CaseMappingEntry* entry = CaseMappingDatabase::Get().Find(codepoint);
		if (entry != nullptr)
		{
			return entry->lowercase;
		}

		return "";
#endif
	}

	std::string lowercase(const std::string& value)
	{
		int32_t errors = 0;

		std::string converted;

		size_t l = utf8tolower(value.c_str(), value.size() - 1, nullptr, 0, &errors);
		if (l == 0 ||
			errors != 0)
		{
			return converted;
		}

		converted.resize(l + 1);
		utf8tolower(value.c_str(), value.size() - 1, &converted[0], l, &errors);

		return converted;
	}

	std::string titlecase(unicode_t codepoint)
	{
#if TEST_ENABLE_TITLECASE
		int32_t errors = 0;

		char input_utf8[16] = { 0 };
		utf32toutf8(&codepoint, sizeof(unicode_t), input_utf8, 16, &errors);
		if (errors != 0)
		{
			return "";
		}

		// TODO: Implement
		return input_utf8;
#else
		CaseMappingEntry* entry = CaseMappingDatabase::Get().Find(codepoint);
		if (entry != nullptr)
		{
			return entry->titlecase;
		}

		return "";
#endif
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

	void printable(std::stringstream& target, bool& wroteHex, char character)
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
			printable(ss, wrote_hex, *it);
		}

		ss << "\"";

		return ss.str();
	}

	CaseMappingDatabase::CaseMappingDatabase()
	{
		std::fstream file("testdata/CaseMapping.txt", std::ios_base::in);
		if (file.is_open())
		{
			char line[1024];
			while (!file.eof())
			{
				file.getline(line, 1024);

				CaseMappingEntry* entry = ParseLine(line);
				if (entry != nullptr)
				{
					m_Database.insert(std::make_pair(entry->codepoint, entry));
				}
			}

			file.close();
		}
	}

	CaseMappingDatabase& CaseMappingDatabase::Get()
	{
		static CaseMappingDatabase instance;
		return instance;
	}

	CaseMappingEntry* CaseMappingDatabase::Find(unicode_t codepoint) const
	{
		std::map<unicode_t, CaseMappingEntry*>::const_iterator found = m_Database.find(codepoint);
		if (found != m_Database.end())
		{
			return found->second;
		}

		return nullptr;
	}

	CaseMappingEntry* CaseMappingDatabase::ParseLine(const char* line) const
	{
		const char* line_src = line;

		if (line_src[0] == '#' ||
			strlen(line_src) == 0)
		{
			return nullptr;
		}

		CaseMappingEntry* entry = new CaseMappingEntry;

		const char* src = line_src;
		size_t offset = 0;

		std::vector<unicode_t> source_codepoint = ReadCodepoints(src, offset);
		entry->codepoint = source_codepoint[0];
		src += offset + 2;

		std::vector<unicode_t> source_uppercase = ReadCodepoints(src, offset);
		entry->uppercase = utf8(source_uppercase);
		src += offset + 2;

		std::vector<unicode_t> source_lowercase = ReadCodepoints(src, offset);
		entry->lowercase = utf8(source_lowercase);
		src += offset + 2;

		std::vector<unicode_t> source_titlecase = ReadCodepoints(src, offset);
		entry->titlecase = utf8(source_titlecase);
		src += offset + 2;

		src += 2;
		entry->name = src;

		return entry;
	}

	std::vector<unicode_t> CaseMappingDatabase::ReadCodepoints(const char* line, size_t& offset) const
	{
		std::vector<unicode_t> codepoints;

		const char* src_start = line;
		const char* src = src_start;

		while (*src != ';')
		{
			if (*src == ' ')
			{
				unicode_t found = StringToCodepoint(src_start, src - 1);
				codepoints.push_back(found);

				src++;

				src_start = src;
				src = src_start;
			}
			else
			{
				src++;
			}
		}

		unicode_t found = StringToCodepoint(src_start, src - 1);
		codepoints.push_back(found);

		offset = src - line;

		return codepoints;
	}

	unicode_t CaseMappingDatabase::StringToCodepoint(const char* inputStart, const char* inputEnd) const
	{
		unicode_t result = 0;
		unicode_t position = 1;

		const char* src = inputEnd;
		while (src != inputStart)
		{
			unicode_t current = 0;

			if (*src >= '0' && *src <= '9')
			{
				current = *src - '0';
			}
			else if (*src >= 'A' && *src <= 'F')
			{
				current = (*src - 'A' + 10);
			}
			else if (*src >= 'a' && *src <= 'f')
			{
				current = (*src - 'a' + 10);
			}

			result += current * position;
			position <<= 4;

			src--;
		}

		return result;
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