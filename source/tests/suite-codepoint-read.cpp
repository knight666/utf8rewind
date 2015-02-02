#include "tests-base.hpp"

extern "C" {
	#include "../internal/codepoint.h"
};

#include "helpers-strings.hpp"

TEST(CodepointRead, OneByte)
{
	const char* i = "I";
	size_t il = strlen(i);
	unicode_t o;
	uint8_t ol;

	EXPECT_EQ(1, (ol = (uint8_t)codepoint_read(&o, i, il)));
	EXPECT_CPEQ(0x0049, o);
}

TEST(CodepointRead, OneByteFirst)
{
	const char* i = "\0";
	size_t il = 1;
	unicode_t o;
	uint8_t ol;

	EXPECT_EQ(1, (ol = (uint8_t)codepoint_read(&o, i, il)));
	EXPECT_CPEQ(0x0000, o);
}

TEST(CodepointRead, OneByteLast)
{
	const char* i = "\x7F";
	size_t il = strlen(i);
	unicode_t o;
	uint8_t ol;

	EXPECT_EQ(1, (ol = (uint8_t)codepoint_read(&o, i, il)));
	EXPECT_CPEQ(0x007F, o);
}

TEST(CodepointRead, OneByteInvalidContinuationLower)
{
	const char* i = "\x80";
	size_t il = strlen(i);
	unicode_t o;
	uint8_t ol;

	EXPECT_EQ(1, (ol = (uint8_t)codepoint_read(&o, i, il)));
	EXPECT_CPEQ(0xFFFD, o);
}

TEST(CodepointRead, OneByteInvalidContinuationUpper)
{
	const char* i = "\xBF";
	size_t il = strlen(i);
	unicode_t o;
	uint8_t ol;

	EXPECT_EQ(1, (ol = (uint8_t)codepoint_read(&o, i, il)));
	EXPECT_CPEQ(0xFFFD, o);
}

TEST(CodepointRead, OneByteIllegalFirst)
{
	const char* i = "\xFE";
	size_t il = strlen(i);
	unicode_t o;
	uint8_t ol;

	EXPECT_EQ(1, (ol = (uint8_t)codepoint_read(&o, i, il)));
	EXPECT_CPEQ(0xFFFD, o);
}

TEST(CodepointRead, OneByteIllegalLast)
{
	const char* i = "\xFE";
	size_t il = strlen(i);
	unicode_t o;
	uint8_t ol;

	EXPECT_EQ(1, (ol = (uint8_t)codepoint_read(&o, i, il)));
	EXPECT_CPEQ(0xFFFD, o);
}

TEST(CodepointRead, TwoBytes)
{
	const char* i = "\xD8\x88";
	size_t il = strlen(i);
	unicode_t o;
	uint8_t ol;

	EXPECT_EQ(2, (ol = (uint8_t)codepoint_read(&o, i, il)));
	EXPECT_CPEQ(0x0608, o);
}

TEST(CodepointRead, TwoBytesFirst)
{
	const char* i = "\xC2\x80";
	size_t il = strlen(i);
	unicode_t o;
	uint8_t ol;

	EXPECT_EQ(2, (ol = (uint8_t)codepoint_read(&o, i, il)));
	EXPECT_CPEQ(0x0080, o);
}

TEST(CodepointRead, TwoBytesLast)
{
	const char* i = "\xDF\xBF";
	size_t il = strlen(i);
	unicode_t o;
	uint8_t ol;

	EXPECT_EQ(2, (ol = (uint8_t)codepoint_read(&o, i, il)));
	EXPECT_CPEQ(0x07FF, o);
}

TEST(CodepointRead, TwoBytesInvalidContinuationLower)
{
	const char* i = "\xCA\x19";
	size_t is = strlen(i);
	unicode_t o;
	uint8_t ol;

	EXPECT_EQ(1, (ol = (uint8_t)codepoint_read(&o, i, is)));
	EXPECT_CPEQ(0xFFFD, o);
}

TEST(CodepointRead, TwoBytesInvalidContinuationUpper)
{
	const char* i = "\xD0\xC8";
	size_t is = strlen(i);
	unicode_t o;
	uint8_t ol;

	EXPECT_EQ(1, (ol = (uint8_t)codepoint_read(&o, i, is)));
	EXPECT_CPEQ(0xFFFD, o);
}

TEST(CodepointRead, TwoBytesNotEnoughData)
{
	const char* i = "\xCA";
	size_t is = strlen(i);
	unicode_t o;
	uint8_t ol;

	EXPECT_EQ(1, (ol = (uint8_t)codepoint_read(&o, i, is)));
	EXPECT_CPEQ(0xFFFD, o);
}

TEST(CodepointRead, TwoBytesOverlong)
{
	const char* i = "\xC0\x9A";
	size_t is = strlen(i);
	unicode_t o;
	uint8_t ol;

	EXPECT_EQ(2, (ol = (uint8_t)codepoint_read(&o, i, is)));
	EXPECT_CPEQ(0xFFFD, o);
}

TEST(CodepointRead, TwoBytesOverlongFirst)
{
	const char* i = "\xC0\x80";
	size_t is = strlen(i);
	unicode_t o;
	uint8_t ol;

	EXPECT_EQ(2, (ol = (uint8_t)codepoint_read(&o, i, is)));
	EXPECT_CPEQ(0xFFFD, o);
}

TEST(CodepointRead, TwoBytesOverlongLast)
{
	const char* i = "\xC1\xBF";
	size_t is = strlen(i);
	unicode_t o;
	uint8_t ol;

	EXPECT_EQ(2, (ol = (uint8_t)codepoint_read(&o, i, is)));
	EXPECT_CPEQ(0xFFFD, o);
}