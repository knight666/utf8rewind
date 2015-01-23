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

	/* Ensure input is valid */

	if (input->src == 0 ||
		input->src_size == 0)
	{
		return 0;
	}

	/* Set up input stream */

	state->input = input;
	state->input->property = (compatibility == 1)
		? UnicodeProperty_Normalization_Compatibility_Compose
		: UnicodeProperty_Normalization_Compose;

	/* Set up output stream */

	state->output = output;
	state->output->property = (compatibility == 1)
		? UnicodeProperty_Normalization_Compatibility_Decompose
		: UnicodeProperty_Normalization_Decompose;
	state->output->codepoint[0] = 0;
	state->output->current = 0;

	return 1;
}

uint8_t decompose_execute(DecomposeState* state)
{
	unicode_t* src_codepoint;
	uint8_t src_left;
	unicode_t* dst_codepoint;
	uint8_t* dst_quick_check;
	uint8_t* dst_canonical_combining_class;

	/* Read next sequence */

	if (state->input == 0 ||
		stream_read(state->input) == 0)
	{
		return 0;
	}

	/* Reset output */

	state->output->current = 0;
	state->output->stable = 1;

	/* Set up source and destination */

	src_codepoint = state->input->codepoint;
	src_left = state->input->current;

	dst_codepoint = state->output->codepoint;
	dst_quick_check = state->output->quick_check;
	dst_canonical_combining_class = state->output->canonical_combining_class;

	while (src_left > 0)
	{
		/* Use quick check to skip stable codepoints */

		*dst_codepoint = *src_codepoint;
		*dst_quick_check = database_queryproperty(*dst_codepoint, state->output->property);

		if (*dst_quick_check != QuickCheckResult_Yes)
		{
			/* Check database for decomposition */

			const char* decomposition = database_querydecomposition(*dst_codepoint, state->output->property);
			if (decomposition != 0)
			{
				/* Write sequence to output */

				const char* src = decomposition;
				size_t src_size = strlen(decomposition);

				while (src_size > 0)
				{
					size_t offset = codepoint_read(dst_codepoint, src, src_size);
					if (offset == 0)
					{
						break;
					}

					*dst_canonical_combining_class++ = database_queryproperty(*dst_codepoint, UnicodeProperty_CanonicalCombiningClass);
					*dst_quick_check++ = QuickCheckResult_Yes;
					dst_codepoint++;

					state->output->current++;

					src += offset;
					src_size -= offset;
				}
			}
		}
		else
		{
			/* Write codepoint to output */			

			*dst_canonical_combining_class++ = database_queryproperty(*dst_codepoint, UnicodeProperty_CanonicalCombiningClass);
			dst_quick_check++;
			dst_codepoint++;

			state->output->current++;
		}

		/* Output is stable as long as only one codepoint or sequence is written */

		if (src_codepoint != state->input->codepoint)
		{
			state->output->stable = 0;
		}

		src_codepoint++;
		src_left--;
	}

	return state->output->current;
}