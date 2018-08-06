#pragma once

#include "property-base.hpp"

class PropertySeekingCurrent
	: public quickcheck::Property<std::string, size_t>
{

	virtual bool holdsFor(const std::string& input, const size_t& offset) override
	{
		const char* seek_next = utf8seek(
			input.c_str(), input.length(), input.c_str(),
			static_cast<off_t>(offset), SEEK_CUR);

		const char* seek_start = utf8seek(
			seek_next, strlen(seek_next), input.c_str(),
			-static_cast<off_t>(offset), SEEK_CUR);

		return seek_start == input.c_str();
	}

	virtual void generateInput(size_t sizeHint, std::string& text, size_t& offset) override
	{
		if (sizeHint < 10)
		{
			offset = 1;
		}
		else if (sizeHint < 80)
		{
			offset = quickcheck::generateInRange(2, 15);
		}
		else
		{
			offset = quickcheck::generateInRange(16, 100);
		}

		for (size_t i = 0; i < offset; ++i)
		{
			std::string grapheme;
			quickcheck::generate(static_cast<size_t>(quickcheck::generateInRange(0, 100)), grapheme);

			text += grapheme;
		}
	}

};