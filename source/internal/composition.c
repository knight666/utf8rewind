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

uint8_t compose_initialize(ComposeState* state, const char** input, size_t* inputSize, uint8_t propertyType)
{
	unicode_t codepoint;
	uint8_t length;

	memset(state, 0, sizeof(ComposeState));

	state->stage = ComposeStage_Processing;
	state->src = input;
	state->src_size = inputSize;
	state->property = propertyType;
	state->stable = 1;

	state->check[0] = QuickCheckResult_No;
	state->check[1] = QuickCheckResult_No;

	state->current = 0;
	state->next = 1;

	return 1;
}

uint8_t compose_execute(ComposeState* state)
{
	if (state->stage == ComposeStage_Flush)
	{
		if (state->current >= 1)
		{
			state->stream_cp[0] = state->stream_cp[state->current - 1];
		}

		state->stage = ComposeStage_Processing;
	}
	else if (state->stage >= ComposeStage_OutOfInput)
	{
		state->stage = ComposeStage_WriteLast;

		return (state->check[state->current] != QuickCheckResult_No) ? state->current : (uint8_t)-1;
	}

	while (state->stage == ComposeStage_Processing)
	{
		unicode_t composed = 0;

		uint8_t result = compose_readcodepoint(state);
		if (result == 0)
		{
			break;
		}
#if 0
		if (*state->src_size > 0)
		{
			/* Read the next codepoint */

			state->length[state->next] = codepoint_read(&state->codepoint[state->next], *state->src, *state->src_size);
			state->check[state->next] = database_queryproperty(state->codepoint[state->next], state->property);
			state->canonical_combining_class[state->next] = database_queryproperty(state->codepoint[state->next], UnicodeProperty_CanonicalCombiningClass);

			if (*state->src_size >= state->length[state->next])
			{
				*state->src += state->length[state->next];
				*state->src_size -= state->length[state->next];

				if (*state->src_size == 0)
				{
					state->stage = ComposeStage_OutOfInput;
				}
			}
			else
			{
				state->stage = ComposeStage_OutOfInput;
			}
		}

		if (state->check[state->current] == QuickCheckResult_Yes &&
			state->check[state->next] == QuickCheckResult_Yes)
		{
			break;
		}

		/*
			Hangul composition

			Algorithm adapted from Unicode Technical Report #15:
			http://www.unicode.org/reports/tr15/tr15-18.html#Hangul
		*/

		if (state->codepoint[state->current] >= HANGUL_L_FIRST &&
			state->codepoint[state->current] <= HANGUL_L_LAST)
		{
			/* Check for Hangul LV pair */ 

			if (state->codepoint[state->next] >= HANGUL_V_FIRST &&
				state->codepoint[state->next] <= HANGUL_V_LAST)
			{
				unicode_t l_index = state->codepoint[state->current] - HANGUL_L_FIRST;
				unicode_t v_index = state->codepoint[state->next] - HANGUL_V_FIRST;

				composed = HANGUL_S_FIRST + (((l_index * HANGUL_V_COUNT) + v_index) * HANGUL_T_COUNT);
			}
			else
			{
				break;
			}
		}
		else if (
			state->codepoint[state->current] >= HANGUL_S_FIRST &&
			state->codepoint[state->current] <= HANGUL_S_LAST)
		{
			/* Check for Hangul LV and T pair */ 

			if (state->codepoint[state->next] >= HANGUL_T_FIRST &&
				state->codepoint[state->next] <= HANGUL_T_LAST)
			{
				unicode_t t_index = state->codepoint[state->next] - HANGUL_T_FIRST;

				composed = state->codepoint[state->current] + t_index;
			}
			else
			{
				break;
			}
		}
		else
		{
			/* Check database for composition */

			composed = database_querycomposition(state->codepoint[state->current], state->codepoint[state->next]);
		}

		if (composed == 0)
		{
			break;
		}

		if (state->check[state->next] == QuickCheckResult_Maybe)
		{
			/* If the composition succeeded but there's no data left, don't output the second codepoint */

			state->check[state->next] = (state->stage >= ComposeStage_OutOfInput) ? QuickCheckResult_No : QuickCheckResult_Yes;
		}

		state->codepoint[state->current] = composed;
		state->length[state->current] = codepoint_encoded_length(composed);
		state->check[state->current] = database_queryproperty(composed, state->property);
		state->canonical_combining_class[state->current] = database_queryproperty(composed, UnicodeProperty_CanonicalCombiningClass);
#endif
	}

	if (state->stable == 0)
	{
		uint8_t dirty = 1;

		/* Reorder */

		//last_combining_class = database_queryproperty(state->stream[i], UnicodeProperty_CanonicalCombiningClass);

		// stable - 0
		// unstable - 67
		// unstable - 112

		while (dirty == 1)
		{
			uint8_t last_combining_class = 0;
			uint8_t i = state->current - 1;

			dirty = 0;

			for ( ; i >= 1; i--)
			{
				if (last_combining_class != 0 &&
					state->stream_ccc[i] > last_combining_class)
				{
					unicode_t swap_cp = state->stream_cp[i];
					uint8_t swap_ccc = state->stream_ccc[i];

					state->stream_cp[i] = state->stream_cp[i - 1];
					state->stream_cp[i - 1] = swap_cp;

					state->stream_ccc[i] = state->stream_ccc[i - 1];
					state->stream_ccc[i - 1] = swap_ccc;

					dirty = 1;
				}

				last_combining_class = state->stream_ccc[i];
			}
		}
	}

	/* Swap buffers */

	state->current = (state->current + 1) % 2;
	state->next = (state->next + 1) % 2;

	return state->next;
}

uint8_t compose_readcodepoint(ComposeState* state)
{
	uint8_t length;
	uint8_t qc;
	uint8_t ccc;

	if (state->current + 1 >= COMPOSITION_MAX)
	{
		goto flush;
	}

	length = codepoint_read(&state->stream_cp[state->current], *state->src, *state->src_size);
	if (*state->src_size > length)
	{
		*state->src += length;
		*state->src_size -= length;
	}
	else
	{
		goto outofinput;
	}

	qc = database_queryproperty(state->stream_cp[state->current], state->property);
	ccc = database_queryproperty(state->stream_cp[state->current], UnicodeProperty_CanonicalCombiningClass);

	state->stream_ccc[state->current] = ccc;
	state->current++;

	if (qc != QuickCheckResult_Yes ||
		ccc > 0)
	{
		if (ccc < state->last_canonical_combining_class)
		{
			state->stable = 0;
		}
	}
	else
	{
		if (state->current > 1)
		{
			goto flush;
		}
	}

	state->last_canonical_combining_class = ccc;
	return 1;

flush:
	state->stage = ComposeStage_Flush;
	return 0;

outofinput:
	state->stage = ComposeStage_OutOfInput;
	return 0;
}