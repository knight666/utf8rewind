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

#include "utf8rewind.h"

#include "normalization.h"

#define MAX_BASIC_MULTILINGUAR_PLANE  0xFFFF
#define MAX_LEGAL_UNICODE             0x10FFFF
#define REPLACEMENT_CHARACTER         0xFFFD
#define SURROGATE_HIGH_START          0xD800
#define SURROGATE_HIGH_END            0xDBFF
#define SURROGATE_LOW_START           0xDC00
#define SURROGATE_LOW_END             0xDFFF

static const size_t Utf8ByteMinimum[6] = {
	0x00000000,
	0x00000080,
	0x00000800,
	0x00010000,
	0x0010FFFF,
	0x0010FFFF
};

static const size_t Utf8ByteMaximum[6] = {
	0x0000007F,
	0x000007FF,
	0x0000FFFF,
	0x0010FFFF,
	0x0010FFFF,
	0x0010FFFF
};

char* safe_strcpy(char* target, size_t targetSize, const char* input, size_t inputSize)
{
#if WIN32 || _WINDOWS
	strncpy_s(target, targetSize, input, inputSize);
	return target;
#else
	size_t copy_size = (targetSize < inputSize) ? targetSize : inputSize;
	return strncpy(target, input, copy_size);
#endif
}

size_t lengthcodepoint(uint8_t input)
{
	if ((input & 0x80) == 0) /* ASCII */
	{
		return 1;
	}
	else if (
		(input & 0xC0) != 0xC0 || /* Malformed continuation byte */
		(input & 0xFE) == 0xFE    /* Illegal byte */
	)
	{
		return 0;
	}
	else if ((input & 0xE0) == 0xC0)
	{
		return 2;
	}
	else if ((input & 0xF0) == 0xE0)
	{
		return 3;
	}
	else if ((input & 0xF8) == 0xF0)
	{
		return 4;
	}
	else if ((input & 0xFC) == 0xF8)
	{
		return 5;
	}
	else // (input & 0xFE) == 0xFC
	{
		return 6;
	}
}

size_t writecodepoint(unicode_t codepoint, char** dst, size_t* dstSize, int32_t* errors)
{
	char* target;
	size_t encoded_length;

	if (codepoint < 0x80)
	{
		encoded_length = 1;
	}
	else if (codepoint < 0x800)
	{
		encoded_length = 2;
	}
	else if (codepoint < 0x10000)
	{
		encoded_length = 3;
	}
	else if (codepoint <= MAX_LEGAL_UNICODE)
	{
		encoded_length = 4;
	}
	else
	{
		codepoint = REPLACEMENT_CHARACTER;
		encoded_length = 3;
	}

	if (*dst != 0)
	{
		if (*dstSize < encoded_length)
		{
			if (errors != 0)
			{
				*errors = UTF8_ERR_NOT_ENOUGH_SPACE;
			}
			return 0;
		}

		target = *dst;

		switch (encoded_length)
		{

		case 1:
			*target++ = (char)codepoint;
			break;

		case 2:
			*target++ = (char)(codepoint >>   6)         | 0xC0;
			*target++ = (char)(codepoint         & 0x3F) | 0x80;
			break;

		case 3:
			*target++ = (char)(codepoint  >> 12)         | 0xE0;
			*target++ = (char)((codepoint >>  6) & 0x3F) | 0x80;
			*target++ = (char)(codepoint         & 0x3F) | 0x80;
			break;

		case 4:
			*target++ = (char)(codepoint  >> 18)         | 0xF0;
			*target++ = (char)((codepoint >> 12) & 0x3F) | 0x80;
			*target++ = (char)((codepoint >>  6) & 0x3F) | 0x80;
			*target++ = (char)(codepoint         & 0x3F) | 0x80;
			break;

		default:
			break;

		}

		*dst += encoded_length;
		*dstSize -= encoded_length;
	}

	return encoded_length;
}

size_t readcodepoint(unicode_t* codepoint, const char* src, size_t srcSize)
{
	uint8_t current = (uint8_t)*src;
	uint8_t mask;
	size_t decoded_length;
	size_t src_index;
	size_t src_size = srcSize;

	if (current == 0)
	{
		*codepoint = 0;
		return 1;
	}

	decoded_length = lengthcodepoint(current);
	if (decoded_length == 0)
	{
		*codepoint = REPLACEMENT_CHARACTER;
		return 1;
	}

	mask = (decoded_length == 1) ? 0xFF : (1 << (7 - decoded_length)) - 1;

	*codepoint = (unicode_t)(current & mask);
	src++;

	for (src_index = 1; src_index < decoded_length && src_size > 0; ++src_index)
	{
		if ((*src & 0x80) == 0)
		{
			/* Not a continuation byte for a multi-byte sequence */

			*codepoint = REPLACEMENT_CHARACTER;
			return src_index;
		}

		*codepoint = (*codepoint << 6) | (*src & 0x3F);
		src++;
		src_size--;
	}

	if (decoded_length > 1)
	{
		/* Check for overlong sequences */

		if (
			(*codepoint < Utf8ByteMinimum[decoded_length - 1] ||
			*codepoint > Utf8ByteMaximum[decoded_length - 1])
		)
		{
			*codepoint = REPLACEMENT_CHARACTER;
		}

		/* Check for surrogate pairs */

		else if (
			(*codepoint >= SURROGATE_HIGH_START && *codepoint <= SURROGATE_HIGH_END) ||
			(*codepoint >= SURROGATE_LOW_START && *codepoint <= SURROGATE_LOW_END)
		)
		{
			*codepoint = REPLACEMENT_CHARACTER;
		}
	}

	return decoded_length;
}

