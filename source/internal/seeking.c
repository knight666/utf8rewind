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

#include "seeking.h"

#include "codepoint.h"

const char* seeking_forward(const char* src, const char* srcEnd, size_t srcLength, off_t offset)
{
	uint8_t current;
	uint8_t codepoint_length;

	if (srcEnd <= src ||
		offset <= 0 ||
		srcLength == 0)
	{
		return src;
	}

	if (offset >= (off_t)srcLength)
	{
		return srcEnd;
	}

	do
	{
		current = (uint8_t)*src;

		src++;
		if (--srcLength == 0)
		{
			/* End of data, codepoint may be invalid */

			break;
		}

		/* First byte determines encoded length */

		codepoint_length = codepoint_decoded_length[current];

		if (codepoint_length > 1 &&
			codepoint_length < 7)
		{
			/* Check all bytes of multi-byte sequence */

			size_t i;
			for (i = 1; i < codepoint_length; ++i)
			{
				current = (uint8_t)*src;

				if (codepoint_decoded_length[current] != 0)
				{
					/* Not a continuation byte */

					break;
				}

				src++;
				if (--srcLength == 0)
				{
					/* End of data, codepoint may be invalid */

					return src;
				}
			}
		}
	}
	while (--offset > 0);

	return src;
}

const char* seeking_rewind(const char* inputStart, const char* input, size_t inputLength, off_t offset)
{
	const char* src;
	size_t marker_length;
	const char* marker_begin;
	const char* marker_end;
	const char* marker_valid;

	if (inputStart >= input ||  /* Swapped parameters */
		offset >= 0 ||          /* Invalid offset */
		inputLength == 0)       /* Nothing to do */
	{
		return input;
	}
	else if (
		-offset >= (off_t)inputLength)  /* Out of bounds */
	{
		return inputStart;
	}

	/* Ignore NUL codepoint at end of input */
	src = input - 1;

	marker_length = 0;
	marker_begin = src;
	marker_end = src;
	marker_valid = src;

	while (1)
	{
		if (marker_end == marker_valid)
		{
			uint8_t codepoint_length = codepoint_decoded_length[(uint8_t)*src];

			if (codepoint_length > 1)
			{
				marker_valid = marker_begin + codepoint_length - 1;
				if (marker_valid == marker_end)
				{
					if (++offset == 0)
					{
						break;
					}

					if (marker_begin == inputStart)
					{
						break;
					}

					src = marker_begin - 1;
					marker_begin = src;
					marker_end = src;
					marker_valid = src;
				}
				else
				{
					src = marker_end;
				}
			}
			else if (codepoint_length == 1)
			{
				if (src == inputStart)
				{
					marker_valid = marker_begin;
					src = marker_end;
				}
				else if (marker_begin == marker_end)
				{
					src = marker_end;
					marker_valid = marker_end + 1;
				}
				else
				{
					marker_begin = marker_end;
					marker_valid = marker_end + 1;
				}
			}
			else
			{
				if (src == inputStart)
				{
					marker_valid = marker_begin;
					src = marker_end;
				}
				else
				{
					marker_begin = --src;
				}
			}
		}
		else
		{
			if (src > marker_valid)
			{
				if (++offset == 0 ||
					src == inputStart)
				{
					break;
				}

				src--;
			}
			else
			{
				src = marker_begin;

				if (++offset == 0 ||
					src == inputStart)
				{
					break;
				}

				src--;

				marker_begin = src;
				marker_end = src;
				marker_valid = src;
			}
		}
	}

	return src;
}