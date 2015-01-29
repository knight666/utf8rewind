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

uint8_t compose_readcodepoint(ComposeState* state, uint8_t index)
{
	if (state->input_left > 0)
	{
		/* Use next codepoint from current sequence */

		state->cache_codepoint[index]                  = state->input->codepoint[state->input_index];
		state->cache_quick_check[index]                = state->input->quick_check[state->input_index];
		state->cache_canonical_combining_class[index]  = state->input->canonical_combining_class[state->input_index];

		state->input_index++;
		state->input_left--;

		return 1;
	}

	/* Read next sequence */

	if (stream_read(state->input) != 0)
	{
		/* First codepoint in next sequence */

		state->cache_codepoint[index]                  = state->input->codepoint[0];
		state->cache_quick_check[index]                = state->input->quick_check[0];
		state->cache_canonical_combining_class[index]  = state->input->canonical_combining_class[0];

		state->input_index = 1;
		state->input_left = state->input->current - 1;

		return 1;
	}
	
	/* End of data */

	state->input_index = 0;
	state->finished = 1;

	return 0;
}

unicode_t compose_execute(ComposeState* state)
{
	unicode_t composed;
	uint8_t buffer_next;
	uint8_t last_combining_class = 0;
	uint8_t reset_index;

	if (state->input == 0 ||
		(state->cache_filled == 0 && state->finished == 1))
	{
		return 0;
	}

	if (state->cache_codepoint[0] == 0)
	{
		if (!compose_readcodepoint(state, 0))
		{
			return 0;
		}

		state->cache_filled = 1;
	}

	state->cache_current = 0;
	state->cache_next = 1;

	if (state->cache_filled < 2)
	{
		if (!compose_readcodepoint(state, state->cache_next))
		{
			state->cache_filled = 0;

			return state->cache_codepoint[0];
		}

		state->cache_filled++;
	}

	composed = state->cache_codepoint[state->cache_current];

	while (1)
	{
		unicode_t current_composed = 0;

		if (state->cache_quick_check[state->cache_current] != QuickCheckResult_Yes ||
			state->cache_canonical_combining_class[state->cache_current] != 0 ||
			state->cache_quick_check[state->cache_next] != QuickCheckResult_Yes ||
			state->cache_canonical_combining_class[state->cache_next] != 0)
		{
			/*
				Hangul composition

				Algorithm adapted from Unicode Technical Report #15:
				http://www.unicode.org/reports/tr15/tr15-18.html#Hangul
			*/

			if (state->cache_codepoint[state->cache_current] >= HANGUL_L_FIRST &&
				state->cache_codepoint[state->cache_current] <= HANGUL_L_LAST)
			{
				/* Check for Hangul LV pair */ 

				if (state->cache_codepoint[state->cache_next] >= HANGUL_V_FIRST &&
					state->cache_codepoint[state->cache_next] <= HANGUL_V_LAST)
				{
					unicode_t l_index = state->cache_codepoint[state->cache_current] - HANGUL_L_FIRST;
					unicode_t v_index = state->cache_codepoint[state->cache_next] - HANGUL_V_FIRST;

					current_composed = HANGUL_S_FIRST + (((l_index * HANGUL_V_COUNT) + v_index) * HANGUL_T_COUNT);
				}
			}
			else if (
				state->cache_codepoint[state->cache_current] >= HANGUL_S_FIRST &&
				state->cache_codepoint[state->cache_current] <= HANGUL_S_LAST)
			{
				/* Check for Hangul LV and T pair */ 

				if (state->cache_codepoint[state->cache_next] >= HANGUL_T_FIRST &&
					state->cache_codepoint[state->cache_next] <= HANGUL_T_LAST)
				{
					unicode_t t_index = state->cache_codepoint[state->cache_next] - HANGUL_T_FIRST;

					current_composed = state->cache_codepoint[state->cache_current] + t_index;
				}
			}
			else
			{
				/* Attempt to compose codepoints */

				current_composed = database_querycomposition(
					state->cache_codepoint[state->cache_current],
					state->cache_codepoint[state->cache_next]);
			}
		}

		if (current_composed != 0)
		{
			state->cache_codepoint[state->cache_current]                  = current_composed;
			state->cache_quick_check[state->cache_current]                = database_queryproperty(current_composed, state->input->property);
			state->cache_canonical_combining_class[state->cache_current]  = database_queryproperty(current_composed, UnicodeProperty_CanonicalCombiningClass);

			composed = current_composed;

			state->cache_codepoint[state->cache_next]                  = 0;
			state->cache_quick_check[state->cache_next]                = 0;
			state->cache_canonical_combining_class[state->cache_next]  = 0;

			state->cache_next = 1;
		}

		state->cache_next = (state->cache_next + 1) % STREAM_BUFFER_MAX;
		if (state->cache_next == state->cache_filled)
		{
			if (!compose_readcodepoint(state, state->cache_next))
			{
				break;
			}

			state->cache_filled++;

			if (state->cache_quick_check[state->cache_next] == QuickCheckResult_Yes &&
				state->cache_canonical_combining_class[state->cache_next] == 0)
			{
				break;
			}
		}

		while (state->cache_codepoint[state->cache_next] == 0)
		{
			state->cache_next++;
		}
	}

	if (state->cache_filled > 0)
	{
		uint8_t write_index = 0;
		uint8_t read_index = 1;

		while (write_index < state->cache_filled - 1)
		{
			while (state->cache_codepoint[read_index] == 0)
			{
				read_index++;
			}

			state->cache_codepoint[write_index]                  = state->cache_codepoint[read_index];
			state->cache_quick_check[write_index]                = state->cache_quick_check[read_index];
			state->cache_canonical_combining_class[write_index]  = state->cache_canonical_combining_class[read_index];

			write_index++;
			read_index++;
		}

		state->cache_filled--;
	}

	return composed;

#if 0
	state->buffer_current = 0;

	/* Get left codepoint */

	if (state->cache_index < state->cache_filled)
	{
		/* Popped from queue */

		state->buffer_codepoint[state->buffer_current]                  = state->cache_codepoint[state->cache_index];
		state->buffer_quick_check[state->buffer_current]                = state->cache_quick_check[state->cache_index];
		state->buffer_canonical_combining_class[state->buffer_current]  = state->cache_canonical_combining_class[state->cache_index];

		state->cache_index++;
		if (state->cache_index == state->cache_filled)
		{
			state->cache_index = 0;
			state->cache_filled = 0;
		}
	}
	else if (state->input_left > 0)
	{
		/* Use next codepoint from current sequence */

		state->buffer_codepoint[state->buffer_current]                  = state->input->codepoint[state->input_index];
		state->buffer_quick_check[state->buffer_current]                = state->input->quick_check[state->input_index];
		state->buffer_canonical_combining_class[state->buffer_current]  = state->input->canonical_combining_class[state->input_index];

		state->input_index++;
		state->input_left--;
	}
	else 
	{
		/* Read next sequence */

		if (stream_read(state->input) != 0)
		{
			/* First codepoint in next sequence */

			state->buffer_codepoint[state->buffer_current]                  = state->input->codepoint[0];
			state->buffer_quick_check[state->buffer_current]                = state->input->quick_check[0];
			state->buffer_canonical_combining_class[state->buffer_current]  = state->input->canonical_combining_class[0];

			state->input_index = 1;
			state->input_left = state->input->current - 1;
		}
		else
		{
			/* End of data */

			state->input_index = 0;
			state->finished = 1;

			return 0;
		}
	}

	buffer_next = !state->buffer_current;

	/* Get right codepoint */

	if (state->cache_index < state->cache_filled)
	{
		state->buffer_codepoint[buffer_next]                  = state->cache_codepoint[state->cache_index];
		state->buffer_quick_check[buffer_next]                = state->cache_quick_check[state->cache_index];
		state->buffer_canonical_combining_class[buffer_next]  = state->cache_canonical_combining_class[state->cache_index];

		state->cache_index++;
		if (state->cache_index == state->cache_filled)
		{
			state->cache_index = 0;
			state->cache_filled = 0;
		}
	}
	else if (state->input_left > 0)
	{
		/* Use next codepoint from current sequence */

		state->buffer_codepoint[buffer_next]                  = state->input->codepoint[state->input_index];
		state->buffer_quick_check[buffer_next]                = state->input->quick_check[state->input_index];
		state->buffer_canonical_combining_class[buffer_next]  = state->input->canonical_combining_class[state->input_index];

		state->input_index++;
		state->input_left--;
	}
	else
	{
		/* Read next sequence */

		if (state->finished == 1)
		{
			/* Finished processing stream */

			return 0;
		}
		else if (stream_read(state->input) != 0)
		{
			/* First codepoint in next sequence */

			state->buffer_codepoint[buffer_next]                  = state->input->codepoint[0];
			state->buffer_quick_check[buffer_next]                = state->input->quick_check[0];
			state->buffer_canonical_combining_class[buffer_next]  = state->input->canonical_combining_class[0];

			state->input_index = 1;
			state->input_left = state->input->current - 1;
		}
		else
		{
			/* No data left, nothing to compose */

			state->finished = 1;

			return state->buffer_codepoint[state->buffer_current];
		}
	}

	/* Try to compose both codepoints as long as either one is unstable */

	composed = 0;

	while (1)
	{
		unicode_t current_composed = 0;

		if (state->buffer_quick_check[state->buffer_current] != QuickCheckResult_Yes ||
			state->buffer_canonical_combining_class[state->buffer_current] != 0 ||
			state->buffer_quick_check[buffer_next] != QuickCheckResult_Yes ||
			state->buffer_canonical_combining_class[buffer_next] != 0)
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

					current_composed = HANGUL_S_FIRST + (((l_index * HANGUL_V_COUNT) + v_index) * HANGUL_T_COUNT);
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

					current_composed = state->buffer_codepoint[state->buffer_current] + t_index;
				}
			}
			else
			{
				/* Attempt to compose codepoints */

				current_composed = database_querycomposition(
					state->buffer_codepoint[state->buffer_current],
					state->buffer_codepoint[buffer_next]);
			}
		}
		else
		{
			state->cache_codepoint[state->cache_filled]                   = state->buffer_codepoint[buffer_next];
			state->cache_quick_check[state->cache_filled]                 = state->buffer_quick_check[buffer_next];
			state->cache_canonical_combining_class[state->cache_filled]   = state->buffer_canonical_combining_class[buffer_next];

			state->cache_filled++;

			break;
		}

		if (current_composed != 0)
		{
			/* Store result */

			composed = current_composed;

			state->buffer_codepoint[state->buffer_current]                  = composed;
			state->buffer_quick_check[state->buffer_current]                = database_queryproperty(composed, state->input->property);
			state->buffer_canonical_combining_class[state->buffer_current]  = database_queryproperty(composed, UnicodeProperty_CanonicalCombiningClass);

			last_combining_class = state->buffer_canonical_combining_class[state->buffer_current];

			/* Reset cache queue */

			state->cache_index = 0;
		}
		else
		{
			/* Save failed result in cache */

			state->cache_codepoint[state->cache_filled]                  = state->buffer_codepoint[buffer_next];
			state->cache_quick_check[state->cache_filled]                = state->buffer_quick_check[buffer_next];
			state->cache_canonical_combining_class[state->cache_filled]  = state->buffer_canonical_combining_class[buffer_next];

			state->cache_index++;
			state->cache_filled++;
		}

		/* Get next codepoint for composition */

		if (current_composed != 0 &&
			state->cache_index < state->cache_filled)
		{
			/* Popped from queue */

			state->buffer_codepoint[buffer_next]                  = state->cache_codepoint[state->cache_index];
			state->buffer_quick_check[buffer_next]                = state->cache_quick_check[state->cache_index];
			state->buffer_canonical_combining_class[buffer_next]  = state->cache_canonical_combining_class[state->cache_index];

			state->cache_index++;
			if (state->cache_index == state->cache_filled)
			{
				state->cache_index = 0;
				state->cache_filled = 0;
			}
		}
		else if (
			state->input_left > 0)
		{
			/* Next in current sequence */

			state->buffer_codepoint[buffer_next]                  = state->input->codepoint[state->input_index];
			state->buffer_quick_check[buffer_next]                = state->input->quick_check[state->input_index];
			state->buffer_canonical_combining_class[buffer_next]  = state->input->canonical_combining_class[state->input_index];

			state->input_index++;
			state->input_left--;
		}
		else if (
			state->cache_filled > 0 &&
			state->cache_index == state->cache_filled)
		{
			state->cache_index = 0;

			break;
		}
		else
		{
			/* Read next sequence */

			if (stream_read(state->input) != 0)
			{
				/* First codepoint in next sequence */

				state->buffer_codepoint[buffer_next]                  = state->input->codepoint[0];
				state->buffer_quick_check[buffer_next]                = state->input->quick_check[0];
				state->buffer_canonical_combining_class[buffer_next]  = state->input->canonical_combining_class[0];

				state->input_index = 1;
				state->input_left = state->input->current - 1;
			}
			else
			{
				/* End of data */

				state->buffer_codepoint[buffer_next]                  = 0;
				state->buffer_quick_check[buffer_next]                = QuickCheckResult_Yes;
				state->buffer_canonical_combining_class[buffer_next]  = 0;

				state->finished = 1;

				break;
			}
		}

		if ((last_combining_class != 0 &&
			state->buffer_canonical_combining_class[buffer_next] == last_combining_class)/* ||
			(state->buffer_quick_check[buffer_next] == QuickCheckResult_Yes &&
			state->buffer_canonical_combining_class[buffer_next] == 0)*/)
		{
			state->cache_codepoint[state->cache_filled]                  = state->buffer_codepoint[buffer_next];
			state->cache_quick_check[state->cache_filled]                = state->buffer_quick_check[buffer_next];
			state->cache_canonical_combining_class[state->cache_filled]  = state->buffer_canonical_combining_class[buffer_next];

			state->cache_filled++;

			state->cache_index = 0;

			break;
		}

		last_combining_class = state->buffer_canonical_combining_class[buffer_next];
	}

	if (composed == 0)
	{
		composed = state->buffer_codepoint[state->buffer_current];
	}

	/* Swap buffer */

	state->buffer_current = !state->buffer_current;

	return composed;
#endif
}