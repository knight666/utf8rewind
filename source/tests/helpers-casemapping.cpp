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
		entry->uppercase = helpers::utf8(source_uppercase);
		src += offset + 2;

		std::vector<unicode_t> source_lowercase = ReadCodepoints(src, offset);
		entry->lowercase = helpers::utf8(source_lowercase);
		src += offset + 2;

		std::vector<unicode_t> source_titlecase = ReadCodepoints(src, offset);
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

	::testing::AssertionResult CompareUtf8Uppercase(
		const char* expressionExpected, const char* expressionActual,
		const CaseMappingEntry& entryExpected, const CaseMappingEntry& entryActual)
	{
		if (entryExpected.codepoint == entryActual.codepoint &&
			entryExpected.uppercase == entryActual.uppercase)
		{
			return ::testing::AssertionSuccess();
		}
		else
		{
			::testing::AssertionResult result = ::testing::AssertionFailure();

			result << std::endl;
			result <<  entryActual.name << " (" << helpers::identifiable(entryExpected.codepoint)  << ")" << std::endl;
			result << std::endl;

			if (entryExpected.uppercase != entryActual.uppercase)
			{
				result << std::endl;
				result << "[Uppercase]" << std::endl;
				result << "    Actual:    " << helpers::printable(entryExpected.uppercase) << " (" << helpers::identifiable(entryExpected.uppercase) << ")" << std::endl;
				result << "  Expected:    " << helpers::printable(entryActual.uppercase) << " (" << helpers::identifiable(entryActual.uppercase) << ")" << std::endl;
				result << std::endl;
			}
			else
			{
				result << "[Uppercase]    " << helpers::printable(entryExpected.uppercase) << " (" << helpers::identifiable(entryExpected.uppercase) << ")" << std::endl;
			}

			return result;
		}
	}

	::testing::AssertionResult CompareUtf8Lowercase(
		const char* expressionLeft, const char* expressionRight,
		const CaseMappingEntry& entryExpected, const CaseMappingEntry& entryActual)
	{
		if (entryExpected.codepoint == entryActual.codepoint &&
			entryExpected.lowercase == entryActual.lowercase)
		{
			return ::testing::AssertionSuccess();
		}
		else
		{
			::testing::AssertionResult result = ::testing::AssertionFailure();

			result << std::endl;
			result <<  entryActual.name << " (" << helpers::identifiable(entryExpected.codepoint)  << ")" << std::endl;
			result << std::endl;

			if (entryExpected.lowercase != entryActual.lowercase)
			{
				result << std::endl;
				result << "[Lowercase]" << std::endl;
				result << "    Actual:    " << helpers::printable(entryExpected.lowercase) << " (" << helpers::identifiable(entryExpected.lowercase) << ")" << std::endl;
				result << "  Expected:    " << helpers::printable(entryActual.lowercase) << " (" << helpers::identifiable(entryActual.lowercase) << ")" << std::endl;
				result << std::endl;
			}
			else
			{
				result << "[Lowercase]    " << helpers::printable(entryExpected.lowercase) << " (" << helpers::identifiable(entryExpected.lowercase) << ")" << std::endl;
			}

			return result;
		}
	}

	::testing::AssertionResult CompareCaseMappingEntry(
		const char* expressionExpected, const char* expressionActual,
		const CaseMappingEntry& entryExpected, const CaseMappingEntry& entryActual)
	{
		if (entryExpected.codepoint == entryActual.codepoint &&
			entryExpected.uppercase == entryActual.uppercase &&
			entryExpected.lowercase == entryActual.lowercase)
		{
			return ::testing::AssertionSuccess();
		}
		else
		{
			::testing::AssertionResult result = ::testing::AssertionFailure();

			result << std::endl;
			result <<  entryActual.name << " (" << helpers::identifiable(entryExpected.codepoint)  << ")" << std::endl;
			result << std::endl;

			if (entryExpected.uppercase != entryActual.uppercase)
			{
				result << std::endl;
				result << "[Uppercase]" << std::endl;
				result << "    Actual:    " << helpers::printable(entryExpected.uppercase) << " (" << helpers::identifiable(entryExpected.uppercase) << ")" << std::endl;
				result << "  Expected:    " << helpers::printable(entryActual.uppercase) << " (" << helpers::identifiable(entryActual.uppercase) << ")" << std::endl;
				result << std::endl;
			}
			else
			{
				result << "[Uppercase]    " << helpers::printable(entryExpected.uppercase) << " (" << helpers::identifiable(entryExpected.uppercase) << ")" << std::endl;
			}

			if (entryExpected.lowercase != entryActual.lowercase)
			{
				result << std::endl;
				result << "[Lowercase]" << std::endl;
				result << "    Actual:    " << helpers::printable(entryExpected.lowercase) << " (" << helpers::identifiable(entryExpected.lowercase) << ")" << std::endl;
				result << "  Expected:    " << helpers::printable(entryActual.lowercase) << " (" << helpers::identifiable(entryActual.lowercase) << ")" << std::endl;
				result << std::endl;
			}
			else
			{
				result << "[Lowercase]    " << helpers::printable(entryExpected.lowercase) << " (" << helpers::identifiable(entryExpected.lowercase) << ")" << std::endl;
			}

			return result;
		}
	}

};