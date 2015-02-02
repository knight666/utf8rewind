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

TEST(CodepointRead, FourBytes)
{
	const char* i = "\xF0\x90\xB0\xAC";
	size_t il = strlen(i);
	unicode_t o;
	uint8_t ol;

	EXPECT_EQ(4, (ol = (uint8_t)codepoint_read(&o, i, il)));
	EXPECT_CPEQ(0x10C2C, o);
}

TEST(CodepointRead, FourBytesFirst)
{
	const char* i = "\xF0\x90\x80\x80";
	size_t il = strlen(i);
	unicode_t o;
	uint8_t ol;

	EXPECT_EQ(4, (ol = (uint8_t)codepoint_read(&o, i, il)));
	EXPECT_CPEQ(0x10000, o);
}

TEST(CodepointRead, FourBytesLast)
{
	const char* i = "\xF4\x8F\xBF\xBF";
	size_t il = strlen(i);
	unicode_t o;
	uint8_t ol;

	EXPECT_EQ(4, (ol = (uint8_t)codepoint_read(&o, i, il)));
	EXPECT_CPEQ(0x10FFFF, o);
}

TEST(CodepointRead, FourBytesInvalidContinuationLower)
{
	const char* i = "\xF1\x9A\xA0\x42";
	size_t il = strlen(i);
	unicode_t o;
	uint8_t ol;

	EXPECT_EQ(3, (ol = (uint8_t)codepoint_read(&o, i, il)));
	EXPECT_CPEQ(0xFFFD, o);
}

TEST(CodepointRead, FourBytesInvalidContinuationUpper)
{
	const char* i = "\xF0\x90\xA8\xD5";
	size_t il = strlen(i);
	unicode_t o;
	uint8_t ol;

	EXPECT_EQ(3, (ol = (uint8_t)codepoint_read(&o, i, il)));
	EXPECT_CPEQ(0xFFFD, o);
}

TEST(CodepointRead, FourBytesNotEnoughDataThreeBytes)
{
	const char* i = "\xF0\x90\xA8";
	size_t il = strlen(i);
	unicode_t o;
	uint8_t ol;

	EXPECT_EQ(3, (ol = (uint8_t)codepoint_read(&o, i, il)));
	EXPECT_CPEQ(0xFFFD, o);
}

TEST(CodepointRead, FourBytesNotEnoughDataTwoBytes)
{
	const char* i = "\xF1\xAF";
	size_t il = strlen(i);
	unicode_t o;
	uint8_t ol;

	EXPECT_EQ(2, (ol = (uint8_t)codepoint_read(&o, i, il)));
	EXPECT_CPEQ(0xFFFD, o);
}

TEST(CodepointRead, FourBytesNotEnoughDataOneByte)
{
	const char* i = "\xF3";
	size_t il = strlen(i);
	unicode_t o;
	uint8_t ol;

	EXPECT_EQ(1, (ol = (uint8_t)codepoint_read(&o, i, il)));
	EXPECT_CPEQ(0xFFFD, o);
}

TEST(CodepointRead, FourBytesOverlongFirst)
{
	const char* i = "\xF4\x90\x80\x80";
	size_t il = strlen(i);
	unicode_t o;
	uint8_t ol;

	EXPECT_EQ(4, (ol = (uint8_t)codepoint_read(&o, i, il)));
	EXPECT_CPEQ(0xFFFD, o);
}

TEST(CodepointRead, FourBytesOverlongLast)
{
	const char* i = "\xF7\xBF\x80\x80";
	size_t il = strlen(i);
	unicode_t o;
	uint8_t ol;

	EXPECT_EQ(4, (ol = (uint8_t)codepoint_read(&o, i, il)));
	EXPECT_CPEQ(0xFFFD, o);
}

TEST(CodepointRead, FiveBytes)
{
	const char* i = "\xF8\xA2\xB1\xA0\x88";
	size_t il = strlen(i);
	unicode_t o;
	uint8_t ol;

	EXPECT_EQ(5, (ol = (uint8_t)codepoint_read(&o, i, il)));
	EXPECT_CPEQ(0xFFFD, o);
}

TEST(CodepointRead, FiveBytesFirst)
{
	const char* i = "\xF8\x80\x80\x80\x80";
	size_t il = strlen(i);
	unicode_t o;
	uint8_t ol;

	EXPECT_EQ(5, (ol = (uint8_t)codepoint_read(&o, i, il)));
	EXPECT_CPEQ(0xFFFD, o);
}

TEST(CodepointRead, FiveBytesLast)
{
	const char* i = "\xFB\x8F\xBF\xBF\x8F";
	size_t il = strlen(i);
	unicode_t o;
	uint8_t ol;

	EXPECT_EQ(5, (ol = (uint8_t)codepoint_read(&o, i, il)));
	EXPECT_CPEQ(0xFFFD, o);
}

TEST(CodepointRead, FiveBytesInvalidContinuationLower)
{
	const char* i = "\xF8\xA2\xB1\xA0\x12";
	size_t il = strlen(i);
	unicode_t o;
	uint8_t ol;

	EXPECT_EQ(4, (ol = (uint8_t)codepoint_read(&o, i, il)));
	EXPECT_CPEQ(0xFFFD, o);
}

TEST(CodepointRead, FiveBytesInvalidContinuationUpper)
{
	const char* i = "\xF8\xAB\xA1\xBA\xF8";
	size_t il = strlen(i);
	unicode_t o;
	uint8_t ol;

	EXPECT_EQ(4, (ol = (uint8_t)codepoint_read(&o, i, il)));
	EXPECT_CPEQ(0xFFFD, o);
}

TEST(CodepointRead, FiveBytesNotEnoughDataFourBytes)
{
	const char* i = "\xF9\x90\xA8\x90";
	size_t il = strlen(i);
	unicode_t o;
	uint8_t ol;

	EXPECT_EQ(4, (ol = (uint8_t)codepoint_read(&o, i, il)));
	EXPECT_CPEQ(0xFFFD, o);
}

TEST(CodepointRead, FiveBytesNotEnoughDataThreeBytes)
{
	const char* i = "\xFA\xA1\x8A";
	size_t il = strlen(i);
	unicode_t o;
	uint8_t ol;

	EXPECT_EQ(3, (ol = (uint8_t)codepoint_read(&o, i, il)));
	EXPECT_CPEQ(0xFFFD, o);
}

TEST(CodepointRead, FiveBytesNotEnoughDataTwoBytes)
{
	const char* i = "\xF9\xAF";
	size_t il = strlen(i);
	unicode_t o;
	uint8_t ol;

	EXPECT_EQ(2, (ol = (uint8_t)codepoint_read(&o, i, il)));
	EXPECT_CPEQ(0xFFFD, o);
}

TEST(CodepointRead, FiveBytesNotEnoughDataOneByte)
{
	const char* i = "\xFB";
	size_t il = strlen(i);
	unicode_t o;
	uint8_t ol;

	EXPECT_EQ(1, (ol = (uint8_t)codepoint_read(&o, i, il)));
	EXPECT_CPEQ(0xFFFD, o);
}