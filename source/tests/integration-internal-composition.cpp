#include "tests-base.hpp"

extern "C" {
	#include "../normalization.h"
}

#include "helpers-strings.hpp"

TEST(Composition, SingletonAngstrom)
{
	int32_t errors = 0;

	// NFD

	{
		const DecompositionRecord* c = finddecomposition(0x212B, DecompositionQuery_Decomposed, &errors);
		ASSERT_NE(nullptr, c);
		EXPECT_EQ(0, errors);

		const char* d = resolvedecomposition(c->offset, &errors);
		ASSERT_NE(nullptr, d);
		EXPECT_EQ(0, errors);

		EXPECT_UTF8EQ("A\xCC\x8A", d);
		EXPECT_STREQ("A\\u30A", helpers::identifiable(d).c_str());
	}

	// NFKD

	{
		const DecompositionRecord* c = finddecomposition(0x212B, DecompositionQuery_Compatibility_Decomposed, &errors);
		ASSERT_NE(nullptr, c);
		EXPECT_EQ(0, errors);

		const char* d = resolvedecomposition(c->offset, &errors);
		ASSERT_NE(nullptr, d);
		EXPECT_EQ(0, errors);

		EXPECT_UTF8EQ("A\xCC\x8A", d);
		EXPECT_STREQ("A\\u30A", helpers::identifiable(d).c_str());
	}
}

TEST(Composition, SingletonOhm)
{
	int32_t errors = 0;

	// NFD

	{
		const DecompositionRecord* c = finddecomposition(0x2126, DecompositionQuery_Decomposed, &errors);
		ASSERT_NE(nullptr, c);
		EXPECT_EQ(0, errors);

		const char* d = resolvedecomposition(c->offset, &errors);
		ASSERT_NE(nullptr, d);
		EXPECT_EQ(0, errors);

		EXPECT_UTF8EQ("\xCE\xA9", d);
		EXPECT_STREQ("\\u3A9", helpers::identifiable(d).c_str());
	}

	// NFKD

	{
		const DecompositionRecord* c = finddecomposition(0x2126, DecompositionQuery_Compatibility_Decomposed, &errors);
		ASSERT_NE(nullptr, c);
		EXPECT_EQ(0, errors);

		const char* d = resolvedecomposition(c->offset, &errors);
		ASSERT_NE(nullptr, d);
		EXPECT_EQ(0, errors);

		EXPECT_UTF8EQ("\xCE\xA9", d);
		EXPECT_STREQ("\\u3A9", helpers::identifiable(d).c_str());
	}
}

TEST(Composition, CanonicalCompositeAWithRing)
{
	int32_t errors = 0;

	// NFD

	{
		const DecompositionRecord* c = finddecomposition(0xC5, DecompositionQuery_Decomposed, &errors);
		ASSERT_NE(nullptr, c);
		EXPECT_EQ(0, errors);

		const char* d = resolvedecomposition(c->offset, &errors);
		ASSERT_NE(nullptr, d);
		EXPECT_EQ(0, errors);

		EXPECT_UTF8EQ("A\xCC\x8A", d);
		EXPECT_STREQ("A\\u30A", helpers::identifiable(d).c_str());
	}

	// NFKD

	{
		const DecompositionRecord* c = finddecomposition(0xC5, DecompositionQuery_Compatibility_Decomposed, &errors);
		ASSERT_NE(nullptr, c);
		EXPECT_EQ(0, errors);

		const char* d = resolvedecomposition(c->offset, &errors);
		ASSERT_NE(nullptr, d);
		EXPECT_EQ(0, errors);

		EXPECT_UTF8EQ("A\xCC\x8A", d);
		EXPECT_STREQ("A\\u30A", helpers::identifiable(d).c_str());
	}
}

TEST(Composition, CanonicalCompositeOWithCircumflex)
{
	int32_t errors = 0;

	// NFD

	{
		const DecompositionRecord* c = finddecomposition(0xF4, DecompositionQuery_Decomposed, &errors);
		ASSERT_NE(nullptr, c);
		EXPECT_EQ(0, errors);

		const char* d = resolvedecomposition(c->offset, &errors);
		ASSERT_NE(nullptr, d);
		EXPECT_EQ(0, errors);

		EXPECT_STREQ("o\xCC\x82", d);
		EXPECT_STREQ("o\\u302", helpers::identifiable(d).c_str());
	}

	// NFKD

	{
		const DecompositionRecord* c = finddecomposition(0xF4, DecompositionQuery_Compatibility_Decomposed, &errors);
		ASSERT_NE(nullptr, c);
		EXPECT_EQ(0, errors);

		const char* d = resolvedecomposition(c->offset, &errors);
		ASSERT_NE(nullptr, d);
		EXPECT_EQ(0, errors);

		EXPECT_STREQ("o\xCC\x82", d);
		EXPECT_STREQ("o\\u302", helpers::identifiable(d).c_str());
	}
}

TEST(Composition, MultipleCombiningMarksSWithDots)
{
	int32_t errors = 0;

	// NFD

	{
		const DecompositionRecord* c = finddecomposition(0x1E69, DecompositionQuery_Decomposed, &errors);
		ASSERT_NE(nullptr, c);
		EXPECT_EQ(0, errors);

		const char* d = resolvedecomposition(c->offset, &errors);
		ASSERT_NE(nullptr, d);
		EXPECT_EQ(0, errors);

		EXPECT_STREQ("s\xCC\xA3\xCC\x87", d);
		EXPECT_STREQ("s\\u323\\u307", helpers::identifiable(d).c_str());
	}

	// NFKD

	{
		const DecompositionRecord* c = finddecomposition(0x1E69, DecompositionQuery_Compatibility_Decomposed, &errors);
		ASSERT_NE(nullptr, c);
		EXPECT_EQ(0, errors);

		const char* d = resolvedecomposition(c->offset, &errors);
		ASSERT_NE(nullptr, d);
		EXPECT_EQ(0, errors);

		EXPECT_STREQ("s\xCC\xA3\xCC\x87", d);
		EXPECT_STREQ("s\\u323\\u307", helpers::identifiable(d).c_str());
	}
}