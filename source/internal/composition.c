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

uint8_t compose_initialize(ComposeState* state, StreamState* input, StreamState* output, uint8_t compatibility)
{
	uint8_t property;

	memset(state, 0, sizeof(ComposeState));

	/* Ensure input is valid */

	if (input->src == 0 ||
		input->src_size == 0)
	{
		return 0;
	}

	property = (compatibility == 1)
		? UnicodeProperty_Normalization_Compatibility_Compose
		: UnicodeProperty_Normalization_Compose;

	/* Set up input stream */

	state->input = input;
	state->input->property = property;

	/* Set up output stream */

	state->output = output;
	state->output->property = property;
	state->output->codepoint[0] = 0;
	state->output->current = 0;

	state->stage = ComposeStage_Processing;

	state->stream_current = 0;
	state->stream_total = 0;
	
	state->current = 0;
	state->next = 1;

	return 1;
}

unicode_t compose_execute(ComposeState* state)
{
	unicode_t composed;

	if (state->input == 0)
	{
		return 0;
	}

	if (state->stream_total == 0)
	{
		if (state->stage == ComposeStage_OutOfInput)
		{
			return 0;
		}

		state->stream_current = 0;

		if (stream_read(state->input) == 0)
		{
			state->stage = ComposeStage_OutOfInput;
		}
		else
		{
			state->output->codepoint[state->current] = state->input->codepoint[0];
			state->output->quick_check[state->current] = state->input->quick_check[0];

			state->stream_current = 1;
			state->stream_total = state->input->current;
		}
	}

	composed = 0;

	if (state->stream_total > 1)
	{
		state->output->codepoint[state->next] = state->input->codepoint[state->stream_current];
		state->output->quick_check[state->next] = state->input->quick_check[state->stream_current];

		state->stream_current++;
		state->stream_total--;

		while (
			state->output->quick_check[state->current] != QuickCheckResult_Yes ||
			state->output->quick_check[state->next] != QuickCheckResult_Yes)
		{
			/*
				Hangul composition

				Algorithm adapted from Unicode Technical Report #15:
				http://www.unicode.org/reports/tr15/tr15-18.html#Hangul
			*/

			if (state->output->codepoint[state->current] >= HANGUL_L_FIRST &&
				state->output->codepoint[state->current] <= HANGUL_L_LAST)
			{
				/* Check for Hangul LV pair */ 

				if (state->output->codepoint[state->next] >= HANGUL_V_FIRST &&
					state->output->codepoint[state->next] <= HANGUL_V_LAST)
				{
					unicode_t l_index = state->output->codepoint[state->current] - HANGUL_L_FIRST;
					unicode_t v_index = state->output->codepoint[state->next] - HANGUL_V_FIRST;

					composed = HANGUL_S_FIRST + (((l_index * HANGUL_V_COUNT) + v_index) * HANGUL_T_COUNT);
				}
			}
			else if (
				state->output->codepoint[state->current] >= HANGUL_S_FIRST &&
				state->output->codepoint[state->current] <= HANGUL_S_LAST)
			{
				/* Check for Hangul LV and T pair */ 

				if (state->output->codepoint[state->next] >= HANGUL_T_FIRST &&
					state->output->codepoint[state->next] <= HANGUL_T_LAST)
				{
					unicode_t t_index = state->output->codepoint[state->next] - HANGUL_T_FIRST;

					composed = state->output->codepoint[state->current] + t_index;
				}
			}
			else
			{
				/* Check database for composition */

				composed = database_querycomposition(state->output->codepoint[state->current], state->output->codepoint[state->next]);
			}

			if (composed != 0)
			{
				/* Store result */

				state->output->codepoint[state->current] = composed;
				state->output->quick_check[state->current] = database_queryproperty(composed, state->input->property);

				if (state->stream_total > 0)
				{
					/* Try to compose with next codepoint */

					state->output->codepoint[state->next] = state->input->codepoint[state->stream_current];
					state->output->quick_check[state->next] = state->input->quick_check[state->stream_current];

					state->stream_current++;
					state->stream_total--;
				}
				else
				{
					/* End of data */

					state->output->codepoint[state->next] = 0;
					state->output->quick_check[state->next] = QuickCheckResult_Yes;

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
	else if (state->stream_total > 0)
	{
		/* Only one codepoint left, nothing to compose */

		state->output->codepoint[state->next] = 0;
		state->output->quick_check[state->next] = QuickCheckResult_Yes;

		state->stream_total--;
	}

	if (composed == 0)
	{
		composed = state->output->codepoint[state->current];
	}

	/* Swap buffers */

	state->current = (state->current + 1) % 2;
	state->next = (state->next + 1) % 2;

	return composed;
}