size_t utf8len(const char* text)
{
	const char* src;
	size_t src_index;
	size_t length = 0;
	size_t codepoint_length = 0;
	size_t text_length = 0;
	uint8_t codepoint = 0;

	if (text == 0)
	{
		return length;
	}

	text_length = strlen(text);
	if (text_length == 0)
	{
		return length;
	}

	while (*text != 0)
	{
		codepoint = (uint8_t)*text;

		if (codepoint == 0)
		{
			break;
		}
		else if (
			(codepoint & 0x80) == 0 ||    /* ASCII */
			(codepoint & 0xC0) != 0xC0 || /* Malformed continuation byte */
			(codepoint & 0xFE) == 0xFE    /* Illegal byte */
		)
		{
			codepoint_length = 1;
		}
		else if ((codepoint & 0xE0) == 0xC0)
		{
			codepoint_length = 2;
		}
		else if ((codepoint & 0xF0) == 0xE0)
		{
			codepoint_length = 3;
		}
		else if ((codepoint & 0xF8) == 0xF0)
		{
			codepoint_length = 4;
		}
		else if ((codepoint & 0xFC) == 0xF8)
		{
			codepoint_length = 5;
		}
		else // (codepoint & 0xFE) == 0xFC
		{
			codepoint_length = 6;
		}

		length++;

		/* Check if encoding is valid */

		src = text + 1;

		for (src_index = 1; src_index < codepoint_length; ++src_index)
		{
			if ((*src & 0x80) == 0)
			{
				/* Not a continuation byte for a multi-byte sequence */

				break;
			}

			src++;
		}

		if (src_index > text_length)
		{
			break;
		}

		text += src_index;
		text_length -= src_index;
	}

	return length;
}

size_t utf16toutf8(const utf16_t* input, size_t inputSize, char* target, size_t targetSize, int32_t* errors)
{
	size_t encoded_length = 0;
	utf16_t surrogate_low;
	utf16_t current;
	unicode_t codepoint;
	const utf16_t* src = (const utf16_t*)input;
	size_t src_size = inputSize;
	char* dst = target;
	size_t dst_size = targetSize;
	size_t bytes_written = 0;

	if (input == 0 || inputSize < sizeof(utf16_t))
	{
		if (errors != 0)
		{
			*errors = UTF8_ERR_INVALID_DATA;
		}
		return bytes_written;
	}

	while (src_size > 0)
	{
		current = *src;

		if (current == 0)
		{
			break;
		}
		else if (current >= SURROGATE_HIGH_START && current <= SURROGATE_LOW_END)
		{
			if (current > SURROGATE_HIGH_END)
			{
				if (errors != 0)
				{
					*errors = UTF8_ERR_UNMATCHED_HIGH_SURROGATE_PAIR;
				}
				return bytes_written;
			}

			if (src_size < sizeof(utf16_t))
			{
				if (errors != 0)
				{
					*errors = UTF8_ERR_INVALID_DATA;
				}
				return bytes_written;
			}

			src++;
			src_size -= sizeof(utf16_t);

			surrogate_low = *src;

			if (surrogate_low < SURROGATE_LOW_START || surrogate_low > SURROGATE_LOW_END)
			{
				if (errors != 0)
				{
					*errors = UTF8_ERR_UNMATCHED_LOW_SURROGATE_PAIR;
				}
				return bytes_written;
			}

			codepoint =
				0x10000 +
				(surrogate_low - SURROGATE_LOW_START) +
				((current - SURROGATE_HIGH_START) << 10);
		}
		else
		{
			codepoint = (unicode_t)current;
		}

		encoded_length = writecodepoint(codepoint, &dst, &dst_size, errors);
		if (encoded_length == 0)
		{
			return bytes_written;
		}

		src++;
		src_size -= sizeof(utf16_t);

		bytes_written += encoded_length;
	}

	return bytes_written;
}

size_t utf32toutf8(const unicode_t* input, size_t inputSize, char* target, size_t targetSize, int32_t* errors)
{
	unicode_t codepoint;
	size_t encoded_length;
	utf16_t surrogate_low;
	const unicode_t* src = (const unicode_t*)input;
	size_t src_size = inputSize;
	char* dst = target;
	size_t dst_size = targetSize;
	size_t bytes_written = 0;

	if (input == 0 || inputSize < sizeof(unicode_t))
	{
		if (errors != 0)
		{
			*errors = UTF8_ERR_INVALID_DATA;
		}
		return bytes_written;
	}

	while (src_size > 0)
	{
		codepoint = *src;

		if (codepoint == 0)
		{
			break;
		}
		else if (codepoint >= SURROGATE_HIGH_START && codepoint <= SURROGATE_LOW_END)
		{
			if (codepoint > SURROGATE_HIGH_END)
			{
				if (errors != 0)
				{
					*errors = UTF8_ERR_UNMATCHED_HIGH_SURROGATE_PAIR;
				}
				return bytes_written;
			}

			if (src_size < sizeof(unicode_t))
			{
				if (errors != 0)
				{
					*errors = UTF8_ERR_INVALID_DATA;
				}
				return bytes_written;
			}

			src++;
			src_size -= sizeof(unicode_t);

			surrogate_low = *src;

			if (surrogate_low < SURROGATE_LOW_START || surrogate_low > SURROGATE_LOW_END)
			{
				if (errors != 0)
				{
					*errors = UTF8_ERR_UNMATCHED_LOW_SURROGATE_PAIR;
				}
				return bytes_written;
			}

			codepoint =
				0x10000 +
				(surrogate_low - SURROGATE_LOW_START) +
				((codepoint - SURROGATE_HIGH_START) << 10);
		}

		encoded_length = writecodepoint(codepoint, &dst, &dst_size, errors);
		if (encoded_length == 0)
		{
			return bytes_written;
		}

		src++;
		src_size -= sizeof(unicode_t);

		bytes_written += encoded_length;
	}

	return bytes_written;
}

