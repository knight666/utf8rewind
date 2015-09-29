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

#include "base.h"
#include "codepoint.h"
#include "database.h"

#define GC_LE  GeneralCategory_Letter
#define GC_CA  GeneralCategory_CaseMapped
#define GC_LC  GeneralCategory_Letter | GeneralCategory_CaseMapped
#define GC_MA  GeneralCategory_Mark
#define GC_NU  GeneralCategory_Number
#define GC_PU  GeneralCategory_Punctuation
#define GC_SY  GeneralCategory_Symbol
#define GC_SE  GeneralCategory_Separator
#define GC_OT  GeneralCategory_Other

#define GC_BLOCK_SHIFT (3)
static const char GC_INDEX_MASK = (1 << GC_BLOCK_SHIFT) - 1;

static const uint32_t general_category_index[16] = {
	0, 0, 0, 0,
	8, 13, 21, 27,
	34, 36, 35, 42,
	50, 35, 35, 55
};

static const uint8_t general_category_data[63] = {
	GC_OT, GC_OT, GC_OT, GC_OT, GC_OT, GC_OT, GC_OT, GC_OT,
	GC_SE, GC_PU, GC_PU, GC_PU, GC_SY, GC_PU, GC_PU, GC_PU,
	GC_SY, GC_PU, GC_PU, GC_PU, GC_PU, GC_NU, GC_NU, GC_NU,
	GC_NU, GC_NU, GC_NU, GC_NU, GC_NU, GC_PU, GC_PU, GC_SY,
	GC_SY, GC_SY, GC_PU, GC_LC, GC_LC, GC_LC, GC_LC, GC_LC,
	GC_LC, GC_LC, GC_LC, GC_LC, GC_LC, GC_PU, GC_PU, GC_PU,
	GC_SY, GC_PU, GC_SY, GC_LC, GC_LC, GC_LC, GC_LC, GC_LC,
	GC_LC, GC_LC, GC_PU, GC_SY, GC_PU, GC_SY, GC_OT
};

static const char basic_latin_lowercase_table[58] = {
	/* LATIN CAPITAL LETTER A - LATIN CAPITAL LETTER Z */
	0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C,
	0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
	0x79, 0x7A,

	0x5B, /* LEFT SQUARE BRACKET */
	0x5C, /* REVERSE SOLIDUS */
	0x5D, /* RIGHT SQUARE BRACKET */
	0x5E, /* CIRCUMFLEX ACCENT */
	0x5F, /* LOW LINE */
	0x60, /* GRAVE ACCENT */

	/* LATIN SMALL LETTER A - LATIN SMALL LETTER Z */
	0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C,
	0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
	0x79, 0x7A
};

static const char basic_latin_uppercase_table[58] = {
	/* LATIN CAPITAL LETTER A - LATIN CAPITAL LETTER Z */
	0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C,
	0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
	0x59, 0x5A,

	0x5B, /* LEFT SQUARE BRACKET */
	0x5C, /* REVERSE SOLIDUS */
	0x5D, /* RIGHT SQUARE BRACKET */
	0x5E, /* CIRCUMFLEX ACCENT */
	0x5F, /* LOW LINE */
	0x60, /* GRAVE ACCENT */

	/* LATIN SMALL LETTER A - LATIN SMALL LETTER Z */
	0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C,
	0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
	0x59, 0x5A
};

#if WIN32 || _WINDOWS
	#define LOCALE_TYPE                     _locale_t
	#define GET_LOCALE()                    _get_current_locale()
	#define CHECK_LOCALE(_name, _ansiCodepage, _oemCodepage) \
		locale->locinfo->lc_codepage == _ansiCodepage || \
		locale->locinfo->lc_codepage == _oemCodepage
#else
	#define LOCALE_TYPE                     const char*
	#define GET_LOCALE()                    setlocale(LC_ALL, 0)
	#define CHECK_LOCALE(_name, _ansiCodepage, _oemCodepage) \
		!strncasecmp(locale, _name, 5)
#endif

uint8_t casemapping_initialize(
	CaseMappingState* state,
	const char* input, size_t inputSize,
	char* target, size_t targetSize,
	const uint32_t* propertyIndex1, const uint32_t* propertyIndex2, const uint32_t* propertyData)
{
	/*
		Sources for locales and code pages

		Windows
		https://msdn.microsoft.com/en-US/goglobal/bb896001.aspx

		POSIX
		https://www-01.ibm.com/support/knowledgecenter/ssw_aix_61/com.ibm.aix.nlsgdrf/support_languages_locales.htm
	*/

	LOCALE_TYPE locale;

	memset(state, 0, sizeof(CaseMappingState));

	state->src = input;
	state->src_size = inputSize;
	state->dst = target;
	state->dst_size = targetSize;
	state->property_index1 = propertyIndex1;
	state->property_index2 = propertyIndex2;
	state->property_data = propertyData;

	locale = GET_LOCALE();

	if (CHECK_LOCALE("el_gr", 1253, 737))
	{
		state->locale = CASEMAPPING_LOCALE_GREEK;
	}
	else if (
		CHECK_LOCALE("lt_lt", 1257, 775))
	{
		state->locale = CASEMAPPING_LOCALE_LITHUANIAN;
	}
	else if (
		CHECK_LOCALE("tr_tr", 1254, 857) ||
		CHECK_LOCALE("az_az", 1254, 857))
	{
		state->locale = CASEMAPPING_LOCALE_TURKISH_OR_AZERI_LATIN;
	}

	return 1;
}

