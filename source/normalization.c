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

extern const size_t UnicodeNFDRecordCount;
extern const DecompositionRecord* UnicodeNFDRecordPtr;

extern const size_t UnicodeNFKDRecordCount;
extern const DecompositionRecord* UnicodeNFKDRecordPtr;

extern const size_t UnicodeUppercaseRecordCount;
extern const DecompositionRecord* UnicodeUppercaseRecordPtr;

extern const size_t UnicodeLowercaseRecordCount;
extern const DecompositionRecord* UnicodeLowercaseRecordPtr;

extern const size_t UnicodeTitlecaseRecordCount;
extern const DecompositionRecord* UnicodeTitlecaseRecordPtr;

extern const size_t UnicodeCompositionRecordCount;
extern const CompositionRecord* UnicodeCompositionRecordPtr;

extern const char* DecompositionData;
extern const size_t DecompositionDataLength;

const DecompositionRecord* finddecomposition(unicode_t codepoint, int8_t query, int32_t* result)
{
	size_t offset_start;
	size_t offset_end;
	size_t offset_pivot;
	const DecompositionRecord* record;
	size_t record_count;
	size_t i;

	if (result == 0)
	{
		return 0;
	}

	if (query == DecompositionQuery_Decomposed)
	{
		record = UnicodeNFDRecordPtr;
		record_count = UnicodeNFDRecordCount;
	}
	else if (query == DecompositionQuery_Compatibility_Decomposed)
	{
		record = UnicodeNFKDRecordPtr;
		record_count = UnicodeNFKDRecordCount;
	}
	else if (query == DecompositionQuery_Uppercase)
	{
		record = UnicodeUppercaseRecordPtr;
		record_count = UnicodeUppercaseRecordCount;
	}
	else if (query == DecompositionQuery_Lowercase)
	{
		record = UnicodeLowercaseRecordPtr;
		record_count = UnicodeLowercaseRecordCount;
	}
	else if (query == DecompositionQuery_Titlecase)
	{
		record = UnicodeTitlecaseRecordPtr;
		record_count = UnicodeTitlecaseRecordCount;
	}
	else
	{
		*result = FindResult_Invalid;
		return 0;
	}

	offset_start = 0;
	offset_end = record_count - 1;

	if (codepoint < record[offset_start].codepoint ||
		codepoint > record[offset_end].codepoint)
	{
		*result = FindResult_OutOfBounds;
		return 0;
	}

	do
	{
		offset_pivot = offset_start + ((offset_end - offset_start) / 2);

		if (codepoint == record[offset_start].codepoint)
		{
			*result = FindResult_Found;
			return &record[offset_start];
		}
		else if (codepoint == record[offset_end].codepoint)
		{
			*result = FindResult_Found;
			return &record[offset_end];
		}
		else if (codepoint == record[offset_pivot].codepoint)
		{
			*result = FindResult_Found;
			return &record[offset_pivot];
		}
		else
		{
			if (codepoint > record[offset_pivot].codepoint)
			{
				offset_start = offset_pivot;
			}
			else
			{
				offset_end = offset_pivot;
			}
		}
	}
	while (offset_end - offset_start > 32);

	for (i = offset_start; i <= offset_end; ++i)
	{
		if (codepoint == record[i].codepoint)
		{
			*result = FindResult_Found;
			return &record[i];
		}
	}

	*result = FindResult_Missing;
	return 0;
}

const char* resolvedecomposition(size_t offset, int32_t* result)
{
	if (result == 0)
	{
		return 0;
	}

	if (offset >= DecompositionDataLength)
	{
		*result = FindResult_OutOfBounds;
		return 0;
	}

	*result = FindResult_Found;
	return DecompositionData + offset;
}

unicode_t querycomposition(unicode_t left, unicode_t right, int32_t* result)
{
	uint64_t key = ((uint64_t)left << 32) + (uint64_t)right;
	size_t offset_start;
	size_t offset_end;
	size_t offset_pivot;
	const CompositionRecord* record = UnicodeCompositionRecordPtr;
	size_t record_count = UnicodeCompositionRecordCount;
	size_t i;

	if (result == 0)
	{
		return 0;
	}

	offset_start = 0;
	offset_end = record_count - 1;

	if (key < record[offset_start].key ||
		key > record[offset_end].key)
	{
		*result = FindResult_OutOfBounds;
		return 0;
	}

	do
	{
		offset_pivot = offset_start + ((offset_end - offset_start) / 2);

		if (key == record[offset_start].key)
		{
			*result = FindResult_Found;
			return record[offset_start].value;
		}
		else if (key == record[offset_end].key)
		{
			*result = FindResult_Found;
			return record[offset_end].value;
		}
		else if (key == record[offset_pivot].key)
		{
			*result = FindResult_Found;
			return record[offset_pivot].value;
		}
		else
		{
			if (key > record[offset_pivot].key)
			{
				offset_start = offset_pivot;
			}
			else
			{
				offset_end = offset_pivot;
			}
		}
	}
	while (offset_end - offset_start > 32);

	for (i = offset_start; i <= offset_end; ++i)
	{
		if (key == record[i].key)
		{
			*result = FindResult_Found;
			return record[i].value;
		}
	}

	*result = FindResult_Missing;
	return 0;
}