#include "tests-base.hpp"

extern "C" {
	#include "../internal/codepoint.h"
};

TEST(CodepointEncodedLength, SingleByte)
{
	EXPECT_EQ(1, codepoint_encoded_length(0x0061));
}

TEST(CodepointEncodedLength, SingleByteFirst)
{
	EXPECT_EQ(1, codepoint_encoded_length(0x0000));
}

TEST(CodepointEncodedLength, SingleByteLast)
{
	EXPECT_EQ(1, codepoint_encoded_length(0x007F));
}

TEST(CodepointEncodedLength, TwoBytes)
{
	EXPECT_EQ(2, codepoint_encoded_length(0x02A0));
}

TEST(CodepointEncodedLength, TwoBytesFirst)
{
	EXPECT_EQ(2, codepoint_encoded_length(0x0080));
}

TEST(CodepointEncodedLength, TwoBytesLast)
{
	EXPECT_EQ(2, codepoint_encoded_length(0x07FF));
}

TEST(CodepointEncodedLength, ThreeBytes)
{
	EXPECT_EQ(3, codepoint_encoded_length(0xA2A3));
}

TEST(CodepointEncodedLength, ThreeBytesFirst)
{
	EXPECT_EQ(3, codepoint_encoded_length(0x0800));
}

TEST(CodepointEncodedLength, ThreeBytesLast)
{
	EXPECT_EQ(3, codepoint_encoded_length(0xFFFF));
}

TEST(CodepointEncodedLength, FourBytes)
{
	EXPECT_EQ(4, codepoint_encoded_length(0x23A88));
}

TEST(CodepointEncodedLength, FourBytesFirst)
{
	EXPECT_EQ(4, codepoint_encoded_length(0x10000));
}

TEST(CodepointEncodedLength, FourBytesLast)
{
	EXPECT_EQ(4, codepoint_encoded_length(0x10FFFF));
}

TEST(CodepointEncodedLength, AboveLegalUnicode)
{
	EXPECT_EQ(0, codepoint_encoded_length(0xDEADC0DE));
}