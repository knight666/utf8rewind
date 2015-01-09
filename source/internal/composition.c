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
	memset(state, 0, sizeof(ComposeState));

	state->src = input;
	state->src_size = inputSize;
	state->property = propertyType;

	state->check[0] = QuickCheckResult_No;
	state->check[1] = QuickCheckResult_No;

	state->current = 0;
	state->next = 1;

	/* Read the first codepoint */

	state->length[0] = codepoint_read(&state->codepoint[0], *state->src, *state->src_size);
	state->check[0] = database_queryproperty(state->codepoint[0], state->property);

	if (*state->src_size > state->length[0])
	{
		*state->src += state->length[0];
		*state->src_size -= state->length[0];
	}
	else
	{
		state->stage = ComposeStage_OutOfInput;
	}

	return 1;
}

uint8_t compose_execute(ComposeState* state)
{
	if (state->stage >= ComposeStage_OutOfInput)
	{
		state->stage = ComposeStage_WriteLast;

		return (state->check[state->current] != QuickCheckResult_No) ? state->current : (uint8_t)-1;
	}

	while (state->stage == ComposeStage_Processing)
	{
		unicode_t composed = 0;

		if (*state->src_size > 0)
		{
			state->length[state->next] = codepoint_read(&state->codepoint[state->next], *state->src, *state->src_size);
			state->check[state->next] = database_queryproperty(state->codepoint[state->next], state->property);

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
	}

	/* Swap buffers */

	state->current = (state->current + 1) % 2;
	state->next = (state->next + 1) % 2;

	return state->next;
}

