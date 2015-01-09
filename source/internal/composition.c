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
	uint8_t i;
	unicode_t composed;
	uint8_t qc[2];
	uint8_t read_index = 1;
	uint8_t write_index = 0;

	if (state->stage == ComposeStage_Flush)
	{
		if (state->current >= 1)
		{
			uint8_t last = state->current;

			state->stream_cp[0] = state->stream_cp[last];
			state->stream_ccc[0] = state->stream_ccc[last];
			state->stream_qc[0] = state->stream_qc[last];

			for (i = 1; i <= state->current; ++i)
			{
				state->stream_cp[i] = 0;
				state->stream_ccc[i] = 0;
				state->stream_qc[i] = 0;
			}

			state->stable = (state->stream_qc[0] == QuickCheckResult_Yes);
			state->current = 1;
		}
		else
		{
			for (i = 0; i <= state->current; ++i)
			{
				state->stream_cp[i] = 0;
				state->stream_ccc[i] = 0;
				state->stream_qc[i] = 0;
			}

			state->stable = 1;
			state->current = 0;
		}

		memset(state->stream_flush, 0, state->stream_flush_count * sizeof(unicode_t));
		state->stream_flush_count = 0;

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

		while (dirty == 1)
		{
			uint8_t last_combining_class = 0;

			dirty = 0;

			for (i = 1; i < state->current; i++)
			{
				if (state->stream_ccc[i] < last_combining_class)
				{
					unicode_t swap_cp = state->stream_cp[i];
					uint8_t swap_qc = state->stream_qc[i];
					uint8_t swap_ccc = state->stream_ccc[i];

					state->stream_cp[i] = state->stream_cp[i - 1];
					state->stream_cp[i - 1] = swap_cp;

					state->stream_qc[i] = state->stream_qc[i - 1];
					state->stream_qc[i - 1] = swap_qc;

					state->stream_ccc[i] = state->stream_ccc[i - 1];
					state->stream_ccc[i - 1] = swap_ccc;

					dirty = 1;
				}

				last_combining_class = state->stream_ccc[i];
			}
		}
	}

	/* Compose */

	composed = state->stream_cp[0];
	qc[0] = state->stream_qc[0];

	for (i = 1; i < state->current; ++i)
	{
		qc[1] = state->stream_qc[read_index];

		while (
			qc[0] != QuickCheckResult_Yes ||
			qc[1] != QuickCheckResult_Yes)
		{
			/* Check database for composition */

			unicode_t local_composed = database_querycomposition(composed, state->stream_cp[read_index]);
			if (local_composed != 0)
			{
				composed = local_composed;
				qc[0] = database_queryproperty(local_composed, state->property);

				read_index++;
				qc[1] = state->stream_qc[read_index];
			}
			else
			{
				break;
			}
		}

		state->stream_flush[state->stream_flush_count++] = composed;

		composed = state->stream_cp[read_index];
		read_index++;

		qc[0] = qc[1];
	}

	if (qc[0] == QuickCheckResult_Maybe)
	{
		state->stream_flush[state->stream_flush_count++] = composed;
	}

	return 1;
}

uint8_t compose_readcodepoint(ComposeState* state)
{
	uint8_t length;
	
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

	state->stream_qc[state->current] = database_queryproperty(state->stream_cp[state->current], state->property);
	state->stream_ccc[state->current] = database_queryproperty(state->stream_cp[state->current], UnicodeProperty_CanonicalCombiningClass);

	state->last_canonical_combining_class = state->stream_ccc[state->current];

	if (state->current > 0)
	{
		uint8_t previous = state->current - 1;

		if (state->stream_qc[previous] != QuickCheckResult_Yes)
		{
			if (state->stream_ccc[previous] > 0)
			{
				state->stable = 0;
			}

			goto flush;
		}
		else
		{
			if (state->stream_ccc[previous] == 0 &&
				previous > 1)
			{
				goto flush;
			}
		}
	}

	state->current++;

	return 1;

flush:
	state->stage = ComposeStage_Flush;
	return 0;

outofinput:
	state->stage = ComposeStage_OutOfInput;
	return 0;
}