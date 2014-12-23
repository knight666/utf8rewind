#include "tests-base.hpp"

#include "helpers-normalization.hpp"
#include "helpers-strings.hpp"

TEST(Normalization, SpecificCases)
{
	CHECK_NORMALIZE(0x1E0A, "D\xCC\x87", "\xE1\xB8\x8A");
	CHECK_NORMALIZE(0x1E0C, "D\xCC\xA3", "\xE1\xB8\x8C");
}

TEST(Normalization, SpecificCasesCompatibility)
{
	CHECK_NORMALIZE_COMPATIBILITY(0x1E0A, "D\xCC\x87", "\xE1\xB8\x8A");
	CHECK_NORMALIZE_COMPATIBILITY(0x1E0C, "D\xCC\xA3", "\xE1\xB8\x8C");
}