#include "tests-base.hpp"

extern "C" {
	#include "../normalization.h"
}

TEST(Composition, SingletonAngstrom)
{
	int32_t errors = 0;

	const CompositionEntry* c = findcomposition(0x212B, &errors);
	ASSERT_NE(nullptr, c);
	EXPECT_EQ(0, errors);

	// NFC

	{
		const char* d = resolvedecomposition(c->offsetC, &errors);
		ASSERT_NE(nullptr, d);
		EXPECT_EQ(0, errors);

		EXPECT_STREQ("\xC3\x85", d);

		unicode_t* u = nullptr;
		size_t ul = utf8toutf32(d, strlen(d), nullptr, 0, &errors);
		EXPECT_EQ(0, errors);

		u = new unicode_t[ul / sizeof(unicode_t)];
		utf8toutf32(d, strlen(d), u, ul, &errors);
		EXPECT_EQ(0, errors);

		EXPECT_EQ(1 * sizeof(unicode_t), ul);
		EXPECT_EQ(0x000000C5, u[0]);

		delete u;
	}

	// NFD

	{
		const char* d = resolvedecomposition(c->offsetD, &errors);
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

	// NFKC

	{
		const char* d = resolvedecomposition(c->offsetKC, &errors);
		ASSERT_NE(nullptr, d);
		EXPECT_EQ(0, errors);

		EXPECT_STREQ("\xC3\x85", d);

		unicode_t* u = nullptr;
		size_t ul = utf8toutf32(d, strlen(d), nullptr, 0, &errors);
		EXPECT_EQ(0, errors);

		u = new unicode_t[ul / sizeof(unicode_t)];
		utf8toutf32(d, strlen(d), u, ul, &errors);
		EXPECT_EQ(0, errors);

		EXPECT_EQ(1 * sizeof(unicode_t), ul);
		EXPECT_EQ(0x000000C5, u[0]);

		delete u;
	}

	// NFKD

	{
		const char* d = resolvedecomposition(c->offsetKD, &errors);
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

	const CompositionEntry* c = findcomposition(0x2126, &errors);
	ASSERT_NE(nullptr, c);
	EXPECT_EQ(0, errors);

	// NFC

	{
		const char* d = resolvedecomposition(c->offsetC, &errors);
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

	// NFD

	{
		const char* d = resolvedecomposition(c->offsetD, &errors);
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

	// NFKC

	{
		const char* d = resolvedecomposition(c->offsetKC, &errors);
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
		const char* d = resolvedecomposition(c->offsetKD, &errors);
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