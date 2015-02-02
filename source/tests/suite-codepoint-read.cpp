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

TEST(CodepointRead, TwoBytesNotEnoughDataOneByte)
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

TEST(CodepointRead, ThreeBytes)
{
	const char* i = "\xE1\x8C\x8A";
	size_t il = strlen(i);
	unicode_t o;
	uint8_t ol;

	EXPECT_EQ(3, (ol = (uint8_t)codepoint_read(&o, i, il)));
	EXPECT_CPEQ(0x130A, o);
}

TEST(CodepointRead, ThreeBytesFirst)
{
	const char* i = "\xE0\xA0\x80";
	size_t il = strlen(i);
	unicode_t o;
	uint8_t ol;

	EXPECT_EQ(3, (ol = (uint8_t)codepoint_read(&o, i, il)));
	EXPECT_CPEQ(0x0800, o);
}

TEST(CodepointRead, ThreeBytesLast)
{
	const char* i = "\xEF\xBF\xBF";
	size_t il = strlen(i);
	unicode_t o;
	uint8_t ol;

	EXPECT_EQ(3, (ol = (uint8_t)codepoint_read(&o, i, il)));
	EXPECT_CPEQ(0xFFFF, o);
}

TEST(CodepointRead, ThreeBytesInvalidContinuationLower)
{
	const char* i = "\xEF\x89\x7A";
	size_t is = strlen(i);
	unicode_t o;
	uint8_t ol;

	EXPECT_EQ(2, (ol = (uint8_t)codepoint_read(&o, i, is)));
	EXPECT_CPEQ(0xFFFD, o);
}

TEST(CodepointRead, ThreeBytesInvalidContinuationUpper)
{
	const char* i = "\xE3\xB8\xC4";
	size_t is = strlen(i);
	unicode_t o;
	uint8_t ol;

	EXPECT_EQ(2, (ol = (uint8_t)codepoint_read(&o, i, is)));
	EXPECT_CPEQ(0xFFFD, o);
}

TEST(CodepointRead, ThreeBytesNotEnoughDataTwoBytes)
{
	const char* i = "\xE3\x81";
	size_t is = strlen(i);
	unicode_t o;
	uint8_t ol;

	EXPECT_EQ(2, (ol = (uint8_t)codepoint_read(&o, i, is)));
	EXPECT_CPEQ(0xFFFD, o);
}

TEST(CodepointRead, ThreeBytesNotEnoughDataOneByte)
{
	const char* i = "\xE1";
	size_t is = strlen(i);
	unicode_t o;
	uint8_t ol;

	EXPECT_EQ(1, (ol = (uint8_t)codepoint_read(&o, i, is)));
	EXPECT_CPEQ(0xFFFD, o);
}

TEST(CodepointRead, ThreeBytesOverlongFirst)
{
	const char* i = "\xE0\x80\x80";
	size_t is = strlen(i);
	unicode_t o;
	uint8_t ol;

	EXPECT_EQ(3, (ol = (uint8_t)codepoint_read(&o, i, is)));
	EXPECT_CPEQ(0xFFFD, o);
}

TEST(CodepointRead, ThreeBytesOverlongLast)
{
	const char* i = "\xE0\x9F\xBF";
	size_t is = strlen(i);
	unicode_t o;
	uint8_t ol;

	EXPECT_EQ(3, (ol = (uint8_t)codepoint_read(&o, i, is)));
	EXPECT_CPEQ(0xFFFD, o);
}