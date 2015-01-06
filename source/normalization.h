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

#define UTF8_INVALID_PROPERTY (uint8_t)-1

enum QuickCheckResult
{
	QuickCheckResult_Yes,
	QuickCheckResult_Maybe,
	QuickCheckResult_No,
};

enum UnicodeProperty
{
	UnicodeProperty_Normalization_Compose,
	UnicodeProperty_Normalization_Decompose,
	UnicodeProperty_Normalization_Compatibility_Compose,
	UnicodeProperty_Normalization_Compatibility_Decompose,
	UnicodeProperty_Uppercase,
	UnicodeProperty_Lowercase,
	UnicodeProperty_Titlecase,
};

enum FindResult
{
	FindResult_Found,
	FindResult_Missing,
	FindResult_OutOfBounds,
	FindResult_Invalid
};

uint8_t queryproperty(unicode_t codepoint, uint8_t property);

const char* finddecomposition(unicode_t codepoint, uint8_t property, int32_t* result);

unicode_t querycomposition(unicode_t left, unicode_t right, int32_t* result);

#endif