size_t widetoutf8(const wchar_t* input, size_t inputSize, char* target, size_t targetSize, int32_t* errors)
{
#if UTF8_WCHAR_UTF16
	return utf16toutf8((const utf16_t*)input, inputSize, target, targetSize, errors);
#elif UTF8_WCHAR_UTF32
	return utf32toutf8((const unicode_t*)input, inputSize, target, targetSize, errors);
#else
	return SIZE_MAX;
#endif
}

size_t utf8toutf16(const char* input, size_t inputSize, utf16_t* target, size_t targetSize, int32_t* errors)
{
	size_t bytes_written = 0;
	size_t decoded_length;
	unicode_t codepoint;
	const char* src = input;
	size_t src_length = inputSize;
	utf16_t* dst = target;
	size_t dst_size = targetSize;

	if (target != 0 && targetSize < sizeof(utf16_t))
	{
		if (errors != 0)
		{
			*errors = UTF8_ERR_NOT_ENOUGH_SPACE;
		}
		return bytes_written;
	}

	if (input == 0 || inputSize == 0)
	{
		if (errors != 0)
		{
			*errors = UTF8_ERR_INVALID_DATA;
		}
		return bytes_written;
	}

	while (src_length > 0)
	{
		decoded_length = readcodepoint(&codepoint, src, src_length);

		if (codepoint <= MAX_BASIC_MULTILINGUAR_PLANE)
		{
			if (dst != 0)
			{
				if (dst_size < sizeof(utf16_t))
				{
					if (errors != 0)
					{
						*errors = UTF8_ERR_NOT_ENOUGH_SPACE;
					}
					return bytes_written;
				}

				if (codepoint >= SURROGATE_HIGH_START && codepoint <= SURROGATE_LOW_END)
				{
					*dst++ = REPLACEMENT_CHARACTER;
				}
				else
				{
					*dst++ = (utf16_t)codepoint;
				}

				dst_size -= sizeof(utf16_t);
			}

			bytes_written += sizeof(utf16_t);
		}
		else
		{
			if (dst != 0)
			{
				/* Codepoint must be converted to a surrogate pair. */

				if (dst_size < sizeof(unicode_t))
				{
					if (errors != 0)
					{
						*errors = UTF8_ERR_NOT_ENOUGH_SPACE;
					}
					return bytes_written;
				}

				codepoint -= 0x10000;
				*dst++ = (codepoint >> 10) + SURROGATE_HIGH_START;
				*dst++ = (codepoint & 0x3FF) + SURROGATE_LOW_START;

				dst_size -= sizeof(unicode_t);
			}

			bytes_written += sizeof(unicode_t);
		}

		src += decoded_length;
		src_length -= decoded_length;
	}

	return bytes_written;
}

size_t utf8toutf32(const char* input, size_t inputSize, unicode_t* target, size_t targetSize, int32_t* errors)
{
	size_t bytes_written = 0;
	size_t decoded_length;
	unicode_t codepoint;
	const char* src = input;
	size_t src_length = inputSize;
	unicode_t* dst = target;
	size_t dst_size = targetSize;

	if (target != 0 && targetSize < sizeof(unicode_t))
	{
		if (errors != 0)
		{
			*errors = UTF8_ERR_NOT_ENOUGH_SPACE;
		}
		return bytes_written;
	}

	if (input == 0 || inputSize == 0)
	{
		if (errors != 0)
		{
			*errors = UTF8_ERR_INVALID_DATA;
		}
		return bytes_written;
	}

	while (src_length > 0)
	{
		decoded_length = readcodepoint(&codepoint, src, src_length);

		if (dst != 0)
		{
			if (dst_size < sizeof(unicode_t))
			{
				if (errors != 0)
				{
					*errors = UTF8_ERR_NOT_ENOUGH_SPACE;
				}
				return bytes_written;
			}

			*dst++ = codepoint;
			dst_size -= sizeof(unicode_t);
		}

		bytes_written += sizeof(unicode_t);

		src += decoded_length;
		src_length -= decoded_length;
	}

	return bytes_written;
}

size_t utf8towide(const char* input, size_t inputSize, wchar_t* target, size_t targetSize, int32_t* errors)
{
#if UTF8_WCHAR_UTF16
	return utf8toutf16(input, inputSize, (utf16_t*)target, targetSize, errors);
#elif UTF8_WCHAR_UTF32
	return utf8toutf32(input, inputSize, (unicode_t*)target, targetSize, errors);
#else
	return SIZE_MAX;
#endif
}

