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

#ifndef _UTF8NORMALIZATION_H_
#define _UTF8NORMALIZATION_H_

#include "utf8rewind.h"

enum CodepointLabel
{
	CodepointLabel_Reserved,
	CodepointLabel_NonCharacter,
	CodepointLabel_Control,
	CodepointLabel_PrivateUse,
	CodepointLabel_Surrogate
};

enum GeneralCategory
{
	GeneralCategory_UppercaseLetter,
	GeneralCategory_LowercaseLetter,
	GeneralCategory_TitlecaseLetter,
	GeneralCategory_ModifierLetter,
	GeneralCategory_OtherLetter,
	GeneralCategory_NonspacingMark,
	GeneralCategory_SpacingMark,
	GeneralCategory_EnclosingMark,
	GeneralCategory_DecimalNumber,
	GeneralCategory_LetterNumber,
	GeneralCategory_OtherNumber,
	GeneralCategory_ConnectorPunctuation,
	GeneralCategory_DashPunctuation,
	GeneralCategory_OpenPunctuation,
	GeneralCategory_ClosePunctuation,
	GeneralCategory_InitialPunctuation,
	GeneralCategory_FinalPunctuation,
	GeneralCategory_OtherPunctuation,
	GeneralCategory_MathSymbol,
	GeneralCategory_CurrencySymbol,
	GeneralCategory_ModifierSymbol,
	GeneralCategory_OtherSymbol,
	GeneralCategory_SpaceSeparator,
	GeneralCategory_LineSeparator,
	GeneralCategory_ParagraphSeparator,
	GeneralCategory_Control,
	GeneralCategory_Format,
	GeneralCategory_Surrogate,
	GeneralCategory_PrivateUse,
	GeneralCategory_Unassigned,
};

enum CanonicalCombiningClass
{
	CanonicalCombiningClass_NotReordered,
	CanonicalCombiningClass_Overlay,
	CanonicalCombiningClass_Nukta,
	CanonicalCombiningClass_KanaVoicing,
	CanonicalCombiningClass_Virama,
	CanonicalCombiningClass_FixedPositionStart,
	CanonicalCombiningClass_FixedPositionEnd,
	CanonicalCombiningClass_AttachedBelowLeft,
	CanonicalCombiningClass_AttachedBelow,
	CanonicalCombiningClass_AttachedTopRight,
	CanonicalCombiningClass_AttachedLeft,
	CanonicalCombiningClass_AttachedRight,
	CanonicalCombiningClass_AttachedTopLeft,
	CanonicalCombiningClass_AttachedAbove,
	CanonicalCombiningClass_AttachedAboveRight,
	CanonicalCombiningClass_BelowLeft,
	CanonicalCombiningClass_Below,
	CanonicalCombiningClass_BelowRight,
	CanonicalCombiningClass_Left,
	CanonicalCombiningClass_Right,
	CanonicalCombiningClass_AboveLeft,
	CanonicalCombiningClass_Above,
	CanonicalCombiningClass_AboveRight,
	CanonicalCombiningClass_DoubleBelow,
	CanonicalCombiningClass_DoubleAbove,
	CanonicalCombiningClass_IotaSubscript,
};

enum BidiClass
{
	BidiClass_LeftToRight,
	BidiClass_LeftToRightEmbedding,
	BidiClass_LeftToRightOverride,
	BidiClass_RightToLeft,
	BidiClass_ArabicLetter,
	BidiClass_RightToLeftEmbedding,
	BidiClass_RightToLeftOverride,
	BidiClass_PopDirectionalFormat,
	BidiClass_EuropeanNumber,
	BidiClass_EuropeanSeparator,
	BidiClass_EuropeanTerminator,
	BidiClass_ArabicNumber,
	BidiClass_CommonSeparator,
	BidiClass_NonspacingMark,
	BidiClass_BoundaryNeutral,
	BidiClass_ParagraphSeparator,
	BidiClass_SegmentSeparator,
	BidiClass_WhiteSpace,
	BidiClass_OtherNeutral,
	BidiClass_LeftToRightIsolate,
	BidiClass_RightToLeftIsolate,
	BidiClass_FirstStrongIsolate,
	BidiClass_PopDirectionalIsolate,
};

enum DecompositionType
{
	DecompositionType_Canonical,
	DecompositionType_Font,
	DecompositionType_NoBreak,
	DecompositionType_InitialArabic,
	DecompositionType_MedialArabic,
	DecompositionType_FinalArabic,
	DecompositionType_IsolatedArabic,
	DecompositionType_Circle,
	DecompositionType_Superscript,
	DecompositionType_Subscript,
	DecompositionType_Vertical,
	DecompositionType_Wide,
	DecompositionType_Narrow,
	DecompositionType_Small,
	DecompositionType_SquaredCJK,
	DecompositionType_Fraction,
	DecompositionType_Unspecified,
};

enum NumericType
{
	NumericType_None,
	NumericType_Decimal,
	NumericType_Digit,
	NumericType_Numeric,
};

typedef struct {
	unicode_t codepoint;
	int8_t generalCategory;
	int8_t canonicalCombiningClass;
	int8_t bidiClass;
	ptrdiff_t offsetNFC;
	ptrdiff_t offsetNFD;
	ptrdiff_t offsetNFKC;
	ptrdiff_t offsetNFKD;
	size_t numericType;
	size_t numericValue;
	int8_t bidiMirrored;
} UnicodeRecord;

typedef struct {
	unicode_t codepoint;
	ptrdiff_t offsetC;
	ptrdiff_t offsetD;
	ptrdiff_t offsetKC;
	ptrdiff_t offsetKD;
} CompositionEntry;

enum FindResult
{
	FindResult_Found,
	FindResult_Missing,
	FindResult_OutOfBounds,
	FindResult_Invalid
};

const CompositionEntry* findcomposition(unicode_t codepoint, int32_t* result);

const char* resolvedecomposition(size_t offset, int32_t* result);

#endif
