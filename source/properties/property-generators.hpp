#pragma once

#include "property-base.hpp"

extern "C" {
	#include "../internal/codepoint.h"
};

namespace quickcheck {

	static inline void generateCodepoint(size_t sizeHint, unicode_t& output)
	{
		output = generateInRange<unicode_t>(0x0000, MAX_LEGAL_UNICODE);
	}

	static inline void generateCodepoint(size_t sizeHint, utf16_t& output)
	{
		output = generateInRange<utf16_t>(0x0000, MAX_BASIC_MULTILINGUAL_PLANE);
	}

	static inline void generate(size_t sizeHint, std::string& output)
	{
		uint8_t first_byte;

		if (sizeHint < 30)
		{
			// Basic Latin only

			first_byte = generateInRange<uint8_t>(0x00, 0x7F);
		}
		else if (sizeHint < 80)
		{
			// Multi-byte sequence

			first_byte = generateInRange<uint8_t>(0xC0, 0xFD);
		}
		else
		{
			// Random

			first_byte = generateInRange<uint8_t>(0x00, 0xFF);
		}

		output.push_back((char)first_byte);

		uint8_t sequence_length = codepoint_decoded_length[first_byte];
		if (sizeHint > 90)
		{
			sequence_length = generateInRange<uint8_t>(0, sequence_length);
		}

		for (uint8_t i = 1; i < sequence_length; ++i)
		{
			output.push_back((char)generateInRange<uint8_t>(0x80, 0xBF));
		}
	}

};