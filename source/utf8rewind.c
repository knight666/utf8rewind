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

#define MAX_BASIC_MULTILINGUAR_PLANE  0xFFFF
#define MAX_LEGAL_UTF32               0x10FFFF
#define REPLACEMENT_CHARACTER         0xFFFD
#define SURROGATE_HIGH_START          0xD800
#define SURROGATE_HIGH_END            0xDBFF
#define SURROGATE_LOW_START           0xDC00
#define SURROGATE_LOW_END             0xDFFF

size_t writecodepoint(unicode_t codepoint, char** dst, size_t* dstSize, int32_t* errors)
{
	char* target = *dst;
	size_t encoded_length = 0;
	unicode_t mask = 0;
	size_t i;

	if (codepoint < 0x80)
	{
		encoded_length = 1;
		mask = 0x00;
	}
	else if (codepoint < 0x800)
	{
		encoded_length = 2;
		mask = 0xC0;
	}
	else if (codepoint < 0x10000)
	{
		encoded_length = 3;
		mask = 0xE0;
	}
	else if (codepoint <= MAX_LEGAL_UTF32)
	{
		encoded_length = 4;
		mask = 0xF0;
	}
	else
	{
		codepoint = REPLACEMENT_CHARACTER;
		encoded_length = 3;
		mask = 0xE0;
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

		for (i = encoded_length - 1; i >= 1; --i)
		{
			target[i] = (char)((codepoint & 0x3F) | 0x80);
			codepoint >>= 6;
		}

		target[0] = (char)(codepoint | mask);

		*dst += encoded_length;
		*dstSize -= encoded_length;
	}

	return encoded_length;
}

size_t readcodepoint(unicode_t* codepoint, const char* src, size_t srcSize, int32_t* errors)
{
	uint8_t current = (uint8_t)*src;
	uint8_t mask;
	size_t decoded_length;
	size_t i;

	if (!utf8charvalid(*src))
	{
		if (errors != 0)
		{
			*errors = UTF8_ERR_INVALID_CHARACTER;
		}
		return 0;
	}

	if (current == 0)
	{
		return 0;
	}
	else if (current <= 0x7F)
	{
		decoded_length = 1;
		mask = 0xFF;
	}
	else if ((current & 0xE0) == 0xC0)
	{
		decoded_length = 2;
		mask = 0x1F;
	}
	else if ((current & 0xF0) == 0xE0)
	{
		decoded_length = 3;
		mask = 0x0F;
	}
	else if ((current & 0xF8) == 0xF0)
	{
		decoded_length = 4;
		mask = 0x07;
	}
	else
	{
		if (errors != 0)
		{
			*errors = UTF8_ERR_INVALID_CHARACTER;
		}
		return 0;
	}

	if (srcSize < decoded_length)
	{
		if (errors != 0)
		{
			*errors = UTF8_ERR_INVALID_DATA;
		}
		return 0;
	}

	*codepoint = (unicode_t)(current & mask);

	for (i = 1; i < decoded_length; ++i)
	{
		*codepoint = (*codepoint << 6) | (src[i] & 0x3F);
	}

	return decoded_length;
}

int8_t utf8charvalid(char encodedCharacter)
{
	return ((((unsigned char)encodedCharacter & 0xFE) != 0xC0) && ((unsigned char)encodedCharacter < 0xF5));
}

size_t utf8charlen(char encodedCharacter)
{
	if (!utf8charvalid(encodedCharacter))
	{
		return SIZE_MAX;
	}
	else if ((unsigned char)encodedCharacter <= 0x7F)
	{
		return 1;
	}
	else if (((unsigned char)encodedCharacter & 0xE0) == 0xC0)
	{
		return 2;
	}
	else if (((unsigned char)encodedCharacter & 0xF0) == 0xE0)
	{
		return 3;
	}
	else if (((unsigned char)encodedCharacter & 0xF8) == 0xF0)
	{
		return 4;
	}
	else
	{
		return SIZE_MAX;
	}
}

