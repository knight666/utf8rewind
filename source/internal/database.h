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

#ifndef _UTF8REWIND_INTERNAL_DATABASE_H_
#define _UTF8REWIND_INTERNAL_DATABASE_H_

/*!
	\file
	\brief Database interface.

	\cond INTERNAL
*/

#include "utf8rewind.h"

#define UTF8_INVALID_PROPERTY (uint8_t)-1

enum UnicodeProperty
{
	UnicodeProperty_GeneralCategory,
	UnicodeProperty_CanonicalCombiningClass,
	UnicodeProperty_Normalization_Compose,
	UnicodeProperty_Normalization_Decompose,
	UnicodeProperty_Normalization_Compatibility_Compose,
	UnicodeProperty_Normalization_Compatibility_Decompose,
	UnicodeProperty_Uppercase,
	UnicodeProperty_Lowercase,
	UnicodeProperty_Titlecase,
};

enum GeneralCategory
{
	GeneralCategory_Letter = 0x01,
	GeneralCategory_CaseMapped = 0x02,
	GeneralCategory_Mark = 0x04,
	GeneralCategory_Number = 0x08,
	GeneralCategory_Punctuation = 0x10,
	GeneralCategory_Symbol = 0x20,
	GeneralCategory_Separator = 0x40,
	GeneralCategory_Other = 0x80,
};

enum QuickCheckResult
{
	QuickCheckResult_Yes,
	QuickCheckResult_Maybe,
	QuickCheckResult_No,
};

const char* database_querydecomposition(unicode_t codepoint, uint8_t property);

unicode_t database_querycomposition(unicode_t left, unicode_t right);

/*! \endcond */

#endif /* _UTF8REWIND_INTERNAL_DATABASE_H_ */