#pragma once

#include "property-base.hpp"

extern "C" {
	#include "../internal/codepoint.h"
};

void generateCodepoint(size_t size, unicode_t& output)
{
	output = quickcheck::generateInRange<unicode_t>(0x0000, MAX_LEGAL_UNICODE);
}

void generateCodepoint(size_t size, utf16_t& output)
{
	output = quickcheck::generateInRange<utf16_t>(0x0000, MAX_BASIC_MULTILINGUAL_PLANE);
}

void generateMultiByteSequence(size_t size, std::string& output)
{
	uint8_t firstByte = quickcheck::generateInRange<uint8_t>(0xC0, 0xFD);
	output.push_back((char)firstByte);

	for (uint8_t i = 0; i < codepoint_decoded_length[firstByte]; ++i)
	{
		output.push_back((char)quickcheck::generateInRange<uint8_t>(0x80, 0xBF));
	}
}