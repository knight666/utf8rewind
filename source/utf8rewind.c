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

static const int8_t Utf8ByteLength[256] = {
	/* Basic Latin */
	1, 1, 1, 1, 1, 1, 1, 1, /* 0x00 - 0x07 */
	1, 1, 1, 1, 1, 1, 1, 1, /* 0x08 - 0x0F */
	1, 1, 1, 1, 1, 1, 1, 1, /* 0x10 - 0x17 */
	1, 1, 1, 1, 1, 1, 1, 1, /* 0x18 - 0x1F */
	1, 1, 1, 1, 1, 1, 1, 1, /* 0x20 - 0x27 */
	1, 1, 1, 1, 1, 1, 1, 1, /* 0x28 - 0x2F */
	1, 1, 1, 1, 1, 1, 1, 1, /* 0x30 - 0x37 */
	1, 1, 1, 1, 1, 1, 1, 1, /* 0x38 - 0x3F */
	1, 1, 1, 1, 1, 1, 1, 1, /* 0x40 - 0x47 */
	1, 1, 1, 1, 1, 1, 1, 1, /* 0x48 - 0x4F */
	1, 1, 1, 1, 1, 1, 1, 1, /* 0x50 - 0x57 */
	1, 1, 1, 1, 1, 1, 1, 1, /* 0x58 - 0x5F */
	1, 1, 1, 1, 1, 1, 1, 1, /* 0x60 - 0x67 */
	1, 1, 1, 1, 1, 1, 1, 1, /* 0x68 - 0x6F */
	1, 1, 1, 1, 1, 1, 1, 1, /* 0x70 - 0x77 */
	1, 1, 1, 1, 1, 1, 1, 1, /* 0x78 - 0x7F */

	/* Malformed continuation byte */
	0, 0, 0, 0, 0, 0, 0, 0, /* 0x80 - 0x87 */
	0, 0, 0, 0, 0, 0, 0, 0, /* 0x88 - 0x8F */
	0, 0, 0, 0, 0, 0, 0, 0, /* 0x90 - 0x97 */
	0, 0, 0, 0, 0, 0, 0, 0, /* 0x98 - 0x9F */
	0, 0, 0, 0, 0, 0, 0, 0, /* 0xA0 - 0xA7 */
	0, 0, 0, 0, 0, 0, 0, 0, /* 0xA8 - 0xAF */
	0, 0, 0, 0, 0, 0, 0, 0, /* 0xB0 - 0xB7 */
	0, 0, 0, 0, 0, 0, 0, 0, /* 0xB8 - 0xBF */

	/* Two bytes */
	2, 2, 2, 2, 2, 2, 2, 2, /* 0xC0 - 0xC7 */
	2, 2, 2, 2, 2, 2, 2, 2, /* 0xC8 - 0xCF */
	2, 2, 2, 2, 2, 2, 2, 2, /* 0xD0 - 0xD7 */
	2, 2, 2, 2, 2, 2, 2, 2, /* 0xD8 - 0xDF */

	/* Three bytes */
	3, 3, 3, 3, 3, 3, 3, 3, /* 0xE0 - 0xE7 */
	3, 3, 3, 3, 3, 3, 3, 3, /* 0xE8 - 0xEF */

	/* Four bytes */
	4, 4, 4, 4, 4, 4, 4, 4, /* 0xF0 - 0xF7 */

	/* Five bytes */
	5, 5, 5, 5,             /* 0xF8 - 0xFB */

	/* Six bytes */
	6, 6,                   /* 0xFC - 0xFD */
	
	/* Invalid */
	0, 0                    /* 0xFE - 0xFF */
};

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

	decoded_length = Utf8ByteLength[current];
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

		if ((*codepoint < Utf8ByteMinimum[decoded_length - 1] ||
			*codepoint > Utf8ByteMaximum[decoded_length - 1]))
		{
			*codepoint = REPLACEMENT_CHARACTER;
		}

		/* Check for surrogate pairs */

		else if (
			(*codepoint >= SURROGATE_HIGH_START && *codepoint <= SURROGATE_HIGH_END) ||
			(*codepoint >= SURROGATE_LOW_START && *codepoint <= SURROGATE_LOW_END))
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

		codepoint_length = Utf8ByteLength[codepoint];
		if (codepoint_length != 0)
		{
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
		}
		else
		{
			src_index = 1;
		}

		length++;

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
		size_t codepoint_length = Utf8ByteLength[(uint8_t)*src];
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

