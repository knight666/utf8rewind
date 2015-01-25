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

#include "composition.h"

#include "codepoint.h"
#include "database.h"

uint8_t compose_initialize(ComposeState* state, StreamState* input, uint8_t compatibility)
{
	memset(state, 0, sizeof(ComposeState));

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

	return 1;
}

unicode_t compose_execute(ComposeState* state)
{
	unicode_t composed;
	uint8_t buffer_next;

	if (state->input == 0)
	{
		return 0;
	}

	if (state->input_left == 0)
	{
		if (state->finished == 1)
		{
			return 0;
		}

		state->input_index = 0;

		if (stream_read(state->input) == 0)
		{
			state->finished = 1;
		}
		else
		{
			state->buffer_codepoint[state->buffer_current] = state->input->codepoint[0];
			state->buffer_quick_check[state->buffer_current] = state->input->quick_check[0];

			state->input_index = 1;
			state->input_left = state->input->current;
		}
	}

	composed = 0;
	buffer_next = !state->buffer_current;

	if (state->input_left > 1)
	{
		state->buffer_codepoint[buffer_next] = state->input->codepoint[state->input_index];
		state->buffer_quick_check[buffer_next] = state->input->quick_check[state->input_index];

		state->input_index++;
		state->input_left--;

		if (state->buffer_codepoint[state->buffer_current] < 0x80 &&
			state->buffer_codepoint[buffer_next] < 0x80)
		{
			/* Basic Latin codepoints cannot be composed */

			composed = state->buffer_codepoint[state->buffer_current];
		}
		else
		{
			while (
				state->buffer_quick_check[state->buffer_current] != QuickCheckResult_Yes ||
				state->buffer_quick_check[buffer_next] != QuickCheckResult_Yes)
			{
				/*
					Hangul composition

					Algorithm adapted from Unicode Technical Report #15:
					http://www.unicode.org/reports/tr15/tr15-18.html#Hangul
				*/

				if (state->buffer_codepoint[state->buffer_current] >= HANGUL_L_FIRST &&
					state->buffer_codepoint[state->buffer_current] <= HANGUL_L_LAST)
				{
					/* Check for Hangul LV pair */ 

					if (state->buffer_codepoint[buffer_next] >= HANGUL_V_FIRST &&
						state->buffer_codepoint[buffer_next] <= HANGUL_V_LAST)
					{
						unicode_t l_index = state->buffer_codepoint[state->buffer_current] - HANGUL_L_FIRST;
						unicode_t v_index = state->buffer_codepoint[buffer_next] - HANGUL_V_FIRST;

						composed = HANGUL_S_FIRST + (((l_index * HANGUL_V_COUNT) + v_index) * HANGUL_T_COUNT);
					}
				}
				else if (
					state->buffer_codepoint[state->buffer_current] >= HANGUL_S_FIRST &&
					state->buffer_codepoint[state->buffer_current] <= HANGUL_S_LAST)
				{
					/* Check for Hangul LV and T pair */ 

					if (state->buffer_codepoint[buffer_next] >= HANGUL_T_FIRST &&
						state->buffer_codepoint[buffer_next] <= HANGUL_T_LAST)
					{
						unicode_t t_index = state->buffer_codepoint[buffer_next] - HANGUL_T_FIRST;

						composed = state->buffer_codepoint[state->buffer_current] + t_index;
					}
				}
				else
				{
					/* Check database for composition */

					composed = database_querycomposition(state->buffer_codepoint[state->buffer_current], state->buffer_codepoint[buffer_next]);
				}

				if (composed != 0)
				{
					/* Store result */

					state->buffer_codepoint[state->buffer_current] = composed;
					state->buffer_quick_check[state->buffer_current] = database_queryproperty(composed, state->input->property);

					if (state->input_left > 0)
					{
						/* Try to compose with next codepoint */

						state->buffer_codepoint[buffer_next] = state->input->codepoint[state->input_index];
						state->buffer_quick_check[buffer_next] = state->input->quick_check[state->input_index];

						state->input_index++;
						state->input_left--;
					}
					else
					{
						/* End of data */

						state->buffer_codepoint[buffer_next] = 0;
						state->buffer_quick_check[buffer_next] = QuickCheckResult_Yes;

						break;
					}
				}
				else
				{
					/* Failed to compose */

					break;
				}
			}
		}
	}
	else if (state->input_left > 0)
	{
		/* Only one codepoint left, nothing to compose */

		state->buffer_codepoint[buffer_next] = 0;
		state->buffer_quick_check[buffer_next] = QuickCheckResult_Yes;

		state->input_left--;
	}

	if (composed == 0)
	{
		composed = state->buffer_codepoint[state->buffer_current];
	}

	/* Swap buffer */

	state->buffer_current = !state->buffer_current;

	return composed;
}