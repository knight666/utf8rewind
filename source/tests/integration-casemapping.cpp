#include "tests-base.hpp"

#include "utf8rewind.h"

struct CaseMappingEntry
{
	CaseMappingEntry()
		: codepoint(0)
	{
	}

	unicode_t codepoint;
	std::string uppercase;
	std::string lowercase;
	std::string titlecase;
	std::string name;
};

std::string CodepointToString(unicode_t codepoint)
{
	std::stringstream ss;
	ss << "U+" << std::setfill('0') << std::setw(8) << std::hex << codepoint;
	return ss.str();
}

std::string Utf8StringToCodepoints(const std::string& text)
{
	if (text == "")
	{
		return "U+00000000";
	}

	std::stringstream ss;
	ss << std::setfill('0') << std::hex;

	int32_t errors = 0;
	size_t size_in_bytes = utf8toutf32(text.c_str(), text.size(), nullptr, 0, &errors);
	if (size_in_bytes == 0 ||
		errors != 0)
	{
		return "";
	}

	unicode_t* result_utf32 = new unicode_t[size_in_bytes / sizeof(unicode_t)];

	utf8toutf32(text.c_str(), text.size(), result_utf32, size_in_bytes, &errors);

	for (size_t i = 0; i < size_in_bytes / sizeof(unicode_t); ++i)
	{
		if (i != 0)
		{
			ss << " ";
		}
		ss << "U+" << std::setw(8) << result_utf32[i];
	}

	delete [] result_utf32;

	return ss.str();
}

inline ::testing::AssertionResult CompareCaseMappingEntry(
	const char* expressionLeft, const char* expressionRight,
	const CaseMappingEntry& entryLeft, const CaseMappingEntry& entryRight
)
{
	if (entryLeft.codepoint == entryRight.codepoint &&
		entryLeft.uppercase == entryRight.uppercase &&
		entryLeft.lowercase == entryRight.lowercase &&
		entryLeft.titlecase == entryRight.titlecase)
	{
		return ::testing::AssertionSuccess();
	}
	else
	{
		::testing::AssertionResult result = ::testing::AssertionFailure();

		result << std::endl;
		result << entryLeft.name << std::endl;
		result << std::endl;

		if (entryLeft.codepoint != entryRight.codepoint)
		{
			result << std::endl;
			result << "[Codepoint]" << std::endl;
			result << "    Actual:    " << CodepointToString(entryRight.codepoint) << std::endl;
			result << "  Expected:    " << CodepointToString(entryLeft.codepoint) << std::endl;
			result << std::endl;
		}
		else
		{
			result << "[Codepoint]    " << CodepointToString(entryLeft.codepoint) << std::endl;
		}

		if (entryLeft.uppercase != entryRight.uppercase)
		{
			result << std::endl;
			result << "[Uppercase]" << std::endl;
			result << "    Actual:    " << Utf8StringToCodepoints(entryRight.uppercase) << std::endl;
			result << "  Expected:    " << Utf8StringToCodepoints(entryLeft.uppercase) << std::endl;
			result << std::endl;
		}
		else
		{
			result << "[Uppercase]    " << Utf8StringToCodepoints(entryLeft.uppercase) << std::endl;
		}

		if (entryLeft.lowercase != entryRight.lowercase)
		{
			result << std::endl;
			result << "[Lowercase]" << std::endl;
			result << "    Actual:    " << Utf8StringToCodepoints(entryRight.lowercase) << std::endl;
			result << "  Expected:    " << Utf8StringToCodepoints(entryLeft.lowercase) << std::endl;
			result << std::endl;
		}
		else
		{
			result << "[Lowercase]    " << Utf8StringToCodepoints(entryLeft.lowercase) << std::endl;
		}

		if (entryLeft.titlecase != entryRight.titlecase)
		{
			result << std::endl;
			result << "[Titlecase]" << std::endl;
			result << "    Actual:    " << Utf8StringToCodepoints(entryRight.titlecase) << std::endl;
			result << "  Expected:    " << Utf8StringToCodepoints(entryLeft.titlecase) << std::endl;
			result << std::endl;
		}
		else
		{
			result << "[Titlecase]    " << Utf8StringToCodepoints(entryLeft.titlecase) << std::endl;
		}

		return result;
	}
}