const char* seekforward(const char* src, const char* srcEnd, size_t srcLength, off_t offset)
{
	size_t i;

	if (srcEnd <= src || offset <= 0 || srcLength == 0)
	{
		return src;
	}
	if (offset >= (off_t)srcLength)
	{
		return srcEnd;
	}

	do
	{
		size_t codepoint_length = lengthcodepoint(*src);
		if (codepoint_length == 0)
		{
			codepoint_length = 1;
		}

		src++;
		srcLength--;

		for (i = 1; i < codepoint_length && srcLength > 0; ++i)
		{
			if (
				(*src & 0x80) == 0    || /* Not a continuation byte */
				(*src & 0xC0) == 0xC0    /* Start of a new sequence */
			)
			{
				break;
			}

			src++;
			srcLength--;
		}
	}
	while (--offset > 0 && srcLength > 0);

	return src;
}

const char* seekrewind(const char* srcStart, const char* src, size_t srcLength, off_t offset)
{
	if (srcStart >= src || offset >= 0 || srcLength == 0)
	{
		return src;
	}
	if (-offset >= (off_t)srcLength)
	{
		return srcStart;
	}

	/* Ignore NUL codepoint */
	src--;

	while (src != srcStart)
	{
		if (
			(*src & 0x80) == 0 ||    /* ASCII */
			(*src & 0xC0) != 0x80 || /* Malformed continuation byte */
			(*src & 0xFE) == 0xFE    /* Illegal byte */
		)
		{
			++offset;
			if (offset == 0)
			{
				break;
			}
		}

		src--;
	}

	return src;
}

const char* utf8seek(const char* text, const char* textStart, off_t offset, int direction)
{
	size_t textLength = strlen(textStart);
	const char* textEnd = textStart + textLength;

	switch (direction)
	{

	case SEEK_CUR:
		{
			if (offset == 0)
			{
				return text;
			}
			else if (offset > 0)
			{
				return seekforward(text, textEnd, textLength, offset);
			}
			else
			{
				return seekrewind(textStart, text, textLength, offset);
			}

		} break;

	case SEEK_SET:
		{
			if (text < textStart)
			{
				return text;
			}

			return seekforward(textStart, textEnd, textLength, offset);

		} break;

	case SEEK_END:
		return seekrewind(textStart, textEnd, textLength, -offset);

	default:
		return text;

	}
}

size_t transform_default(int8_t query, unicode_t codepoint, size_t codepointLength, char* target, size_t targetSize, int32_t* errors)
{
	const DecompositionRecord* record;
	int32_t find_result;
	const char* resolved;
	size_t resolved_size;

	record = finddecomposition(codepoint, query, &find_result);
	if (find_result == FindResult_Found)
	{
		resolved = resolvedecomposition(record->offset, &find_result);
		resolved_size = strlen(resolved);

		if (target != 0 && resolved_size > 0)
		{
			if (targetSize < resolved_size)
			{
				goto outofspace;
			}

			memcpy(target, resolved, resolved_size);
		}

		return resolved_size;
	}
	else
	{
		if (target != 0 && targetSize < codepointLength)
		{
			goto outofspace;
		}

		return writecodepoint(codepoint, &target, &targetSize, errors);
	}

outofspace:
	if (errors != 0)
	{
		*errors = UTF8_ERR_NOT_ENOUGH_SPACE;
	}
	return 0;
}

size_t transform_decomposition(unicode_t codepoint, size_t codepointLength, char* target, size_t targetSize, int32_t* errors)
{
	size_t resolved_size;
	unicode_t SIndex;
	unicode_t L;
	unicode_t V;
	unicode_t T;

	if (codepointLength == 1)
	{
		/* ASCII codepoints are already decomposed */

		if (target != 0)
		{
			if (targetSize < 1)
			{
				goto outofspace;
			}

			*target = (char)codepoint;
		}

		return 1;
	}
	else if (codepoint >= 0xAC00 && codepoint <= 0xD7A3)
	{
		/*
			Hangul decomposition
			
			Algorithm adapted from Unicode Technical Report #15:
			http://www.unicode.org/reports/tr15/tr15-18.html#Hangul
		*/

		static const unicode_t SBase = 0xAC00;
		static const unicode_t LBase = 0x1100;
		static const unicode_t VBase = 0x1161;
		static const unicode_t TBase = 0x11A7;
		static const unicode_t TCount = 28;
		static const unicode_t NCount = 588; /* VCount * TCount */
		static const unicode_t SCount = 11172; /* LCount * NCount */

		SIndex = codepoint - SBase;
		L = LBase + (SIndex / NCount);
		V = VBase + (SIndex % NCount) / TCount;
		T = TBase + (SIndex % TCount);

		/* hangul syllables are always three bytes */
		resolved_size = (T != TBase) ? 9 : 6;

		if (target != 0 && targetSize < resolved_size)
		{
			goto outofspace;
		}

		writecodepoint(L, &target, &targetSize, errors);
		writecodepoint(V, &target, &targetSize, errors);
		if (T != TBase)
		{
			writecodepoint(T, &target, &targetSize, errors);
		}

		return resolved_size;
	}
	else
	{
		return transform_default(DecompositionQuery_Decomposed, codepoint, codepointLength, target, targetSize, errors);
	}

outofspace:
	if (errors != 0)
	{
		*errors = UTF8_ERR_NOT_ENOUGH_SPACE;
	}
	return 0;
}

