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

uint8_t utf8isnormalized(const char* input, size_t inputSize, size_t flags)
{
	const char* src = input;
	size_t src_size = inputSize;
	uint8_t property;
	uint8_t last_canonical_class;
	uint8_t result;

	/* Validate input and flags */

	if (input == NULL ||
		inputSize == 0 ||
		(flags & (UTF8_NORMALIZE_DECOMPOSE | UTF8_NORMALIZE_COMPOSE)) == 0)
	{
		return UTF8_NORMALIZATION_RESULT_YES;
	}

	/* Determine normalization property */

	if ((flags & UTF8_NORMALIZE_COMPOSE) != 0)
	{
		property = ((flags & UTF8_NORMALIZE_COMPATIBILITY) != 0)
			? UnicodeProperty_Normalization_Compatibility_Compose
			: UnicodeProperty_Normalization_Compose;
	}
	else
	{
		property =
			((flags & UTF8_NORMALIZE_COMPATIBILITY) != 0)
			? UnicodeProperty_Normalization_Compatibility_Decompose
			: UnicodeProperty_Normalization_Decompose;
	}

	/* Loop over input */

	last_canonical_class = 0;
	result = UTF8_NORMALIZATION_RESULT_YES;

	while (src_size > 0)
	{
		unicode_t decoded;
		uint8_t canonical_class;
		uint8_t quick_check;

		uint8_t read = codepoint_read(src, src_size, &decoded);
		if (read == 0)
		{
			break;
		}

		canonical_class = database_queryproperty(decoded, UnicodeProperty_CanonicalCombiningClass);
		if (last_canonical_class > canonical_class &&
			canonical_class > 0)
		{
			return UTF8_NORMALIZATION_RESULT_NO;
		}

		quick_check = database_queryproperty(decoded, property);
		if (quick_check == QuickCheckResult_No)
		{
			return UTF8_NORMALIZATION_RESULT_NO;
		}
		else if (quick_check == QuickCheckResult_Maybe)
		{
			result = UTF8_NORMALIZATION_RESULT_MAYBE;
		}

		last_canonical_class = canonical_class;

		src += read;
		src_size -= read;
	}

	return result;
}

size_t utf8normalize(const char* input, size_t inputSize, char* target, size_t targetSize, size_t flags, int32_t* errors)
{
	char* dst = target;
	size_t dst_size = targetSize;
	StreamState stream[4];
	DecomposeState decompose_state;
	ComposeState compose_state;
	uint8_t compatibility = (flags & UTF8_NORMALIZE_COMPATIBILITY) != 0;
	StreamState* stream_output;
	uint8_t finished = 0;
	size_t bytes_written = 0;

	/*
		Decomposition uses the following process:

		input         -->  stream[0]  -->
		(decompose)   -->  stream[1]  -->
		(accumulate)  -->  stream[2]  -->
		output

		The accumulation step is necessary in order to prevent buffer overflow
		attacks.

		Composition adds another stream buffer:

		input         --> stream[0]  -->
		(decompose)   --> stream[1]  -->
		(accumulate)  --> stream[2]  -->
		(compose)     --> stream[3]  -->
		output

		Although four streaming buffers may seem excessive, they are necessary
		for preventing allocations on the heap.
	*/

	/* Check for valid flags */

	if ((flags & (UTF8_NORMALIZE_DECOMPOSE | UTF8_NORMALIZE_COMPOSE)) == 0)
	{
		goto invalidflag;
	}

	/* Validate input */

	UTF8_VALIDATE_INPUT;

	/* Initialize decomposition */

	memset(stream, 0, 4 * sizeof(StreamState));

	if (!stream_initialize(&stream[0], input, inputSize) ||
		!decompose_initialize(&decompose_state, &stream[0], &stream[1], compatibility))
	{
		goto invaliddata;
	}

	stream_output = &stream[2];

	if ((flags & UTF8_NORMALIZE_COMPOSE) != 0)
	{
		/* Initialize composition */

		if (!compose_initialize(&compose_state, &stream[2], &stream[3], compatibility))
		{
			goto invaliddata;
		}

		stream_output = &stream[3];
	}

	do
	{
		uint8_t write = 0;

		/* Accumulate decomposed input in next stream */

		if (stream[1].current > 0)
		{
			unicode_t* src_codepoint = stream[1].codepoint;
			unicode_t* dst_codepoint = stream[2].codepoint + stream[2].filled;
			uint8_t* src_qc = stream[1].quick_check;
			uint8_t* dst_qc = stream[2].quick_check + stream[2].filled;
			uint8_t* src_ccc = stream[1].canonical_combining_class;
			uint8_t* dst_ccc = stream[2].canonical_combining_class + stream[2].filled;

			if ((flags & UTF8_NORMALIZE_COMPOSE) != 0)
			{
				uint8_t i;

				/* Update stream properties to use composition values */

				for (i = 0; i < stream[1].current; ++i)
				{
					*dst_qc++ = database_queryproperty(*src_codepoint, compose_state.property);
					*dst_ccc++ = *src_ccc++;
					*dst_codepoint++ = *src_codepoint++;
				}
			}
			else
			{
				/* Copy directly */

				memcpy(dst_codepoint, src_codepoint, stream[1].current * sizeof(unicode_t));
				memcpy(dst_qc, src_qc, stream[1].current * sizeof(uint8_t));
				memcpy(dst_ccc, src_ccc, stream[1].current * sizeof(uint8_t));
			}

			stream[2].current += stream[1].current;
			stream[2].filled += stream[1].current;
		}

		/* Decompose input sequence into next stream */

		finished = !decompose_execute(&decompose_state);
		if (!finished)
		{
			/* Output current stream it it could overflow accumulation buffer */

			write = (stream[1].current + stream[2].filled) >= STREAM_SAFE_MAX;
		}

		/* Reorder potentially unordered decomposed stream */

		if (!stream[1].stable)
		{
			stream_reorder(&stream[1]);
		}

		/* Write stream to output when overflowing or when accumulation buffer is empty*/

		if (write ||
			finished)
		{
			uint8_t i;

			/* Compose accumulation buffer */

			if ((flags & UTF8_NORMALIZE_COMPOSE) != 0 &&
				!compose_execute(&compose_state))
			{
				break;
			}

			/* Write to output buffer */

			for (i = 0; i < stream_output->current; ++i)
			{
				uint8_t encoded_size = codepoint_write(stream_output->codepoint[i], &dst, &dst_size);
				if (encoded_size == 0)
				{
					goto outofspace;
				}

				bytes_written += encoded_size;
			}

			/* Reset accumulation buffer */

			stream[2].current = 0;
			stream[2].filled = 0;
		}
	}
	while (!finished);

	return bytes_written;

invaliddata:
	if (errors != 0)
	{
		*errors = UTF8_ERR_INVALID_DATA;
	}
	return bytes_written;

invalidflag:
	if (errors != 0)
	{
		*errors = UTF8_ERR_INVALID_FLAG;
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