size_t transform_default(int8_t query, unicode_t codepoint, size_t codepointLength, char** target, size_t* targetSize, int32_t* errors)
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

		if (*target != 0 && resolved_size > 0)
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
	}
	else
	{
		return writecodepoint(codepoint, target, targetSize, errors);
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
		return transform_default(DecompositionQuery_Decomposed, codepoint, codepointLength, &target, &targetSize, errors);
	}

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
	const char* src = input;
	size_t src_size = inputSize;
	char* dst = target;
	size_t dst_size = targetSize;
	size_t bytes_written = 0;
	unicode_t codepoint;
	size_t codepoint_length;
	size_t result;

	if (input == 0)
	{
		goto invaliddata;
	}

	while (src_size > 0)
	{
		if ((uint8_t)*src <= 0x7F)
		{
			/* Basic Latin */

			if (dst != 0)
			{
				if (dst_size < 1)
				{
					goto outofspace;
				}

				*dst++ = (*src >= 0x61 && *src <= 0x7A) ? *src - 0x20 : *src;
				dst_size--;
			}

			bytes_written++;

			src++;
			src_size--;
		}
		else
		{
			codepoint_length = readcodepoint(&codepoint, src, src_size);

			if ((codepoint >= 0x80 && codepoint <= 0x2AF) ||       /* Latin-1 Supplement, Latin Extended-A, Latin Extended-B, IPA Extensions */
				(codepoint >= 0x300 && codepoint <= 0x58F) ||      /* Combining Diacritical Marks, Greek and Coptic, Cyrillic, Cyrillic Supplement, Armenian */
				(codepoint >= 0x10A0 && codepoint <= 0x10FF) ||    /* Georgian */
				(codepoint >= 0x1D00 && codepoint <= 0x1D7F) ||    /* Phonetic Extensions */
				(codepoint >= 0x1E00 && codepoint <= 0x1FFF) ||    /* Latin Extended Additional, Greek Extended */
				(codepoint >= 0x2100 && codepoint <= 0x218F) ||    /* Letterlike Symbols, Number Forms */
				(codepoint >= 0x2460 && codepoint <= 0x24FF) ||    /* Enclosed Alphanumerics */
				(codepoint >= 0x2C00 && codepoint <= 0x2D2F) ||    /* Glagolitic, Latin Extended-C, Coptic, Georgian Supplement */
				(codepoint >= 0xA640 && codepoint <= 0xA69F) ||    /* Cyrillic Extended-B */
				(codepoint >= 0xA720 && codepoint <= 0xA7FF) ||    /* Latin Extended-D */
				(codepoint >= 0xFB00 && codepoint <= 0xFB4F) ||    /* Alphabetic Presentation Forms */
				(codepoint >= 0xFF00 && codepoint <= 0xFFEF) ||    /* Halfwidth and Fullwidth Forms */
				(codepoint >= 0x10400 && codepoint <= 0x1044F) ||  /* Deseret */
				(codepoint >= 0x118A0 && codepoint <= 0x118FF))    /* Warang Citi */
			{
				result = transform_default(DecompositionQuery_Uppercase, codepoint, codepoint_length, &dst, &dst_size, errors);
			}
			else
			{
				result = writecodepoint(codepoint, &dst, &dst_size, errors);
			}

			if (result == 0)
			{
				break;
			}

			bytes_written += result;

			src += codepoint_length;
			src_size -= codepoint_length;
		}
	}

	return bytes_written;

