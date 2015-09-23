#include "tests-base.hpp"

extern "C" {
	#include "../internal/codepoint.h"
	#include "../internal/database.h"
	#include "../internal/compressedproperties.h"
}

TEST(QueryProperty, AllGeneralCategory)
{
	for (unicode_t i = 0; i <= MAX_LEGAL_UNICODE; ++i)
	{
		uint8_t ve = database_queryproperty(i, UnicodeProperty_GeneralCategory);
		uint8_t va = PROPERTY_GET_GC(i);
		EXPECT_EQ(database_queryproperty(i, UnicodeProperty_GeneralCategory), PROPERTY_GET_GC(i));
		if (ve != va)
		{
			int bleh = 0;
		}
	}
}

TEST(QueryProperty, AllCanonicalCombiningClass)
{
	for (unicode_t i = 0; i <= MAX_LEGAL_UNICODE; ++i)
	{
		uint8_t ve = database_queryproperty(i, UnicodeProperty_CanonicalCombiningClass);
		uint8_t va = PROPERTY_GET_CCC(i);
		EXPECT_EQ(database_queryproperty(i, UnicodeProperty_CanonicalCombiningClass), PROPERTY_GET_CCC(i));
		if (ve != va)
		{
			int bleh = 0;
		}
	}
}

TEST(QueryProperty, AllQuickCheckNFC)
{
	for (unicode_t i = 0; i <= MAX_LEGAL_UNICODE; ++i)
	{
		uint8_t ve = database_queryproperty(i, UnicodeProperty_Normalization_Compose);
		uint8_t va = PROPERTY_GET_NFC(i);
		EXPECT_EQ(database_queryproperty(i, UnicodeProperty_Normalization_Compose), PROPERTY_GET_NFC(i));
		if (ve != va)
		{
			int bleh = 0;
		}
	}
}

TEST(QueryProperty, AllQuickCheckNFD)
{
	for (unicode_t i = 0; i <= MAX_LEGAL_UNICODE; ++i)
	{
		uint8_t ve = database_queryproperty(i, UnicodeProperty_Normalization_Decompose);
		uint8_t va = PROPERTY_GET_NFD(i);
		EXPECT_EQ(database_queryproperty(i, UnicodeProperty_Normalization_Decompose), PROPERTY_GET_NFD(i));
		if (ve != va)
		{
			int bleh = 0;
		}
	}
}

TEST(QueryProperty, AllQuickCheckNFKC)
{
	for (unicode_t i = 0; i <= MAX_LEGAL_UNICODE; ++i)
	{
		uint8_t ve = database_queryproperty(i, UnicodeProperty_Normalization_Compatibility_Compose);
		uint8_t va = PROPERTY_GET_NFKC(i);
		EXPECT_EQ(database_queryproperty(i, UnicodeProperty_Normalization_Compatibility_Compose), PROPERTY_GET_NFKC(i));
		if (ve != va)
		{
			int bleh = 0;
		}
	}
}

TEST(QueryProperty, AllQuickCheckNFKD)
{
	for (unicode_t i = 0; i <= MAX_LEGAL_UNICODE; ++i)
	{
		uint8_t ve = database_queryproperty(i, UnicodeProperty_Normalization_Compatibility_Decompose);
		uint8_t va = PROPERTY_GET_NFKD(i);
		EXPECT_EQ(database_queryproperty(i, UnicodeProperty_Normalization_Compatibility_Decompose), PROPERTY_GET_NFKD(i));
		if (ve != va)
		{
			int bleh = 0;
		}
	}
}

TEST(QueryProperty, InvalidProperty)
{
	EXPECT_EQ(UTF8_INVALID_PROPERTY, database_queryproperty(0x00C0, 198));
}

// General category

TEST(QueryPropertyGeneralCategory, Found)
{
	EXPECT_EQ(GeneralCategory_Letter, database_queryproperty(0x671, UnicodeProperty_GeneralCategory));
	EXPECT_EQ(GeneralCategory_CaseMapped | GeneralCategory_Letter, database_queryproperty(0x10400, UnicodeProperty_GeneralCategory));
	EXPECT_EQ(GeneralCategory_Mark, database_queryproperty(0x16AF0, UnicodeProperty_GeneralCategory));
	EXPECT_EQ(GeneralCategory_Number, database_queryproperty(0x10B78, UnicodeProperty_GeneralCategory));
	EXPECT_EQ(GeneralCategory_Punctuation, database_queryproperty(0x111C5, UnicodeProperty_GeneralCategory));
	EXPECT_EQ(GeneralCategory_Symbol, database_queryproperty(0x1FED, UnicodeProperty_GeneralCategory));
	EXPECT_EQ(GeneralCategory_Separator, database_queryproperty(0x1680, UnicodeProperty_GeneralCategory));
	EXPECT_EQ(GeneralCategory_Other, database_queryproperty(0x200B, UnicodeProperty_GeneralCategory));
}

