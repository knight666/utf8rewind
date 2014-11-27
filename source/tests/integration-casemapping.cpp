#include "tests-base.hpp"

#include "utf8rewind.h"

#define TEST_ENABLE_UPPERCASE (1)
#define TEST_ENABLE_LOWERCASE (0)
#define TEST_ENABLE_TITLECASE (0)

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

std::string CodepointToLowercaseString(unicode_t codepoint)
{
#if TEST_ENABLE_LOWERCASE
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
		return entry->lowercase;
	}

	return "";
#endif
}

std::string CodepointToTitlecaseString(unicode_t codepoint)
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

	for (std::string::const_iterator it = text.begin(); it != text.end(); ++it)
	{
		if (*it < 0x20 || *it > 0x7F)
		{
			ss << "\\x" << std::setfill('0') << std::hex << std::setw(2) << ((unicode_t)*it & 0x000000FF);
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
		result <<  entryRight.name << " (" << CodepointToString(entryLeft.codepoint)  << ")" << std::endl;
		result << std::endl;

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
	CaseMappingEntry e; \
	e.codepoint = _codepoint; \
	e.lowercase = CodepointToLowercaseString(_codepoint); \
	e.uppercase = CodepointToUppercaseString(_codepoint); \
	e.titlecase = CodepointToTitlecaseString(_codepoint); \
	CaseMappingEntry* a = CaseMappingDatabase::Get().Find(_codepoint); \
	if (a != nullptr) { \
		EXPECT_PRED_FORMAT2(CompareCaseMappingEntry, e, *a); \
	} else { \
		CaseMappingEntry af; \
		af.codepoint = _codepoint; \
		af.lowercase = CodepointToLowercaseString(_codepoint); \
		af.uppercase = CodepointToUppercaseString(_codepoint); \
		af.titlecase = CodepointToTitlecaseString(_codepoint); \
		EXPECT_PRED_FORMAT2(CompareCaseMappingEntry, e, af); \
	} \
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

TEST_F(CaseMapping, CreateDatabase)
{
}

// U+0000 U+007F Basic Latin
// U+0080 U+00FF Latin-1 Supplement
// U+0100 U+017F Latin Extended-A
// U+0180 U+024F Latin Extended-B 
// U+0250 U+02AF IPA Extensions
// U+0300 U+036F Combining Diacritical Marks
// U+0370 U+03FF Greek and Coptic
// U+0400 U+04FF Cyrillic
// U+0500 U+052F Cyrillic Supplement
// U+0530 U+058F Armenian
// U+10A0 U+10FF Georgian
// U+1D00 U+1D7F Phonetic Extensions
// U+1E00 U+1EFF Latin Extended Additional
// U+1F00 U+1FFF Greek Extended
// U+2100 U+214F Letterlike Symbols
// U+2150 U+218F Number Forms
// U+2460 U+24FF Enclosed Alphanumerics
// U+2C00 U+2C5F Glagolitic
// U+2C60 U+2C7F Latin Extended-C
// U+2C80 U+2CFF Coptic
// U+2D00 U+2D2F Georgian Supplement
// U+A640 U+A69F Cyrillic Extended-B
// U+A720 U+A7FF Latin Extended-D
// U+FF00 U+FFEF Latin Extended-D
// U+10400 U+1044F Deseret
// U+1118A0 U+118FF Warang Citi

TEST_F(CaseMapping, BasicLatinCapitalLetters)
{
	CHECK_CASE_MAPPING(0x41); // LATIN CAPITAL LETTER A
	CHECK_CASE_MAPPING(0x42); // LATIN CAPITAL LETTER B
	CHECK_CASE_MAPPING(0x43); // LATIN CAPITAL LETTER C
	CHECK_CASE_MAPPING(0x44); // LATIN CAPITAL LETTER D
	CHECK_CASE_MAPPING(0x45); // LATIN CAPITAL LETTER E
	CHECK_CASE_MAPPING(0x46); // LATIN CAPITAL LETTER F
	CHECK_CASE_MAPPING(0x47); // LATIN CAPITAL LETTER G
	CHECK_CASE_MAPPING(0x48); // LATIN CAPITAL LETTER H
	CHECK_CASE_MAPPING(0x49); // LATIN CAPITAL LETTER I
	CHECK_CASE_MAPPING(0x4a); // LATIN CAPITAL LETTER J
	CHECK_CASE_MAPPING(0x4b); // LATIN CAPITAL LETTER K
	CHECK_CASE_MAPPING(0x4c); // LATIN CAPITAL LETTER L
	CHECK_CASE_MAPPING(0x4d); // LATIN CAPITAL LETTER M
	CHECK_CASE_MAPPING(0x4e); // LATIN CAPITAL LETTER N
	CHECK_CASE_MAPPING(0x4f); // LATIN CAPITAL LETTER O
	CHECK_CASE_MAPPING(0x50); // LATIN CAPITAL LETTER P
	CHECK_CASE_MAPPING(0x51); // LATIN CAPITAL LETTER Q
	CHECK_CASE_MAPPING(0x52); // LATIN CAPITAL LETTER R
	CHECK_CASE_MAPPING(0x53); // LATIN CAPITAL LETTER S
	CHECK_CASE_MAPPING(0x54); // LATIN CAPITAL LETTER T
	CHECK_CASE_MAPPING(0x55); // LATIN CAPITAL LETTER U
	CHECK_CASE_MAPPING(0x56); // LATIN CAPITAL LETTER V
	CHECK_CASE_MAPPING(0x57); // LATIN CAPITAL LETTER W
	CHECK_CASE_MAPPING(0x58); // LATIN CAPITAL LETTER X
	CHECK_CASE_MAPPING(0x59); // LATIN CAPITAL LETTER Y
	CHECK_CASE_MAPPING(0x5a); // LATIN CAPITAL LETTER Z
}

TEST_F(CaseMapping, BasicLatinSmallLetters)
{
	CHECK_CASE_MAPPING(0x61); // LATIN SMALL LETTER A
	CHECK_CASE_MAPPING(0x62); // LATIN SMALL LETTER B
	CHECK_CASE_MAPPING(0x63); // LATIN SMALL LETTER C
	CHECK_CASE_MAPPING(0x64); // LATIN SMALL LETTER D
	CHECK_CASE_MAPPING(0x65); // LATIN SMALL LETTER E
	CHECK_CASE_MAPPING(0x66); // LATIN SMALL LETTER F
	CHECK_CASE_MAPPING(0x67); // LATIN SMALL LETTER G
	CHECK_CASE_MAPPING(0x68); // LATIN SMALL LETTER H
	CHECK_CASE_MAPPING(0x69); // LATIN SMALL LETTER I
	CHECK_CASE_MAPPING(0x6a); // LATIN SMALL LETTER J
	CHECK_CASE_MAPPING(0x6b); // LATIN SMALL LETTER K
	CHECK_CASE_MAPPING(0x6c); // LATIN SMALL LETTER L
	CHECK_CASE_MAPPING(0x6d); // LATIN SMALL LETTER M
	CHECK_CASE_MAPPING(0x6e); // LATIN SMALL LETTER N
	CHECK_CASE_MAPPING(0x6f); // LATIN SMALL LETTER O
	CHECK_CASE_MAPPING(0x70); // LATIN SMALL LETTER P
	CHECK_CASE_MAPPING(0x71); // LATIN SMALL LETTER Q
	CHECK_CASE_MAPPING(0x72); // LATIN SMALL LETTER R
	CHECK_CASE_MAPPING(0x73); // LATIN SMALL LETTER S
	CHECK_CASE_MAPPING(0x74); // LATIN SMALL LETTER T
	CHECK_CASE_MAPPING(0x75); // LATIN SMALL LETTER U
	CHECK_CASE_MAPPING(0x76); // LATIN SMALL LETTER V
	CHECK_CASE_MAPPING(0x77); // LATIN SMALL LETTER W
	CHECK_CASE_MAPPING(0x78); // LATIN SMALL LETTER X
	CHECK_CASE_MAPPING(0x79); // LATIN SMALL LETTER Y
	CHECK_CASE_MAPPING(0x7a); // LATIN SMALL LETTER Z
}

TEST_F(CaseMapping, Latin1SupplementCapitalLetters)
{
	CHECK_CASE_MAPPING(0xc0); // LATIN CAPITAL LETTER A WITH GRAVE
	CHECK_CASE_MAPPING(0xc1); // LATIN CAPITAL LETTER A WITH ACUTE
	CHECK_CASE_MAPPING(0xc2); // LATIN CAPITAL LETTER A WITH CIRCUMFLEX
	CHECK_CASE_MAPPING(0xc3); // LATIN CAPITAL LETTER A WITH TILDE
	CHECK_CASE_MAPPING(0xc4); // LATIN CAPITAL LETTER A WITH DIAERESIS
	CHECK_CASE_MAPPING(0xc5); // LATIN CAPITAL LETTER A WITH RING ABOVE
	CHECK_CASE_MAPPING(0xc6); // LATIN CAPITAL LETTER AE
	CHECK_CASE_MAPPING(0xc7); // LATIN CAPITAL LETTER C WITH CEDILLA
	CHECK_CASE_MAPPING(0xc8); // LATIN CAPITAL LETTER E WITH GRAVE
	CHECK_CASE_MAPPING(0xc9); // LATIN CAPITAL LETTER E WITH ACUTE
	CHECK_CASE_MAPPING(0xca); // LATIN CAPITAL LETTER E WITH CIRCUMFLEX
	CHECK_CASE_MAPPING(0xcb); // LATIN CAPITAL LETTER E WITH DIAERESIS
	CHECK_CASE_MAPPING(0xcc); // LATIN CAPITAL LETTER I WITH GRAVE
	CHECK_CASE_MAPPING(0xcd); // LATIN CAPITAL LETTER I WITH ACUTE
	CHECK_CASE_MAPPING(0xce); // LATIN CAPITAL LETTER I WITH CIRCUMFLEX
	CHECK_CASE_MAPPING(0xcf); // LATIN CAPITAL LETTER I WITH DIAERESIS
	CHECK_CASE_MAPPING(0xd0); // LATIN CAPITAL LETTER ETH
	CHECK_CASE_MAPPING(0xd1); // LATIN CAPITAL LETTER N WITH TILDE
	CHECK_CASE_MAPPING(0xd2); // LATIN CAPITAL LETTER O WITH GRAVE
	CHECK_CASE_MAPPING(0xd3); // LATIN CAPITAL LETTER O WITH ACUTE
	CHECK_CASE_MAPPING(0xd4); // LATIN CAPITAL LETTER O WITH CIRCUMFLEX
	CHECK_CASE_MAPPING(0xd5); // LATIN CAPITAL LETTER O WITH TILDE
	CHECK_CASE_MAPPING(0xd6); // LATIN CAPITAL LETTER O WITH DIAERESIS
	CHECK_CASE_MAPPING(0xd8); // LATIN CAPITAL LETTER O WITH STROKE
	CHECK_CASE_MAPPING(0xd9); // LATIN CAPITAL LETTER U WITH GRAVE
	CHECK_CASE_MAPPING(0xda); // LATIN CAPITAL LETTER U WITH ACUTE
	CHECK_CASE_MAPPING(0xdb); // LATIN CAPITAL LETTER U WITH CIRCUMFLEX
	CHECK_CASE_MAPPING(0xdc); // LATIN CAPITAL LETTER U WITH DIAERESIS
	CHECK_CASE_MAPPING(0xdd); // LATIN CAPITAL LETTER Y WITH ACUTE
	CHECK_CASE_MAPPING(0xde); // LATIN CAPITAL LETTER THORN
}

TEST_F(CaseMapping, Latin1SupplementSmallLetters)
{
	CHECK_CASE_MAPPING(0xb5); // MICRO SIGN

	CHECK_CASE_MAPPING(0xe0); // LATIN SMALL LETTER A WITH GRAVE
	CHECK_CASE_MAPPING(0xe1); // LATIN SMALL LETTER A WITH ACUTE
	CHECK_CASE_MAPPING(0xe2); // LATIN SMALL LETTER A WITH CIRCUMFLEX
	CHECK_CASE_MAPPING(0xe3); // LATIN SMALL LETTER A WITH TILDE
	CHECK_CASE_MAPPING(0xe4); // LATIN SMALL LETTER A WITH DIAERESIS
	CHECK_CASE_MAPPING(0xe5); // LATIN SMALL LETTER A WITH RING ABOVE
	CHECK_CASE_MAPPING(0xe6); // LATIN SMALL LETTER AE
	CHECK_CASE_MAPPING(0xe7); // LATIN SMALL LETTER C WITH CEDILLA
	CHECK_CASE_MAPPING(0xe8); // LATIN SMALL LETTER E WITH GRAVE
	CHECK_CASE_MAPPING(0xe9); // LATIN SMALL LETTER E WITH ACUTE
	CHECK_CASE_MAPPING(0xea); // LATIN SMALL LETTER E WITH CIRCUMFLEX
	CHECK_CASE_MAPPING(0xeb); // LATIN SMALL LETTER E WITH DIAERESIS
	CHECK_CASE_MAPPING(0xec); // LATIN SMALL LETTER I WITH GRAVE
	CHECK_CASE_MAPPING(0xed); // LATIN SMALL LETTER I WITH ACUTE
	CHECK_CASE_MAPPING(0xee); // LATIN SMALL LETTER I WITH CIRCUMFLEX
	CHECK_CASE_MAPPING(0xef); // LATIN SMALL LETTER I WITH DIAERESIS
	CHECK_CASE_MAPPING(0xf0); // LATIN SMALL LETTER ETH
	CHECK_CASE_MAPPING(0xf1); // LATIN SMALL LETTER N WITH TILDE
	CHECK_CASE_MAPPING(0xf2); // LATIN SMALL LETTER O WITH GRAVE
	CHECK_CASE_MAPPING(0xf3); // LATIN SMALL LETTER O WITH ACUTE
	CHECK_CASE_MAPPING(0xf4); // LATIN SMALL LETTER O WITH CIRCUMFLEX
	CHECK_CASE_MAPPING(0xf5); // LATIN SMALL LETTER O WITH TILDE
	CHECK_CASE_MAPPING(0xf6); // LATIN SMALL LETTER O WITH DIAERESIS
	CHECK_CASE_MAPPING(0xf8); // LATIN SMALL LETTER O WITH STROKE
	CHECK_CASE_MAPPING(0xf9); // LATIN SMALL LETTER U WITH GRAVE
	CHECK_CASE_MAPPING(0xfa); // LATIN SMALL LETTER U WITH ACUTE
	CHECK_CASE_MAPPING(0xfb); // LATIN SMALL LETTER U WITH CIRCUMFLEX
	CHECK_CASE_MAPPING(0xfc); // LATIN SMALL LETTER U WITH DIAERESIS
	CHECK_CASE_MAPPING(0xfd); // LATIN SMALL LETTER Y WITH ACUTE
	CHECK_CASE_MAPPING(0xfe); // LATIN SMALL LETTER THORN
	CHECK_CASE_MAPPING(0xff); // LATIN SMALL LETTER Y WITH DIAERESIS
}

TEST_F(CaseMapping, LatinExtendedACapitalLetters)
{
	CHECK_CASE_MAPPING(0x100); // LATIN CAPITAL LETTER A WITH MACRON
	CHECK_CASE_MAPPING(0x102); // LATIN CAPITAL LETTER A WITH BREVE
	CHECK_CASE_MAPPING(0x104); // LATIN CAPITAL LETTER A WITH OGONEK
	CHECK_CASE_MAPPING(0x106); // LATIN CAPITAL LETTER C WITH ACUTE
	CHECK_CASE_MAPPING(0x108); // LATIN CAPITAL LETTER C WITH CIRCUMFLEX
	CHECK_CASE_MAPPING(0x10a); // LATIN CAPITAL LETTER C WITH DOT ABOVE
	CHECK_CASE_MAPPING(0x10c); // LATIN CAPITAL LETTER C WITH CARON
	CHECK_CASE_MAPPING(0x10e); // LATIN CAPITAL LETTER D WITH CARON
	CHECK_CASE_MAPPING(0x110); // LATIN CAPITAL LETTER D WITH STROKE
	CHECK_CASE_MAPPING(0x112); // LATIN CAPITAL LETTER E WITH MACRON
	CHECK_CASE_MAPPING(0x114); // LATIN CAPITAL LETTER E WITH BREVE
	CHECK_CASE_MAPPING(0x116); // LATIN CAPITAL LETTER E WITH DOT ABOVE
	CHECK_CASE_MAPPING(0x118); // LATIN CAPITAL LETTER E WITH OGONEK
	CHECK_CASE_MAPPING(0x11a); // LATIN CAPITAL LETTER E WITH CARON
	CHECK_CASE_MAPPING(0x11c); // LATIN CAPITAL LETTER G WITH CIRCUMFLEX
	CHECK_CASE_MAPPING(0x11e); // LATIN CAPITAL LETTER G WITH BREVE
	CHECK_CASE_MAPPING(0x120); // LATIN CAPITAL LETTER G WITH DOT ABOVE
	CHECK_CASE_MAPPING(0x122); // LATIN CAPITAL LETTER G WITH CEDILLA
	CHECK_CASE_MAPPING(0x124); // LATIN CAPITAL LETTER H WITH CIRCUMFLEX
	CHECK_CASE_MAPPING(0x126); // LATIN CAPITAL LETTER H WITH STROKE
	CHECK_CASE_MAPPING(0x128); // LATIN CAPITAL LETTER I WITH TILDE
	CHECK_CASE_MAPPING(0x12a); // LATIN CAPITAL LETTER I WITH MACRON
	CHECK_CASE_MAPPING(0x12c); // LATIN CAPITAL LETTER I WITH BREVE
	CHECK_CASE_MAPPING(0x12e); // LATIN CAPITAL LETTER I WITH OGONEK
	CHECK_CASE_MAPPING(0x130); // LATIN CAPITAL LETTER I WITH DOT ABOVE
	CHECK_CASE_MAPPING(0x132); // LATIN CAPITAL LIGATURE IJ
	CHECK_CASE_MAPPING(0x134); // LATIN CAPITAL LETTER J WITH CIRCUMFLEX
	CHECK_CASE_MAPPING(0x136); // LATIN CAPITAL LETTER K WITH CEDILLA
	CHECK_CASE_MAPPING(0x139); // LATIN CAPITAL LETTER L WITH ACUTE
	CHECK_CASE_MAPPING(0x13b); // LATIN CAPITAL LETTER L WITH CEDILLA
	CHECK_CASE_MAPPING(0x13d); // LATIN CAPITAL LETTER L WITH CARON
	CHECK_CASE_MAPPING(0x13f); // LATIN CAPITAL LETTER L WITH MIDDLE DOT
	CHECK_CASE_MAPPING(0x141); // LATIN CAPITAL LETTER L WITH STROKE
	CHECK_CASE_MAPPING(0x143); // LATIN CAPITAL LETTER N WITH ACUTE
	CHECK_CASE_MAPPING(0x145); // LATIN CAPITAL LETTER N WITH CEDILLA
	CHECK_CASE_MAPPING(0x147); // LATIN CAPITAL LETTER N WITH CARON
	CHECK_CASE_MAPPING(0x14a); // LATIN CAPITAL LETTER ENG
	CHECK_CASE_MAPPING(0x14c); // LATIN CAPITAL LETTER O WITH MACRON
	CHECK_CASE_MAPPING(0x14e); // LATIN CAPITAL LETTER O WITH BREVE
	CHECK_CASE_MAPPING(0x150); // LATIN CAPITAL LETTER O WITH DOUBLE ACUTE
	CHECK_CASE_MAPPING(0x152); // LATIN CAPITAL LIGATURE OE
	CHECK_CASE_MAPPING(0x154); // LATIN CAPITAL LETTER R WITH ACUTE
	CHECK_CASE_MAPPING(0x156); // LATIN CAPITAL LETTER R WITH CEDILLA
	CHECK_CASE_MAPPING(0x158); // LATIN CAPITAL LETTER R WITH CARON
	CHECK_CASE_MAPPING(0x15a); // LATIN CAPITAL LETTER S WITH ACUTE
	CHECK_CASE_MAPPING(0x15c); // LATIN CAPITAL LETTER S WITH CIRCUMFLEX
	CHECK_CASE_MAPPING(0x15e); // LATIN CAPITAL LETTER S WITH CEDILLA
	CHECK_CASE_MAPPING(0x160); // LATIN CAPITAL LETTER S WITH CARON
	CHECK_CASE_MAPPING(0x162); // LATIN CAPITAL LETTER T WITH CEDILLA
	CHECK_CASE_MAPPING(0x164); // LATIN CAPITAL LETTER T WITH CARON
	CHECK_CASE_MAPPING(0x166); // LATIN CAPITAL LETTER T WITH STROKE
	CHECK_CASE_MAPPING(0x168); // LATIN CAPITAL LETTER U WITH TILDE
	CHECK_CASE_MAPPING(0x16a); // LATIN CAPITAL LETTER U WITH MACRON
	CHECK_CASE_MAPPING(0x16c); // LATIN CAPITAL LETTER U WITH BREVE
	CHECK_CASE_MAPPING(0x16e); // LATIN CAPITAL LETTER U WITH RING ABOVE
	CHECK_CASE_MAPPING(0x170); // LATIN CAPITAL LETTER U WITH DOUBLE ACUTE
	CHECK_CASE_MAPPING(0x172); // LATIN CAPITAL LETTER U WITH OGONEK
	CHECK_CASE_MAPPING(0x174); // LATIN CAPITAL LETTER W WITH CIRCUMFLEX
	CHECK_CASE_MAPPING(0x176); // LATIN CAPITAL LETTER Y WITH CIRCUMFLEX
	CHECK_CASE_MAPPING(0x178); // LATIN CAPITAL LETTER Y WITH DIAERESIS
	CHECK_CASE_MAPPING(0x179); // LATIN CAPITAL LETTER Z WITH ACUTE
	CHECK_CASE_MAPPING(0x17b); // LATIN CAPITAL LETTER Z WITH DOT ABOVE
	CHECK_CASE_MAPPING(0x17d); // LATIN CAPITAL LETTER Z WITH CARON
}

TEST_F(CaseMapping, LatinExtendedASmallLetters)
{
	CHECK_CASE_MAPPING(0x101); // LATIN SMALL LETTER A WITH MACRON
	CHECK_CASE_MAPPING(0x103); // LATIN SMALL LETTER A WITH BREVE
	CHECK_CASE_MAPPING(0x105); // LATIN SMALL LETTER A WITH OGONEK
	CHECK_CASE_MAPPING(0x107); // LATIN SMALL LETTER C WITH ACUTE
	CHECK_CASE_MAPPING(0x109); // LATIN SMALL LETTER C WITH CIRCUMFLEX
	CHECK_CASE_MAPPING(0x10b); // LATIN SMALL LETTER C WITH DOT ABOVE
	CHECK_CASE_MAPPING(0x10d); // LATIN SMALL LETTER C WITH CARON
	CHECK_CASE_MAPPING(0x10f); // LATIN SMALL LETTER D WITH CARON
	CHECK_CASE_MAPPING(0x111); // LATIN SMALL LETTER D WITH STROKE
	CHECK_CASE_MAPPING(0x113); // LATIN SMALL LETTER E WITH MACRON
	CHECK_CASE_MAPPING(0x115); // LATIN SMALL LETTER E WITH BREVE
	CHECK_CASE_MAPPING(0x117); // LATIN SMALL LETTER E WITH DOT ABOVE
	CHECK_CASE_MAPPING(0x119); // LATIN SMALL LETTER E WITH OGONEK
	CHECK_CASE_MAPPING(0x11b); // LATIN SMALL LETTER E WITH CARON
	CHECK_CASE_MAPPING(0x11d); // LATIN SMALL LETTER G WITH CIRCUMFLEX
	CHECK_CASE_MAPPING(0x11f); // LATIN SMALL LETTER G WITH BREVE
	CHECK_CASE_MAPPING(0x121); // LATIN SMALL LETTER G WITH DOT ABOVE
	CHECK_CASE_MAPPING(0x123); // LATIN SMALL LETTER G WITH CEDILLA
	CHECK_CASE_MAPPING(0x125); // LATIN SMALL LETTER H WITH CIRCUMFLEX
	CHECK_CASE_MAPPING(0x127); // LATIN SMALL LETTER H WITH STROKE
	CHECK_CASE_MAPPING(0x129); // LATIN SMALL LETTER I WITH TILDE
	CHECK_CASE_MAPPING(0x12b); // LATIN SMALL LETTER I WITH MACRON
	CHECK_CASE_MAPPING(0x12d); // LATIN SMALL LETTER I WITH BREVE
	CHECK_CASE_MAPPING(0x12f); // LATIN SMALL LETTER I WITH OGONEK
	CHECK_CASE_MAPPING(0x131); // LATIN SMALL LETTER DOTLESS I
	CHECK_CASE_MAPPING(0x133); // LATIN SMALL LIGATURE IJ
	CHECK_CASE_MAPPING(0x135); // LATIN SMALL LETTER J WITH CIRCUMFLEX
	CHECK_CASE_MAPPING(0x137); // LATIN SMALL LETTER K WITH CEDILLA
	CHECK_CASE_MAPPING(0x13a); // LATIN SMALL LETTER L WITH ACUTE
	CHECK_CASE_MAPPING(0x13c); // LATIN SMALL LETTER L WITH CEDILLA
	CHECK_CASE_MAPPING(0x13e); // LATIN SMALL LETTER L WITH CARON
	CHECK_CASE_MAPPING(0x140); // LATIN SMALL LETTER L WITH MIDDLE DOT
	CHECK_CASE_MAPPING(0x142); // LATIN SMALL LETTER L WITH STROKE
	CHECK_CASE_MAPPING(0x144); // LATIN SMALL LETTER N WITH ACUTE
	CHECK_CASE_MAPPING(0x146); // LATIN SMALL LETTER N WITH CEDILLA
	CHECK_CASE_MAPPING(0x148); // LATIN SMALL LETTER N WITH CARON
	CHECK_CASE_MAPPING(0x14b); // LATIN SMALL LETTER ENG
	CHECK_CASE_MAPPING(0x14d); // LATIN SMALL LETTER O WITH MACRON
	CHECK_CASE_MAPPING(0x14f); // LATIN SMALL LETTER O WITH BREVE
	CHECK_CASE_MAPPING(0x151); // LATIN SMALL LETTER O WITH DOUBLE ACUTE
	CHECK_CASE_MAPPING(0x153); // LATIN SMALL LIGATURE OE
	CHECK_CASE_MAPPING(0x155); // LATIN SMALL LETTER R WITH ACUTE
	CHECK_CASE_MAPPING(0x157); // LATIN SMALL LETTER R WITH CEDILLA
	CHECK_CASE_MAPPING(0x159); // LATIN SMALL LETTER R WITH CARON
	CHECK_CASE_MAPPING(0x15b); // LATIN SMALL LETTER S WITH ACUTE
	CHECK_CASE_MAPPING(0x15d); // LATIN SMALL LETTER S WITH CIRCUMFLEX
	CHECK_CASE_MAPPING(0x15f); // LATIN SMALL LETTER S WITH CEDILLA
	CHECK_CASE_MAPPING(0x161); // LATIN SMALL LETTER S WITH CARON
	CHECK_CASE_MAPPING(0x163); // LATIN SMALL LETTER T WITH CEDILLA
	CHECK_CASE_MAPPING(0x165); // LATIN SMALL LETTER T WITH CARON
	CHECK_CASE_MAPPING(0x167); // LATIN SMALL LETTER T WITH STROKE
	CHECK_CASE_MAPPING(0x169); // LATIN SMALL LETTER U WITH TILDE
	CHECK_CASE_MAPPING(0x16b); // LATIN SMALL LETTER U WITH MACRON
	CHECK_CASE_MAPPING(0x16d); // LATIN SMALL LETTER U WITH BREVE
	CHECK_CASE_MAPPING(0x16f); // LATIN SMALL LETTER U WITH RING ABOVE
	CHECK_CASE_MAPPING(0x171); // LATIN SMALL LETTER U WITH DOUBLE ACUTE
	CHECK_CASE_MAPPING(0x173); // LATIN SMALL LETTER U WITH OGONEK
	CHECK_CASE_MAPPING(0x175); // LATIN SMALL LETTER W WITH CIRCUMFLEX
	CHECK_CASE_MAPPING(0x177); // LATIN SMALL LETTER Y WITH CIRCUMFLEX
	CHECK_CASE_MAPPING(0x17a); // LATIN SMALL LETTER Z WITH ACUTE
	CHECK_CASE_MAPPING(0x17c); // LATIN SMALL LETTER Z WITH DOT ABOVE
	CHECK_CASE_MAPPING(0x17e); // LATIN SMALL LETTER Z WITH CARON
	CHECK_CASE_MAPPING(0x17f); // LATIN SMALL LETTER LONG S
}