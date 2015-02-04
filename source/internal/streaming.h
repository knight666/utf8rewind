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

#ifndef _UTFREWIND_INTERNAL_STREAMING_H_
#define _UTFREWIND_INTERNAL_STREAMING_H_

#include "utf8rewind.h"

/*
	UAX15-D3. Stream-Safe Text Format

	A Unicode string is said to be in Stream-Safe Text Format if it would not
	contain any sequences of non-starters longer than 30 characters in length
	when normalized to NFKD
*/

#define STREAM_BUFFER_MAX 32

typedef struct {
	const char* src;
	size_t src_size;
	uint8_t property;
	uint8_t index;
	uint8_t current;
	uint8_t stable;
	uint8_t last_length;
	unicode_t codepoint[STREAM_BUFFER_MAX];
	uint8_t quick_check[STREAM_BUFFER_MAX];
	uint8_t canonical_combining_class[STREAM_BUFFER_MAX];
} StreamState;

uint8_t stream_initialize(StreamState* state, const char* input, size_t inputSize, uint8_t property);

uint8_t stream_read(StreamState* state);

uint8_t stream_write(StreamState* state, char* output, size_t outputSize, uint8_t* written);

uint8_t stream_reorder(StreamState* state);

#endif // _UTFREWIND_INTERNAL_STREAMING_H_