TEST(QueryPropertyGeneralCategory, FoundFirst)
{
	EXPECT_EQ(GeneralCategory_Other, database_queryproperty(0x0000, UnicodeProperty_GeneralCategory));
}

TEST(QueryPropertyGeneralCategory, FoundLast)
{
	EXPECT_EQ(GeneralCategory_Other, database_queryproperty(0x10FFFD, UnicodeProperty_GeneralCategory));
}

TEST(QueryPropertyGeneralCategory, OutOfUpperBounds)
{
	EXPECT_EQ(0, database_queryproperty(0x1F001A, UnicodeProperty_GeneralCategory));
}

TEST(QueryPropertyGeneralCategory, BetweenBlocks)
{
	EXPECT_EQ(0, database_queryproperty(0x110FC, UnicodeProperty_GeneralCategory));
}

// Canonical combining class

TEST(QueryPropertyCanonicalCombiningClass, Found)
{
	EXPECT_EQ(0xDC, database_queryproperty(0x0655, UnicodeProperty_CanonicalCombiningClass));
}

TEST(QueryPropertyCanonicalCombiningClass, FoundFirst)
{
	EXPECT_EQ(0xE6, database_queryproperty(0x0300, UnicodeProperty_CanonicalCombiningClass));
}

TEST(QueryPropertyCanonicalCombiningClass, FoundLast)
{
	EXPECT_EQ(0xDC, database_queryproperty(0x1E8D6, UnicodeProperty_CanonicalCombiningClass));
}

TEST(QueryPropertyCanonicalCombiningClass, OutOfLowerBounds)
{
	EXPECT_EQ(0, database_queryproperty(0x0118, UnicodeProperty_CanonicalCombiningClass));
}

TEST(QueryPropertyCanonicalCombiningClass, OutOfUpperBounds)
{
	EXPECT_EQ(0, database_queryproperty(0x2A2BB, UnicodeProperty_CanonicalCombiningClass));
}

TEST(QueryPropertyCanonicalCombiningClass, BetweenBlocks)
{
	EXPECT_EQ(0, database_queryproperty(0x0A5C, UnicodeProperty_CanonicalCombiningClass));
}

// Composed

TEST(QueryPropertyComposed, FoundNo)
{
	EXPECT_EQ(QuickCheckResult_No, database_queryproperty(0x1FEE, UnicodeProperty_Normalization_Compose));
}

TEST(QueryPropertyComposed, FoundMaybe)
{
	EXPECT_EQ(QuickCheckResult_Maybe, database_queryproperty(0x0DCF, UnicodeProperty_Normalization_Compose));
}

TEST(QueryPropertyComposed, FoundFirst)
{
	EXPECT_EQ(QuickCheckResult_Maybe, database_queryproperty(0x0300, UnicodeProperty_Normalization_Compose));
}

TEST(QueryPropertyComposed, FoundLast)
{
	EXPECT_EQ(QuickCheckResult_No, database_queryproperty(0x2FA1D, UnicodeProperty_Normalization_Compose));
}

TEST(QueryPropertyComposed, OutOfLowerBounds)
{
	EXPECT_EQ(QuickCheckResult_Yes, database_queryproperty(0x00FF, UnicodeProperty_Normalization_Compose));
}

TEST(QueryPropertyComposed, OutOfUpperBounds)
{
	EXPECT_EQ(QuickCheckResult_Yes, database_queryproperty(0x30000, UnicodeProperty_Normalization_Compose));
}

TEST(QueryPropertyComposed, BetweenBlocks)
{
	EXPECT_EQ(QuickCheckResult_Yes, database_queryproperty(0x2B81, UnicodeProperty_Normalization_Compose));
}

// Decomposed

TEST(QueryPropertyDecomposed, FoundNo)
{
	EXPECT_EQ(QuickCheckResult_No, database_queryproperty(0x0374, UnicodeProperty_Normalization_Decompose));
}

TEST(QueryPropertyDecomposed, FoundFirst)
{
	EXPECT_EQ(QuickCheckResult_No, database_queryproperty(0x00C0, UnicodeProperty_Normalization_Decompose));
}

TEST(QueryPropertyDecomposed, FoundLast)
{
	EXPECT_EQ(QuickCheckResult_No, database_queryproperty(0x2FA1D, UnicodeProperty_Normalization_Decompose));
}

TEST(QueryPropertyDecomposed, OutOfLowerBounds)
{
	EXPECT_EQ(QuickCheckResult_Yes, database_queryproperty(0x007F, UnicodeProperty_Normalization_Decompose));
}

TEST(QueryPropertyDecomposed, OutOfUpperBounds)
{
	EXPECT_EQ(QuickCheckResult_Yes, database_queryproperty(0x30000, UnicodeProperty_Normalization_Decompose));
}

