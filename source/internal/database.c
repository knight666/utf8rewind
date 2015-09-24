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

#include "database.h"

#include "../unicodedatabase.h"
#include "codepoint.h"

#define DECOMPOSE_INDEX1_SHIFT (12)
#define DECOMPOSE_INDEX2_SHIFT (5)

static const unicode_t DECOMPOSE_INDEX2_MASK = (1 << DECOMPOSE_INDEX1_SHIFT) - 1;
static const unicode_t DECOMPOSE_DATA_MASK = (1 << DECOMPOSE_INDEX2_SHIFT) - 1;

const char* database_querydecomposition(unicode_t codepoint, uint8_t property)
{
	const DecompositionRecord* record;
	size_t record_count;
	const DecompositionRecord* record_found = 0;
	size_t offset_start;
	size_t offset_end;
	size_t offset_pivot;
	size_t i;

	switch (property)
	{

	case UnicodeProperty_Normalization_Decompose:
		record = UnicodeNFDRecordPtr;
		record_count = UnicodeNFDRecordCount;
		break;

	case UnicodeProperty_Normalization_Compatibility_Decompose:
		record = UnicodeNFKDRecordPtr;
		record_count = UnicodeNFKDRecordCount;
		break;

	case UnicodeProperty_Uppercase:
		record = UnicodeUppercaseRecordPtr;
		record_count = UnicodeUppercaseRecordCount;
		break;

	case UnicodeProperty_Lowercase:
		record = UnicodeLowercaseRecordPtr;
		record_count = UnicodeLowercaseRecordCount;
		break;

	case UnicodeProperty_Titlecase:
		record = UnicodeTitlecaseRecordPtr;
		record_count = UnicodeTitlecaseRecordCount;
		break;

	default:
		return 0;

	}

	offset_start = 0;
	offset_end = record_count - 1;

	if (codepoint < record[offset_start].codepoint ||
		codepoint > record[offset_end].codepoint)
	{
		return 0;
	}

	do
	{
		offset_pivot = offset_start + ((offset_end - offset_start) / 2);

		if (codepoint == record[offset_start].codepoint)
		{
			record_found = &record[offset_start];
			goto found;
		}
		else if (codepoint == record[offset_end].codepoint)
		{
			record_found = &record[offset_end];
			goto found;
		}
		else if (codepoint == record[offset_pivot].codepoint)
		{
			record_found = &record[offset_pivot];
			goto found;
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
			record_found = &record[i];
			goto found;
		}
	}

	return 0;

found:
	if (record_found->offset == 0 ||
		record_found->offset >= DecompositionDataLength)
	{
		return 0;
	}

	return DecompositionData + record_found->offset;
}

uint8_t database_querydecomposition2(
	char** target, size_t* targetSize,
	unicode_t codepoint,
	const uint32_t* index1Array, const uint32_t* index2Array, const uint32_t* dataArray)
{
	size_t length;
	size_t index = index2Array[
		index1Array[codepoint >> DECOMPOSE_INDEX1_SHIFT] +
		((codepoint & DECOMPOSE_INDEX2_MASK) >> DECOMPOSE_INDEX2_SHIFT)] +
			(codepoint & DECOMPOSE_DATA_MASK);

	if (index == 0 ||
		dataArray[index] == 0)
	{
		return codepoint_write(codepoint, target, targetSize);
	}

	length = (dataArray[index] & 0xFF000000) >> 24;

	if (*target != 0)
	{
		if (length > *targetSize)
		{
			return 0;
		}

		memcpy(*target, CompressedStringData + (dataArray[index] & 0x00FFFFFF), length);

		*target += length;
		*targetSize -= length;
	}

	return (uint8_t)length;
}

unicode_t database_querycomposition(unicode_t left, unicode_t right)
{
	uint64_t key = ((uint64_t)left << 32) + (uint64_t)right;
	size_t offset_start = 0;
	size_t offset_end = UnicodeCompositionRecordCount - 1;
	size_t offset_pivot;
	size_t i;

	if (key < UnicodeCompositionRecordPtr[offset_start].key ||
		key > UnicodeCompositionRecordPtr[offset_end].key)
	{
		return 0;
	}

	do
	{
		offset_pivot = offset_start + ((offset_end - offset_start) / 2);

		if (key == UnicodeCompositionRecordPtr[offset_start].key)
		{
			return UnicodeCompositionRecordPtr[offset_start].value;
		}
		else if (key == UnicodeCompositionRecordPtr[offset_end].key)
		{
			return UnicodeCompositionRecordPtr[offset_end].value;
		}
		else if (key == UnicodeCompositionRecordPtr[offset_pivot].key)
		{
			return UnicodeCompositionRecordPtr[offset_pivot].value;
		}
		else
		{
			if (key > UnicodeCompositionRecordPtr[offset_pivot].key)
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
		if (key == UnicodeCompositionRecordPtr[i].key)
		{
			return UnicodeCompositionRecordPtr[i].value;
		}
	}

	return 0;
}