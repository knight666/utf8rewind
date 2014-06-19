#include "tests-base.hpp"

#include "utf8rewind.h"

TEST(Decode, Ascii)
{
	unicode_t o = 0;
	int32_t errors = 0;

	EXPECT_EQ(1, utf8decode("\x5F", &o, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x5F, o);
}

TEST(Decode, AsciiMinimum)
{
	unicode_t o = 0;
	int32_t errors = 0;

	EXPECT_EQ(SIZE_MAX, utf8decode("\0", &o, &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
	EXPECT_EQ(0, o);
}

TEST(Decode, AsciiMaximum)
{
	unicode_t o = 0;
	int32_t errors = 0;

	EXPECT_EQ(1, utf8decode("\x7F", &o, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x7F, o);
}

TEST(Decode, AsciiInvalid)
{
	unicode_t o = 0;
	int32_t errors = 0;

	EXPECT_EQ(SIZE_MAX, utf8decode("\x88", &o, &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_CHARACTER, errors);
	EXPECT_EQ(0x00, o);
}

TEST(Decode, TwoBytes)
{
	unicode_t o = 0;
	int32_t errors = 0;

	EXPECT_EQ(2, utf8decode("\xC2\xA2", &o, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x00A2, o);
}

TEST(Decode, TwoBytesMinimum)
{
	unicode_t o = 0;
	int32_t errors = 0;

	EXPECT_EQ(2, utf8decode("\xC2\x80", &o, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0080, o);
}

TEST(Decode, TwoBytesMaximum)
{
	unicode_t o = 0;
	int32_t errors = 0;

	EXPECT_EQ(2, utf8decode("\xDF\xBF", &o, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x07FF, o);
}

TEST(Decode, TwoBytesNotEnoughData)
{
	unicode_t o = 0;
	int32_t errors = 0;

	EXPECT_EQ(SIZE_MAX, utf8decode("\xC2", &o, &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
	EXPECT_EQ(0x00, o);
}

TEST(Decode, ThreeBytes)
{
	unicode_t o = 0;
	int32_t errors = 0;

	EXPECT_EQ(3, utf8decode("\xE1\x8C\x8A", &o, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x130A, o);
}

TEST(Decode, ThreeBytesMinimum)
{
	unicode_t o = 0;
	int32_t errors = 0;

	EXPECT_EQ(3, utf8decode("\xE0\xA0\x80", &o, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0800, o);
}

TEST(Decode, ThreeBytesMaximum)
{
	unicode_t o = 0;
	int32_t errors = 0;

	EXPECT_EQ(3, utf8decode("\xEF\xBF\xBF", &o, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0xFFFF, o);
}

TEST(Decode, ThreeBytesNotEnoughData)
{
	unicode_t o = 0;
	int32_t errors = 0;

	EXPECT_EQ(SIZE_MAX, utf8decode("\xEF\xBF", &o, &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
	EXPECT_EQ(0x00, o);
}

TEST(Decode, FourBytes)
{
	unicode_t o = 0;
	int32_t errors = 0;

	EXPECT_EQ(4, utf8decode("\xF0\x9F\x98\x8E", &o, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x01F60E, o);
}

TEST(Decode, FourBytesMinimum)
{
	unicode_t o = 0;
	int32_t errors = 0;

	EXPECT_EQ(4, utf8decode("\xF0\x90\x80\x80", &o, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x010000, o);
}

TEST(Decode, FourBytesMaximum)
{
	unicode_t o = 0;
	int32_t errors = 0;

	EXPECT_EQ(4, utf8decode("\xF4\x80\x83\xBF", &o, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x1000FF, o);
}

TEST(Decode, FourBytesNotEnoughData)
{
	unicode_t o = 0;
	int32_t errors = 0;

	EXPECT_EQ(SIZE_MAX, utf8decode("\xF0\x90\x80", &o, &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
	EXPECT_EQ(0x00, o);
}

TEST(Decode, NoOutputSpecified)
{
	int32_t errors = 0;

	EXPECT_EQ(SIZE_MAX, utf8decode("\3E", nullptr, &errors));
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}