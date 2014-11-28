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

namespace helpers {

	std::string identifiable(unicode_t codepoint);
	std::string identifiable(const std::string& text);

	std::string uppercase(unicode_t codepoint);
	std::string uppercase(const std::string& text);

	std::string lowercase(unicode_t codepoint);
	std::string lowercase(const std::string& text);

	std::string titlecase(unicode_t codepoint);
	std::string titlecase(const std::string& text);

	std::string utf8(unicode_t codepoint);
	std::string utf8(const std::vector<unicode_t>& codepoints);

	std::string printable(const std::string& text);

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

		std::vector<unicode_t> ReadCodepoints(const char* line, size_t& offset) const;

		unicode_t StringToCodepoint(const char* inputStart, const char* inputEnd) const;

		std::map<unicode_t, CaseMappingEntry*> m_Database;
	};

};