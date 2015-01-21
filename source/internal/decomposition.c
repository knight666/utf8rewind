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

uint8_t decompose_initialize(DecomposeState* state, StreamState* input, StreamState* output, uint8_t compatibility)
{
	memset(state, 0, sizeof(DecomposeState));

	state->input = input;
	state->input->property = (compatibility == 1)
		? UnicodeProperty_Normalization_Compatibility_Compose
		: UnicodeProperty_Normalization_Compose;

	state->output = output;
	memset(state->output, 0, sizeof(StreamState));
	state->output->property = (compatibility == 1)
		? UnicodeProperty_Normalization_Compatibility_Decompose
		: UnicodeProperty_Normalization_Decompose;

	return 1;
}

uint8_t decompose_execute(DecomposeState* state)
{
	uint8_t input_current;
	uint8_t input_total;
	unicode_t* dst_codepoint;
	uint8_t* dst_canonical_combining_class;
	uint8_t* dst_quick_check;

	if (stream_execute(state->input) == 0)
	{
		return 0;
	}

	input_current = 0;
	input_total = state->input->current;
	state->output->current = 0;
	dst_codepoint = state->output->codepoint;
	dst_canonical_combining_class = state->output->canonical_combining_class;
	dst_quick_check = state->output->quick_check;

	while (input_total > 0)
	{
		*dst_codepoint = state->input->codepoint[input_current];
		*dst_quick_check = database_queryproperty(*dst_codepoint, state->output->property);

		if (*dst_quick_check != QuickCheckResult_Yes)
		{
			const char* decomposition = database_querydecomposition(*dst_codepoint, state->output->property);
			if (decomposition != 0)
			{
				const char* src = decomposition;
				size_t src_size = strlen(decomposition);

				while (src_size > 0)
				{
					size_t offset = codepoint_read(dst_codepoint, src, src_size);
					if (offset == 0)
					{
						break;
					}

					state->output->current++;

					*dst_quick_check++ = database_queryproperty(*dst_codepoint, state->output->property);
					*dst_canonical_combining_class++ = database_queryproperty(*dst_codepoint, UnicodeProperty_CanonicalCombiningClass);
					dst_codepoint++;

					src += offset;
					src_size -= offset;
				}
			}
		}
		else
		{
			state->output->current++;

			dst_quick_check++;
			*dst_canonical_combining_class++ = database_queryproperty(*dst_codepoint, UnicodeProperty_CanonicalCombiningClass);
			dst_codepoint++;
		}

		input_current++;
		input_total--;
	}

	return state->output->current;
}