size_t utf8len(const char* text)
{
	size_t length = 0;
	unsigned char codepoint = 0;
	int codepoint_length = 0;
	int text_length = 0;

	if (text == 0)
	{
		return length;
	}

	text_length = (int)strlen(text);
	if (text_length == 0)
	{
		return length;
	}

	while (*text != 0 && text_length > 0)
	{
		if (!utf8charvalid(*text))
		{
			return SIZE_MAX;
		}

		codepoint = (unsigned char)*text;

		if (codepoint == 0)
		{
			break;
		}
		else if (codepoint <= 0x7F)
		{
			codepoint_length = 1;
		}
		else if ((codepoint & 0xE0) == 0xC0)
		{
			if (text_length < 2)
			{
				return SIZE_MAX;
			}

			codepoint_length = 2;
		}
		else if ((codepoint & 0xF0) == 0xE0)
		{
			if (text_length < 3)
			{
				return SIZE_MAX;
			}

			codepoint_length = 3;
		}
		else if ((codepoint & 0xF8) == 0xF0)
		{
			if (text_length < 4)
			{
				return SIZE_MAX;
			}

			codepoint_length = 4;
		}
		else
		{
			return SIZE_MAX;
		}

		length++;
		text += codepoint_length;
		text_length -= codepoint_length;
	}

	return length;
}

size_t utf8encode(unicode_t codepoint, char* target, size_t targetSize, int32_t* errors)
{
	size_t length = 1;
	unicode_t mask = 0;
	size_t i;

	if (target == 0 || targetSize < 1)
	{
		if (errors != 0)
		{
			*errors = UTF8_ERR_NOT_ENOUGH_SPACE;
		}
		return SIZE_MAX;
	}

	if (codepoint < 0x80)
	{
		target[0] = (char)codepoint;

		return 1;
	}

	if (codepoint < 0x800)
	{
		length = 2;
		mask = 0xC0;
	}
	else if (codepoint < 0x10000)
	{
		length = 3;
		mask = 0xE0;
	}
	else if (codepoint <= MAX_LEGAL_UTF32)
	{
		length = 4;
		mask = 0xF0;
	}
	else
	{
		codepoint = REPLACEMENT_CHARACTER;
		length = 3;
		mask = 0xE0;
	}

	if (length >= targetSize)
	{
		if (errors != 0)
		{
			*errors = UTF8_ERR_NOT_ENOUGH_SPACE;
		}
		return SIZE_MAX;
	}

	for (i = length - 1; i >= 1; --i)
	{
		target[i] = (char)((codepoint & 0x3F) | 0x80);
		codepoint >>= 6;
	}

	target[0] = (char)(codepoint | mask);

	return length;
}

size_t utf8convertucs2(ucs2_t codepoint, char* target, size_t targetSize, int32_t* errors)
{
	if (codepoint <= 0x7F)
	{
		if (target != 0)
		{
			if (targetSize < 1)
			{
				if (errors != 0)
				{
					*errors = UTF8_ERR_NOT_ENOUGH_SPACE;
				}
				return SIZE_MAX;
			}

			target[0] = (char)codepoint;
		}

		return 1;
	}
	else if (codepoint <= 0x7FF)
	{
		if (target != 0)
		{
			if (targetSize < 2)
			{
				if (errors != 0)
				{
					*errors = UTF8_ERR_NOT_ENOUGH_SPACE;
				}
				return SIZE_MAX;
			}

			target[1] = (char)((codepoint       & 0x3F) | 0x80);
			target[0] = (char)((codepoint >> 6)         | 0xC0);
		}

		return 2;
	}
	else
	{
		if (codepoint >= SURROGATE_HIGH_START && codepoint <= SURROGATE_LOW_END)
		{
			/*
				The range between U+D800 and U+DFFF is reserved
				for lead and trail surrogate pairs.
			*/

			if (errors != 0)
			{
				*errors = UTF8_ERR_UNHANDLED_SURROGATE_PAIR;
			}
			return SIZE_MAX;
		}
		
		if (target != 0)
		{
			if (targetSize < 3)
			{
				if (errors != 0)
				{
					*errors = UTF8_ERR_NOT_ENOUGH_SPACE;
				}
				return SIZE_MAX;
			}

			target[2] = (char)(( codepoint        & 0x3F) | 0x80);
			target[1] = (char)(((codepoint >>  6) & 0x3F) | 0x80);
			target[0] = (char)( (codepoint >> 12)         | 0xE0);
		}

		return 3;
	}
}

size_t utf8encodeutf16(const utf16_t* input, size_t inputSize, char* target, size_t targetSize, int32_t* errors)
{
	size_t encoded_length = 0;
	utf16_t surrogate_low;
	utf16_t current;
	unicode_t codepoint;
	const utf16_t* src = (const utf16_t*)input;
	ptrdiff_t src_size = (ptrdiff_t)inputSize;
	char* dst = target;
	size_t dst_size = targetSize;
	size_t bytes_written = 0;

	if (input == 0 || inputSize < 2)
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

			if (src_size < 4)
			{
				if (errors != 0)
				{
					*errors = UTF8_ERR_INVALID_DATA;
				}
				return bytes_written;
			}

			src++;
			src_size -= 2;

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
		src_size -= 2;

		bytes_written += encoded_length;
	}

	return bytes_written;
}

