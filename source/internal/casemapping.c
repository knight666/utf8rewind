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

#include "casemapping.h"

#include "codepoint.h"
#include "database.h"

uint8_t casemapping_initialize(CaseMappingState* state, const char* input, size_t inputSize, char* target, size_t targetSize, uint8_t property)
{
	memset(state, 0, sizeof(CaseMappingState));

	if (input == 0 ||
		inputSize == 0)
	{
		return 0;
	}

	state->src = input;
	state->src_size = inputSize;
	state->dst = target;
	state->dst_size = targetSize;
	state->property = property;

	return 1;
}

size_t casemapping_execute2(CaseMappingState* state)
{
	size_t written = 0;

	if ((*state->src & 0x80) == 0)
	{
		/* Basic Latin does not have to be converted to UTF-32 */

		if (state->dst != 0)
		{
			if (state->dst_size < 1)
			{
				return 0;
			}

			/* Lowercase letters are U+0061 ('a') to U+007A ('z') */
			/* Uppercase letters are U+0041 ('A') to U+005A ('Z') */
			/* All other codepoints in Basic Latin are unaffected by case mapping */

			if (state->property == UnicodeProperty_Lowercase)
			{
				*state->dst =
					(*state->src >= 0x41 && *state->src <= 0x5A)
					? *state->src + 0x20
					: *state->src;
			}
			else
			{
				*state->dst =
					(*state->src >= 0x61 && *state->src <= 0x7A)
					? *state->src - 0x20
					: *state->src;
			}

			state->dst++;
			state->dst_size--;
		}

		/* Store codepoint's general category */

		if ((*state->src >= 0x41 && *state->src <= 0x5A) ||
			(*state->src >= 0x61 && *state->src <= 0x7A))
		{
			state->last_general_category = GeneralCategory_Letter | GeneralCategory_CaseMapped;
		}
		else
		{
			/* Don't care about non-letter codepoints */

			state->last_general_category = GeneralCategory_Other;
		}

		written++;

		state->src++;
		state->src_size--;
	}
	else
	{
		unicode_t decoded;
		size_t resolved_size = 0;
		uint8_t decoded_size;

		/* Decode current codepoint */

		decoded_size = codepoint_read(state->src, state->src_size, &decoded);

		/* Check if the codepoint's general category property indicates case mapping */

		state->last_general_category = database_queryproperty(decoded, UnicodeProperty_GeneralCategory);
		if ((state->last_general_category & GeneralCategory_CaseMapped) != 0)
		{
			/* Resolve the codepoint's decomposition */

			const char* resolved = database_querydecomposition(decoded, state->property);
			if (resolved != 0)
			{
				resolved_size = strlen(resolved);

				/* Copy the decomposition to the output buffer */

				if (state->dst != 0 &&
					resolved_size > 0)
				{
					if (state->dst_size < resolved_size)
					{
						return 0;
					}

					memcpy(state->dst, resolved, resolved_size);

					state->dst += resolved_size;
					state->dst_size -= resolved_size;
				}

				written += resolved_size;
			}
		}

		/* Check if codepoint was unaffected */

		if (resolved_size == 0)
		{
			/* Write the codepoint to the output buffer */
			/* This ensures that invalid codepoints in the input are always converted to U+FFFD in the output */

			if (!codepoint_write(decoded, &state->dst, &state->dst_size))
			{
				return 0;
			}

			/* Reuse the decoded size unless the codepoint was replaced */

			written += (decoded != REPLACEMENT_CHARACTER) ? decoded_size : 3;
		}

		/* Invalid codepoints can be longer than the source length indicates */

		if (state->src_size <= decoded_size)
		{
			state->src_size = 0;

			return written;
		}

		state->src += decoded_size;
		state->src_size -= decoded_size;
	}

	return written;
}

size_t casemapping_execute(unicode_t codepoint, char** target, size_t* targetSize, uint8_t generalCategory, uint8_t propertyType, int32_t* errors)
{
	const char* resolved;
	uint8_t encoded_size;
	size_t resolved_size;

	if (codepoint <= 0x7A)
	{
		/* Basic Latin */

		if (*target != 0 &&
			*targetSize < 1)
		{
			goto outofspace;
		}

		if (propertyType == UnicodeProperty_Lowercase)
		{
			if (codepoint >= 0x41 && codepoint <= 0x5A)
			{
				codepoint += 0x20;
			}
		}
		else
		{
			if (codepoint >= 0x61)
			{
				codepoint -= 0x20;
			}
		}

		goto unresolved;
	}
	
	if ((generalCategory & GeneralCategory_CaseMapped) == 0)
	{
		goto unresolved;
	}

	resolved = database_querydecomposition(codepoint, propertyType);
	if (resolved == 0)
	{
		goto unresolved;
	}

	resolved_size = strlen(resolved);

	if (*target != 0 &&
		resolved_size > 0)
	{
		if (*targetSize < resolved_size)
		{
			goto outofspace;
		}

		memcpy(*target, resolved, resolved_size);

		*target += resolved_size;
		*targetSize -= resolved_size;
	}

	return resolved_size;

unresolved:
	encoded_size = codepoint_write(codepoint, target, targetSize);
	if (encoded_size == 0)
	{
		goto outofspace;
	}

	return encoded_size;

outofspace:
	if (errors != 0)
	{
		*errors = UTF8_ERR_NOT_ENOUGH_SPACE;
	}
	return 0;
}