TEST(QueryPropertyDecomposed, BetweenBlocks)
{
	EXPECT_EQ(QuickCheckResult_Yes, database_queryproperty(0xFB20, UnicodeProperty_Normalization_Decompose));
}

TEST(QueryPropertyDecomposed, HangulSyllable)
{
	EXPECT_EQ(QuickCheckResult_No, database_queryproperty(0xB1A1, UnicodeProperty_Normalization_Decompose));
}

TEST(QueryPropertyDecomposed, HangulSyllableStart)
{
	EXPECT_EQ(QuickCheckResult_No, database_queryproperty(0xAC00, UnicodeProperty_Normalization_Decompose));
}

TEST(QueryPropertyDecomposed, HangulSyllableEnd)
{
	EXPECT_EQ(QuickCheckResult_No, database_queryproperty(0xD7A3, UnicodeProperty_Normalization_Decompose));
}

// Compatibility composed

TEST(QueryPropertyCompatibilityComposed, FoundNo)
{
	EXPECT_EQ(QuickCheckResult_No, database_queryproperty(0x0A5B, UnicodeProperty_Normalization_Compatibility_Compose));
}

TEST(QueryPropertyCompatibilityComposed, FoundMaybe)
{
	EXPECT_EQ(QuickCheckResult_Maybe, database_queryproperty(0x0B57, UnicodeProperty_Normalization_Compatibility_Compose));
}

TEST(QueryPropertyCompatibilityComposed, FoundFirst)
{
	EXPECT_EQ(QuickCheckResult_No, database_queryproperty(0x00A0, UnicodeProperty_Normalization_Compatibility_Compose));
}

TEST(QueryPropertyCompatibilityComposed, FoundLast)
{
	EXPECT_EQ(QuickCheckResult_No, database_queryproperty(0x2FA1D, UnicodeProperty_Normalization_Compatibility_Compose));
}

TEST(QueryPropertyCompatibilityComposed, OutOfLowerBounds)
{
	EXPECT_EQ(QuickCheckResult_Yes, database_queryproperty(0x007F, UnicodeProperty_Normalization_Compatibility_Compose));
}

TEST(QueryPropertyCompatibilityComposed, OutOfUpperBounds)
{
	EXPECT_EQ(QuickCheckResult_Yes, database_queryproperty(0x30000, UnicodeProperty_Normalization_Compatibility_Compose));
}

TEST(QueryPropertyCompatibilityComposed, BetweenBlocks)
{
	EXPECT_EQ(QuickCheckResult_Yes, database_queryproperty(0x1EE5C, UnicodeProperty_Normalization_Compatibility_Compose));
}

// Compatibility decomposed

TEST(QueryPropertyCompatibilityDecomposed, FoundNo)
{
	EXPECT_EQ(QuickCheckResult_No, database_queryproperty(0x03F9, UnicodeProperty_Normalization_Compatibility_Decompose));
}

TEST(QueryPropertyCompatibilityDecomposed, FoundFirst)
{
	EXPECT_EQ(QuickCheckResult_No, database_queryproperty(0x00A0, UnicodeProperty_Normalization_Compatibility_Decompose));
}

TEST(QueryPropertyCompatibilityDecomposed, FoundLast)
{
	EXPECT_EQ(QuickCheckResult_No, database_queryproperty(0x2FA1D, UnicodeProperty_Normalization_Compatibility_Decompose));
}

TEST(QueryPropertyCompatibilityDecomposed, OutOfLowerBounds)
{
	EXPECT_EQ(QuickCheckResult_Yes, database_queryproperty(0x007F, UnicodeProperty_Normalization_Compatibility_Decompose));
}

TEST(QueryPropertyCompatibilityDecomposed, OutOfUpperBounds)
{
	EXPECT_EQ(QuickCheckResult_Yes, database_queryproperty(0x30000, UnicodeProperty_Normalization_Compatibility_Decompose));
}

TEST(QueryPropertyCompatibilityDecomposed, BetweenBlocks)
{
	EXPECT_EQ(QuickCheckResult_Yes, database_queryproperty(0x038D, UnicodeProperty_Normalization_Compatibility_Decompose));
}

TEST(QueryPropertyCompatibilityDecomposed, HangulSyllable)
{
	EXPECT_EQ(QuickCheckResult_No, database_queryproperty(0xCCCA, UnicodeProperty_Normalization_Compatibility_Decompose));
}

TEST(QueryPropertyCompatibilityDecomposed, HangulSyllableStart)
{
	EXPECT_EQ(QuickCheckResult_No, database_queryproperty(0xAC00, UnicodeProperty_Normalization_Compatibility_Decompose));
}

TEST(QueryPropertyCompatibilityDecomposed, HangulSyllableEnd)
{
	EXPECT_EQ(QuickCheckResult_No, database_queryproperty(0xD7A3, UnicodeProperty_Normalization_Compatibility_Decompose));
}