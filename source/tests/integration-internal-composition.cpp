#include "tests-base.hpp"

extern "C" {
	#include "../internal/database.h"
	#include "../internal/normalization.h"
}

#include "helpers-strings.hpp"
#include "helpers-normalization.hpp"

TEST(Composition, SingletonAngstrom)
{
	// NFD

	{
		const char* d = database_querydecomposition(0x212B, UnicodeProperty_Normalization_Decompose);
		ASSERT_NE(nullptr, d);

		EXPECT_UTF8EQ("A\xCC\x8A", d);
		EXPECT_STREQ("A\\u30A", helpers::identifiable(d).c_str());
	}

	// NFKD

	{
		const char* d = database_querydecomposition(0x212B, UnicodeProperty_Normalization_Compatibility_Decompose);
		ASSERT_NE(nullptr, d);

		EXPECT_UTF8EQ("A\xCC\x8A", d);
		EXPECT_STREQ("A\\u30A", helpers::identifiable(d).c_str());
	}
}

TEST(Composition, SingletonOhm)
{
	// NFD

	{
		const char* d = database_querydecomposition(0x2126, UnicodeProperty_Normalization_Decompose);
		ASSERT_NE(nullptr, d);

		EXPECT_UTF8EQ("\xCE\xA9", d);
		EXPECT_STREQ("\\u3A9", helpers::identifiable(d).c_str());
	}

	// NFKD

	{
		const char* d = database_querydecomposition(0x2126, UnicodeProperty_Normalization_Compatibility_Decompose);
		ASSERT_NE(nullptr, d);

		EXPECT_UTF8EQ("\xCE\xA9", d);
		EXPECT_STREQ("\\u3A9", helpers::identifiable(d).c_str());
	}
}

TEST(Composition, CanonicalCompositeAWithRing)
{
	// NFD

	{
		const char* d = database_querydecomposition(0xC5, UnicodeProperty_Normalization_Decompose);
		ASSERT_NE(nullptr, d);

		EXPECT_UTF8EQ("A\xCC\x8A", d);
		EXPECT_STREQ("A\\u30A", helpers::identifiable(d).c_str());
	}

	// NFKD

	{
		const char* d = database_querydecomposition(0xC5, UnicodeProperty_Normalization_Compatibility_Decompose);
		ASSERT_NE(nullptr, d);

		EXPECT_UTF8EQ("A\xCC\x8A", d);
		EXPECT_STREQ("A\\u30A", helpers::identifiable(d).c_str());
	}
}

TEST(Composition, CanonicalCompositeOWithCircumflex)
{
	// NFD

	{
		const char* d = database_querydecomposition(0xF4, UnicodeProperty_Normalization_Decompose);
		ASSERT_NE(nullptr, d);

		EXPECT_UTF8EQ("o\xCC\x82", d);
		EXPECT_STREQ("o\\u302", helpers::identifiable(d).c_str());
	}

	// NFKD

	{
		const char* d = database_querydecomposition(0xF4, UnicodeProperty_Normalization_Compatibility_Decompose);
		ASSERT_NE(nullptr, d);

		EXPECT_UTF8EQ("o\xCC\x82", d);
		EXPECT_STREQ("o\\u302", helpers::identifiable(d).c_str());
	}
}

TEST(Composition, MultipleCombiningMarksSWithDots)
{
	// NFD

	{
		const char* d = database_querydecomposition(0x1E69, UnicodeProperty_Normalization_Decompose);
		ASSERT_NE(nullptr, d);

		EXPECT_UTF8EQ("s\xCC\xA3\xCC\x87", d);
		EXPECT_STREQ("s\\u323\\u307", helpers::identifiable(d).c_str());
	}

	// NFKD

	{
		const char* d = database_querydecomposition(0x1E69, UnicodeProperty_Normalization_Compatibility_Decompose);
		ASSERT_NE(nullptr, d);

		EXPECT_UTF8EQ("s\xCC\xA3\xCC\x87", d);
		EXPECT_STREQ("s\\u323\\u307", helpers::identifiable(d).c_str());
	}
}