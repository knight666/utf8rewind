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
#include "streaming.h"

typedef struct {
	StreamState* input;
	uint8_t input_index;
	uint8_t input_left;
	uint8_t finished;
	unicode_t cache_codepoint[STREAM_BUFFER_MAX];
	uint8_t cache_quick_check[STREAM_BUFFER_MAX];
	uint8_t cache_index;
	uint8_t cache_filled;
	unicode_t buffer_codepoint[2];
	uint8_t buffer_quick_check[2];
	uint8_t buffer_current;
	uint8_t buffer_left;
} ComposeState;

uint8_t compose_initialize(ComposeState* state, StreamState* input, uint8_t compatibility);

unicode_t compose_execute(ComposeState* state);

#endif /* _UTFREWIND_INTERNAL_COMPOSITION_H_ */