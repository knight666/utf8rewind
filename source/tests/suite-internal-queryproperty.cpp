#include "tests-base.hpp"

extern "C" {
	#include "../normalization.h"
}

TEST(QueryProperty, NoResult)
{
	EXPECT_EQ(0, queryproperty(0x81288, UnicodeProperty_QC_NFC));
}

TEST(QueryProperty, InvalidNormalizationForm)
{
	EXPECT_EQ(UTF8_INVALID_PROPERTY, queryproperty(0x00C0, 198));
}

// Composed

TEST(QueryProperty, ComposedFoundNo)
{
	EXPECT_EQ(QuickCheckResult_No, queryproperty(0x1FEE, UnicodeProperty_QC_NFC));
}

TEST(QueryProperty, ComposedFoundMaybe)
{
	EXPECT_EQ(QuickCheckResult_Maybe, queryproperty(0x0DCF, UnicodeProperty_QC_NFC));
}

TEST(QueryProperty, ComposedFoundFirst)
{
	EXPECT_EQ(QuickCheckResult_Maybe, queryproperty(0x0300, UnicodeProperty_QC_NFC));
}

TEST(QueryProperty, ComposedFoundLast)
{
	EXPECT_EQ(QuickCheckResult_No, queryproperty(0x2FA1D, UnicodeProperty_QC_NFC));
}

TEST(QueryProperty, ComposedOutOfLowerBounds)
{
	EXPECT_EQ(QuickCheckResult_Yes, queryproperty(0x00FF, UnicodeProperty_QC_NFC));
}

TEST(QueryProperty, ComposedOutOfUpperBounds)
{
	EXPECT_EQ(QuickCheckResult_Yes, queryproperty(0x30000, UnicodeProperty_QC_NFC));
}

TEST(QueryProperty, ComposedBetweenBlocks)
{
	EXPECT_EQ(QuickCheckResult_Yes, queryproperty(0x2B81, UnicodeProperty_QC_NFC));
}

// Decomposed

TEST(QueryProperty, DecomposedFoundNo)
{
	EXPECT_EQ(QuickCheckResult_No, queryproperty(0x0374, UnicodeProperty_QC_NFD));
}

TEST(QueryProperty, DecomposedFoundFirst)
{
	EXPECT_EQ(QuickCheckResult_No, queryproperty(0x00C0, UnicodeProperty_QC_NFD));
}

TEST(QueryProperty, DecomposedFoundLast)
{
	EXPECT_EQ(QuickCheckResult_No, queryproperty(0x2FA1D, UnicodeProperty_QC_NFD));
}

TEST(QueryProperty, DecomposedOutOfLowerBounds)
{
	EXPECT_EQ(QuickCheckResult_Yes, queryproperty(0x007F, UnicodeProperty_QC_NFD));
}

TEST(QueryProperty, DecomposedOutOfUpperBounds)
{
	EXPECT_EQ(QuickCheckResult_Yes, queryproperty(0x30000, UnicodeProperty_QC_NFD));
}

TEST(QueryProperty, DecomposedBetweenBlocks)
{
	EXPECT_EQ(QuickCheckResult_Yes, queryproperty(0xFB20, UnicodeProperty_QC_NFD));
}

TEST(QueryProperty, DecomposedHangulSyllable)
{
	EXPECT_EQ(QuickCheckResult_No, queryproperty(0xB1A1, UnicodeProperty_QC_NFD));
}

TEST(QueryProperty, DecomposedHangulSyllableStart)
{
	EXPECT_EQ(QuickCheckResult_No, queryproperty(0xAC00, UnicodeProperty_QC_NFD));
}

TEST(QueryProperty, DecomposedHangulSyllableEnd)
{
	EXPECT_EQ(QuickCheckResult_No, queryproperty(0xD7A3, UnicodeProperty_QC_NFD));
}

// Compatibility composed

TEST(QueryProperty, CompatibilityComposedFoundNo)
{
	EXPECT_EQ(QuickCheckResult_No, queryproperty(0x0A5B, UnicodeProperty_QC_NFKC));
}

TEST(QueryProperty, CompatibilityComposedFoundMaybe)
{
	EXPECT_EQ(QuickCheckResult_Maybe, queryproperty(0x0B57, UnicodeProperty_QC_NFKC));
}

TEST(QueryProperty, CompatibilityComposedFoundFirst)
{
	EXPECT_EQ(QuickCheckResult_No, queryproperty(0x00A0, UnicodeProperty_QC_NFKC));
}

TEST(QueryProperty, CompatibilityComposedFoundLast)
{
	EXPECT_EQ(QuickCheckResult_No, queryproperty(0x2FA1D, UnicodeProperty_QC_NFKC));
}

TEST(QueryProperty, CompatibilityComposedOutOfLowerBounds)
{
	EXPECT_EQ(QuickCheckResult_Yes, queryproperty(0x007F, UnicodeProperty_QC_NFKC));
}

TEST(QueryProperty, CompatibilityComposedOutOfUpperBounds)
{
	EXPECT_EQ(QuickCheckResult_Yes, queryproperty(0x30000, UnicodeProperty_QC_NFKC));
}

TEST(QueryProperty, CompatibilityComposedBetweenBlocks)
{
	EXPECT_EQ(QuickCheckResult_Yes, queryproperty(0x1EE5C, UnicodeProperty_QC_NFKC));
}

