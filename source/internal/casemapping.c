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

static const uint8_t basic_latin_general_category_table[128] = {
	GC_OT, GC_OT, GC_OT, GC_OT, GC_OT, GC_OT, GC_OT, GC_OT,
	GC_OT, GC_OT, GC_OT, GC_OT, GC_OT, GC_OT, GC_OT, GC_OT,
	GC_OT, GC_OT, GC_OT, GC_OT, GC_OT, GC_OT, GC_OT, GC_OT,
	GC_OT, GC_OT, GC_OT, GC_OT, GC_OT, GC_OT, GC_OT, GC_OT,
	GC_SE, GC_PU, GC_PU, GC_PU, GC_SY, GC_PU, GC_PU, GC_PU,
	GC_PU, GC_PU, GC_PU, GC_SY, GC_PU, GC_PU, GC_PU, GC_PU,
	GC_NU, GC_NU, GC_NU, GC_NU, GC_NU, GC_NU, GC_NU, GC_NU,
	GC_NU, GC_NU, GC_PU, GC_PU, GC_SY, GC_SY, GC_SY, GC_PU,
	GC_PU, GC_LC, GC_LC, GC_LC, GC_LC, GC_LC, GC_LC, GC_LC,
	GC_LC, GC_LC, GC_LC, GC_LC, GC_LC, GC_LC, GC_LC, GC_LC,
	GC_LC, GC_LC, GC_LC, GC_LC, GC_LC, GC_LC, GC_LC, GC_LC,
	GC_LC, GC_LC, GC_LC, GC_PU, GC_PU, GC_PU, GC_SY, GC_PU,
	GC_SY, GC_LC, GC_LC, GC_LC, GC_LC, GC_LC, GC_LC, GC_LC,
	GC_LC, GC_LC, GC_LC, GC_LC, GC_LC, GC_LC, GC_LC, GC_LC,
	GC_LC, GC_LC, GC_LC, GC_LC, GC_LC, GC_LC, GC_LC, GC_LC,
	GC_LC, GC_LC, GC_LC, GC_PU, GC_SY, GC_PU, GC_SY, GC_OT
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

uint8_t casemapping_initialize(CaseMappingState* state, const char* input, size_t inputSize, char* target, size_t targetSize, uint8_t property)
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
	state->property = property;

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

size_t casemapping_execute(CaseMappingState* state)
{
	uint8_t decoded_size;
	size_t written = 0;

	if (state->src_size == 0)
	{
		return 0;
	}

	if ((*state->src & 0x80) == 0)
	{
		/* Basic Latin does not have to be converted to UTF-32 */

		uint8_t decoded = (uint8_t)*state->src;
		decoded_size = 1;

		if (state->dst != 0)
		{
			if (state->dst_size < 1)
			{
				goto outofspace;
			}

			if (decoded >= 0x41 &&
				decoded <= 0x7A)
			{
				/* Uppercase letters are U+0041 ('A') to U+005A ('Z') */
				/* Lowercase letters are U+0061 ('a') to U+007A ('z') */

				if (state->property == UnicodeProperty_Lowercase)
				{
					*state->dst = basic_latin_lowercase_table[decoded - 0x41];
				}
				else
				{
					*state->dst = basic_latin_uppercase_table[decoded - 0x41];
				}
			}
			else
			{
				/*
					All other code points in Basic Latin are unaffected by case
					mapping
				*/

				*state->dst = *state->src;
			}

			state->dst++;
			state->dst_size--;
		}

		/* Store code point's general category */

		state->last_general_category = basic_latin_general_category_table[decoded];

		written++;
	}
	else
	{
		unicode_t decoded;
		const char* resolved;
		size_t resolved_size = 0;

		/* Decode current code point */

		decoded_size = codepoint_read(state->src, state->src_size, &decoded);

		/* Check if the code point's general category indicates case mapping */

		state->last_general_category = database_queryproperty(decoded, UnicodeProperty_GeneralCategory);
		if ((state->last_general_category & GeneralCategory_CaseMapped) != 0)
		{
			/* Resolve the code point's decomposition */

			resolved = database_querydecomposition(decoded, state->property, &resolved_size);
			if (resolved != 0)
			{
				/* Copy the decomposition to the output buffer */

				if (state->dst != 0 &&
					resolved_size > 0)
				{
					if (state->dst_size < resolved_size)
					{
						goto outofspace;
					}

					memcpy(state->dst, resolved, resolved_size);

					state->dst += resolved_size;
					state->dst_size -= resolved_size;
				}

				written += resolved_size;
			}
		}

		/* Check if codepoint was unaffected */

		if (resolved_size == 0)
		{
			/*
				Write the codepoint to the output buffer
				This ensures that invalid code points in the input are always
				converted to U+FFFD in the output
			*/

			uint8_t decoded_written = codepoint_write(decoded, &state->dst, &state->dst_size);
			if (decoded_written == 0)
			{
				goto outofspace;
			}

			written += decoded_written;
		}
	}

	/* Invalid code points can be longer than the source length indicates */

	if (state->src_size >= decoded_size)
	{
		state->src += decoded_size;
		state->src_size -= decoded_size;
	}
	else
	{
		state->src_size = 0;
	}

	return written;

outofspace:
	state->src_size = 0;

	return 0;
}