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

#include "internal/base.h"
#include "internal/casemapping.h"
#include "internal/codepoint.h"
#include "internal/composition.h"
#include "internal/decomposition.h"
#include "internal/database.h"
#include "internal/normalization.h"
#include "internal/seeking.h"
#include "internal/streaming.h"

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

		codepoint_length = codepoint_decoded_length[codepoint];
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
	const utf16_t* src = input;
	size_t src_size = inputSize;
	char* dst = target;
	size_t dst_size = targetSize;
	size_t bytes_written = 0;

	if (input == 0 ||
		inputSize < sizeof(utf16_t))
	{
		goto invaliddata;
	}

	while (src_size > 0)
	{
		unicode_t codepoint = (unicode_t)*src;
		uint8_t encoded_size;

		if (codepoint == 0)
		{
			break;
		}

		if (codepoint >= SURROGATE_HIGH_START &&
			codepoint <= SURROGATE_LOW_END)
		{
			utf16_t surrogate_low;

			if (codepoint > SURROGATE_HIGH_END)
			{
				goto unmatchedhigh;
			}

			if (src_size < sizeof(utf16_t))
			{
				goto invaliddata;
			}

			src++;
			src_size -= sizeof(utf16_t);

			surrogate_low = *src;

			if (surrogate_low < SURROGATE_LOW_START ||
				surrogate_low > SURROGATE_LOW_END)
			{
				goto unmatchedlow;
			}

			codepoint =
				0x10000 +
				(surrogate_low - SURROGATE_LOW_START) +
				((codepoint - SURROGATE_HIGH_START) << 10);
		}

		encoded_size = codepoint_write(codepoint, &dst, &dst_size);
		if (encoded_size == 0)
		{
			goto outofspace;
		}

		bytes_written += encoded_size;

		src++;
		src_size -= sizeof(utf16_t);
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

unmatchedhigh:
	if (errors != 0)
	{
		*errors = UTF8_ERR_UNMATCHED_HIGH_SURROGATE_PAIR;
	}
	return bytes_written;

unmatchedlow:
	if (errors != 0)
	{
		*errors = UTF8_ERR_UNMATCHED_LOW_SURROGATE_PAIR;
	}
	return bytes_written;
}

