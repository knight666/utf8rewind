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

#include "streaming.h"

#include "codepoint.h"
#include "database.h"

uint8_t stream_initialize(StreamState* state, const char* input, size_t inputSize, uint8_t property)
{
	memset(state, 0, sizeof(StreamState));

	state->src = input;
	state->src_size = inputSize;
	state->property = property;

	state->stable = 1;

	return 1;
}

uint8_t stream_execute(StreamState* state)
{
	if (state->src_size == 0)
	{
		return 0;
	}

	/* Reset after the first frame */

	if (state->current >= 1)
	{
		uint8_t i;

		if (state->src_size <= state->last_length &&
			state->codepoint[state->current] == 0)
		{
			state->src_size = 0;
			state->current = 0;

			return 0;
		}

		state->codepoint[0] = state->codepoint[state->current];
		state->canonical_combining_class[0] = state->canonical_combining_class[state->current];
		state->quick_check[0] = state->quick_check[state->current];

		for (i = 1; i <= state->current; ++i)
		{
			state->codepoint[i] = 0;
			state->canonical_combining_class[i] = 0;
			state->quick_check[i] = 0;
		}

		if (state->quick_check[0] == QuickCheckResult_Yes &&
			state->canonical_combining_class[0] == 0)
		{
			state->starter_count = 1;
			state->stable = 1;
		}
		else
		{
			state->starter_count = 0;
			state->stable = 0;
		}

		state->current = 1;
	}

	/* Read codepoints */

	while (1)
	{
		if (state->current + 1 >= STREAM_BUFFER_MAX)
		{
			break;
		}

		if (state->last_length > 0)
		{
			if (state->src_size <= state->last_length)
			{
				break;
			}

			state->src += state->last_length;
			state->src_size -= state->last_length;
		}

		state->last_length = codepoint_read(&state->codepoint[state->current], state->src, state->src_size);

		state->quick_check[state->current] = database_queryproperty(state->codepoint[state->current], state->property);
		state->canonical_combining_class[state->current] = database_queryproperty(state->codepoint[state->current], UnicodeProperty_CanonicalCombiningClass);

		/* Check if codepoint is a starter */

		if (state->quick_check[state->current] == QuickCheckResult_Yes &&
			state->canonical_combining_class[state->current] == 0)
		{
			state->starter_count++;
			if (state->starter_count > 1)
			{
				break;
			}
		}
		else if (state->current > 0)
		{
			state->stable = 0;
		}

		state->current++;
	}

	/* Reorder codepoints if potentially unstable */

	if (state->stable == 0)
	{
		uint8_t dirty = 1;

		while (dirty == 1)
		{
			uint8_t i;
			uint8_t last_combining_class = 0;

			dirty = 0;

			for (i = 1; i < state->current; i++)
			{
				if (state->canonical_combining_class[i] < last_combining_class)
				{
					unicode_t swap_cp;
					uint8_t swap_qc;
					uint8_t swap_ccc;

					swap_cp = state->codepoint[i];
					state->codepoint[i] = state->codepoint[i - 1];
					state->codepoint[i - 1] = swap_cp;

					swap_qc = state->quick_check[i];
					state->quick_check[i] = state->quick_check[i - 1];
					state->quick_check[i - 1] = swap_qc;

					swap_ccc = state->canonical_combining_class[i];
					state->canonical_combining_class[i] = state->canonical_combining_class[i - 1];
					state->canonical_combining_class[i - 1] = swap_ccc;

					dirty = 1;
				}

				last_combining_class = state->canonical_combining_class[i];
			}
		}
	}

	return 1;
}