invaliddata:
	if (errors != 0)
	{
		*errors = UTF8_ERR_INVALID_DATA;
	}
	return bytes_written;

outofspace:
	if (errors != 0)
	{
		*errors = UTF8_ERR_NOT_ENOUGH_SPACE;
	}
	return bytes_written;
}

size_t utf8tolower(const char* input, size_t inputSize, char* target, size_t targetSize, int32_t* errors)
{
	const char* src = input;
	size_t src_size = inputSize;
	char* dst = target;
	size_t dst_size = targetSize;
	size_t bytes_written = 0;
	unicode_t codepoint;
	size_t codepoint_length;
	size_t result;

	if (input == 0)
	{
		goto invaliddata;
	}

	while (src_size > 0)
	{
		if ((uint8_t)*src <= 0x7F)
		{
			/* Basic Latin */

			if (dst != 0)
			{
				if (dst_size < 1)
				{
					goto outofspace;
				}

				*dst++ = (*src >= 0x41 && *src <= 0x5A) ? *src + 0x20 : *src;
				dst_size--;
			}

			bytes_written++;

			src++;
			src_size--;
		}
		else
		{
			codepoint_length = readcodepoint(&codepoint, src, src_size);

			if ((codepoint >= 0x80 && codepoint <= 0x2AF) ||       /* Latin-1 Supplement, Latin Extended-A, Latin Extended-B, IPA Extensions */
				(codepoint >= 0x300 && codepoint <= 0x58F) ||      /* Combining Diacritical Marks, Greek and Coptic, Cyrillic, Cyrillic Supplement, Armenian */
				(codepoint >= 0x10A0 && codepoint <= 0x10FF) ||    /* Georgian */
				(codepoint >= 0x1D00 && codepoint <= 0x1D7F) ||    /* Phonetic Extensions */
				(codepoint >= 0x1E00 && codepoint <= 0x1FFF) ||    /* Latin Extended Additional, Greek Extended */
				(codepoint >= 0x2100 && codepoint <= 0x218F) ||    /* Letterlike Symbols, Number Forms */
				(codepoint >= 0x2460 && codepoint <= 0x24FF) ||    /* Enclosed Alphanumerics */
				(codepoint >= 0x2C00 && codepoint <= 0x2D2F) ||    /* Glagolitic, Latin Extended-C, Coptic, Georgian Supplement */
				(codepoint >= 0xA640 && codepoint <= 0xA69F) ||    /* Cyrillic Extended-B */
				(codepoint >= 0xA720 && codepoint <= 0xA7FF) ||    /* Latin Extended-D */
				(codepoint >= 0xFB00 && codepoint <= 0xFB4F) ||    /* Alphabetic Presentation Forms */
				(codepoint >= 0xFF00 && codepoint <= 0xFFEF) ||    /* Halfwidth and Fullwidth Forms */
				(codepoint >= 0x10400 && codepoint <= 0x1044F) ||  /* Deseret */
				(codepoint >= 0x118A0 && codepoint <= 0x118FF))    /* Warang Citi */
			{
				result = transform_default(DecompositionQuery_Lowercase, codepoint, codepoint_length, &dst, &dst_size, errors);
			}
			else
			{
				result = writecodepoint(codepoint, &dst, &dst_size, errors);
			}

			if (result == 0)
			{
				break;
			}

			bytes_written += result;

			src += codepoint_length;
			src_size -= codepoint_length;
		}
	}

	return bytes_written;

invaliddata:
	if (errors != 0)
	{
		*errors = UTF8_ERR_INVALID_DATA;
	}
	return bytes_written;

outofspace:
	if (errors != 0)
	{
		*errors = UTF8_ERR_NOT_ENOUGH_SPACE;
	}
	return bytes_written;
}

size_t utf8transform(const char* input, size_t inputSize, char* target, size_t targetSize, size_t flags, int32_t* errors)
{
	return processtransform(&transform_decomposition, input, inputSize, target, targetSize, errors);
}