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

#ifndef _UTF8REWIND_INTERNAL_BASE_H_
#define _UTF8REWIND_INTERNAL_BASE_H_

/*!
	\file
	\brief Base header for internal interface.

	\cond INTERNAL
*/

#include "utf8rewind.h"

#if defined(__GNUC__) && !defined(COMPILER_ICC)
	#define UTF8_UNUSED(_parameter) _parameter __attribute__ ((unused))
#else
	#define UTF8_UNUSED(_parameter) _parameter
#endif

/* Validates input before transforming */
/* Check for parameter overlap using the separating axis theorem */

#define UTF8_VALIDATE_PARAMETERS(_inputType, _targetType) \
	if (input == 0 || inputSize < sizeof(_inputType)) { goto invaliddata; } \
	if (target != 0 && targetSize < sizeof(_targetType)) { goto outofspace; } \
	if ((char*)input == (char*)target) { goto overlap; } \
	{ \
		char* input_center = (char*)input + (inputSize / 2); \
		char* target_center = (char*)target + (targetSize / 2); \
		size_t delta = (size_t)((input_center > target_center) ? (input_center - target_center) : (target_center - input_center)); \
		if (delta < (inputSize + targetSize) / 2) { goto overlap; } \
	}

/*! \endcond */

#endif /* _UTF8REWIND_INTERNAL_BASE_H_ */