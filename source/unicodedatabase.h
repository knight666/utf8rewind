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

#ifndef _UTFREWIND_UNICODEDATABASE_H_
#define _UTFREWIND_UNICODEDATABASE_H_

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

extern const size_t UnicodeQuickCheckNFCRecordCount;
extern const QuickCheckRecord* UnicodeQuickCheckNFCRecordPtr;

extern const size_t UnicodeQuickCheckNFDRecordCount;
extern const QuickCheckRecord* UnicodeQuickCheckNFDRecordPtr;

extern const size_t UnicodeQuickCheckNFKCRecordCount;
extern const QuickCheckRecord* UnicodeQuickCheckNFKCRecordPtr;

extern const size_t UnicodeQuickCheckNFKDRecordCount;
extern const QuickCheckRecord* UnicodeQuickCheckNFKDRecordPtr;

extern const size_t UnicodeQuickCheckGeneralCategoryRecordCount;
extern const QuickCheckRecord* UnicodeQuickCheckGeneralCategoryRecordPtr;

extern const size_t UnicodeQuickCheckUppercaseRecordCount;
extern const QuickCheckRecord* UnicodeQuickCheckUppercaseRecordPtr;

extern const size_t UnicodeQuickCheckLowercaseRecordCount;
extern const QuickCheckRecord* UnicodeQuickCheckLowercaseRecordPtr;

extern const size_t UnicodeQuickCheckTitlecaseRecordCount;
extern const QuickCheckRecord* UnicodeQuickCheckTitlecaseRecordPtr;

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

#endif