size_t utf8encodeutf32(const unicode_t* input, size_t inputSize, char* target, size_t targetSize, int32_t* errors)
{
	unicode_t codepoint;
	size_t encoded_length;
	utf16_t surrogate_low;
	unicode_t mask = 0;
	const unicode_t* src = (const unicode_t*)input;
	ptrdiff_t src_size = (ptrdiff_t)inputSize;
	char* dst = target;
	size_t dst_size = targetSize;
	size_t bytes_written = 0;

	if (input == 0 || inputSize < 4)
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

			if (src_size < 8)
			{
				if (errors != 0)
				{
					*errors = UTF8_ERR_INVALID_DATA;
				}
				return bytes_written;
			}

			src++;
			src_size -= 4;

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
		src_size -= 4;

		bytes_written += encoded_length;
	}

	return bytes_written;
}

size_t wctoutf8(const wchar_t* input, size_t inputSize, char* target, size_t targetSize, int32_t* errors)
{
#if UTF8_WCHAR_UTF16
	return utf8encodeutf16((const utf16_t*)input, inputSize, target, targetSize, errors);
#elif UTF8_WCHAR_UTF32
	return utf8encodeutf32((const unicode_t*)input, inputSize, target, targetSize, errors);
#else
	return SIZE_MAX;
#endif
}

size_t utf8decode(const char* text, unicode_t* result, int32_t* errors)
{
	size_t text_length;
	const unsigned char* src;

	if (result == 0)
	{
		if (errors != 0)
		{
			*errors = UTF8_ERR_NOT_ENOUGH_SPACE;
		}
		return SIZE_MAX;
	}

	if (text == 0)
	{
		if (errors != 0)
		{
			*errors = UTF8_ERR_INVALID_DATA;
		}
		return SIZE_MAX;
	}

	text_length = strlen(text);
	if (text_length == 0)
	{
		if (errors != 0)
		{
			*errors = UTF8_ERR_INVALID_DATA;
		}
		return SIZE_MAX;
	}

	if (!utf8charvalid(text[0]))
	{
		if (errors != 0)
		{
			*errors = UTF8_ERR_INVALID_CHARACTER;
		}
		return SIZE_MAX;
	}

	src = (const unsigned char*)text;

	if (src[0] <= 0x7F)
	{
		*result = (unicode_t)src[0];

		return 1;
	}
	else if ((src[0] & 0xE0) == 0xC0)
	{
		if (text_length < 2)
		{
			*result = 0;

			if (errors != 0)
			{
				*errors = UTF8_ERR_INVALID_DATA;
			}
			return SIZE_MAX;
		}

		*result = src[0] & 0x1F;
		*result = (*result << 6) | (src[1] & 0x3F);

		return 2;
	}
	else if ((src[0] & 0xF0) == 0xE0)
	{
		if (text_length < 3)
		{
			*result = 0;

			if (errors != 0)
			{
				*errors = UTF8_ERR_INVALID_DATA;
			}
			return SIZE_MAX;
		}

		*result = src[0] & 0x0F;
		*result = (*result << 6) | (src[1] & 0x3F);
		*result = (*result << 6) | (src[2] & 0x3F);

		return 3;
	}
	else if ((src[0] & 0xF8) == 0xF0)
	{
		if (text_length < 4)
		{
			result = 0;

			if (errors != 0)
			{
				*errors = UTF8_ERR_INVALID_DATA;
			}
			return SIZE_MAX;
		}

		*result = src[0] & 0x07;
		*result = (*result << 6) | (src[1] & 0x3F);
		*result = (*result << 6) | (src[2] & 0x3F);
		*result = (*result << 6) | (src[3] & 0x3F);

		return 4;
	}
	else
	{
		if (errors != 0)
		{
			*errors = UTF8_ERR_INVALID_CHARACTER;
		}
		return SIZE_MAX;
	}
}

size_t utf8decodeutf16(const char* input, size_t inputSize, utf16_t* target, size_t targetSize, int32_t* errors)
{
	size_t bytes_written = 0;
	size_t decoded_length;
	unicode_t codepoint;
	const char* src = input;
	size_t src_length = inputSize;
	utf16_t* dst = target;
	size_t dst_size = targetSize;

	if (target != 0 && targetSize < 4)
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
		decoded_length = readcodepoint(&codepoint, src, src_length, errors);
		if (decoded_length == 0)
		{
			return bytes_written;
		}

		if (codepoint <= MAX_BASIC_MULTILINGUAR_PLANE)
		{
			if (dst != 0)
			{
				if (dst_size < 2)
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

				dst_size -= 2;
			}

			bytes_written += 2;
		}
		else
		{
			if (dst != 0)
			{
				/* Codepoint must be converted to a surrogate pair. */

				if (dst_size < 4)
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

				dst_size -= 4;
			}

			bytes_written += 4;
		}

		src += decoded_length;
		src_length -= decoded_length;
	}

	return bytes_written;
}

