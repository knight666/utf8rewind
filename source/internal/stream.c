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

#include "stream.h"

#include "codepoint.h"
#include "database.h"

uint8_t stream_initialize(StreamState* state, const char** input, size_t* inputSize, uint8_t property)
{
	memset(state, 0, sizeof(StreamState));

	state->src = input;
	state->src_size = inputSize;
	state->property = property;

	state->stable = 1;
	state->stable = ReorderResult_Next;

	return 1;
}

uint8_t stream_execute(StreamState* state)
{
	uint8_t i;

	if (state->stage == ReorderResult_Flush)
	{
		if (state->current >= 1)
		{
			uint8_t last = state->current;

			state->codepoint[0] = state->codepoint[last];
			state->canonical_combining_class[0] = state->canonical_combining_class[last];
			state->quick_check[0] = state->quick_check[last];

			for (i = 1; i <= state->current; ++i)
			{
				state->codepoint[i] = 0;
				state->canonical_combining_class[i] = 0;
				state->quick_check[i] = 0;
			}

			state->stable = (state->quick_check[0] == QuickCheckResult_Yes);
			state->current = 1;
		}
		else
		{
			for (i = 0; i <= state->current; ++i)
			{
				state->codepoint[i] = 0;
				state->canonical_combining_class[i] = 0;
				state->quick_check[i] = 0;
			}

			state->stable = 1;
			state->current = 0;
		}

		state->stage = ReorderResult_Next;
	}

	while (state->stage == ReorderResult_Next)
	{
		state->stage = stream_readcodepoint(state);
	}

	if (state->stable == 0)
	{
		uint8_t dirty = 1;

		/* Reorder */

		while (dirty == 1)
		{
			uint8_t last_combining_class = 0;

			dirty = 0;

			for (i = 1; i < state->current; i++)
			{
				if (state->canonical_combining_class[i] < last_combining_class)
				{
					unicode_t swap_cp = state->codepoint[i];
					uint8_t swap_qc = state->quick_check[i];
					uint8_t swap_ccc = state->canonical_combining_class[i];

					state->codepoint[i] = state->codepoint[i - 1];
					state->codepoint[i - 1] = swap_cp;

					state->quick_check[i] = state->quick_check[i - 1];
					state->quick_check[i - 1] = swap_qc;

					state->canonical_combining_class[i] = state->canonical_combining_class[i - 1];
					state->canonical_combining_class[i - 1] = swap_ccc;

					dirty = 1;
				}

				last_combining_class = state->canonical_combining_class[i];
			}
		}
	}

	return state->stage;
}

uint8_t stream_readcodepoint(StreamState* state)
{
	uint8_t length;

	if (state->current + 1 >= STREAM_BUFFER_MAX)
	{
		goto flush;
	}

	length = codepoint_read(&state->codepoint[state->current], *state->src, *state->src_size);
	if (*state->src_size > length)
	{
		*state->src += length;
		*state->src_size -= length;
	}
	else
	{
		goto outofinput;
	}

	state->quick_check[state->current] = database_queryproperty(state->codepoint[state->current], state->property);
	state->canonical_combining_class[state->current] = database_queryproperty(state->codepoint[state->current], UnicodeProperty_CanonicalCombiningClass);

	if (state->current > 0)
	{
		uint8_t previous = state->current - 1;

		if (state->quick_check[previous] != QuickCheckResult_Yes)
		{
			if (state->canonical_combining_class[previous] > 0)
			{
				state->stable = 0;
			}

			goto flush;
		}
		else
		{
			if (state->canonical_combining_class[previous] == 0 &&
				previous > 1)
			{
				goto flush;
			}
		}
	}

	state->current++;

	return 1;

flush:
	return ReorderResult_Flush;

outofinput:
	return ReorderResult_OutOfData;
}