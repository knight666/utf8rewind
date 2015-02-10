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
			unicode_t decoded;
			uint8_t decoded_size = codepoint_read(src, src_size, &decoded);

			if (database_queryproperty(decoded, propertyType) == QuickCheckResult_No)
			{
				if (decoded >= HANGUL_S_FIRST &&
					decoded <= HANGUL_S_LAST)
				{
					/*
						Hangul decomposition

						Algorithm adapted from Unicode Technical Report #15:
						http://www.unicode.org/reports/tr15/tr15-18.html#Hangul
					*/

					unicode_t s_index = decoded - HANGUL_S_FIRST;
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

					codepoint_write(l, &dst, &dst_size);
					codepoint_write(v, &dst, &dst_size);
					if (t != HANGUL_T_FIRST)
					{
						codepoint_write(t, &dst, &dst_size);
					}
				}
				else
				{
					const char* resolved = database_querydecomposition(decoded, propertyType);
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
						resolved_size = codepoint_write(decoded, &dst, &dst_size);
					}
				}
			}
			else
			{
				resolved_size = codepoint_write(decoded, &dst, &dst_size);
			}

			if (resolved_size == 0)
			{
				goto outofspace;
			}
			bytes_written += resolved_size;

			src += decoded_size;
			src_size -= decoded_size;
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
	StreamState input_stream;
	StreamState output_stream;
	ComposeState state;

	if (src == 0 ||
		src_size == 0)
	{
		goto invaliddata;
	}

	stream_initialize(&input_stream, input, inputSize);

	compose_initialize(&state, &input_stream, &output_stream, 0);

	while (state.finished == 0)
	{
		unicode_t result = compose_execute(&state);
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
