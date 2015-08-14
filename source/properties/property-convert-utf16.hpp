#pragma once

#include "property-base.hpp"

#include "utf8rewind.h"

class PropertyConvertUtf16
	: public quickcheck::Property<utf16_t>
{

	bool holdsFor(const utf16_t& input)
	{
		if (input == 0)
		{
			return true;
		}

		int32_t errors;
		size_t converted_size;

		char text_utf8[256] = { 0 };

		converted_size = utf16toutf8(
			&input, sizeof(input),
			text_utf8, 255,
			&errors);
		if (converted_size == 0 ||
			errors != UTF8_ERR_NONE)
		{
			return false;
		}

		utf16_t output = 0;

		converted_size = utf8toutf16(
			text_utf8, strlen(text_utf8),
			&output, sizeof(output),
			&errors);
		if (converted_size == 0 ||
			errors != UTF8_ERR_NONE)
		{
			return false;
		}

		return output == input;
	}

};