size_t transform_toupper(unicode_t codepoint, size_t codepointLength, char* target, size_t targetSize, int32_t* errors)
{
	if (codepointLength == 1 &&
		codepoint <= 0x7F)
	{
		/* Basic Latin */

		if (target != 0)
		{
			if (targetSize < 1)
			{
				goto outofspace;
			}

			if (codepoint >= 0x61 && codepoint <= 0x7A)
			{
				codepoint -= 32;
			}

			*target = (char)codepoint;
		}

		return 1;
	}
	else if (
		codepoint >= 0x80 &&
		codepoint <= 0xFF)
	{
		/* Latin-1 Supplement */

		if (codepoint >= 0xE0 && codepoint <= 0xFE)
		{
			if (codepoint != 0xF7) /* DIVISION SIGN */
			{
				codepoint -= 32;
			}

			goto write;
		}
		else if (
			codepoint == 0xB5 ||  /* MICRO SIGN */
			codepoint == 0xDF ||  /* LATIN SMALL LETTER SHARP S */
			codepoint == 0xFF)    /* LATIN SMALL LETTER Y WITH DIAERESIS */
		{
			goto query;
		}
	}
	else if (
		codepoint >= 0x100 &&
		codepoint <= 0x17F)
	{
		/* Latin Extended-A */

		if ((codepoint >= 0x139 && codepoint <= 0x148) ||
			(codepoint >= 0x179 && codepoint <= 0x17E))
		{
			if ((codepoint & 1) == 0)
			{
				/* capital letters are odd, small letters are even */

				codepoint--;
			}

			goto write;
		}
		else if (
			codepoint == 0x131 ||  /* LATIN SMALL LETTER DOTLESS I */
			codepoint == 0x149 ||  /* LATIN SMALL LETTER N PRECEDED BY APOSTROPHE */
			codepoint == 0x17F)    /* LATIN SMALL LETTER LONG S */
		{
			goto query;
		}
		else
		{
			if ((codepoint & 1) == 1)
			{
				/* capital letters are even, small letters are odd */

				codepoint--;
			}

			goto write;
		}
	}
	else if (
		codepoint >= 0x180 &&
		codepoint <= 0x24F)
	{
		/* Latin Extended-B */

		if (
			(codepoint >= 0x246)                       || /* 10 */
			(codepoint >= 0x1F8 && codepoint <= 0x21F) || /* 40 */
			(codepoint >= 0x222 && codepoint <= 0x233) || /* 18 */
			(codepoint >= 0x1DE && codepoint <= 0x1EF) || /* 16 */
			(codepoint >= 0x1A0 && codepoint <= 0x1A5) || /* 6 */
			(codepoint >= 0x182 && codepoint <= 0x185) || /* 4 */
			(codepoint >= 0x1B8 && codepoint <= 0x1BD) || /* 4 */
			(codepoint == 0x198 || codepoint == 0x199) || /* 2 */
			(codepoint == 0x1AC || codepoint == 0x1AD) || /* 2 */
			(codepoint == 0x1F4 || codepoint == 0x1F5))   /* 2 */
		{
			if ((codepoint & 1) == 1 &&
				codepoint != 0x01BB) /* LATIN LETTER TWO BAR */
			{
				/* capital letters are even, small letters are odd */

				codepoint--;
			}

			goto write;
		}
		else if (
			(codepoint >= 0x1CD && codepoint <= 0x1DC) || /* 16 */
			(codepoint >= 0x1B3 && codepoint <= 0x1B6) || /* 4 */
			(codepoint == 0x18B || codepoint == 0x18C) || /* 4 */
			(codepoint == 0x191 || codepoint == 0x192) || /* 2 */
			(codepoint == 0x1A7 || codepoint == 0x1A8) || /* 2 */
			(codepoint == 0x1AF || codepoint == 0x1A8) || /* 2 */
			(codepoint == 0x241 || codepoint == 0x242) || /* 2 */
			(codepoint == 0x23B || codepoint == 0x23C))   /* 2 */
		{
			if ((codepoint & 1) == 0)
			{
				/* capital letters are odd, small letters are even */

				codepoint--;
			}

			goto write;
		}
		else if (codepoint >= 0x1C4 && codepoint <= 0x1CC) /* 9 */
		{
			codepoint -= (codepoint - 0x1C4) % 3;

			goto write;
		}
		else if (codepoint >= 0x1F1 && codepoint <= 0x1F3) /* 3 */
		{
			codepoint -= (codepoint - 0x1F1) % 3;

			goto write;
		}
	}
	else if (
		codepoint >= 0x250 &&
		codepoint <= 0x2AF)
	{
		/* IPA Extensions */

		if (codepoint > 0x29E)
		{
			goto write;
		}
		else
		{
			goto query;
		}
	}
	else if (
		codepoint >= 0x300 &&
		codepoint <= 0x36F)
	{
		/* Combining Diacritical Marks */

		if (codepoint == 0x345) /* COMBINING GREEK YPOGEGRAMMENI */
		{
			goto query;
		}
		else
		{
			goto write;
		}
	}
	else if (
		codepoint >= 0x370 &&
		codepoint <= 0x3FF)
	{
		/* Greek and Coptic */

		if (
			(codepoint >= 0x3D8 && codepoint <= 0x3EF) || /* 24 */
			(codepoint >= 0x370 && codepoint <= 0x373) || /* 4 */
			(codepoint == 0x376 || codepoint == 0x377))   /* 2 */
		{
			if ((codepoint & 1) == 1)
			{
				/* capital letters are even, small letters are odd */

				codepoint--;
			}

			goto write;
		}
		else if (
			(codepoint >= 0x391 && codepoint <= 0x3AB) || /* 26 */
			(codepoint >= 0x386 && codepoint <= 0x38A) || /* 4 */
			(codepoint >= 0x38C && codepoint <= 0x38D))   /* 2 */
		{
			goto write;
		}
		else if (
			codepoint >= 0x3B1 && codepoint <= 0x3CB) /* 27 */
		{
			codepoint -= (codepoint != 0x3C2) ? 0x20 : 0x1F;

			goto write;
		}
		else if (
			codepoint >= 0x3AC && codepoint <= 0x3AF) /* 4 */
		{
			codepoint -= (codepoint != 0x3AC) ? 0x25 : 0x26;

			goto write;
		}
		else if (
			codepoint >= 0x37B && codepoint <= 0x37D) /* 3 */
		{
			codepoint += 0x82;

			goto write;
		}
		else
		{
			goto query;
		}
	}
	else if (
		codepoint >= 0x400 &&
		codepoint <= 0x4FF)
	{
		/* Cyrillic */

		if (codepoint <= 0x45F)
		{
			if (codepoint >= 0x450)
			{
				codepoint -= 0x50;
			}
			else if (codepoint >= 0x430)
			{
				codepoint -= 0x20;
			}

			goto write;
		}
		else if (codepoint >= 0x483 && codepoint <= 0x489)
		{
			goto write;
		}
		else if (codepoint <= 0x4C0 || codepoint >= 0x4D0)
		{
			if ((codepoint & 1) == 1)
			{
				/* capital letters are even, small letters are odd */

				codepoint--;
			}

			goto write;
		}
		else
		{
			if (codepoint == 0x4CF)
			{
				codepoint -= 0xF;
			}
			else if ((codepoint & 1) == 0)
			{
				/* capital letters are odd, small letters are even */

				codepoint--;
			}

			goto write;
		}
	}
	else if (
		codepoint >= 0x500 &&
		codepoint <= 0x52F)
	{
		/* Cyrillic Supplement */

		if ((codepoint & 1) == 1)
		{
			/* capital letters are even, small letters are odd */

			codepoint--;
		}

		goto write;
	}
	else if (
		codepoint >= 0x530 &&
		codepoint <= 0x58F)
	{
		/* Armenian */

		if (codepoint >= 0x531 && codepoint <= 0x586)
		{
			if (codepoint >= 0x561)
			{
				codepoint -= 0x30;
			}

			goto write;
		}
		else if (codepoint == 0x587) /* ARMENIAN SMALL LIGATURE ECH YIWN */
		{
			goto query;
		}
		else
		{
			goto write;
		}
	}
	else if (
		codepoint >= 0x10A0 &&
		codepoint <= 0x10FF)
	{
		/* Georgian */

		goto write;
	}
	else if (
		codepoint >= 0x1D00 &&
		codepoint <= 0x1D7F)
	{
		/* Phonetic Extensions */

		if (codepoint == 0x1D79 || /* LATIN SMALL LETTER INSULAR G */
			codepoint == 0x1D7D)   /* LATIN SMALL LETTER P WITH STROKE */
		{
			goto query;
		}
		else
		{
			goto write;
		}
	}
	else if (
		codepoint >= 0x1E00 &&
		codepoint <= 0x1EFF)
	{
		/* Latin Extended Additional */

		if (codepoint >= 0x1E96 && codepoint <= 0x1E9B)
		{
			goto query;
		}
		else
		{
			if ((codepoint & 1) == 1)
			{
				/* capital letters are even, small letters are odd */

				codepoint--;
			}

			goto write;
		}
	}
	else if (
		codepoint >= 0x1F00 &&
		codepoint <= 0x1FFF)
	{
		/* Greek Extended */

		goto query;
	}
	else if (
		codepoint >= 0x2100 &&
		codepoint <= 0x214F)
	{
		/* Letterlike Symbols */

		goto query;
	}
	else if (
		codepoint >= 0x2150 &&
		codepoint <= 0x218F)
	{
		/* Number Forms */

		goto query;
	}
	else if (
		codepoint >= 0x2460 &&
		codepoint <= 0x24FF)
	{
		/* Enclosed Alphanumerics */

		goto query;
	}
	else if (
		codepoint >= 0x2C00 &&
		codepoint <= 0x2C5F)
	{
		/* Glagolitic */

		goto query;
	}
	else if (
		codepoint >= 0x2C60 &&
		codepoint <= 0x2C7F)
	{
		/* Latin Extended-C */

		goto query;
	}
	else if (
		codepoint >= 0x2C80 &&
		codepoint <= 0x2CFF)
	{
		/* Coptic */

		goto query;
	}
	else if (
		codepoint >= 0x2D00 &&
		codepoint <= 0x2D2F)
	{
		/* Coptic */

		goto query;
	}
	else if (
		codepoint >= 0xA640 &&
		codepoint <= 0xA69F)
	{
		/* Cyrillic Extended-B */

		goto query;
	}
	else if (
		codepoint >= 0xA720 &&
		codepoint <= 0xA7FF)
	{
		/* Latin Extended-D */

		goto query;
	}
	else if (
		codepoint >= 0xFB00 &&
		codepoint <= 0xFB4F)
	{
		/* Alphabetic Presentation Forms */

		goto query;
	}
	else if (
		codepoint >= 0xFF00 &&
		codepoint <= 0xFFEF)
	{
		/* Halfwidth and Fullwidth Forms */

		goto query;
	}
	else if (
		codepoint >= 0x10400 &&
		codepoint <= 0x1044F)
	{
		/* Deseret */

		goto query;
	}
	else if (
		codepoint >= 0x118A0 &&
		codepoint <= 0x118FF)
	{
		/* Warang Citi */

		goto query;
	}
	else
	{
		goto write;
	}

query:
	return transform_default(DecompositionQuery_Uppercase, codepoint, codepointLength, target, targetSize, errors);

write:
	if (target != 0 && targetSize < codepointLength)
	{
		goto outofspace;
	}
	return writecodepoint(codepoint, &target, &targetSize, errors);

outofspace:
	if (errors != 0)
	{
		*errors = UTF8_ERR_NOT_ENOUGH_SPACE;
	}
	return 0;
}

