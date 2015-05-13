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
		uint8_t current = (uint8_t)*src;
		size_t codepoint_length = codepoint_decoded_length[current];
		
		src++;
		srcLength--;

		if (codepoint_length > 1)
		{
			size_t i;
			for (i = 1; i < codepoint_length && srcLength > 0; ++i)
			{
				current = (uint8_t)*src;

				if (current < 0x80 ||  /* Single byte codepoint */
					current > 0xBF)    /* Start of a new sequence */
				{
					break;
				}

				src++;
				srcLength--;
			}
		}
	}
	while (--offset > 0 && srcLength > 0);

	return src;
}

const char* seeking_rewind(const char* srcStart, const char* src, size_t srcLength, off_t offset)
{
	if (srcStart >= src || offset >= 0 || srcLength == 0)
	{
		return src;
	}
	if (-offset >= (off_t)srcLength)
	{
		return srcStart;
	}

	/* Ignore NUL codepoint */
	src--;

	while (src != srcStart)
	{
		if (
			(*src & 0x80) == 0 ||    /* ASCII */
			(*src & 0xC0) != 0x80 || /* Malformed continuation byte */
			(*src & 0xFE) == 0xFE    /* Illegal byte */
		)
		{
			++offset;
			if (offset == 0)
			{
				break;
			}
		}

		src--;
	}

	return src;
}