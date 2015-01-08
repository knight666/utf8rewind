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

#include "casemapping.h"

#include "codepoint.h"
#include "database.h"

size_t casemapping_execute(unicode_t codepoint, char** target, size_t* targetSize, uint8_t generalCategory, uint8_t propertyType, int32_t* errors)
{
	const char* resolved;
	size_t resolved_size;

	if (codepoint <= 0x7A)
	{
		/* Basic Latin */

		if (*target != 0 &&
			*targetSize < 1)
		{
			goto outofspace;
		}

		if (propertyType == UnicodeProperty_Lowercase)
		{
			if (codepoint >= 0x41 && codepoint <= 0x5A)
			{
				codepoint += 0x20;
			}
		}
		else
		{
			if (codepoint >= 0x61)
			{
				codepoint -= 0x20;
			}
		}

		return codepoint_write(codepoint, target, targetSize, errors);
	}
	
	if ((generalCategory & GeneralCategory_CaseMapped) == 0)
	{
		goto unresolved;
	}

	resolved = database_querydecomposition(codepoint, propertyType);
	if (resolved == 0)
	{
		goto unresolved;
	}

	resolved_size = strlen(resolved);

	if (*target != 0 &&
		resolved_size > 0)
	{
		if (*targetSize < resolved_size)
		{
			goto outofspace;
		}

		memcpy(*target, resolved, resolved_size);

		*target += resolved_size;
		*targetSize -= resolved_size;
	}

	return resolved_size;

unresolved:
	return codepoint_write(codepoint, target, targetSize, errors);

outofspace:
	if (errors != 0)
	{
		*errors = UTF8_ERR_NOT_ENOUGH_SPACE;
	}
	return 0;
}