size_t utf8decodeutf32(const char* input, size_t inputSize, unicode_t* target, size_t targetSize, int32_t* errors)
{
	size_t bytes_written = 0;
	size_t decoded_length;
	unicode_t codepoint;
	const char* src = input;
	size_t src_length = inputSize;
	unicode_t* dst = target;
	size_t dst_size = targetSize;

	if (target != 0 && targetSize < 4)
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
		decoded_length = readcodepoint(&codepoint, src, src_length, errors);
		if (decoded_length == 0)
		{
			return bytes_written;
		}

		if (dst != 0)
		{
			if (dst_size < 4)
			{
				if (errors != 0)
				{
					*errors = UTF8_ERR_NOT_ENOUGH_SPACE;
				}
				return bytes_written;
			}

			*dst++ = codepoint;
			dst_size -= 4;
		}

		bytes_written += 4;

		src += decoded_length;
		src_length -= decoded_length;
	}

	return bytes_written;
}

size_t utf8towc(const char* input, size_t inputSize, wchar_t* target, size_t targetSize, int32_t* errors)
{
#if UTF8_WCHAR_UTF16
	return utf8decodeutf16(input, inputSize, (utf16_t*)target, targetSize, errors);
#elif UTF8_WCHAR_UTF32
	return utf8decodeutf32(input, inputSize, (unicode_t*)target, targetSize, errors);
#else
	return SIZE_MAX;
#endif
}

const char* seekforward(const char* src, off_t offset)
{
	size_t length;
	const char* end;
	off_t i;
	size_t char_length;

	length = strlen(src);
	if (length == 0)
	{
		return src;
	}

	end = src + length;

	for (i = 0; i < offset; ++i)
	{
		char_length = utf8charlen(*src);
		if (char_length == (size_t)SIZE_MAX)
		{
			break;
		}

		if (src + char_length >= end)
		{
			return end;
		}

		src += char_length;
	}

	return src;
}

const char* seekrewind(const char* src, const char* srcStart, off_t offset)
{
	int8_t is_ascii;
	int8_t is_last_byte;
	size_t i;

	if (srcStart >= src)
	{
		return src;
	}

	while (src != srcStart)
	{
		is_ascii = (*src & 0x80) != 0x80;
		is_last_byte = 0;

		if (is_ascii)
		{
			src--;

			if (offset + 1 == 0)
			{
				is_last_byte = ((*src & 0x80) == 0x80);
			}
		}

		if (!is_ascii || is_last_byte)
		{
			for (i = 0; i < 4; ++i)
			{
				if (src < srcStart)
				{
					src++;
					break;
				}

				src--;
				if ((*src & 0xC0) != 0x80)
				{
					break;
				}
			}
		}

		if (++offset == 0)
		{
			break;
		}
	}

	return src;
}

const char* seekatend(const char* srcStart, off_t offset)
{
	off_t length;
	int8_t is_ascii;
	const char* src;
	const char* src_current;
	size_t i;

	length = (off_t)strlen(srcStart);

	if (length <= offset)
	{
		return srcStart;
	}

	src = srcStart + length;

	if (offset < 0 || length < 1)
	{
		return src;
	}

	offset *= -1;

	for (src_current = src; src_current != srcStart; src_current--)
	{
		is_ascii = (*src_current & 0x80) != 0x80;
		if (!is_ascii)
		{
			for (i = 0; i < 6; ++i)
			{
				if (src_current < srcStart)
				{
					break;
				}

				src_current--;
				if ((*src_current & 0xC0) != 0x80)
				{
					break;
				}
			}
		}

		src = src_current;

		if (++offset == 0)
		{
			break;
		}
	}

	return src;
}

const char* utf8seek(const char* text, const char* textStart, off_t offset, int direction)
{
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
				return seekforward(text, offset);
			}
			else
			{
				return seekrewind(text, textStart, offset);
			}

		} break;

	case SEEK_SET:
		return seekforward(textStart, offset);

	case SEEK_END:
		return seekatend(textStart, offset);

	default:
		return text;

	}
}