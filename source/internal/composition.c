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

	state->output = output;
	state->output->current = 0;

	if (!compose_readcodepoint(state, 0))
	{
		state->finished = 1;

		return 0;
	}

	return 1;
}

uint8_t compose_readcodepoint(ComposeState* state, uint8_t index)
{
	if (state->input_left == 0)
	{
		if (!stream_read(state->input))
		{
			/* End of data */

			state->input_index = 0;
			state->input_left = 0;
			state->finished = 1;

			return 0;
		}
		else
		{
			/* First codepoint in next sequence */

			state->output->codepoint[index]                  = state->input->codepoint[0];
			state->output->quick_check[index]                = state->input->quick_check[0];
			state->output->canonical_combining_class[index]  = state->input->canonical_combining_class[0];

			state->input_index = 1;
			state->input_left = state->input->current - 1;
		}
	}
	else
	{
		/* Use next codepoint from current sequence */

		state->output->codepoint[index]                  = state->input->codepoint[state->input_index];
		state->output->quick_check[index]                = state->input->quick_check[state->input_index];
		state->output->canonical_combining_class[index]  = state->input->canonical_combining_class[state->input_index];

		state->input_index++;
		state->input_left--;
	}

	state->output->current++;

	return 1;
}

unicode_t compose_execute(ComposeState* state)
{
	unicode_t composed;
	uint8_t last_combining_class = 0;
	uint8_t finished = 0;
	uint8_t cache_start = 0;

	if (state->input == 0 ||
		state->finished)
	{
		return 0;
	}

	state->cache_current = cache_start;
	state->cache_next = cache_start + 1;

	while (1)
	{
		if (!compose_readcodepoint(state, state->cache_next))
		{
			return 1;
		}

		composed = state->output->codepoint[state->cache_current];

		finished = 0;

		do
		{
			unicode_t current_composed = 0;

			if (state->output->quick_check[state->cache_current] != QuickCheckResult_Yes ||
				state->output->canonical_combining_class[state->cache_current] != 0 ||
				state->output->quick_check[state->cache_next] != QuickCheckResult_Yes ||
				state->output->canonical_combining_class[state->cache_next] != 0)
			{
				/*
					Hangul composition

					Algorithm adapted from Unicode Technical Report #15:
					http://www.unicode.org/reports/tr15/tr15-18.html#Hangul
				*/

				if (state->output->codepoint[state->cache_current] >= HANGUL_L_FIRST &&
					state->output->codepoint[state->cache_current] <= HANGUL_L_LAST)
				{
					/* Check for Hangul LV pair */ 

					if (state->output->codepoint[state->cache_next] >= HANGUL_V_FIRST &&
						state->output->codepoint[state->cache_next] <= HANGUL_V_LAST)
					{
						unicode_t l_index = state->output->codepoint[state->cache_current] - HANGUL_L_FIRST;
						unicode_t v_index = state->output->codepoint[state->cache_next] - HANGUL_V_FIRST;

						current_composed = HANGUL_S_FIRST + (((l_index * HANGUL_V_COUNT) + v_index) * HANGUL_T_COUNT);
					}
				}
				else if (
					state->output->codepoint[state->cache_current] >= HANGUL_S_FIRST &&
					state->output->codepoint[state->cache_current] <= HANGUL_S_LAST)
				{
					/* Check for Hangul LV and T pair */ 

					if (state->output->codepoint[state->cache_next] >= HANGUL_T_FIRST &&
						state->output->codepoint[state->cache_next] <= HANGUL_T_LAST)
					{
						unicode_t t_index = state->output->codepoint[state->cache_next] - HANGUL_T_FIRST;

						current_composed = state->output->codepoint[state->cache_current] + t_index;
					}
				}
				else
				{
					/* Attempt to compose codepoints */

					current_composed = database_querycomposition(
						state->output->codepoint[state->cache_current],
						state->output->codepoint[state->cache_next]);
				}
			}
			else
			{
				break;
			}

			if (current_composed != 0)
			{
				state->output->codepoint[state->cache_current]                  = current_composed;
				state->output->quick_check[state->cache_current]                = database_queryproperty(current_composed, state->input->property);
				state->output->canonical_combining_class[state->cache_current]  = database_queryproperty(current_composed, UnicodeProperty_CanonicalCombiningClass);

				composed = current_composed;

				last_combining_class = state->output->canonical_combining_class[state->cache_next];

				state->output->codepoint[state->cache_next]                  = 0;
				state->output->quick_check[state->cache_next]                = 0;
				state->output->canonical_combining_class[state->cache_next]  = 0;

				if (state->cache_next == state->output->current - 1)
				{
					state->output->current--;
				}

				state->cache_current = cache_start;
				state->cache_next = cache_start + 1;
			}
			else
			{
				last_combining_class = state->output->canonical_combining_class[state->cache_next];

				state->cache_next++;
			}

			while (
				state->cache_next < state->cache_current &&
				state->output->codepoint[state->cache_next] == 0)
			{
				state->cache_next++;
			}
		
			if (state->cache_next == state->output->current)
			{
				do 
				{
					if (!compose_readcodepoint(state, state->cache_next))
					{
						finished = 1;
						break;
					}

					if (state->output->canonical_combining_class[state->cache_next] == 0)
					{
						if (state->output->quick_check[state->cache_next] == QuickCheckResult_Yes)
						{
							finished = 1;
							break;
						}
					}
					else if (
						state->cache_next > 1 &&
						state->output->canonical_combining_class[state->cache_next] <= state->output->canonical_combining_class[state->cache_next - 1])
					{
						state->cache_next++;
					}
				}
				while (state->cache_next == state->output->current);
			}
		}
		while (!finished);

		if (state->output->current > 1)
		{
			uint8_t write_index = 0;
			uint8_t read_index = 1;

			while (write_index < state->output->current)
			{
				if (read_index < state->output->current &&
					state->output->codepoint[read_index] == 0)
				{
					while (
						read_index < state->output->current &&
						state->output->codepoint[read_index] == 0)
					{
						read_index++;
					}

					if (read_index == state->output->current)
					{
						break;
					}

					state->output->codepoint[write_index]                  = state->output->codepoint[read_index];
					state->output->quick_check[write_index]                = state->output->quick_check[read_index];
					state->output->canonical_combining_class[write_index]  = state->output->canonical_combining_class[read_index];
				}

				write_index++;
				read_index++;
			}

			state->output->current = write_index;
			state->cache_next = state->output->current;
		}
		else
		{
			state->finished = 1;

			break;
		}

		cache_start++;
		state->cache_current++;
	}

	return 1;
}