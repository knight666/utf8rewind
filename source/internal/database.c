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

#include "database.h"

#include "../unicodedatabase.h"

uint8_t queryproperty(unicode_t codepoint, uint8_t checkType)
{
	const QuickCheckRecord* record;
	size_t record_count;
	const QuickCheckRecord* record_found = 0;
	size_t offset_start;
	size_t offset_end;
	size_t offset_pivot;
	size_t i;

	switch (checkType)
	{

	case UnicodeProperty_Normalization_Compose:
		record = UnicodeQuickCheckNFCRecordPtr;
		record_count = UnicodeQuickCheckNFCRecordCount;
		break;

	case UnicodeProperty_Normalization_Decompose:
		record = UnicodeQuickCheckNFDRecordPtr;
		record_count = UnicodeQuickCheckNFDRecordCount;
		break;

	case UnicodeProperty_Normalization_Compatibility_Compose:
		record = UnicodeQuickCheckNFKCRecordPtr;
		record_count = UnicodeQuickCheckNFKCRecordCount;
		break;

	case UnicodeProperty_Normalization_Compatibility_Decompose:
		record = UnicodeQuickCheckNFKDRecordPtr;
		record_count = UnicodeQuickCheckNFKDRecordCount;
		break;

	case UnicodeProperty_Uppercase:
		record = UnicodeQuickCheckUppercaseRecordPtr;
		record_count = UnicodeQuickCheckUppercaseRecordCount;
		break;

	case UnicodeProperty_Lowercase:
		record = UnicodeQuickCheckLowercaseRecordPtr;
		record_count = UnicodeQuickCheckLowercaseRecordCount;
		break;

	case UnicodeProperty_Titlecase:
		record = UnicodeQuickCheckTitlecaseRecordPtr;
		record_count = UnicodeQuickCheckTitlecaseRecordCount;
		break;

	default:
		return UTF8_INVALID_PROPERTY;

	}

	offset_start = 0;
	offset_end = record_count - 1;

	if (codepoint < record[offset_start].start ||
		codepoint > record[offset_end].end)
	{
		return 0;
	}

	do
	{
		offset_pivot = offset_start + ((offset_end - offset_start) / 2);

		if (codepoint >= record[offset_start].start &&
			codepoint <= record[offset_start].end)
		{
			record_found = &record[offset_start]; 
			goto found;
		}
		else if (
			codepoint >= record[offset_end].start &&
			codepoint <= record[offset_end].end)
		{
			record_found = &record[offset_end]; 
			goto found;
		}
		else if (
			codepoint >= record[offset_pivot].start &&
			codepoint <= record[offset_pivot].end)
		{
			record_found = &record[offset_pivot]; 
			goto found;
		}
		else
		{
			if (codepoint > record[offset_pivot].end)
			{
				offset_start = offset_pivot;
			}
			else
			{
				offset_end = offset_pivot;
			}
		}
	}
	while (offset_end - offset_start > 8);

	for (i = offset_start; i <= offset_end; ++i)
	{
		if (codepoint >= record[i].start &&
			codepoint <= record[i].end)
		{
			record_found = &record[i];
			goto found;
		}
	}

	return 0;

found:
	if (codepoint <= (record_found->start + (record_found->count_and_value & 0x00FFFFFF)))
	{
		return ((record_found->count_and_value & 0xFF000000) >> 24);
	}
	else
	{
		return 0;
	}
}

const char* finddecomposition(unicode_t codepoint, uint8_t property, int32_t* result)
{
	const DecompositionRecord* record;
	size_t record_count;
	const DecompositionRecord* record_found = 0;
	size_t offset_start;
	size_t offset_end;
	size_t offset_pivot;
	size_t i;

	if (result == 0)
	{
		return 0;
	}

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

	*result = FindResult_Missing;
	return 0;

found:
	if (record_found->offset == 0 ||
		record_found->offset >= DecompositionDataLength)
	{
		*result = FindResult_OutOfBounds;
		return 0;
	}
	else
	{
		*result = FindResult_Found;
		return DecompositionData + record_found->offset;
	}
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