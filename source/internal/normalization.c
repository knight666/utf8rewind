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

#include "normalization.h"

#include "codepoint.h"
#include "composition.h"
#include "database.h"

size_t normalize_decomposition(const char* input, size_t inputSize, char* target, size_t targetSize, uint8_t propertyType, int32_t* errors)
{
	size_t bytes_written = 0;
	const char* src = input;
	size_t src_size = inputSize;
	char* dst = target;
	size_t dst_size = targetSize;

	if (src == 0 ||
		src_size == 0)
	{
		goto invaliddata;
	}

	while (src_size > 0)
	{
		if ((*src & 0x80) == 0)
		{
			/* Basic Latin codepoints are already decomposed */

			if (dst != 0)
			{
				if (dst_size < 1)
				{
					goto outofspace;
				}

				*dst = *src;

				dst++;
				dst_size--;
			}

			bytes_written++;

			src++;
			src_size--;
		}
		else
		{
			size_t resolved_size = 0;
			unicode_t codepoint;
			size_t codepoint_length = codepoint_read(&codepoint, src, src_size);

			if (database_queryproperty(codepoint, propertyType) == QuickCheckResult_No)
			{
				if (codepoint >= HANGUL_S_FIRST &&
					codepoint <= HANGUL_S_LAST)
				{
					/*
					Hangul decomposition

					Algorithm adapted from Unicode Technical Report #15:
					http://www.unicode.org/reports/tr15/tr15-18.html#Hangul
					*/

					unicode_t s_index = codepoint - HANGUL_S_FIRST;
					unicode_t l = HANGUL_L_FIRST + (s_index / HANGUL_N_COUNT);
					unicode_t v = HANGUL_V_FIRST + (s_index % HANGUL_N_COUNT) / HANGUL_T_COUNT;
					unicode_t t = HANGUL_T_FIRST + (s_index % HANGUL_T_COUNT);

					/* Hangul syllables are always three bytes */
					resolved_size = (t != HANGUL_T_FIRST) ? 9 : 6;

					if (dst != 0 &&
						dst_size < resolved_size)
					{
						goto outofspace;
					}

					codepoint_write(l, &dst, &dst_size, errors);
					codepoint_write(v, &dst, &dst_size, errors);
					if (t != HANGUL_T_FIRST)
					{
						codepoint_write(t, &dst, &dst_size, errors);
					}
				}
				else
				{
					const char* resolved = database_querydecomposition(codepoint, propertyType);
					if (resolved != 0)
					{
						resolved_size = strlen(resolved);

						if (dst != 0 &&
							resolved_size > 0)
						{
							if (dst_size < resolved_size)
							{
								goto outofspace;
							}

							memcpy(dst, resolved, resolved_size);

							dst += resolved_size;
							dst_size -= resolved_size;
						}
					}
					else
					{
						resolved_size = codepoint_write(codepoint, &dst, &dst_size, errors);
					}
				}
			}
			else
			{
				resolved_size = codepoint_write(codepoint, &dst, &dst_size, errors);
			}

			if (resolved_size == 0)
			{
				break;
			}
			bytes_written += resolved_size;

			src += codepoint_length;
			src_size -= codepoint_length;
		}
	}

	return bytes_written;

invaliddata:
	if (errors != 0)
	{
		*errors = UTF8_ERR_INVALID_DATA;
	}
	return bytes_written;

outofspace:
	if (errors != 0)
	{
		*errors = UTF8_ERR_NOT_ENOUGH_SPACE;
	}
	return bytes_written;
}

size_t normalize_composition(const char* input, size_t inputSize, char* target, size_t targetSize, uint8_t propertyType, int32_t* errors)
{
	size_t bytes_written = 0;
	const char* src = input;
	size_t src_size = inputSize;
	char* dst = target;
	size_t dst_size = targetSize;
	ComposeState state;

	if (src == 0 ||
		src_size == 0)
	{
		goto invaliddata;
	}

	compose_initialize(&state, &src, &src_size, propertyType);

	while (state.stage < ComposeStage_OutOfInput)
	{
		uint8_t index;
		size_t written;

		index = compose_execute(&state);

		//if (state.stage == ComposeStage_Flush)
		{
			uint8_t i;
			for (i = 0; i < state.stream_flush_count; ++i)
			{
				written = codepoint_write(state.stream_flush[i], &dst, &dst_size, errors);
				if (written == 0)
				{
					return bytes_written;
				}
				bytes_written += written;
			}
		}
#if 0
		if (state.stage == ComposeStage_Flush)
		{
			uint8_t last_combining_class = 0;
			uint8_t i = 0;

			/* Reorder */

			for ( ; i < state.current; ++i)
			{
				uint8_t combining_class = database_queryproperty(state.stream_cp[i], UnicodeProperty_CanonicalCombiningClass);
				if (combining_class > last_combining_class &&
					last_combining_class != 0)
				{
					unicode_t swap = state.stream_cp[i];
					state.stream_cp[i] = state.stream_cp[i - 1];
					state.stream_cp[i - 1] = swap;

					last_combining_class = 0;
					i = 0;
				}
			}

			/* Write */

			for ( ; i < state.current; ++i)
			{
			}
		}

		if (index != (uint8_t)-1)
		{
			if (dst != 0 &&
				dst_size < state.length[index])
			{
				goto outofspace;
			}

			written = codepoint_write(state.codepoint[index], &dst, &dst_size, errors);
			if (written == 0)
			{
				break;
			}
			bytes_written += written;
		}
#endif
	}

	return bytes_written;

invaliddata:
	if (errors != 0)
	{
		*errors = UTF8_ERR_INVALID_DATA;
	}
	return bytes_written;

outofspace:
	if (errors != 0)
	{
		*errors = UTF8_ERR_NOT_ENOUGH_SPACE;
	}
	return bytes_written;
}
