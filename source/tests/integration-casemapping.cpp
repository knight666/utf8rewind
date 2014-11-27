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

class CaseMappingDatabase
{

public:

	static CaseMappingDatabase& Get()
	{
		static CaseMappingDatabase instance;
		return instance;
	}

	bool IsValid() const { return m_Database.size() > 0; }

	CaseMappingEntry* Find(unicode_t codepoint) const
	{
		std::map<unicode_t, CaseMappingEntry*>::const_iterator found = m_Database.find(codepoint);
		if (found != m_Database.end())
		{
			return found->second;
		}

		return nullptr;
	}

private:

	CaseMappingDatabase()
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

	CaseMappingEntry* ParseLine(const char* line) const
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
		entry->uppercase = CodepointsToUtf8(source_uppercase);
		src += offset + 2;

		std::vector<unicode_t> source_lowercase = ReadCodepoints(src, offset);
		entry->lowercase = CodepointsToUtf8(source_lowercase);
		src += offset + 2;

		std::vector<unicode_t> source_titlecase = ReadCodepoints(src, offset);
		entry->titlecase = CodepointsToUtf8(source_titlecase);
		src += offset + 2;

		src += 2;
		entry->name = src;

		return entry;
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

	std::map<unicode_t, CaseMappingEntry*> m_Database;
};

std::string CodepointToString(unicode_t codepoint)
{
	std::stringstream ss;
	ss << "U+" << std::setfill('0') << std::setw(8) << std::hex << codepoint;
	return ss.str();
}

std::string CodepointToUppercaseString(unicode_t codepoint)
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

std::string CodepointToLowercaseString(unicode_t codepoint)
{
	int32_t errors = 0;

	char input_utf8[16] = { 0 };
	utf32toutf8(&codepoint, sizeof(unicode_t), input_utf8, 16, &errors);
	if (errors != 0)
	{
		return "";
	}

	// TODO: Implement
	return input_utf8;
}

std::string CodepointToTitlecaseString(unicode_t codepoint)
{
	int32_t errors = 0;

	char input_utf8[16] = { 0 };
	utf32toutf8(&codepoint, sizeof(unicode_t), input_utf8, 16, &errors);
	if (errors != 0)
	{
		return "";
	}

	// TODO: Implement
	return input_utf8;
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

std::string Utf8StringPrintable(const std::string& text)
{
	std::stringstream ss;
	ss << std::setfill('0') << std::hex;

	for (std::string::const_iterator it = text.begin(); it != text.end(); ++it)
	{
		if (*it < 0x20 || *it > 0x7F)
		{
			ss << "\\x" << std::setw(2) << *it;
		}
		else
		{
			ss << std::string(it, it + 1);
		}
	}

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
			result << "    Actual:    " << Utf8StringPrintable(entryRight.uppercase) << " (" << Utf8StringToCodepoints(entryRight.uppercase) << ")" << std::endl;
			result << "  Expected:    " << Utf8StringPrintable(entryLeft.uppercase) << " (" << Utf8StringToCodepoints(entryLeft.uppercase) << ")" << std::endl;
			result << std::endl;
		}
		else
		{
			result << "[Uppercase]    " << Utf8StringPrintable(entryLeft.uppercase) << " (" << Utf8StringToCodepoints(entryLeft.uppercase) << ")" << std::endl;
		}

		if (entryLeft.lowercase != entryRight.lowercase)
		{
			result << std::endl;
			result << "[Lowercase]" << std::endl;
			result << "    Actual:    " << Utf8StringPrintable(entryRight.lowercase) << " (" << Utf8StringToCodepoints(entryRight.lowercase) << ")" << std::endl;
			result << "  Expected:    " << Utf8StringPrintable(entryLeft.lowercase) << " (" << Utf8StringToCodepoints(entryLeft.lowercase) << ")" << std::endl;
			result << std::endl;
		}
		else
		{
			result << "[Lowercase]    " << Utf8StringPrintable(entryLeft.lowercase) << " (" << Utf8StringToCodepoints(entryLeft.lowercase) << ")" << std::endl;
		}

		if (entryLeft.titlecase != entryRight.titlecase)
		{
			result << std::endl;
			result << "[Titlecase]" << std::endl;
			result << "    Actual:    " << Utf8StringPrintable(entryRight.titlecase) << " (" << Utf8StringToCodepoints(entryRight.titlecase) << ")" << std::endl;
			result << "  Expected:    " << Utf8StringPrintable(entryLeft.titlecase) << " (" << Utf8StringToCodepoints(entryLeft.titlecase) << ")" << std::endl;
			result << std::endl;
		}
		else
		{
			result << "[Titlecase]    " << Utf8StringPrintable(entryLeft.titlecase) << " (" << Utf8StringToCodepoints(entryLeft.titlecase) << ")" << std::endl;
		}

		return result;
	}
}

