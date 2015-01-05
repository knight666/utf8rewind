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

#ifndef _UTFREWIND_INTERNAL_COMPOSITION_H_
#define _UTFREWIND_INTERNAL_COMPOSITION_H_

#include "utf8rewind.h"

enum ComposeStage
{
	ComposeStage_Processing,
	ComposeStage_OutOfInput,
	ComposeStage_WriteLast,
};

typedef struct {
	const char** src;
	size_t* src_size;
	uint8_t property;
	uint8_t stage;
	unicode_t codepoint[2];
	size_t length[2];
	uint8_t check[2];
	uint8_t current;
	uint8_t next;
} ComposeState;

uint8_t compose_initialize(ComposeState* state, const char** input, size_t* inputSize, uint8_t propertyType);

uint8_t compose_execute(ComposeState* state);

#endif