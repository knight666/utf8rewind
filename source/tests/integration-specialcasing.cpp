#include "tests-base.hpp"

#include "helpers-casemapping.hpp"
#include "helpers-strings.hpp"

TEST(SpecialCasing, LithuanianRemoveDotAbove)
{
	EXPECT_STREQ("lt-LT", setlocale(LC_ALL, "lt-LT"));

	// Remove DOT ABOVE after "i" with upper or titlecase

	EXPECT_CASEMAPPING_EQ("i\xCC\x87", "i\xCC\x87", "I", "I"); // COMBINING DOT ABOVE

	setlocale(LC_ALL, "C");
}

TEST(SpecialCasing, LithuanianIntroduceExplicitDot)
{
	EXPECT_STREQ("lt-LT", setlocale(LC_ALL, "lt-LT"));

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