#define CHECK_CASE_MAPPING(_codepoint) { \
	CaseMappingEntry* a = CaseMappingDatabase::Get().Find(_codepoint); \
	ASSERT_NE(nullptr, a); \
	CaseMappingEntry e; \
	e.codepoint = _codepoint; \
	e.lowercase = CodepointToLowercaseString(_codepoint); \
	e.uppercase = CodepointToUppercaseString(_codepoint); \
	e.titlecase = a->titlecase; \
	e.name = a->name; \
	EXPECT_PRED_FORMAT2(CompareCaseMappingEntry, e, *a); \
}

class CaseMapping
	: public ::testing::Test
{

protected:

	void SetUp()
	{
		ASSERT_TRUE(CaseMappingDatabase::Get().IsValid());
	}

	void TearDown()
	{
	}

};

TEST_F(CaseMapping, AsciiUppercase)
{
	CHECK_CASE_MAPPING(0x61);
	CHECK_CASE_MAPPING(0x62);
	CHECK_CASE_MAPPING(0x63);
	CHECK_CASE_MAPPING(0x64);
	CHECK_CASE_MAPPING(0x65);
	CHECK_CASE_MAPPING(0x66);
	CHECK_CASE_MAPPING(0x67);
	CHECK_CASE_MAPPING(0x68);
	CHECK_CASE_MAPPING(0x69);
	CHECK_CASE_MAPPING(0x6A);
	CHECK_CASE_MAPPING(0x6B);
	CHECK_CASE_MAPPING(0x6C);
	CHECK_CASE_MAPPING(0x6D);
	CHECK_CASE_MAPPING(0x6E);
	CHECK_CASE_MAPPING(0x6F);
	CHECK_CASE_MAPPING(0x70);
	CHECK_CASE_MAPPING(0x71);
	CHECK_CASE_MAPPING(0x72);
	CHECK_CASE_MAPPING(0x73);
	CHECK_CASE_MAPPING(0x74);
	CHECK_CASE_MAPPING(0x75);
	CHECK_CASE_MAPPING(0x76);
	CHECK_CASE_MAPPING(0x77);
	CHECK_CASE_MAPPING(0x78);
	CHECK_CASE_MAPPING(0x79);
	CHECK_CASE_MAPPING(0x7A);
}

TEST_F(CaseMapping, AsciiLowercase)
{
	CHECK_CASE_MAPPING(0x41);
	CHECK_CASE_MAPPING(0x42);
	CHECK_CASE_MAPPING(0x43);
	CHECK_CASE_MAPPING(0x44);
	CHECK_CASE_MAPPING(0x45);
	CHECK_CASE_MAPPING(0x46);
	CHECK_CASE_MAPPING(0x47);
	CHECK_CASE_MAPPING(0x48);
	CHECK_CASE_MAPPING(0x49);
	CHECK_CASE_MAPPING(0x4A);
	CHECK_CASE_MAPPING(0x4B);
	CHECK_CASE_MAPPING(0x4C);
	CHECK_CASE_MAPPING(0x4D);
	CHECK_CASE_MAPPING(0x4E);
	CHECK_CASE_MAPPING(0x4F);
	CHECK_CASE_MAPPING(0x50);
	CHECK_CASE_MAPPING(0x51);
	CHECK_CASE_MAPPING(0x52);
	CHECK_CASE_MAPPING(0x53);
	CHECK_CASE_MAPPING(0x54);
	CHECK_CASE_MAPPING(0x55);
	CHECK_CASE_MAPPING(0x56);
	CHECK_CASE_MAPPING(0x57);
	CHECK_CASE_MAPPING(0x58);
	CHECK_CASE_MAPPING(0x59);
	CHECK_CASE_MAPPING(0x5A);
}