#define EXPECT_CASE_MAPPING(_line, _codepoint, _uppercase, _lowercase, _titlecase) { \
	CaseMappingEntry r = GetEntryAtLine(_line); \
	CaseMappingEntry e; \
	e.codepoint = _codepoint; \
	e.uppercase = _uppercase; \
	e.lowercase = _lowercase; \
	e.titlecase = _titlecase; \
	e.name = r.name; \
	EXPECT_PRED_FORMAT2(CompareCaseMappingEntry, e, r); \
}

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
			lines.push_back(line);
		}

		file.close();
	}

	void TearDown()
	{
		file.close();
	}

	CaseMappingEntry GetEntryAtLine(size_t lineIndex) const
	{
		CaseMappingEntry result;

		if (lineIndex >= lines.size())
		{
			return result;
		}

		const char* line_src = lines[lineIndex].c_str();

		if (line_src[0] == '#' ||
			strlen(line_src) == 0)
		{
			return result;
		}

		const char* src = line_src;
		size_t offset = 0;

		std::vector<unicode_t> source_codepoint = ReadCodepoints(src, offset);
		result.codepoint = source_codepoint[0];
		src += offset + 2;

		std::vector<unicode_t> source_uppercase = ReadCodepoints(src, offset);
		result.uppercase = CodepointsToUtf8(source_uppercase);
		src += offset + 2;

		std::vector<unicode_t> source_lowercase = ReadCodepoints(src, offset);
		result.lowercase = CodepointsToUtf8(source_lowercase);
		src += offset + 2;

		std::vector<unicode_t> source_titlecase = ReadCodepoints(src, offset);
		result.titlecase = CodepointsToUtf8(source_titlecase);
		src += offset + 2;

		src += 2;
		result.name = src;

		return result;
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

	std::fstream file;
	std::vector<std::string> lines;
	std::vector<CaseMappingEntry> entries;

};

TEST_F(CaseMapping, Parser)
{
	// first

	EXPECT_CASE_MAPPING(0, 0, "", "", "");
	
	// commented out

	EXPECT_CASE_MAPPING(11, 0, "", "", "");

	// empty

	EXPECT_CASE_MAPPING(14, 0, "", "", "");

	// valid

	EXPECT_CASE_MAPPING(991, 0x10b3, "\xE1\x82\xB3", "\xE2\xB4\x93", "\xE2\xB4\x93");
}

TEST_F(CaseMapping, Ascii)
{
	EXPECT_CASE_MAPPING(15, 0x41, "A", "a", "a");
	EXPECT_CASE_MAPPING(16, 0x42, "B", "b", "b");
	EXPECT_CASE_MAPPING(17, 0x43, "C", "c", "c");
	EXPECT_CASE_MAPPING(18, 0x44, "D", "d", "d");
	EXPECT_CASE_MAPPING(19, 0x45, "E", "e", "e");
	EXPECT_CASE_MAPPING(20, 0x46, "F", "f", "f");
	EXPECT_CASE_MAPPING(21, 0x47, "G", "g", "g");
	EXPECT_CASE_MAPPING(22, 0x48, "H", "h", "h");
	EXPECT_CASE_MAPPING(23, 0x49, "I", "i", "i");
	EXPECT_CASE_MAPPING(24, 0x4A, "J", "j", "j");
	EXPECT_CASE_MAPPING(25, 0x4B, "K", "k", "k");
	EXPECT_CASE_MAPPING(26, 0x4C, "L", "l", "l");
	EXPECT_CASE_MAPPING(27, 0x4D, "M", "m", "m");
	EXPECT_CASE_MAPPING(28, 0x4E, "N", "n", "n");
	EXPECT_CASE_MAPPING(29, 0x4F, "O", "o", "o");
	EXPECT_CASE_MAPPING(30, 0x50, "P", "p", "p");
	EXPECT_CASE_MAPPING(31, 0x51, "Q", "q", "q");
	EXPECT_CASE_MAPPING(32, 0x52, "R", "r", "r");
	EXPECT_CASE_MAPPING(33, 0x53, "S", "s", "s");
	EXPECT_CASE_MAPPING(34, 0x54, "T", "t", "t");
	EXPECT_CASE_MAPPING(35, 0x55, "U", "u", "u");
	EXPECT_CASE_MAPPING(36, 0x56, "V", "v", "v");
	EXPECT_CASE_MAPPING(37, 0x57, "W", "w", "w");
	EXPECT_CASE_MAPPING(38, 0x58, "X", "x", "x");
	EXPECT_CASE_MAPPING(39, 0x59, "Y", "y", "y");
	EXPECT_CASE_MAPPING(40, 0x5A, "Z", "z", "z");
}