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

#ifndef _UTF8NORMALIZATION_H_
#define _UTF8NORMALIZATION_H_

#include "utf8rewind.h"

typedef struct {
	unicode_t start;
	unicode_t end;
	unicode_t count_and_value;
} QuickCheckRecord;

typedef struct {
	unicode_t codepoint;
	size_t offset;
} DecompositionRecord;

typedef struct {
	uint64_t key;
	unicode_t value;
} CompositionRecord;

enum QuickCheckResult
{
	QuickCheckResult_Yes,
	QuickCheckResult_Maybe,
	QuickCheckResult_No,
	QuickCheckResult_Invalid
};

enum NormalizationForm
{
	NormalizationForm_Composed,
	NormalizationForm_Decomposed,
	NormalizationForm_Compatibility_Composed,
	NormalizationForm_Compatibility_Decomposed,
};

enum FindResult
{
	FindResult_Found,
	FindResult_Missing,
	FindResult_OutOfBounds,
	FindResult_Invalid
};

enum DecompositionQuery
{
	DecompositionQuery_Decomposed,
	DecompositionQuery_Compatibility_Decomposed,
	DecompositionQuery_Uppercase,
	DecompositionQuery_Lowercase,
	DecompositionQuery_Titlecase,
};

uint8_t quickcheck(unicode_t codepoint, uint8_t normalizationForm);

const char* finddecomposition(unicode_t codepoint, int8_t query, int32_t* result);

unicode_t querycomposition(unicode_t left, unicode_t right, int32_t* result);

#endif