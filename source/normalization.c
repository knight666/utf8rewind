/*
	Copyright (C) 2014 Quinten Lansu

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

extern const size_t CompositionDataCount;
extern const CompositionEntry* CompositionDataPtr;

extern const size_t DecompositionDataPageCount;
extern const char** DecompositionDataPtr;
extern const size_t* DecompositionDataLengthPtr;

const CompositionEntry* findcomposition(unicode_t codepoint, int32_t* result)
{
	size_t offset_start = 0;
	size_t offset_end = CompositionDataCount - 1;
	size_t offset_center;
	const CompositionEntry* src;
	size_t i;

	if (result == 0)
	{
		return 0;
	}

	if (codepoint >= 0x10FFFF)
	{
		*result = FindResult_Invalid;
		return 0;
	}
	else if (
		codepoint < CompositionDataPtr[0].codepoint ||
		codepoint > CompositionDataPtr[CompositionDataCount - 1].codepoint)
	{
		*result = FindResult_OutOfBounds;
		return 0;
	}

	do 
	{
		offset_center = offset_start + ((offset_end - offset_start) / 2);

		if (codepoint == CompositionDataPtr[offset_start].codepoint)
		{
			*result = FindResult_Found;
			return &CompositionDataPtr[offset_start];
		}
		else if (codepoint == CompositionDataPtr[offset_end].codepoint)
		{
			*result = FindResult_Found;
			return &CompositionDataPtr[offset_end];
		}
		else if (codepoint == CompositionDataPtr[offset_center].codepoint)
		{
			*result = FindResult_Found;
			return &CompositionDataPtr[offset_center];
		}
		else if (offset_end - offset_start <= 32)
		{
			src = &CompositionDataPtr[offset_start];
			for (i = offset_start; i < offset_end; ++i)
			{
				if (src->codepoint == codepoint)
				{
					*result = FindResult_Found;
					return src;
				}

				src++;
			}

			break;
		}
		else
		{
			if (codepoint >= CompositionDataPtr[offset_center].codepoint)
			{
				offset_start = offset_center;
			}
			else
			{
				offset_end = offset_center;
			}
		}
	}
	while (offset_start != offset_end);

	*result = FindResult_Missing;
	return 0;
}

const char* resolvedecomposition(size_t offset, int32_t* result)
{
	size_t pageIndex;

	if (result == 0)
	{
		return 0;
	}

	pageIndex = (offset & 0xFF000000) >> 24;

	if (pageIndex >= DecompositionDataPageCount)
	{
		*result = FindResult_Invalid;
		return 0;
	}

	offset &= 0x00FFFFFF;

	if (offset >= DecompositionDataLengthPtr[pageIndex])
	{
		*result = FindResult_OutOfBounds;
		return 0;
	}
	
	*result = FindResult_Found;
	return DecompositionDataPtr[pageIndex] + offset;
}