size_t transform_tolower(unicode_t codepoint, size_t codepointLength, char* target, size_t targetSize, int32_t* errors)
{
	if (codepoint <= 0x7F)
	{
		/* Basic Latin */

		if (target != 0)
		{
			if (targetSize < 1)
			{
				goto outofspace;
			}

			if (codepoint >= 0x41 && codepoint <= 0x5A)
			{
				codepoint += 32;
			}

			*target = (char)codepoint;
		}

		return 1;
	}
	else if (
		codepoint >= 0x80 &&
		codepoint <= 0xFF)
	{
		/* Latin-1 Supplement */

		if (codepoint >= 0xC0 && codepoint <= 0xDE)
		{
			if (codepoint != 0xD7) /* MULTIPLICATION SIGN */
			{
				codepoint += 32;
			}
		}

		goto write;
	}
	else if (
		codepoint >= 0x100 &&
		codepoint <= 0x17F)
	{
		/* Latin Extended-A */

		if ((codepoint >= 0x138 && codepoint <= 0x148) ||
			(codepoint >= 0x179 && codepoint <= 0x17E))
		{
			if ((codepoint & 1) == 1)
			{
				/* capital letters are odd, small letters are even */

				codepoint++;
			}

			goto write;
		}
		else if (
			codepoint == 0x130 ||  /* LATIN CAPITAL LETTER I WITH DOT ABOVE */
			codepoint == 0x178)    /* LATIN CAPITAL LETTER Y WITH DIAERESIS */
		{
			goto query;
		}
		else
		{
			if ((codepoint & 1) == 0)
			{
				/* capital letters are even, small letters are odd */

				codepoint++;
			}

			goto write;
		}
	}
	else if (
		codepoint >= 0x180 &&
		codepoint <= 0x24F)
	{
		/* Latin Extended-B */

		if (
			(codepoint >= 0x246)                       || /* 10 */
			(codepoint >= 0x1F8 && codepoint <= 0x21F) || /* 40 */
			(codepoint >= 0x222 && codepoint <= 0x233) || /* 18 */
			(codepoint >= 0x1DE && codepoint <= 0x1EF) || /* 16 */
			(codepoint >= 0x1A0 && codepoint <= 0x1A5) || /* 6 */
			(codepoint >= 0x182 && codepoint <= 0x185) || /* 4 */
			(codepoint >= 0x1B8 && codepoint <= 0x1BD) || /* 4 */
			(codepoint == 0x198 || codepoint == 0x199) || /* 2 */
			(codepoint == 0x1AC || codepoint == 0x1AD) || /* 2 */
			(codepoint == 0x1F4 || codepoint == 0x1F5))   /* 2 */
		{
			if ((codepoint & 1) == 0 &&
				codepoint != 0x01BA) /* LATIN SMALL LETTER YOGH WITH TAIL */
			{
				/* capital letters are even, small letters are odd */

				codepoint++;
			}

			goto write;
		}
		else if (
			(codepoint >= 0x1CD && codepoint <= 0x1DC) || /* 16 */
			(codepoint >= 0x1B3 && codepoint <= 0x1B6) || /* 4 */
			(codepoint == 0x18B || codepoint == 0x18C) || /* 4 */
			(codepoint == 0x191 || codepoint == 0x192) || /* 2 */
			(codepoint == 0x1A7 || codepoint == 0x1A8) || /* 2 */
			(codepoint == 0x1AF || codepoint == 0x1A8) || /* 2 */
			(codepoint == 0x241 || codepoint == 0x242) || /* 2 */
			(codepoint == 0x23B || codepoint == 0x23C))   /* 2 */
		{
			if ((codepoint & 1) == 1)
			{
				/* capital letters are odd, small letters are even */

				codepoint++;
			}

			goto write;
		}
		else if (codepoint >= 0x1C4 && codepoint <= 0x1CC) /* 9 */
		{
			codepoint += 2 - ((codepoint - 0x1C4) % 3);

			goto write;
		}
		else if (codepoint >= 0x1F1 && codepoint <= 0x1F3) /* 3 */
		{
			codepoint += 2 - ((codepoint - 0x1F1) % 3);

			goto write;
		}
	}
	else if (
		codepoint >= 0x250 &&
		codepoint <= 0x2AF)
	{
		/* IPA Extensions */

		goto query;
	}
	else if (
		codepoint >= 0x300 &&
		codepoint <= 0x36F)
	{
		/* Combining Diacritical Marks */

		goto query;
	}
	else if (
		codepoint >= 0x370 &&
		codepoint <= 0x3FF)
	{
		/* Greek and Coptic */

		goto query;
	}
	else if (
		codepoint >= 0x400 &&
		codepoint <= 0x4FF)
	{
		/* Cyrillic */

		goto query;
	}
	else if (
		codepoint >= 0x500 &&
		codepoint <= 0x52F)
	{
		/* Cyrillic Supplement */

		goto query;
	}
	else if (
		codepoint >= 0x530 &&
		codepoint <= 0x58F)
	{
		/* Armenian */

		goto query;
	}
	else if (
		codepoint >= 0x10A0 &&
		codepoint <= 0x10FF)
	{
		/* Georgian */

		goto query;
	}
	else if (
		codepoint >= 0x1D00 &&
		codepoint <= 0x1D7F)
	{
		/* Phonetic Extensions */

		goto query;
	}
	else if (
		codepoint >= 0x1E00 &&
		codepoint <= 0x1EFF)
	{
		/* Latin Extended Additional */

		goto query;
	}
	else if (
		codepoint >= 0x1F00 &&
		codepoint <= 0x1FFF)
	{
		/* Greek Extended */

		goto query;
	}
	else if (
		codepoint >= 0x2100 &&
		codepoint <= 0x214F)
	{
		/* Letterlike Symbols */

		goto query;
	}
	else if (
		codepoint >= 0x2150 &&
		codepoint <= 0x218F)
	{
		/* Number Forms */

		goto query;
	}
	else if (
		codepoint >= 0x2460 &&
		codepoint <= 0x24FF)
	{
		/* Enclosed Alphanumerics */

		goto query;
	}
	else if (
		codepoint >= 0x2C00 &&
		codepoint <= 0x2C5F)
	{
		/* Glagolitic */

		goto query;
	}
	else if (
		codepoint >= 0x2C60 &&
		codepoint <= 0x2C7F)
	{
		/* Latin Extended-C */

		goto query;
	}
	else if (
		codepoint >= 0x2C80 &&
		codepoint <= 0x2CFF)
	{
		/* Coptic */

		goto query;
	}
	else if (
		codepoint >= 0x2D00 &&
		codepoint <= 0x2D2F)
	{
		/* Coptic */

		goto query;
	}
	else if (
		codepoint >= 0xA640 &&
		codepoint <= 0xA69F)
	{
		/* Cyrillic Extended-B */

		goto query;
	}
	else if (
		codepoint >= 0xA720 &&
		codepoint <= 0xA7FF)
	{
		/* Latin Extended-D */

		goto query;
	}
	else if (
		codepoint >= 0xFB00 &&
		codepoint <= 0xFB4F)
	{
		/* Alphabetic Presentation Forms */

		goto query;
	}
	else if (
		codepoint >= 0xFF00 &&
		codepoint <= 0xFFEF)
	{
		/* Halfwidth and Fullwidth Forms */

		goto query;
	}
	else if (
		codepoint >= 0x10400 &&
		codepoint <= 0x1044F)
	{
		/* Deseret */

		goto query;
	}
	else if (
		codepoint >= 0x118A0 &&
		codepoint <= 0x118FF)
	{
		/* Warang Citi */

		goto query;
	}
	else
	{
		goto write;
	}

query:
	return transform_default(DecompositionQuery_Lowercase, codepoint, codepointLength, target, targetSize, errors);

write:
	if (target != 0 && targetSize < codepointLength)
	{
		goto outofspace;
	}

	return writecodepoint(codepoint, &target, &targetSize, errors);

outofspace:
	if (errors != 0)
	{
		*errors = UTF8_ERR_NOT_ENOUGH_SPACE;
	}
	return 0;
}

