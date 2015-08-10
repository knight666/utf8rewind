#include "tests-base.hpp"

#include "helpers-casemapping.hpp"
#include "helpers-strings.hpp"

TEST(SpecialCasing, LithuanianRemoveDotAbove)
{
	// Remove DOT ABOVE after "i" with upper or titlecase

	// U+0049 U+0307
	// U+0069

	EXPECT_STREQ("lt-LT", setlocale(LC_ALL, "lt-LT"));
	EXPECT_CASEMAPPING_EQ("i\xCC\x87", "i\xCC\x87", "I", "I");
	setlocale(LC_ALL, "C");
}