// Compatibility decomposed

TEST(QueryProperty, CompatibilityDecomposedFoundNo)
{
	EXPECT_EQ(QuickCheckResult_No, queryproperty(0x03F9, UnicodeProperty_QC_NFKD));
}

TEST(QueryProperty, CompatibilityDecomposedFoundFirst)
{
	EXPECT_EQ(QuickCheckResult_No, queryproperty(0x00A0, UnicodeProperty_QC_NFKD));
}

TEST(QueryProperty, CompatibilityDecomposedFoundLast)
{
	EXPECT_EQ(QuickCheckResult_No, queryproperty(0x2FA1D, UnicodeProperty_QC_NFKD));
}

TEST(QueryProperty, CompatibilityDecomposedOutOfLowerBounds)
{
	EXPECT_EQ(QuickCheckResult_Yes, queryproperty(0x007F, UnicodeProperty_QC_NFKD));
}

TEST(QueryProperty, CompatibilityDecomposedOutOfUpperBounds)
{
	EXPECT_EQ(QuickCheckResult_Yes, queryproperty(0x30000, UnicodeProperty_QC_NFKD));
}

TEST(QueryProperty, CompatibilityDecomposedBetweenBlocks)
{
	EXPECT_EQ(QuickCheckResult_Yes, queryproperty(0x038D, UnicodeProperty_QC_NFKD));
}

TEST(QueryProperty, CompatibilityDecomposedHangulSyllable)
{
	EXPECT_EQ(QuickCheckResult_No, queryproperty(0xCCCA, UnicodeProperty_QC_NFKD));
}

TEST(QueryProperty, CompatibilityDecomposedHangulSyllableStart)
{
	EXPECT_EQ(QuickCheckResult_No, queryproperty(0xAC00, UnicodeProperty_QC_NFKD));
}

TEST(QueryProperty, CompatibilityDecomposedHangulSyllableEnd)
{
	EXPECT_EQ(QuickCheckResult_No, queryproperty(0xD7A3, UnicodeProperty_QC_NFKD));
}

// Uppercase

TEST(QueryProperty, UppercaseFound)
{
	EXPECT_EQ(1, queryproperty(0x0101, UnicodeProperty_Uppercase));
}

TEST(QueryProperty, UppercaseFoundFirst)
{
	EXPECT_EQ(1, queryproperty(0x0061, UnicodeProperty_Uppercase));
}

TEST(QueryProperty, UppercaseFoundLast)
{
	EXPECT_EQ(1, queryproperty(0x118DF, UnicodeProperty_Uppercase));
}

TEST(QueryProperty, UppercaseOutOfLowerBounds)
{
	EXPECT_EQ(0, queryproperty(0x0020, UnicodeProperty_Uppercase));
}

TEST(QueryProperty, UppercaseOutOfUpperBounds)
{
	EXPECT_EQ(0, queryproperty(0x0020, UnicodeProperty_Uppercase));
}

TEST(QueryProperty, UppercaseBetweenBlocks)
{
	EXPECT_EQ(0, queryproperty(0x2C5F, UnicodeProperty_Uppercase));
}

// Lowercase

TEST(QueryProperty, LowercaseFound)
{
	EXPECT_EQ(1, queryproperty(0x01B5, UnicodeProperty_Lowercase));
}

TEST(QueryProperty, LowercaseFoundFirst)
{
	EXPECT_EQ(1, queryproperty(0x0041, UnicodeProperty_Lowercase));
}

TEST(QueryProperty, LowercaseFoundLast)
{
	EXPECT_EQ(1, queryproperty(0x118Bf, UnicodeProperty_Lowercase));
}

TEST(QueryProperty, LowercaseOutOfLowerBounds)
{
	EXPECT_EQ(0, queryproperty(0x002F, UnicodeProperty_Lowercase));
}

TEST(QueryProperty, LowercaseOutOfUpperBounds)
{
	EXPECT_EQ(0, queryproperty(0x121F4, UnicodeProperty_Lowercase));
}

TEST(QueryProperty, LowercaseBetweenBlocks)
{
	EXPECT_EQ(0, queryproperty(0x2180, UnicodeProperty_Lowercase));
}

// Titlecase

TEST(QueryProperty, TitlecaseFound)
{
	EXPECT_EQ(1, queryproperty(0x028B, UnicodeProperty_Titlecase));
}

TEST(QueryProperty, TitlecaseFoundFirst)
{
	EXPECT_EQ(1, queryproperty(0x0061, UnicodeProperty_Titlecase));
}

TEST(QueryProperty, TitlecaseFoundLast)
{
	EXPECT_EQ(1, queryproperty(0x118DF, UnicodeProperty_Titlecase));
}

TEST(QueryProperty, TitlecaseOutOfLowerBounds)
{
	EXPECT_EQ(0, queryproperty(0x0018, UnicodeProperty_Titlecase));
}

TEST(QueryProperty, TitlecaseOutOfUpperBounds)
{
	EXPECT_EQ(0, queryproperty(0x1D20FF, UnicodeProperty_Titlecase));
}

TEST(QueryProperty, TitlecaseBetweenBlocks)
{
	EXPECT_EQ(0, queryproperty(0x2180, UnicodeProperty_Titlecase));
}