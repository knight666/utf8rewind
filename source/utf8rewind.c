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

#define HANGUL_L_FIRST 0x1100
#define HANGUL_L_LAST 0x1112
#define HANGUL_L_COUNT 19

#define HANGUL_V_FIRST 0x1161
#define HANGUL_V_LAST 0x1175
#define HANGUL_V_COUNT 21

#define HANGUL_T_FIRST 0x11A7
#define HANGUL_T_LAST 0x11C2
#define HANGUL_T_COUNT 28

#define HANGUL_N_COUNT 588 /* VCount * TCount */

#define HANGUL_S_FIRST 0xAC00
#define HANGUL_S_LAST 0xD7A3
#define HANGUL_S_COUNT 11172 /* LCount * NCount */

#if defined(__GNUC__) && !defined(COMPILER_ICC)
	#define UTF8_UNUSED(_parameter) _parameter __attribute__ ((unused))
#else
	#define UTF8_UNUSED(_parameter) _parameter
#endif

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

size_t lengthcodepoint(unicode_t codepoint)
{
	if (codepoint < 0x80)
	{
		return 1;
	}
	else if (codepoint < 0x800)
	{
		return 2;
	}
	else if (codepoint < 0x10000)
	{
		return 3;
	}
	else if (codepoint <= MAX_LEGAL_UNICODE)
	{
		return 4;
	}
	else
	{
		return 3;
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

size_t readcodepoint(unicode_t* codepoint, const char* input, size_t inputSize)
{
	const uint8_t* src = (const uint8_t*)input;
	size_t decoded_length;
	static const uint8_t ReadMask[7] = { 0x7F, 0xFF, 0x1F, 0x0F, 0x07, 0x03, 0x01 };

	if (*src == 0)
	{
		*codepoint = 0;
		return 1;
	}

	decoded_length = Utf8ByteLength[*src];
	if (decoded_length == 0)
	{
		*codepoint = REPLACEMENT_CHARACTER;
		return 1;
	}

	*codepoint = (unicode_t)(*src & ReadMask[decoded_length]);

	if (decoded_length > 1)
	{
		size_t src_index;
		size_t src_size = inputSize;

		for (src_index = 1; src_index < decoded_length; ++src_index)
		{
			src++;

			if (src_size == 0 ||    /* Not enough data */
				(*src & 0x80) == 0) /* Not a continuation byte */
			{
				*codepoint = REPLACEMENT_CHARACTER;
				return src_index;
			}

			src_size--;

			*codepoint = (*codepoint << 6) | (*src & 0x3F);
		}

		/* Overlong sequences and surrogate pairs */

		if (*codepoint < Utf8ByteMinimum[decoded_length - 1] ||
			*codepoint > Utf8ByteMaximum[decoded_length - 1] ||
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

typedef struct {
	const char** src;
	size_t* src_size;
	uint8_t transform;
	uint8_t finished;
	unicode_t codepoint[2];
	size_t length[2];
	uint8_t check[2];
	uint8_t current;
	uint8_t next;
} ComposeState;

uint8_t compose_initialize(ComposeState* state, const char** input, size_t* inputSize, uint8_t transformType)
{
	memset(state, 0, sizeof(ComposeState));

	state->src = input;
	state->src_size = inputSize;
	state->transform = transformType;

	state->check[0] = QuickCheckResult_No;
	state->check[1] = QuickCheckResult_No;

	state->current = 0;
	state->next = 1;

	/* Read the first codepoint */

	state->length[0] = readcodepoint(&state->codepoint[0], *state->src, *state->src_size);
	state->check[0] = queryproperty(state->codepoint[0], state->transform);

	if (*state->src_size > state->length[0])
	{
		*state->src += state->length[0];
		*state->src_size -= state->length[0];
	}
	else
	{
		state->finished = 1;
	}

	return 1;
}

uint8_t compose_execute(ComposeState* state)
{
	if (state->finished >= 1)
	{
		state->finished = 2;
		return state->current;
	}

	while (state->finished == 0)
	{
		unicode_t composed = 0;

		if (*state->src_size > 0)
		{
			state->length[state->next] = readcodepoint(&state->codepoint[state->next], *state->src, *state->src_size);
			state->check[state->next] = queryproperty(state->codepoint[state->next], state->transform);

			if (*state->src_size >= state->length[state->next])
			{
				*state->src += state->length[state->next];
				*state->src_size -= state->length[state->next];

				state->finished = (*state->src_size == 0);
			}
			else
			{
				state->finished = 1;
			}
		}

		if (state->check[state->current] == QuickCheckResult_Yes &&
			state->check[state->next] == QuickCheckResult_Yes)
		{
			break;
		}

		/*
			Hangul composition
			
			Algorithm adapted from Unicode Technical Report #15:
			http://www.unicode.org/reports/tr15/tr15-18.html#Hangul
		*/

		if (state->codepoint[state->current] >= HANGUL_L_FIRST &&
			state->codepoint[state->current] <= HANGUL_L_LAST)
		{
			/* Check for Hangul LV pair */ 

			if (state->codepoint[state->next] >= HANGUL_V_FIRST &&
				state->codepoint[state->next] <= HANGUL_V_LAST)
			{
				unicode_t l_index = state->codepoint[state->current] - HANGUL_L_FIRST;
				unicode_t v_index = state->codepoint[state->next] - HANGUL_V_FIRST;

				composed = HANGUL_S_FIRST + (((l_index * HANGUL_V_COUNT) + v_index) * HANGUL_T_COUNT);
			}
			else
			{
				break;
			}
		}
		else if (
			state->codepoint[state->current] >= HANGUL_S_FIRST &&
			state->codepoint[state->current] <= HANGUL_S_LAST)
		{
			/* Check for Hangul LV and T pair */ 

			if (state->codepoint[state->next] >= HANGUL_T_FIRST &&
				state->codepoint[state->next] <= HANGUL_T_LAST)
			{
				unicode_t t_index = state->codepoint[state->next] - HANGUL_T_FIRST;

				composed = state->codepoint[state->current] + t_index;
			}
			else
			{
				break;
			}
		}
		else
		{
			/* Check database for composition */

			int32_t find_result;
			composed = querycomposition(state->codepoint[state->current], state->codepoint[state->next], &find_result);
		}

		if (composed == 0)
		{
			break;
		}
		
		if (state->check[state->next] == QuickCheckResult_Maybe)
		{
			/* If the composition succeeded but there's no data left, don't output the second codepoint */

			state->check[state->next] = state->finished ? QuickCheckResult_No : QuickCheckResult_Yes;
		}

		state->codepoint[state->current] = composed;
		state->length[state->current] = lengthcodepoint(composed);
		state->check[state->current] = queryproperty(composed, state->transform);
	}

	/* Swap buffers */

	state->current = (state->current + 1) % 2;
	state->next = (state->next + 1) % 2;

	return state->next;
}

size_t transform_decomposition(const char* input, size_t inputSize, char* target, size_t targetSize, uint8_t propertyType, uint8_t transformType, int32_t* errors)
{
	size_t bytes_written = 0;
	const char* src = input;
	size_t src_size = inputSize;
	char* dst = target;
	size_t dst_size = targetSize;

	if (src == 0 ||
		src_size == 0)
	{
		goto invaliddata;
	}

	while (src_size > 0)
	{
		if ((*src & 0x80) == 0)
		{
			/* Basic Latin codepoints are already decomposed */

			if (dst != 0)
			{
				if (dst_size < 1)
				{
					goto outofspace;
				}

				*dst = *src;

				dst++;
				dst_size--;
			}

			bytes_written++;

			src++;
			src_size--;
		}
		else
		{
			size_t resolved_size = 0;
			unicode_t codepoint;
			size_t codepoint_length = readcodepoint(&codepoint, src, src_size);

			if (queryproperty(codepoint, propertyType) == QuickCheckResult_No)
			{
				if (codepoint >= HANGUL_S_FIRST &&
					codepoint <= HANGUL_S_LAST)
				{
					/*
						Hangul decomposition
			
						Algorithm adapted from Unicode Technical Report #15:
						http://www.unicode.org/reports/tr15/tr15-18.html#Hangul
					*/

					unicode_t s_index = codepoint - HANGUL_S_FIRST;
					unicode_t l = HANGUL_L_FIRST + (s_index / HANGUL_N_COUNT);
					unicode_t v = HANGUL_V_FIRST + (s_index % HANGUL_N_COUNT) / HANGUL_T_COUNT;
					unicode_t t = HANGUL_T_FIRST + (s_index % HANGUL_T_COUNT);

					/* Hangul syllables are always three bytes */
					resolved_size = (t != HANGUL_T_FIRST) ? 9 : 6;

					if (dst != 0 &&
						dst_size < resolved_size)
					{
						goto outofspace;
					}

					writecodepoint(l, &dst, &dst_size, errors);
					writecodepoint(v, &dst, &dst_size, errors);
					if (t != HANGUL_T_FIRST)
					{
						writecodepoint(t, &dst, &dst_size, errors);
					}
				}
				else
				{
					int32_t find_result;
					const char* resolved = finddecomposition(codepoint, transformType, &find_result);

					if (find_result == FindResult_Found)
					{
						resolved_size = strlen(resolved);

						if (dst != 0 &&
							resolved_size > 0)
						{
							if (dst_size < resolved_size)
							{
								goto outofspace;
							}

							memcpy(dst, resolved, resolved_size);

							dst += resolved_size;
							dst_size -= resolved_size;
						}
					}
					else
					{
						resolved_size = writecodepoint(codepoint, &dst, &dst_size, errors);
					}
				}
			}
			else
			{
				resolved_size = writecodepoint(codepoint, &dst, &dst_size, errors);
			}

			if (resolved_size == 0)
			{
				break;
			}
			bytes_written += resolved_size;

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

size_t transform_composition(const char* input, size_t inputSize, char* target, size_t targetSize, uint8_t transformType, int32_t* errors)
{
	size_t bytes_written = 0;
	const char* src = input;
	size_t src_size = inputSize;
	char* dst = target;
	size_t dst_size = targetSize;
	ComposeState state;

	if (src == 0 ||
		src_size == 0)
	{
		goto invaliddata;
	}

	compose_initialize(&state, &src, &src_size, transformType);

	while (state.finished != 2)
	{
		uint8_t index;
		size_t written;

		index = compose_execute(&state);

		if (state.check[index] != QuickCheckResult_No)
		{
			if (dst != 0 &&
				dst_size < state.length[index])
			{
				goto outofspace;
			}

			written = writecodepoint(state.codepoint[index], &dst, &dst_size, errors);
			if (written == 0)
			{
				break;
			}
			bytes_written += written;
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

size_t transform_uppercase(const char* input, size_t inputSize, char* target, size_t targetSize, int32_t* errors)
{
	const char* src = input;
	size_t src_size = inputSize;
	char* dst = target;
	size_t dst_size = targetSize;
	size_t bytes_written = 0;

	if (src == 0 ||
		src_size == 0)
	{
		goto invaliddata;
	}

	while (src_size > 0)
	{
		if ((*src & 0x80) == 0)
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
			size_t result = 0;
			unicode_t codepoint;
			size_t codepoint_length = readcodepoint(&codepoint, src, src_size);

			if (queryproperty(codepoint, UnicodeProperty_Uppercase) == 1)
			{
				int32_t find_result;
				const char* resolved = finddecomposition(codepoint, DecompositionQuery_Uppercase, &find_result);

				if (find_result == FindResult_Found)
				{
					size_t resolved_size = strlen(resolved);

					if (dst != 0 &&
						resolved_size > 0)
					{
						if (dst_size < resolved_size)
						{
							goto outofspace;
						}

						memcpy(dst, resolved, resolved_size);

						dst += resolved_size;
						dst_size -= resolved_size;
					}

					result = resolved_size;
				}
				else
				{
					result = writecodepoint(codepoint, &dst, &dst_size, errors);
				}
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

size_t transform_lowercase(const char* input, size_t inputSize, char* target, size_t targetSize, int32_t* errors)
{
	size_t bytes_written = 0;
	const char* src = input;
	size_t src_size = inputSize;
	char* dst = target;
	size_t dst_size = targetSize;
	ComposeState state;

	if (src == 0 ||
		src_size == 0)
	{
		goto invaliddata;
	}

	compose_initialize(&state, &src, &src_size, UnicodeProperty_QC_NFC);

	while (state.finished != 2)
	{
		uint8_t index;
		size_t written;

		index = compose_execute(&state);

		if (queryproperty(state.codepoint[index], UnicodeProperty_Lowercase) == 1)
		{
			int32_t find_result;
			const char* resolved = finddecomposition(state.codepoint[index], DecompositionQuery_Lowercase, &find_result);

			if (find_result == FindResult_Found)
			{
				size_t resolved_size = strlen(resolved);

				if (dst != 0 &&
					resolved_size > 0)
				{
					if (dst_size < resolved_size)
					{
						goto outofspace;
					}

					memcpy(dst, resolved, resolved_size);

					dst += resolved_size;
					dst_size -= resolved_size;
				}

				written = resolved_size;
			}
			else
			{
				written = writecodepoint(state.codepoint[index], &dst, &dst_size, errors);
			}
		}
		else
		{
			written = writecodepoint(state.codepoint[index], &dst, &dst_size, errors);
		}

		if (written == 0)
		{
			break;
		}

		bytes_written += written;
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

size_t transform_lowercase_fast(const char* input, size_t inputSize, char* target, size_t targetSize, int32_t* errors)
{
	const char* src = input;
	size_t src_size = inputSize;
	char* dst = target;
	size_t dst_size = targetSize;
	size_t bytes_written = 0;

	if (src == 0 ||
		src_size == 0)
	{
		goto invaliddata;
	}

	while (src_size > 0)
	{
		if ((*src & 0x80) == 0)
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
			size_t result = 0;
			unicode_t codepoint;
			size_t codepoint_length = readcodepoint(&codepoint, src, src_size);

			if (queryproperty(codepoint, UnicodeProperty_Lowercase) == 1)
			{
				int32_t find_result;
				const char* resolved = finddecomposition(codepoint, DecompositionQuery_Lowercase, &find_result);

				if (find_result == FindResult_Found)
				{
					size_t resolved_size = strlen(resolved);

					if (dst != 0 &&
						resolved_size > 0)
					{
						if (dst_size < resolved_size)
						{
							goto outofspace;
						}

						memcpy(dst, resolved, resolved_size);

						dst += resolved_size;
						dst_size -= resolved_size;
					}

					result = resolved_size;
				}
				else
				{
					result = writecodepoint(codepoint, &dst, &dst_size, errors);
				}
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

size_t utf8toupper(const char* input, size_t inputSize, char* target, size_t targetSize, int32_t* errors)
{
	return utf8transform(input, inputSize, target, targetSize, UTF8_TRANSFORM_UPPERCASE, errors);
}

size_t utf8tolower(const char* input, size_t inputSize, char* target, size_t targetSize, int32_t* errors)
{
	return utf8transform(input, inputSize, target, targetSize, UTF8_TRANSFORM_LOWERCASE | UTF8_TRANSFORM_NORMALIZED, errors);
}

size_t utf8transform(const char* input, size_t inputSize, char* target, size_t targetSize, size_t flags, int32_t* errors)
{
	if ((flags & UTF8_TRANSFORM_UPPERCASE) != 0)
	{
		return transform_uppercase(input, inputSize, target, targetSize, errors);
	}
	else if (
		(flags & UTF8_TRANSFORM_LOWERCASE) != 0)
	{
		if ((flags & UTF8_TRANSFORM_NORMALIZED) != 0)
		{
			return transform_lowercase_fast(input, inputSize, target, targetSize, errors);
		}
		else
		{
			return transform_lowercase(input, inputSize, target, targetSize, errors);
		}
	}
	else if (
		(flags & UTF8_TRANSFORM_DECOMPOSED) != 0)
	{
		return transform_decomposition(input, inputSize, target, targetSize, UnicodeProperty_QC_NFD, DecompositionQuery_Decomposed, errors);
	}
	else if (
		(flags & UTF8_TRANSFORM_COMPATIBILITY_DECOMPOSED) != 0)
	{
		return transform_decomposition(input, inputSize, target, targetSize, UnicodeProperty_QC_NFKD, DecompositionQuery_Compatibility_Decomposed, errors);
	}
	else if (
		(flags & UTF8_TRANSFORM_COMPOSED) != 0)
	{
		return transform_composition(input, inputSize, target, targetSize, UnicodeProperty_QC_NFC, errors);
	}
	else if (
		(flags & UTF8_TRANSFORM_COMPATIBILITY_COMPOSED) != 0)
	{
		return transform_composition(input, inputSize, target, targetSize, UnicodeProperty_QC_NFKC, errors);
	}
	else
	{
		if (errors != 0)
		{
			*errors = UTF8_ERR_INVALID_TRANSFORM;
		}
		return 0;
	}
}