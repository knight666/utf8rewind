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

int utf8charvalid(char encodedCharacter)
{
	return ((((unsigned char)encodedCharacter & 0xFE) != 0xC0) && ((unsigned char)encodedCharacter < 0xF5));
}

int utf8charlen(char encodedCharacter)
{
	if (!utf8charvalid(encodedCharacter))
	{
		return UTF8_ERR_INVALID_CHARACTER;
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
		return UTF8_ERR_INVALID_CHARACTER;
	}
}

size_t utf8len(const char* text)
{
	int length = 0;
	unsigned char codepoint = 0;
	int codepoint_length = 0;
	int text_length = 0;

	if (text == 0)
	{
		return 0;
	}

	text_length = (int)strlen(text);
	if (text_length == 0)
	{
		return 0;
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

int wctoutf8(const wchar_t* input, size_t inputSize, char* target, size_t targetSize)
{
	int result = 0;
	utf16_t surrogate_high;
	utf16_t surrogate_low;
	utf16_t current;
	unicode_t codepoint;
	const char* src = (const char*)input;
	int src_size = (int)inputSize;
	char* dst = target;
	size_t dst_size = targetSize;
	int bytes_written = 0;
	int32_t errors = 0;

	if (input == 0 || inputSize < 2)
	{
		return UTF8_ERR_INVALID_DATA;
	}

	while (src_size > 0)
	{
		current = *(utf16_t*)src;

		if (current < SURROGATE_HIGH_START || current > SURROGATE_LOW_END)
		{
			result = utf8convertucs2(*(const ucs2_t*)src, dst, dst_size, &errors);
			if (result == SIZE_MAX)
			{
				return errors;
			}

			src += 2;
			src_size -= 2;

			if (dst != 0)
			{
				dst += result;
				dst_size -= result;
			}

			bytes_written += result;
		}
		else
		{
			if (src_size < 4)
			{
				return UTF8_ERR_INVALID_DATA;
			}

			surrogate_high = *(utf16_t*)src;

			if (surrogate_high < SURROGATE_HIGH_START || surrogate_high > SURROGATE_HIGH_END)
			{
				return UTF8_ERR_UNMATCHED_HIGH_SURROGATE_PAIR;
			}

			surrogate_low = *(utf16_t*)(src + 2);

			if (surrogate_low < SURROGATE_LOW_START || surrogate_low > SURROGATE_LOW_END)
			{
				return UTF8_ERR_UNMATCHED_LOW_SURROGATE_PAIR;
			}

			codepoint =
				0x10000 +
				(surrogate_low - SURROGATE_LOW_START) +
				((surrogate_high - SURROGATE_HIGH_START) << 10);

			if (codepoint > MAX_LEGAL_UTF32)
			{
				/* Unicode characters must be encoded in a maximum of four bytes. */

				return UTF8_ERR_INVALID_CHARACTER;
			}

			src += 4;
			src_size -= 4;

			if (dst != 0)
			{
				if (dst_size < 4)
				{
					return UTF8_ERR_NOT_ENOUGH_SPACE;
				}

				dst[3] = (char)(( codepoint        & 0x3F) | 0x80);
				dst[2] = (char)(((codepoint >>  6) & 0x3F) | 0x80);
				dst[1] = (char)(((codepoint >> 12) & 0x3F) | 0x80);
				dst[0] = (char)( (codepoint >> 18)         | 0xF0);

				dst += 4;
				dst_size -= 4;
			}

			bytes_written += 4;
		}
	}

	return bytes_written;
}

int utf8decode(const char* text, unicode_t* result)
{
	size_t text_length;
	const unsigned char* src;

	if (result == 0)
	{
		return UTF8_ERR_NOT_ENOUGH_SPACE;
	}

	if (text == 0)
	{
		return UTF8_ERR_INVALID_DATA;
	}

	text_length = strlen(text);
	if (text_length == 0)
	{
		return UTF8_ERR_INVALID_DATA;
	}

	if (!utf8charvalid(text[0]))
	{
		return UTF8_ERR_INVALID_CHARACTER;
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

			return UTF8_ERR_INVALID_DATA;
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

			return UTF8_ERR_INVALID_DATA;
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

			return UTF8_ERR_INVALID_DATA;
		}

		*result = src[0] & 0x07;
		*result = (*result << 6) | (src[1] & 0x3F);
		*result = (*result << 6) | (src[2] & 0x3F);
		*result = (*result << 6) | (src[3] & 0x3F);

		return 4;
	}
	else
	{
		return UTF8_ERR_INVALID_CHARACTER;
	}
}

int utf8towc(const char* input, size_t inputSize, wchar_t* target, size_t targetSize)
{
	unicode_t codepoint;
	const char* src = input;
	int src_size = inputSize;
	wchar_t* dst = target;
	size_t dst_size = targetSize;
	int bytes_written = 0;

	if (target == 0)
	{
		return UTF8_ERR_NOT_ENOUGH_SPACE;
	}

	if (src == 0 || src_size == 0)
	{
		return UTF8_ERR_INVALID_DATA;
	}

	while (src_size > 0)
	{
		codepoint = *(utf16_t*)src;

		if (codepoint <= 0x7F)
		{
			src++;
			src_size--;
		}
		else if ((codepoint & 0xE0) == 0xC0)
		{
			if (src_size < 2)
			{
				return UTF8_ERR_INVALID_DATA;
			}

			codepoint = src[0] & 0x1F;
			codepoint = (codepoint << 6) | (src[1] & 0x3F);

			src += 2;
			src_size -= 2;
		}
		else if ((codepoint & 0xF0) == 0xE0)
		{
			if (src_size < 3)
			{
				return UTF8_ERR_INVALID_DATA;
			}

			codepoint = src[0] & 0x0F;
			codepoint = (codepoint << 6) | (src[1] & 0x3F);
			codepoint = (codepoint << 6) | (src[2] & 0x3F);

			src += 3;
			src_size -= 3;
		}
		else if ((codepoint & 0xF8) == 0xF0)
		{
			if (src_size < 4)
			{
				return UTF8_ERR_INVALID_DATA;
			}

			codepoint = src[0] & 0x07;
			codepoint = (codepoint << 6) | (src[1] & 0x3F);
			codepoint = (codepoint << 6) | (src[2] & 0x3F);
			codepoint = (codepoint << 6) | (src[3] & 0x3F);

			src += 4;
			src_size -= 4;
		}

		if (codepoint <= MAX_BASIC_MULTILINGUAR_PLANE)
		{
			if (dst_size < 2)
			{
				return UTF8_ERR_NOT_ENOUGH_SPACE;
			}

			if (codepoint >= SURROGATE_HIGH_START && codepoint <= SURROGATE_LOW_END)
			{
				*dst++ = REPLACEMENT_CHARACTER;

				src += 2;
				src_size -= 2;
			}
			else
			{
				*dst++ = (utf16_t)codepoint;
			}

			dst_size -= 2;

			bytes_written += 2;
		}
		else
		{
			/* Codepoint must be converted to a surrogate pair. */

			if (dst_size < 4)
			{
				return UTF8_ERR_NOT_ENOUGH_SPACE;
			}

			codepoint -= 0x10000;
			*dst++ = (codepoint >> 10) + SURROGATE_HIGH_START;
			*dst++ = (codepoint & 0x3FF) + SURROGATE_LOW_START;

			dst_size -= 4;

			bytes_written += 4;
		}
	}

	return bytes_written;
}

const char* seekforward(const char* src, off_t offset)
{
	size_t length;
	const char* end;
	off_t i;

	length = strlen(src);
	if (length == 0)
	{
		return src;
	}

	end = src + length;

	for (i = 0; i < offset; ++i)
	{
		int char_length = utf8charlen(*src);
		if (char_length <= 0)
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
	int is_ascii;
	int last_check;
	int i;

	if (srcStart >= src)
	{
		return src;
	}

	while (src != srcStart)
	{
		is_ascii = (*src & 0x80) != 0x80;
		last_check = 0;

		if (is_ascii)
		{
			src--;

			if (offset + 1 == 0)
			{
				last_check = ((*src & 0x80) == 0x80);
			}
		}

		if (!is_ascii || last_check)
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
	int length;
	int is_ascii;
	const char* src;
	const char* src_current;
	int i;

	length = (int)strlen(srcStart);

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