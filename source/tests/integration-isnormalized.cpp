#include "tests-base.hpp"

#include "helpers-normalization.hpp"

TEST(IsNormalized, Latin1Supplement)
{
	CHECK_IS_NORMALIZED(0x000000A0, YES, YES, NO, NO, "NO-BREAK SPACE");
}