uint8_t casemapping_readcodepoint(CaseMappingState* state)
{
	if (state->src_size == 0)
	{
		return 0;
	}
	else if (
		(*state->src & 0x80) == 0)
	{
		/* Basic Latin can be converted to UTF-32 by padding the value to 32 bits with 0. */

		state->last_code_point = (unicode_t)*state->src;
		state->last_code_point_size = 1;
	}
	else
	{
		/* Decode current code point */

		state->last_code_point_size = codepoint_read(state->src, state->src_size, &state->last_code_point);
	}

	/* Get general category */

	state->last_general_category = PROPERTY_GET_GC(state->last_code_point);

	/* Move source cursor */

	if (state->src_size >= state->last_code_point_size)
	{
		state->src += state->last_code_point_size;
		state->src_size -= state->last_code_point_size;
	}
	else
	{
		state->src_size = 0;
	}

	return state->last_code_point_size;
}

size_t casemapping_write(CaseMappingState* state)
{
	size_t bytes_needed = 0;

	if (state->last_code_point == REPLACEMENT_CHARACTER)
	{
		/* Write replacement character to output */

		if (state->dst != 0)
		{
			if (state->dst_size < REPLACEMENT_CHARACTER_STRING_LENGTH)
			{
				goto outofspace;
			}

			memcpy(
				state->dst,
				REPLACEMENT_CHARACTER_STRING,
				REPLACEMENT_CHARACTER_STRING_LENGTH);

			state->dst += REPLACEMENT_CHARACTER_STRING_LENGTH;
			state->dst_size -= REPLACEMENT_CHARACTER_STRING_LENGTH;
		}

		bytes_needed = REPLACEMENT_CHARACTER_STRING_LENGTH;
	}
	else if (
		/* GREEK CAPITAL LETTER SIGMA */
		state->last_code_point == 0x03A3 &&
		/* Converting to lowercase */
		state->property_data == LowercaseDataPtr)
	{
		/*
			If the final letter of a word (defined as "a collection of code
			points with the General Category 'Letter'") is a GREEK CAPITAL
			LETTER SIGMA and more than one code point was processed, the
			lowercase version is U+03C2 GREEK SMALL LETTER FINAL SIGMA
			instead of U+03C3 GREEK SMALL LETTER SIGMA.
		*/

		/* At least one code point should have been read */

		uint8_t should_convert = state->total_bytes_needed > 0;

		if (state->src_size > 0)
		{
			/* Read the next code point without moving the cursor */

			state->last_code_point_size = codepoint_read(
				state->src,
				state->src_size,
				&state->last_code_point);

			/* Retrieve the General Category of the next code point */

			state->last_general_category = PROPERTY_GET_GC(state->last_code_point);

			/* Convert if the "word" has ended */

			should_convert = (state->last_general_category & GeneralCategory_Letter) == 0;
		}

		/* Write the converted code point to the output buffer */

		bytes_needed = 2;

		if (state->dst != 0)
		{
			if (state->dst_size < bytes_needed)
			{
				goto outofspace;
			}

			memcpy(
				state->dst,
				should_convert ? "\xCF\x82" : "\xCF\x83",
				bytes_needed);

			state->dst += bytes_needed;
			state->dst_size -= bytes_needed;
		}
	}
	else if (
		state->last_code_point_size == 1)
	{
		/* Basic Latin */

		if (state->dst != 0)
		{
			if (state->dst_size < 1)
			{
				goto outofspace;
			}

			/*
				Uppercase letters are U+0041 ('A') to U+005A ('Z')
				Lowercase letters are U+0061 ('a') to U+007A ('z')
			*/

			if (state->last_code_point >= 0x41 &&
				state->last_code_point <= 0x7A)
			{
				if (state->property_data == LowercaseDataPtr)
				{
					*state->dst =
						basic_latin_lowercase_table[
							state->last_code_point - 0x41];
				}
				else
				{
					*state->dst = 
						basic_latin_uppercase_table[
							state->last_code_point - 0x41];
				}
			}
			else
			{
				/*
					All other code points in Basic Latin are unaffected by case
					mapping
				*/

				*state->dst = (char)state->last_code_point;
			}

			state->dst++;
			state->dst_size--;
		}

		bytes_needed = 1;
	}
	else
	{
		uint8_t resolved_size = 0;

		/* Check if the code point is case mapped */

		if ((state->last_general_category & GeneralCategory_CaseMapped) != 0)
		{
			/* Attempt to resolve the case mapping */

			const char* resolved = database_querydecomposition(
				state->last_code_point,
				state->property_index1,
				state->property_index2,
				state->property_data,
				&resolved_size);

			if (resolved != 0 &&
				resolved_size > 0)
			{
				if (state->dst != 0)
				{
					if (state->dst_size < resolved_size)
					{
						goto outofspace;
					}

					memcpy(state->dst, resolved, resolved_size);

					state->dst += resolved_size;
					state->dst_size -= resolved_size;
				}

				bytes_needed = resolved_size;
			}
		}

		if (resolved_size == 0)
		{
			uint8_t decoded_written = codepoint_write(
				state->last_code_point,
				&state->dst,
				&state->dst_size);

			if (decoded_written == 0)
			{
				goto outofspace;
			}

			bytes_needed = decoded_written;
		}
	}

	return bytes_needed;

outofspace:
	state->src_size = 0;

	return 0;
}