typedef size_t (*TransformFunc)(unicode_t, size_t, char*, size_t, int32_t*);

size_t processtransform(TransformFunc transform, const char* input, size_t inputSize, char* target, size_t targetSize, int32_t* errors)
{
	const char* src = input;
	size_t src_size = inputSize;
	const char* src_end = input + inputSize;
	char* dst = target;
	size_t dst_size = targetSize;
	size_t bytes_written = 0;
	unicode_t codepoint;
	size_t codepoint_length;
	size_t result;

	if (input == 0 ||
		transform == 0)
	{
		goto invaliddata;
	}

	do
	{
		codepoint_length = readcodepoint(&codepoint, src, src_size);

		result = transform(codepoint, codepoint_length, dst, dst_size, errors);
		if (result == 0)
		{
			return bytes_written;
		}

		if (target != 0)
		{
			dst += result;
			dst_size -= result;
		}

		bytes_written += result;

		src = seekforward(src, src_end, src_size, 1);
		src_size -= codepoint_length;
	}
	while (src_size > 0);

	return bytes_written;

invaliddata:
	if (errors != 0)
	{
		*errors = UTF8_ERR_INVALID_DATA;
	}
	return bytes_written;
}

size_t utf8toupper(const char* input, size_t inputSize, char* target, size_t targetSize, int32_t* errors)
{
	return processtransform(&transform_toupper, input, inputSize, target, targetSize, errors);
}

size_t utf8tolower(const char* input, size_t inputSize, char* target, size_t targetSize, int32_t* errors)
{
	return processtransform(&transform_tolower, input, inputSize, target, targetSize, errors);
}

size_t utf8transform(const char* input, size_t inputSize, char* target, size_t targetSize, size_t flags, int32_t* errors)
{
	return processtransform(&transform_decomposition, input, inputSize, target, targetSize, errors);
}