size_t utf32toutf8(const unicode_t* input, size_t inputSize, char* target, size_t targetSize, int32_t* errors)
{
	const unicode_t* src = input;
	size_t src_size = inputSize;
	char* dst = target;
	size_t dst_size = targetSize;
	size_t bytes_written = 0;

	if (input == 0 ||
		inputSize < sizeof(unicode_t))
	{
		goto invaliddata;
	}

	while (src_size > 0)
	{
		unicode_t codepoint = *src;
		uint8_t encoded_size;

		if (codepoint == 0)
		{
			break;
		}

		if (codepoint >= SURROGATE_HIGH_START &&
			codepoint <= SURROGATE_LOW_END)
		{
			unicode_t surrogate_low;

			if (codepoint > SURROGATE_HIGH_END)
			{
				goto unmatchedhigh;
			}

			if (src_size < sizeof(unicode_t))
			{
				goto invaliddata;
			}

			src++;
			src_size -= sizeof(unicode_t);

			surrogate_low = *src;

			if (surrogate_low < SURROGATE_LOW_START ||
				surrogate_low > SURROGATE_LOW_END)
			{
				goto unmatchedlow;
			}

			codepoint =
				0x10000 +
				(surrogate_low - SURROGATE_LOW_START) +
				((codepoint - SURROGATE_HIGH_START) << 10);
		}

		encoded_size = codepoint_write(codepoint, &dst, &dst_size);
		if (encoded_size == 0)
		{
			goto outofspace;
		}

		bytes_written += encoded_size;

		src++;
		src_size -= sizeof(unicode_t);
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

unmatchedhigh:
	if (errors != 0)
	{
		*errors = UTF8_ERR_UNMATCHED_HIGH_SURROGATE_PAIR;
	}
	return bytes_written;

unmatchedlow:
	if (errors != 0)
	{
		*errors = UTF8_ERR_UNMATCHED_LOW_SURROGATE_PAIR;
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
	const char* src = input;
	size_t src_length = inputSize;
	utf16_t* dst = target;
	size_t dst_size = targetSize;
	size_t bytes_written = 0;

	if (input == 0 ||
		inputSize == 0)
	{
		goto invaliddata;
	}

	if (target != 0 &&
		targetSize < sizeof(utf16_t))
	{
		goto outofspace;
	}

	while (src_length > 0)
	{
		unicode_t decoded;
		uint8_t decoded_size = codepoint_read(src, src_length, &decoded);

		if (decoded <= MAX_BASIC_MULTILINGUAR_PLANE)
		{
			if (dst != 0)
			{
				if (dst_size < sizeof(utf16_t))
				{
					goto outofspace;
				}

				if (decoded >= SURROGATE_HIGH_START &&
					decoded <= SURROGATE_LOW_END)
				{
					*dst++ = REPLACEMENT_CHARACTER;
				}
				else
				{
					*dst++ = (utf16_t)decoded;
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
					goto outofspace;
				}

				decoded -= 0x10000;
				*dst++ = (decoded >> 10) + SURROGATE_HIGH_START;
				*dst++ = (decoded & 0x3FF) + SURROGATE_LOW_START;

				dst_size -= sizeof(unicode_t);
			}

			bytes_written += sizeof(unicode_t);
		}

		src += decoded_size;
		src_length -= decoded_size;
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

size_t utf8toutf32(const char* input, size_t inputSize, unicode_t* target, size_t targetSize, int32_t* errors)
{
	const char* src = input;
	size_t src_length = inputSize;
	unicode_t* dst = target;
	size_t dst_size = targetSize;
	size_t bytes_written = 0;

	if (input == 0 ||
		inputSize == 0)
	{
		goto invaliddata;
	}

	if (target != 0 &&
		targetSize < sizeof(unicode_t))
	{
		goto outofspace;
	}

	while (src_length > 0)
	{
		unicode_t decoded;
		uint8_t decoded_length = codepoint_read(src, src_length, &decoded);

		if (dst != 0)
		{
			if (dst_size < sizeof(unicode_t))
			{
				goto outofspace;
			}

			*dst++ = decoded;
			dst_size -= sizeof(unicode_t);
		}

		bytes_written += sizeof(unicode_t);

		src += decoded_length;
		src_length -= decoded_length;
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
				return seeking_forward(text, textEnd, textLength, offset);
			}
			else
			{
				return seeking_rewind(textStart, text, textLength, offset);
			}

		} break;

	case SEEK_SET:
		{
			if (text < textStart)
			{
				return text;
			}

			return seeking_forward(textStart, textEnd, textLength, offset);

		} break;

	case SEEK_END:
		return seeking_rewind(textStart, textEnd, textLength, -offset);

	default:
		return text;

	}
}

size_t utf8toupper(const char* input, size_t inputSize, char* target, size_t targetSize, int32_t* errors)
{
	CaseMappingState state;
	size_t bytes_written = 0;

	/* Validate input */

	UTF8_VALIDATE_INPUT;

	/* Initialize case mapping */

	if (!casemapping_initialize(&state, input, inputSize, target, targetSize, UnicodeProperty_Uppercase))
	{
		return bytes_written;
	}

	/* Execute case mapping as long as input remains */

	while (state.src_size > 0)
	{
		size_t result = casemapping_execute(&state);
		if (!result)
		{
			goto outofspace;
		}

		bytes_written += result;
	}

	return bytes_written;

invaliddata:
	if (errors != 0)
	{
		*errors = UTF8_ERR_INVALID_DATA;
	}
	return bytes_written;

overlap:
	if (errors != 0)
	{
		*errors = UTF8_ERR_OVERLAPPING_PARAMETERS;
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
	CaseMappingState state;
	size_t bytes_written = 0;

	/* Validate input */

	UTF8_VALIDATE_INPUT;

	/* Initialize case mapping */

	if (!casemapping_initialize(&state, input, inputSize, target, targetSize, UnicodeProperty_Lowercase))
	{
		return bytes_written;
	}

	/* Execute case mapping as long as input remains */

	while (state.src_size > 0)
	{
		size_t result = casemapping_execute(&state);
		if (!result)
		{
			goto outofspace;
		}

		bytes_written += result;
	}

	return bytes_written;

invaliddata:
	if (errors != 0)
	{
		*errors = UTF8_ERR_INVALID_DATA;
	}
	return bytes_written;

overlap:
	if (errors != 0)
	{
		*errors = UTF8_ERR_OVERLAPPING_PARAMETERS;
	}
	return bytes_written;

outofspace:
	if (errors != 0)
	{
		*errors = UTF8_ERR_NOT_ENOUGH_SPACE;
	}
	return bytes_written;
}

size_t utf8totitle(const char* input, size_t inputSize, char* target, size_t targetSize, int32_t* errors)
{
	CaseMappingState state;
	size_t bytes_written = 0;

	/* Validate input */

	UTF8_VALIDATE_INPUT;

	/* Initialize case mapping */

	if (!casemapping_initialize(&state, input, inputSize, target, targetSize, UnicodeProperty_Titlecase))
	{
		return bytes_written;
	}

	/* Execute case mapping as long as input remains */

	while (state.src_size > 0)
	{
		size_t result = casemapping_execute(&state);
		if (!result)
		{
			goto outofspace;
		}

		/* The first letter of every word should be titlecase, the rest lowercase */

		if (state.property == UnicodeProperty_Titlecase)
		{
			if ((state.last_general_category & GeneralCategory_Letter) != 0)
			{
				state.property = UnicodeProperty_Lowercase;
			}
		}
		else if ((state.last_general_category & GeneralCategory_Letter) == 0)
		{
			state.property = UnicodeProperty_Titlecase;
		}

		bytes_written += result;
	}

	return bytes_written;

invaliddata:
	if (errors != 0)
	{
		*errors = UTF8_ERR_INVALID_DATA;
	}
	return bytes_written;

overlap:
	if (errors != 0)
	{
		*errors = UTF8_ERR_OVERLAPPING_PARAMETERS;
	}
	return bytes_written;

outofspace:
	if (errors != 0)
	{
		*errors = UTF8_ERR_NOT_ENOUGH_SPACE;
	}
	return bytes_written;
}

size_t utf8normalize(const char* input, size_t inputSize, char* target, size_t targetSize, size_t flags, int32_t* errors)
{
	size_t bytes_written = 0;
	StreamState stream[3];
	StreamState* src_compose = &stream[0];
	uint8_t compatibility = (flags & UTF8_NORMALIZE_COMPATIBILITY) != 0;

	if (stream_initialize(&stream[0], input, inputSize) == 0)
	{
		goto invaliddata;
	}

	if ((flags & UTF8_NORMALIZE_DECOMPOSE) != 0)
	{
		DecomposeState decompose;

		if (decompose_initialize(&decompose, &stream[0], &stream[1], compatibility) == 0)
		{
			goto invaliddata;
		}
		
		while (decompose_execute(&decompose) != 0)
		{
			uint8_t written = 0;

			if (stream[1].stable == 0)
			{
				stream_reorder(&stream[1]);
			}

			if (stream_write(&stream[1], target, targetSize, &written) == 0)
			{
				bytes_written += written;

				goto outofspace;
			}

			if (target != 0)
			{
				target += written;
				targetSize -= written;
			}

			bytes_written += written;
		}

		src_compose = &stream[1];
	}

	if ((flags & UTF8_NORMALIZE_COMPOSE) != 0)
	{
		ComposeState compose;
		unicode_t result;

		if (!compose_initialize(&compose, src_compose, &stream[2], compatibility))
		{
			goto invaliddata;
		}

		while (1)
		{
			uint8_t encoded_size;

			result = compose_execute(&compose);
			if (result == 0)
			{
				break;
			}

			encoded_size = codepoint_write(result, &target, &targetSize);
			if (encoded_size == 0)
			{
				goto outofspace;
			}

			bytes_written += encoded_size;
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
	static const size_t MaskCaseMapping =
		UTF8_TRANSFORM_UPPERCASE | UTF8_TRANSFORM_LOWERCASE |
		UTF8_TRANSFORM_TITLECASE | UTF8_TRANSFORM_CASEFOLD;
	static const size_t MaskCaseMappingDecompose =
		(UTF8_TRANSFORM_UPPERCASE | UTF8_TRANSFORM_LOWERCASE |
		UTF8_TRANSFORM_TITLECASE | UTF8_TRANSFORM_CASEFOLD) &
		~UTF8_TRANSFORM_FAST;
	static const size_t MaskDecomposition =
		((UTF8_TRANSFORM_UPPERCASE | UTF8_TRANSFORM_LOWERCASE |
		UTF8_TRANSFORM_TITLECASE | UTF8_TRANSFORM_CASEFOLD) &
		~UTF8_TRANSFORM_FAST) |
		UTF8_TRANSFORM_DECOMPOSED | UTF8_TRANSFORM_COMPOSED;
	static const size_t MaskComposition = 
		((UTF8_TRANSFORM_UPPERCASE | UTF8_TRANSFORM_LOWERCASE |
		UTF8_TRANSFORM_TITLECASE | UTF8_TRANSFORM_CASEFOLD) &
		~UTF8_TRANSFORM_FAST) |
		UTF8_TRANSFORM_COMPOSED;

	char* dst = target;
	size_t dst_size = targetSize;
	StreamState stream[4];
	DecomposeState decompose_state;
	ComposeState compose_state;
	//CaseMappingState casemapping_state;
	uint8_t compatibility = (flags & UTF8_TRANSFORM_COMPATIBILITY) != 0;
	StreamState* stream_output;
	size_t bytes_written = 0;

	memset(stream, 0, 4 * sizeof(StreamState));

	if (!stream_initialize(&stream[0], input, inputSize))
	{
		goto invaliddata;
	}

	stream_output = &stream[0];

	if ((flags & MaskDecomposition) != 0)
	{
		if (!decompose_initialize(&decompose_state, &stream[0], &stream[1], compatibility))
		{
			goto invaliddata;
		}

		stream_output = &stream[2];
	}

	if ((flags & MaskComposition) != 0)
	{
		if (!compose_initialize(&compose_state, &stream[2], &stream[3], compatibility))
		{
			goto invaliddata;
		}

		stream_output = &stream[3];
	}

	if ((flags & MaskCaseMapping) != 0)
	{
		uint8_t property = 0;

		stream_output = &stream[0];

		if ((flags & UTF8_TRANSFORM_TITLECASE) != 0)
		{
			property = UnicodeProperty_Titlecase;
		}
		else if ((flags & UTF8_TRANSFORM_LOWERCASE) != 0)
		{
			property = UnicodeProperty_Lowercase;
		}
		else if ((flags & UTF8_TRANSFORM_UPPERCASE) != 0)
		{
			property = UnicodeProperty_Uppercase;
		}

		/*if (!casemapping_initialize(&casemapping_state, stream_input, property))
		{
			goto invaliddata;
		}*/
	}

	while (1)
	{
		uint8_t finished = 0;
		uint8_t write = 0;
		uint8_t i;
		unicode_t* src_codepoint;
		unicode_t* dst_codepoint;
		uint8_t* src_qc;
		uint8_t* dst_qc;
		uint8_t* src_ccc;
		uint8_t* dst_ccc;

		if ((flags & MaskDecomposition) != 0)
		{
			if (stream[1].current > 0)
			{
				src_codepoint = stream[1].codepoint;
				dst_codepoint = stream[2].codepoint + stream[2].filled;
				src_qc = stream[1].quick_check;
				dst_qc = stream[2].quick_check + stream[2].filled;
				src_ccc = stream[1].canonical_combining_class;
				dst_ccc = stream[2].canonical_combining_class + stream[2].filled;

				if ((flags & MaskComposition) != 0)
				{
					for (i = 0; i < stream[1].current; ++i)
					{
						*dst_qc++ = database_queryproperty(*src_codepoint, compose_state.property);
						*dst_ccc++ = *src_ccc++;
						*dst_codepoint++ = *src_codepoint++;
					}
				}
				else
				{
					for (i = 0; i < stream[1].current; ++i)
					{
						*dst_codepoint++ = *src_codepoint++;
						*dst_qc++ = *src_qc++;
						*dst_ccc++ = *src_ccc++;
					}
				}

				stream[2].current += stream[1].current;
				stream[2].filled += stream[1].current;
			}

			finished = !decompose_execute(&decompose_state);
			if (!finished)
			{
				if (!stream[1].stable)
				{
					stream_reorder(&stream[1]);
				}

				write = (stream[1].current + stream[2].filled) >= STREAM_SAFE_MAX;
			}
		}

		if (write ||
			finished)
		{
			uint8_t i;

			if ((flags & MaskComposition) != 0)
			{
				if (!compose_execute(&compose_state))
				{
					break;
				}
			}

			for (i = 0; i < stream_output->current; ++i)
			{
				uint8_t encoded_size = codepoint_write(stream_output->codepoint[i], &dst, &dst_size);
				if (encoded_size == 0)
				{
					goto outofspace;
				}

				bytes_written += encoded_size;
			}

			if (finished)
			{
				break;
			}

			stream[2].current = 0;
			stream[2].filled = 0;
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