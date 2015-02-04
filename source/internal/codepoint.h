/*
	Copyright (C) 2014-2015 Quinten Lansu

	Permission is hereby granted, free of charge, to any person
	obtaining a copy of this software and associated documentation
	files (the "Software"), to deal in the Software without
	restriction, including without limitation the rights to use,
	copy, modify, merge, publish, distribute, sublicense, and/or
	sell copies of the Software, and to permit persons to whom the
	Software is furnished to do so, subject to the following
	conditions:

	The above copyright notice and this permission notice shall be
	included in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
	OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef _UTFREWIND_INTERNAL_CODEPOINT_H_
#define _UTFREWIND_INTERNAL_CODEPOINT_H_

#include "utf8rewind.h"

#define MAX_BASIC_MULTILINGUAR_PLANE  0xFFFF
#define MAX_LEGAL_UNICODE             0x10FFFF
#define REPLACEMENT_CHARACTER         0xFFFD

#define SURROGATE_HIGH_START          0xD800
#define SURROGATE_HIGH_END            0xDBFF
#define SURROGATE_LOW_START           0xDC00
#define SURROGATE_LOW_END             0xDFFF

#define HANGUL_L_FIRST                0x1100
#define HANGUL_L_LAST                 0x1112
#define HANGUL_L_COUNT                19
#define HANGUL_V_FIRST                0x1161
#define HANGUL_V_LAST                 0x1175
#define HANGUL_V_COUNT                21
#define HANGUL_T_FIRST                0x11A7
#define HANGUL_T_LAST                 0x11C2
#define HANGUL_T_COUNT                28
#define HANGUL_N_COUNT                588 /* VCount * TCount */
#define HANGUL_S_FIRST                0xAC00
#define HANGUL_S_LAST                 0xD7A3
#define HANGUL_S_COUNT                11172 /* LCount * NCount */

extern const uint8_t codepoint_decoded_length[256];

uint8_t codepoint_encoded_length(unicode_t codepoint);

uint8_t codepoint_write(unicode_t encoded, char** target, size_t* targetSize);

uint8_t codepoint_read(const char* input, size_t inputSize, unicode_t* decoded);

#endif