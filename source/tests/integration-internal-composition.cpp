#include "tests-base.hpp"

extern "C" {
	#include "../normalization.h"
}

TEST(Composition, SingletonAngstrom)
{
	int32_t errors = 0;

	// NFD

	{
		const DecompositionRecord* c = finddecomposition(0x212B, NormalizationForm_Decomposed, &errors);
		ASSERT_NE(nullptr, c);
		EXPECT_EQ(0, errors);

		const char* d = resolvedecomposition(c->offset, &errors);
		ASSERT_NE(nullptr, d);
		EXPECT_EQ(0, errors);

		EXPECT_STREQ("\x41\xCC\x8A", d);

		unicode_t* u = nullptr;
		size_t ul = utf8toutf32(d, strlen(d), nullptr, 0, &errors);
		EXPECT_EQ(0, errors);

		u = new unicode_t[ul / sizeof(unicode_t)];
		utf8toutf32(d, strlen(d), u, ul, &errors);
		EXPECT_EQ(0, errors);

		EXPECT_EQ(2 * sizeof(unicode_t), ul);
		EXPECT_EQ(0x00000041, u[0]);
		EXPECT_EQ(0x0000030A, u[1]);

		delete u;
	}

	// NFKD

	{
		const DecompositionRecord* c = finddecomposition(0x212B, NormalizationForm_Compatibility_Decomposed, &errors);
		ASSERT_NE(nullptr, c);
		EXPECT_EQ(0, errors);

		const char* d = resolvedecomposition(c->offset, &errors);
		ASSERT_NE(nullptr, d);
		EXPECT_EQ(0, errors);

		EXPECT_STREQ("\x41\xCC\x8A", d);

		unicode_t* u = nullptr;
		size_t ul = utf8toutf32(d, strlen(d), nullptr, 0, &errors);
		EXPECT_EQ(0, errors);

		u = new unicode_t[ul / sizeof(unicode_t)];
		utf8toutf32(d, strlen(d), u, ul, &errors);
		EXPECT_EQ(0, errors);

		EXPECT_EQ(2 * sizeof(unicode_t), ul);
		EXPECT_EQ(0x00000041, u[0]);
		EXPECT_EQ(0x0000030A, u[1]);

		delete u;
	}
}

TEST(Composition, SingletonOhm)
{
	int32_t errors = 0;

	// NFD

	{
		const DecompositionRecord* c = finddecomposition(0x2126, NormalizationForm_Decomposed, &errors);
		ASSERT_NE(nullptr, c);
		EXPECT_EQ(0, errors);

		const char* d = resolvedecomposition(c->offset, &errors);
		ASSERT_NE(nullptr, d);
		EXPECT_EQ(0, errors);

		EXPECT_STREQ("\xCE\xA9", d);

		unicode_t* u = nullptr;
		size_t ul = utf8toutf32(d, strlen(d), nullptr, 0, &errors);
		EXPECT_EQ(0, errors);

		u = new unicode_t[ul / sizeof(unicode_t)];
		utf8toutf32(d, strlen(d), u, ul, &errors);
		EXPECT_EQ(0, errors);

		EXPECT_EQ(1 * sizeof(unicode_t), ul);
		EXPECT_EQ(0x000003A9, u[0]);

		delete u;
	}

	// NFKD

	{
		const DecompositionRecord* c = finddecomposition(0x2126, NormalizationForm_Compatibility_Decomposed, &errors);
		ASSERT_NE(nullptr, c);
		EXPECT_EQ(0, errors);

		const char* d = resolvedecomposition(c->offset, &errors);
		ASSERT_NE(nullptr, d);
		EXPECT_EQ(0, errors);

		EXPECT_STREQ("\xCE\xA9", d);

		unicode_t* u = nullptr;
		size_t ul = utf8toutf32(d, strlen(d), nullptr, 0, &errors);
		EXPECT_EQ(0, errors);

		u = new unicode_t[ul / sizeof(unicode_t)];
		utf8toutf32(d, strlen(d), u, ul, &errors);
		EXPECT_EQ(0, errors);

		EXPECT_EQ(1 * sizeof(unicode_t), ul);
		EXPECT_EQ(0x000003A9, u[0]);

		delete u;
	}
}

TEST(Composition, CanonicalCompositeAWithRing)
{
	int32_t errors = 0;

	// NFD

	{
		const DecompositionRecord* c = finddecomposition(0xC5, NormalizationForm_Decomposed, &errors);
		ASSERT_NE(nullptr, c);
		EXPECT_EQ(0, errors);

		const char* d = resolvedecomposition(c->offset, &errors);
		ASSERT_NE(nullptr, d);
		EXPECT_EQ(0, errors);

		EXPECT_STREQ("\x41\xCC\x8A", d);

		unicode_t* u = nullptr;
		size_t ul = utf8toutf32(d, strlen(d), nullptr, 0, &errors);
		EXPECT_EQ(0, errors);

		u = new unicode_t[ul / sizeof(unicode_t)];
		utf8toutf32(d, strlen(d), u, ul, &errors);
		EXPECT_EQ(0, errors);

		EXPECT_EQ(2 * sizeof(unicode_t), ul);
		EXPECT_EQ(0x00000041, u[0]);
		EXPECT_EQ(0x0000030A, u[1]);

		delete u;
	}

	// NFKD

	{
		const DecompositionRecord* c = finddecomposition(0xC5, NormalizationForm_Compatibility_Decomposed, &errors);
		ASSERT_NE(nullptr, c);
		EXPECT_EQ(0, errors);

		const char* d = resolvedecomposition(c->offset, &errors);
		ASSERT_NE(nullptr, d);
		EXPECT_EQ(0, errors);

		EXPECT_STREQ("\x41\xCC\x8A", d);

		unicode_t* u = nullptr;
		size_t ul = utf8toutf32(d, strlen(d), nullptr, 0, &errors);
		EXPECT_EQ(0, errors);

		u = new unicode_t[ul / sizeof(unicode_t)];
		utf8toutf32(d, strlen(d), u, ul, &errors);
		EXPECT_EQ(0, errors);

		EXPECT_EQ(2 * sizeof(unicode_t), ul);
		EXPECT_EQ(0x00000041, u[0]);
		EXPECT_EQ(0x0000030A, u[1]);

		delete u;
	}
}

