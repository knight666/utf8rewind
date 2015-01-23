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

enum ComposeStage
{
	ComposeStage_Processing,
	ComposeStage_Flush,
	ComposeStage_OutOfInput,
	ComposeStage_WriteLast,
};

typedef struct {
	StreamState* input;
	StreamState* output;
	uint8_t stream_current;
	uint8_t stream_total;
	uint8_t stage;
	uint8_t stable;
	uint8_t last_canonical_combining_class;
	uint8_t stable_index;
	uint8_t current;
	uint8_t next;
} ComposeState;

uint8_t compose_initialize(ComposeState* state, StreamState* input, StreamState* output, uint8_t compatibility);

unicode_t compose_execute(ComposeState* state);

#endif