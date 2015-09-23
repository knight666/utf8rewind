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

#ifndef _UTF8REWIND_COMPRESEDPROPERTIES_H_
#define _UTF8REWIND_COMPRESEDPROPERTIES_H_

/*!
	\file
	\brief Compressed code point properties.

	\cond INTERNAL
*/

#include "utf8rewind.h"

#define PROPERTY_BLOCK_SHIFT (5)
static const unicode_t PROPERTY_INDEX_MASK = (1 << PROPERTY_BLOCK_SHIFT) - 1;

#define PROPERTY_GET(_indexArray, _dataArray, _cp) \
	(_dataArray)[ \
		(_indexArray)[(_cp) >> PROPERTY_BLOCK_SHIFT] + \
		((_cp) & PROPERTY_INDEX_MASK)]

#define PROPERTY_GET_GC(_cp) \
	PROPERTY_GET(GeneralCategoryIndexPtr, GeneralCategoryDataPtr, _cp)

extern const size_t* GeneralCategoryIndexPtr;
extern const uint8_t* GeneralCategoryDataPtr;

#define PROPERTY_GET_CCC(_cp) \
	PROPERTY_GET(CanonicalCombiningClassIndexPtr, CanonicalCombiningClassDataPtr, _cp)

extern const size_t* CanonicalCombiningClassIndexPtr;
extern const uint8_t* CanonicalCombiningClassDataPtr;

#define PROPERTY_GET_NFC(_cp) \
	PROPERTY_GET(QuickCheckNFCIndexPtr, QuickCheckNFCDataPtr, _cp)

extern const size_t* QuickCheckNFCIndexPtr;
extern const uint8_t* QuickCheckNFCDataPtr;

#define PROPERTY_GET_NFD(_cp) \
	PROPERTY_GET(QuickCheckNFDIndexPtr, QuickCheckNFDDataPtr, _cp)

extern const size_t* QuickCheckNFDIndexPtr;
extern const uint8_t* QuickCheckNFDDataPtr;

#define PROPERTY_GET_NFKC(_cp) \
	PROPERTY_GET(QuickCheckNFKCIndexPtr, QuickCheckNFKCDataPtr, _cp)

extern const size_t* QuickCheckNFKCIndexPtr;
extern const uint8_t* QuickCheckNFKCDataPtr;

#define PROPERTY_GET_NFKD(_cp) \
	PROPERTY_GET(QuickCheckNFKDIndexPtr, QuickCheckNFKDDataPtr, _cp)

extern const size_t* QuickCheckNFKDIndexPtr;
extern const uint8_t* QuickCheckNFKDDataPtr;

/*! \endcond */

#endif