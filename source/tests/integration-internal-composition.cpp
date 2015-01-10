#include "tests-base.hpp"

extern "C" {
	#include "../internal/database.h"
	#include "../internal/normalization.h"
	#include "../internal/reorder.h"
}

#include "helpers-strings.hpp"
#include "helpers-normalization.hpp"

TEST(Composition, CanonicalCombiningClass)
{
	const char* c = "a\xCC\x95\xCC\x80\xD6\xAE\xCC\x80" "b";
	size_t cl = strlen(c);
	char d[256] = { 0 };
	size_t dl = 255;
	int32_t errors = 0;

	/*ComposeState state;
	compose_initialize(&state, &c, &cl, UnicodeProperty_Normalization_Compose);
	compose_execute(&state);*/

	StreamState state;
	stream_initialize(&state, &c, &cl, &d, &dl, UnicodeProperty_Normalization_Compose);
	stream_execute(&state);

	std::string nfd = helpers::nfd("a\xCC\x95\xCC\x80\xD6\xAE\xCC\x80" "b");
	std::string id = helpers::identifiable(nfd);

	normalize_composition(c, cl, d, dl, UnicodeProperty_Normalization_Compose, &errors);

	EXPECT_UTF8EQ("\xC3\xA0\xD6\xAE\xCC\x80\xCC\x95" "b", d);

	int i = 0;
}

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