TEST(Composition, CanonicalCompositeOWithCircumflex)
{
	int32_t errors = 0;

	// NFD

	{
		const DecompositionRecord* c = finddecomposition(0xF4, NormalizationForm_Decomposed, &errors);
		ASSERT_NE(nullptr, c);
		EXPECT_EQ(0, errors);

		const char* d = resolvedecomposition(c->offset, &errors);
		ASSERT_NE(nullptr, d);
		EXPECT_EQ(0, errors);

		EXPECT_STREQ("\x6F\xCC\x82", d);

		unicode_t* u = nullptr;
		size_t ul = utf8toutf32(d, strlen(d), nullptr, 0, &errors);
		EXPECT_EQ(0, errors);

		u = new unicode_t[ul / sizeof(unicode_t)];
		utf8toutf32(d, strlen(d), u, ul, &errors);
		EXPECT_EQ(0, errors);

		EXPECT_EQ(2 * sizeof(unicode_t), ul);
		EXPECT_EQ(0x0000006F, u[0]);
		EXPECT_EQ(0x00000302, u[1]);

		delete u;
	}

	// NFKD

	{
		const DecompositionRecord* c = finddecomposition(0xF4, NormalizationForm_Compatibility_Decomposed, &errors);
		ASSERT_NE(nullptr, c);
		EXPECT_EQ(0, errors);

		const char* d = resolvedecomposition(c->offset, &errors);
		ASSERT_NE(nullptr, d);
		EXPECT_EQ(0, errors);

		EXPECT_STREQ("\x6F\xCC\x82", d);

		unicode_t* u = nullptr;
		size_t ul = utf8toutf32(d, strlen(d), nullptr, 0, &errors);
		EXPECT_EQ(0, errors);

		u = new unicode_t[ul / sizeof(unicode_t)];
		utf8toutf32(d, strlen(d), u, ul, &errors);
		EXPECT_EQ(0, errors);

		EXPECT_EQ(2 * sizeof(unicode_t), ul);
		EXPECT_EQ(0x0000006F, u[0]);
		EXPECT_EQ(0x00000302, u[1]);

		delete u;
	}
}

TEST(Composition, MultipleCombiningMarksSWithDots)
{
	int32_t errors = 0;

	// NFD

	{
		const DecompositionRecord* c = finddecomposition(0x1E69, NormalizationForm_Decomposed, &errors);
		ASSERT_NE(nullptr, c);
		EXPECT_EQ(0, errors);

		const char* d = resolvedecomposition(c->offset, &errors);
		ASSERT_NE(nullptr, d);
		EXPECT_EQ(0, errors);

		EXPECT_STREQ("\x73\xCC\xA3\xCC\x87", d);

		unicode_t* u = nullptr;
		size_t ul = utf8toutf32(d, strlen(d), nullptr, 0, &errors);
		EXPECT_EQ(0, errors);

		u = new unicode_t[ul / sizeof(unicode_t)];
		utf8toutf32(d, strlen(d), u, ul, &errors);
		EXPECT_EQ(0, errors);

		EXPECT_EQ(3 * sizeof(unicode_t), ul);
		EXPECT_EQ(0x00000073, u[0]);
		EXPECT_EQ(0x00000323, u[1]);
		EXPECT_EQ(0x00000307, u[2]);

		delete u;
	}

	// NFKD

	{
		const DecompositionRecord* c = finddecomposition(0x1E69, NormalizationForm_Compatibility_Decomposed, &errors);
		ASSERT_NE(nullptr, c);
		EXPECT_EQ(0, errors);

		const char* d = resolvedecomposition(c->offset, &errors);
		ASSERT_NE(nullptr, d);
		EXPECT_EQ(0, errors);

		EXPECT_STREQ("\x73\xCC\xA3\xCC\x87", d);

		unicode_t* u = nullptr;
		size_t ul = utf8toutf32(d, strlen(d), nullptr, 0, &errors);
		EXPECT_EQ(0, errors);

		u = new unicode_t[ul / sizeof(unicode_t)];
		utf8toutf32(d, strlen(d), u, ul, &errors);
		EXPECT_EQ(0, errors);

		EXPECT_EQ(3 * sizeof(unicode_t), ul);
		EXPECT_EQ(0x00000073, u[0]);
		EXPECT_EQ(0x00000323, u[1]);
		EXPECT_EQ(0x00000307, u[2]);

		delete u;
	}
}