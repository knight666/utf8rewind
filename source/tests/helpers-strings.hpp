#pragma once

// C/C++

#include <stdint.h>

// STL

#include <iomanip>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "utf8rewind.h"

#define TEST_ENABLE_UPPERCASE (1)
#define TEST_ENABLE_LOWERCASE (1)
#define TEST_ENABLE_TITLECASE (0)

namespace helpers {

	std::string CodepointToString(unicode_t codepoint);
	std::string CodepointToUppercaseString(unicode_t codepoint);
	std::string CodepointToLowercaseString(unicode_t codepoint);
	std::string CodepointToTitlecaseString(unicode_t codepoint);
	std::string Utf8StringToCodepoints(const std::string& text);
	std::string Utf8StringPrintable(const std::string& text);
	std::string Utf8StringPrintableCodepoints(const std::string& text);

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

		static CaseMappingDatabase& Get();

		bool IsValid() const { return m_Database.size() > 0; }

		CaseMappingEntry* Find(unicode_t codepoint) const;

	private:

		CaseMappingDatabase();

		CaseMappingEntry* ParseLine(const char* line) const;

		std::string CodepointsToUtf8(const std::vector<unicode_t>& codepoints) const;

		std::vector<unicode_t> ReadCodepoints(const char* line, size_t& offset) const;

		unicode_t StringToCodepoint(const char* inputStart, const char* inputEnd) const;

		std::map<unicode_t, CaseMappingEntry*> m_Database;
	};

};