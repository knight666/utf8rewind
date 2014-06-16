#include "tests-base.hpp"

#include "utf8rewind.h"

TEST(ConvertUcs2LE, OneByte)
{
	ucs2_t c = L'A';
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1, utf8convertucs2_le(c, b, s, &errors));
	EXPECT_STREQ("A", b);
	EXPECT_EQ(0, errors);
}

TEST(ConvertUcs2LE, OneByteMinimum)
{
	ucs2_t c = 0;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1, utf8convertucs2_le(c, b, s, &errors));
	EXPECT_STREQ("", b);
	EXPECT_EQ(0, errors);
}

TEST(ConvertUcs2LE, OneByteMaximum)
{
	ucs2_t c = 0x7F;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1, utf8convertucs2_le(c, b, s, &errors));
	EXPECT_STREQ("\x7F", b);
	EXPECT_EQ(0, errors);
}

TEST(ConvertUcs2LE, OneByteNotEnoughSpace)
{
	ucs2_t c = L'(';
	const size_t s = 0;
	char b[1] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(SIZE_MAX, utf8convertucs2_le(c, b, s, &errors));
	EXPECT_STREQ("", b);
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(ConvertUcs2LE, TwoBytes)
{
	ucs2_t c = 0x80;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8convertucs2_le(c, b, s, &errors));
	EXPECT_STREQ("\xC2\x80", b);
	EXPECT_EQ(0, errors);
}

TEST(ConvertUcs2LE, TwoBytesMinimum)
{
	ucs2_t c = 0x80;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8convertucs2_le(c, b, s, &errors));
	EXPECT_STREQ("\xC2\x80", b);
	EXPECT_EQ(0, errors);
}

TEST(ConvertUcs2LE, TwoBytesMaximum)
{
	ucs2_t c = 0x7FF;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8convertucs2_le(c, b, s, &errors));
	EXPECT_STREQ("\xDF\xBF", b);
	EXPECT_EQ(0, errors);
}

TEST(ConvertUcs2LE, TwoBytesNotEnoughSpace)
{
	ucs2_t c = 0x912;
	const size_t s = 1;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(SIZE_MAX, utf8convertucs2_le(c, b, s, &errors));
	EXPECT_STREQ("", b);
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(ConvertUcs2LE, ThreeByteString)
{
	const size_t s = 256;
	char b[s] = { 0 };
	char* d = b;
	int32_t errors = 0;

	{
		ucs2_t c = L'㒱';
		size_t l = utf8convertucs2_le(c, d, s, &errors);
		EXPECT_EQ(3, l);
		EXPECT_EQ(0, errors);
		d += l;
	}

	{
		ucs2_t c = L'㒳';
		size_t l = utf8convertucs2_le(c, d, s, &errors);
		EXPECT_EQ(3, l);
		EXPECT_EQ(0, errors);
		d += l;
	}

	EXPECT_STREQ("\xE3\x92\xB1\xE3\x92\xB3", b);
}

TEST(ConvertUcs2LE, ThreeBytesMinimum)
{
	ucs2_t c = 0x800;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8convertucs2_le(c, b, s, &errors));
	EXPECT_STREQ("\xE0\xA0\x80", b);
	EXPECT_EQ(0, errors);
}

TEST(ConvertUcs2LE, ThreeBytesBeforeSurrogatePairs)
{
	ucs2_t c = 0xD7FF;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8convertucs2_le(c, b, s, &errors));
	EXPECT_STREQ("\xED\x9F\xBF", b);
	EXPECT_EQ(0, errors);
}

TEST(ConvertUcs2LE, ThreeBytesAfterSurrogatePairs)
{
	ucs2_t c = 0xE000;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8convertucs2_le(c, b, s, &errors));
	EXPECT_STREQ("\xEE\x80\x80", b);
	EXPECT_EQ(0, errors);
}

TEST(ConvertUcs2LE, ThreeBytesMaximum)
{
	ucs2_t c = 0xFFFF;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8convertucs2_le(c, b, s, &errors));
	EXPECT_STREQ("\xEF\xBF\xBF", b);
	EXPECT_EQ(0, errors);
}

TEST(ConvertUcs2LE, ThreeBytesNotEnoughSpace)
{
	ucs2_t c = 0xFEED;
	const size_t s = 2;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(SIZE_MAX, utf8convertucs2_le(c, b, s, &errors));
	EXPECT_STREQ("", b);
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(ConvertUcs2LE, SurrogateStart)
{
	ucs2_t c = 0xD800;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(SIZE_MAX, utf8convertucs2_le(c, b, s, &errors));
	EXPECT_STREQ("", b);
	EXPECT_EQ(UTF8_ERR_UNHANDLED_SURROGATE_PAIR, errors);
}

TEST(ConvertUcs2LE, SurrogateEnd)
{
	ucs2_t c = 0xDFFF;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(SIZE_MAX, utf8convertucs2_le(c, b, s, &errors));
	EXPECT_STREQ("", b);
	EXPECT_EQ(UTF8_ERR_UNHANDLED_SURROGATE_PAIR, errors);
}

TEST(ConvertUcs2LE, ZeroLength)
{
	ucs2_t c = 0x8812;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(SIZE_MAX, utf8convertucs2_le(c, b, 0, &errors));
	EXPECT_STREQ("", b);
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(ConvertUcs2LE, OutputLengthOneByte)
{
	ucs2_t c = 'B';
	int32_t errors = 0;

	EXPECT_EQ(1, utf8convertucs2_le(c, nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(ConvertUcs2LE, OutputLengthTwoBytes)
{
	ucs2_t c = 0x112;
	int32_t errors = 0;

	EXPECT_EQ(2, utf8convertucs2_le(c, nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(ConvertUcs2LE, OutputLengthThreeBytes)
{
	ucs2_t c = L'㒱';
	int32_t errors = 0;

	EXPECT_EQ(3, utf8convertucs2_le(c, nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(ConvertUcs2LE, OutputLengthSurrogate)
{
	ucs2_t c = 0xD801;
	int32_t errors = 0;

	EXPECT_EQ(SIZE_MAX, utf8convertucs2_le(c, nullptr, 0, &errors));
	EXPECT_EQ(UTF8_ERR_UNHANDLED_SURROGATE_PAIR, errors);
}