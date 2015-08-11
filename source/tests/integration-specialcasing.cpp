#include "tests-base.hpp"

#include "helpers-casemapping.hpp"
#include "helpers-strings.hpp"

TEST(SpecialCasing, Greek)
{
#if _WINDOWS
	EXPECT_STREQ("greek", setlocale(LC_ALL, "greek"));
#else
	EXPECT_STREQ("el_GR", setlocale(LC_ALL, "el_GR"));
#endif

	// Special case for final form of sigma

	// GREEK CAPITAL LETTER SIGMA
	EXPECT_CASEMAPPING_EQ("\xE1\xBE\xB7\xCE\xA3", "\xE1\xBE\xB7\xCF\x82", "\xCE\x91\xCD\x82\xCE\x99\xCE\xA3", "\xCE\x91\xCD\x82\xCD\x85\xCE\xA3");

	// Note: the following cases for non-final are already in the UnicodeData.txt file.

	// GREEK CAPITAL LETTER SIGMA
	EXPECT_CASEMAPPING_EQ("\xCE\xA3", "\xCF\x83", "\xCE\xA3", "\xCE\xA3");

	// GREEK SMALL LETTER SIGMA
	EXPECT_CASEMAPPING_EQ("\xCF\x83", "\xCF\x83", "\xCE\xA3", "\xCE\xA3");

	// GREEK SMALL LETTER FINAL SIGMA
	EXPECT_CASEMAPPING_EQ("\xCF\x82", "\xCF\x82", "\xCE\xA3", "\xCE\xA3");

	setlocale(LC_ALL, "C");
}

TEST(SpecialCasing, Lithuanian)
{
#if _WINDOWS
	EXPECT_STREQ("lithuanian", setlocale(LC_ALL, "lithuanian"));
#else
	EXPECT_STREQ("lt_LT", setlocale(LC_ALL, "lt_LT"));
#endif
	// Remove DOT ABOVE after "i" with upper or titlecase

	// COMBINING DOT ABOVE
	EXPECT_CASEMAPPING_EQ("i\xCC\x87", "i\xCC\x87", "I", "I");

	// Introduce an explicit dot above when lowercasing capital I's and J's
	// whenever there are more accents above.
	// (of the accents used in Lithuanian: grave, acute, tilde above, and ogonek)

	// LATIN CAPITAL LETTER I
	EXPECT_CASEMAPPING_EQ("I", "i", "I", "I");

	// LATIN CAPITAL LETTER J
	EXPECT_CASEMAPPING_EQ("J", "j", "J", "J");

	// LATIN CAPITAL LETTER I WITH OGONEK
	EXPECT_CASEMAPPING_EQ("\xC4\xAE", "\xC4\xAF\xCC\x87", "\xC4\xAE", "\xC4\xAE");
	EXPECT_CASEMAPPING_EQ("I\xCC\xA8", "I\xCC\xA8\xCC\x87", "I\xCC\xA8", "I\xCC\xA8");

	// LATIN CAPITAL LETTER I WITH GRAVE
	EXPECT_CASEMAPPING_EQ("\xC3\x8C", "i\xCC\x87\xCC\x80", "\xC3\x8C", "\xC3\x8C");
	EXPECT_CASEMAPPING_EQ("I\xCC\x80", "i\xCC\x87\xCC\x80", "I\xCC\x80", "I\xCC\x80");

	// LATIN CAPITAL LETTER I WITH ACUTE
	EXPECT_CASEMAPPING_EQ("\xC3\x8D", "i\xCC\x87\xCC\x81", "\xC3\x8D", "\xC3\x8D");
	EXPECT_CASEMAPPING_EQ("I\xCC\x81", "i\xCC\x87\xCC\x81", "I\xCC\x81", "I\xCC\x81");

	// LATIN CAPITAL LETTER I WITH TILDE
	EXPECT_CASEMAPPING_EQ("\xC4\xA8", "i\xCC\x87\xCC\x83", "\xC4\xA8", "\xC4\xA8");
	EXPECT_CASEMAPPING_EQ("I\xCC\x83", "i\xCC\x87\xCC\x83", "I\xCC\x83", "I\xCC\x83");

	setlocale(LC_ALL, "C");
}

TEST(SpecialCasing, Turkish)
{
#if _WINDOWS
	EXPECT_STREQ("turkish", setlocale(LC_ALL, "turkish"));
#else
	EXPECT_STREQ("tr_TR", setlocale(LC_ALL, "tr_TR"));
#endif

	// I and i-dotless; I-dot and i are case pairs in Turkish and Azeri
	// The following rules handle those cases.

	// LATIN CAPITAL LETTER I WITH DOT ABOVE
	EXPECT_CASEMAPPING_EQ("\xC4\xB0", "i", "\xC4\xB0", "\xC4\xB0");

	// When lowercasing, remove dot_above in the sequence I + dot_above, which will turn into i.
	// This matches the behavior of the canonically equivalent I-dot_above

	// COMBINING DOT ABOVE
	EXPECT_CASEMAPPING_EQ("I\xCC\x87", "i", "I\xCC\x87", "I\xCC\x87");

	// When lowercasing, unless an I is before a dot_above, it turns into a dotless i.

	// LATIN CAPITAL LETTER I
	EXPECT_CASEMAPPING_EQ("I", "\xC4\xB1", "I", "I");

	// When uppercasing, i turns into a dotted capital I

	// LATIN SMALL LETTER I
	EXPECT_CASEMAPPING_EQ("i", "i", "\xC4\xB1", "\xC4\xB1");

	// Note: the following case is already in the UnicodeData.txt file.

	// LATIN SMALL LETTER DOTLESS I
	EXPECT_CASEMAPPING_EQ("\xC4\xB1", "\xC4\xB1", "I", "I");

	setlocale(LC_ALL, "C");
}

TEST(SpecialCasing, Azeri)
{
#if _WINDOWS
	EXPECT_STREQ("azeri", setlocale(LC_ALL, "azeri"));
#else
	EXPECT_STREQ("az_AZ", setlocale(LC_ALL, "az_AZ"));
#endif

	// I and i-dotless; I-dot and i are case pairs in Turkish and Azeri
	// The following rules handle those cases.

	// LATIN CAPITAL LETTER I WITH DOT ABOVE
	EXPECT_CASEMAPPING_EQ("\xC4\xB0", "i", "\xC4\xB0", "\xC4\xB0");

	// When lowercasing, remove dot_above in the sequence I + dot_above, which will turn into i.
	// This matches the behavior of the canonically equivalent I-dot_above

	// COMBINING DOT ABOVE
	EXPECT_CASEMAPPING_EQ("I\xCC\x87", "i", "I\xCC\x87", "I\xCC\x87");

	// When lowercasing, unless an I is before a dot_above, it turns into a dotless i.

	// LATIN CAPITAL LETTER I
	EXPECT_CASEMAPPING_EQ("I", "\xC4\xB1", "I", "I");

	// When uppercasing, i turns into a dotted capital I

	// LATIN SMALL LETTER I
	EXPECT_CASEMAPPING_EQ("i", "i", "\xC4\xB1", "\xC4\xB1");

	// Note: the following case is already in the UnicodeData.txt file.

	// LATIN SMALL LETTER DOTLESS I
	EXPECT_CASEMAPPING_EQ("\xC4\xB1", "\xC4\xB1", "I", "I");

	setlocale(LC_ALL, "C");
}