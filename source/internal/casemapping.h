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

#ifndef _UTF8REWIND_INTERNAL_CASEMAPPING_H_
#define _UTF8REWIND_INTERNAL_CASEMAPPING_H_

/*!
	\file
	\brief Case mapping interface.

	\cond INTERNAL
*/

#include "utf8rewind.h"

#define CASEMAPPING_LOCALE_DEFAULT                 0x00000000
#define CASEMAPPING_LOCALE_GREEK                   0x00000001
#define CASEMAPPING_LOCALE_LITHUANIAN              0x00000002
#define CASEMAPPING_LOCALE_TURKISH_OR_AZERI_LATIN  0x00000004

typedef struct {
	const char* src;
	size_t src_size;
	char* dst;
	size_t dst_size;
	uint8_t property;
	unicode_t last_code_point;
	uint8_t last_code_point_size;
	uint8_t last_general_category;
	uint32_t locale;
} CaseMappingState;

uint8_t casemapping_initialize(CaseMappingState* state, const char* input, size_t inputSize, char* target, size_t targetSize, uint8_t property);

uint8_t casemapping_readcodepoint(CaseMappingState* state);

size_t casemapping_execute(CaseMappingState* state);

size_t casemapping_execute2(CaseMappingState* state);

/*! \endcond */

#endif /* _UTF8REWIND_INTERNAL_CASEMAPPING_H_ */