#include "tests-base.hpp"

#include "helpers-strings.hpp"

#include "utf8rewind.h"

TEST(PerformanceCaseMapping, EveryCodepointUppercase)
{
	char d[16] = { 0 };
	char du[16] = { 0 };

	for (unicode_t u = 0; u < 0x10FFFF; ++u)
	{
		utf32toutf8(&u, sizeof(unicode_t), d, 16, nullptr);
		utf8toupper(d, 16, du, 16, 0, nullptr);
	}
}

TEST(PerformanceCaseMapping, EveryCodepointLowercase)
{
	char d[16] = { 0 };
	char dl[16] = { 0 };

	for (unicode_t u = 0; u < 0x10FFFF; ++u)
	{
		utf32toutf8(&u, sizeof(unicode_t), d, 16, nullptr);
		utf8tolower(d, 16, dl, 16, 0, nullptr);
	}
}

TEST(PerformanceCaseMapping, EveryCodepointLowercaseNormalized)
{
	char d[16] = { 0 };
	char dl[16] = { 0 };

	for (unicode_t u = 0; u < 0x10FFFF; ++u)
	{
		utf32toutf8(&u, sizeof(unicode_t), d, 16, nullptr);
		utf8tolower(d, 16, dl, 16, UTF8_TRANSFORM_NORMALIZED, nullptr);
	}
}

TEST(PerformanceCaseMapping, QuickbrownUppercase)
{
	std::fstream f("testdata/quickbrown.txt", std::ios_base::in);
	ASSERT_TRUE(f.is_open());

	char i[8192] = { 0 };
	f.read(i, 8192);
	ASSERT_EQ(4833, strlen(i));

	char o[8192] = { 0 };
	utf8toupper(i, strlen(i), o, 8192, 0, nullptr);
}

TEST(PerformanceCaseMapping, QuickbrownLowercase)
{
	std::fstream f("testdata/quickbrown.txt", std::ios_base::in);
	ASSERT_TRUE(f.is_open());

	char i[8192] = { 0 };
	f.read(i, 8192);
	ASSERT_EQ(4833, strlen(i));

	char o[8192] = { 0 };
	utf8tolower(i, strlen(i), o, 8192, 0, nullptr);
}

TEST(PerformanceCaseMapping, QuickbrownLowercaseNormalized)
{
	std::fstream f("testdata/quickbrown.txt", std::ios_base::in);
	ASSERT_TRUE(f.is_open());

	char i[8192] = { 0 };
	f.read(i, 8192);
	ASSERT_EQ(4833, strlen(i));

	char o[8192] = { 0 };
	utf8tolower(i, strlen(i), o, 8192, UTF8_TRANSFORM_NORMALIZED, nullptr);
}