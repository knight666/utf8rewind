#include "tests-base.hpp"

#include "utf8rewind.h"

struct CaseMappingEntry
{
	std::string codepoint;
	std::string uppercase;
	std::string lowercase;
	std::string titlecase;
	std::string name;
};

class CaseMapping
	: public ::testing::Test
{

protected:

	void SetUp()
	{
		// load into file

		file.open("testdata/CaseMapping.txt", std::ios_base::in);
		ASSERT_TRUE(file.is_open());

		char line[1024];
		while (!file.eof())
		{
			file.getline(line, 1024);
			if (line[0] == '#' ||
				strlen(line) == 0)
			{
				continue;
			}

			CaseMappingEntry current;
			const char* src = line;
			size_t offset = 0;

			std::vector<unicode_t> source_codepoint = ReadCodepoints(src, offset);
			current.codepoint = CodepointsToUtf8(source_codepoint);
			src += offset + 2;

			std::vector<unicode_t> source_uppercase = ReadCodepoints(src, offset);
			current.uppercase = CodepointsToUtf8(source_uppercase);
			src += offset + 2;

			std::vector<unicode_t> source_lowercase = ReadCodepoints(src, offset);
			current.lowercase = CodepointsToUtf8(source_lowercase);
			src += offset + 2;

			std::vector<unicode_t> source_titlecase = ReadCodepoints(src, offset);
			current.titlecase = CodepointsToUtf8(source_titlecase);
			src += offset + 2;

			src += 2;
			current.name = src;

			entries.push_back(current);
		}

		file.close();
	}

	void TearDown()
	{
		file.close();
	}

	std::string CodepointsToUtf8(const std::vector<unicode_t>& codepoints) const
	{
		std::string result;

		int32_t errors = 0;
		size_t size_in_bytes = utf32toutf8(&codepoints[0], codepoints.size() * sizeof(unicode_t), nullptr, 0, &errors);

		if (size_in_bytes == 0 ||
			errors != 0)
		{
			return result;
		}

		char* result_utf8 = new char[size_in_bytes + 1];
		memset(result_utf8, 0, size_in_bytes + 1);

		utf32toutf8(&codepoints[0], codepoints.size() * sizeof(unicode_t), result_utf8, size_in_bytes + 1, &errors);

		result = std::string(result_utf8);

		delete [] result_utf8;

		return result;
	}

	std::vector<unicode_t> ReadCodepoints(const char* line, size_t& offset) const
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

	unicode_t StringToCodepoint(const char* inputStart, const char* inputEnd) const
	{
		unicode_t result = 0;
		size_t position = 0;

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

			if (position == 0)
			{
				result += current;
			}
			else
			{
				result += current * position * 16;
			}
			position++;

			src--;
		}

		return result;
	}

	std::fstream file;
	std::vector<CaseMappingEntry> entries;

};

TEST_F(CaseMapping, Ascii)
{

}