#pragma once

#include "property-base.hpp"

extern "C" {
	#include "../internal/codepoint.h"
};

namespace quickcheck {

	static inline void generateCodepoint(size_t size, unicode_t& output)
	{
		output = generateInRange<unicode_t>(0x0000, MAX_LEGAL_UNICODE);
	}

	static inline void generateCodepoint(size_t size, utf16_t& output)
	{
		output = generateInRange<utf16_t>(0x0000, MAX_BASIC_MULTILINGUAL_PLANE);
	}

	static inline void generate(size_t size, std::string& output)
	{
		uint8_t first_byte = 0x80;

		while (
			first_byte >= 0x80 &&
			first_byte <= 0xBF)
		{
			first_byte = generateInRange<uint8_t>(0x00, 0xFD);
		}

		output.push_back((char)first_byte);

		for (uint8_t i = 1; i < codepoint_decoded_length[first_byte]; ++i)
		{
			output.push_back((char)generateInRange<uint8_t>(0x80, 0xBF));
		}
	}

};