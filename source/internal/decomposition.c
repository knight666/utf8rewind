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

#include "decomposition.h"

#include "codepoint.h"
#include "database.h"

uint8_t decompose_initialize(DecomposeState* state, StreamState* stream)
{
	memset(state, 0, sizeof(DecomposeState));

	state->stream = stream;

	return 1;
}

uint8_t decompose_execute(DecomposeState* state)
{
	uint8_t stream_current;
	uint8_t stream_total;
	unicode_t* dst;

	if (stream_execute(state->stream) == 0)
	{
		return 0;
	}

	state->filled = 0;
	stream_current = 0;
	stream_total = state->stream->current;
	dst = state->codepoint;

	while (stream_total > 0)
	{
		if (state->stream->quick_check[stream_current] != QuickCheckResult_Yes)
		{
			unicode_t codepoint = state->stream->codepoint[stream_current];
			const char* decomposition;

			decomposition = database_querydecomposition(codepoint, state->stream->property);
			if (decomposition != 0)
			{
				const char* src = decomposition;
				size_t src_size = strlen(decomposition);

				while (src_size > 0)
				{
					size_t offset = codepoint_read(dst, src, src_size);
					if (offset == 0)
					{
						break;
					}

					dst++;
					state->filled++;

					src += offset;
					src_size -= offset;
				}
			}
		}
		else
		{
			*dst++ = state->stream->codepoint[stream_current];
			state->filled++;
		}

		stream_current++;
		stream_total--;
	}

	return state->filled;
}