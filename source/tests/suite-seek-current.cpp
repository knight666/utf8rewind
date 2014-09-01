#include "tests-base.hpp"

#include "utf8rewind.h"

TEST(SeekCurrent, SwappedParameters)
{
	const char* t = "\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80";

	const char* r = utf8seek(t, t + strlen(t), 2, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x10480, o);
}

TEST(SeekCurrent, ZeroOffset)
{
	const char* t = "Banana";

	const char* r = utf8seek(t + 2, t, 0, SEEK_CUR);

	EXPECT_EQ(t + 2, r);
	EXPECT_STREQ("nana", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x6E, o);
}

TEST(SeekForward, Valid)
{
	const char* t = "\xCF\x84\xE1\xBD\xB4\xCE\xBD \xCE\xBA\xE1\xBD\xB9\xCF\x88\xCE\xB7";

	const char* r = utf8seek(t, t, 4, SEEK_CUR);

	EXPECT_EQ(t + 8, r);
	EXPECT_STREQ("\xCE\xBA\xE1\xBD\xB9\xCF\x88\xCE\xB7", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x3BA, o);
}

TEST(SeekForward, PastEnd)
{
	const char* t = "\xCE\xBC\xCE\xB5\xCF\x84\xCF\x81\xE1\xBD\xB1\xCE\xB5\xCE\xB9";

	const char* r = utf8seek(t + 2, t, 18, SEEK_CUR);

	EXPECT_EQ(t + 15, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = 0;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(SeekForward, SwappedParameters)
{
	const char* t = "10-12 \xD0\xBC\xD0\xB0\xD1\x80\xD1\x82\xD0\xB0 1997";

	const char* r = utf8seek(t, t + strlen(t), 6, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("10-12 \xD0\xBC\xD0\xB0\xD1\x80\xD1\x82\xD0\xB0 1997", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('1', o);
}

TEST(SeekForward, Ascii)
{
	const char* t = "Bringing down the ship.";

	const char* r = utf8seek(t, t, 14, SEEK_CUR);

	EXPECT_EQ(t + 14, r);
	EXPECT_STREQ("the ship.", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('t', o);
}

TEST(SeekForward, AsciiFromMiddle)
{
	const char* t = "Armageddon";

	const char* r = utf8seek(t + 4, t, 2, SEEK_CUR);

	EXPECT_EQ(t + 6, r);
	EXPECT_STREQ("ddon", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('d', o);
}

TEST(SeekForward, AsciiPastEnd)
{
	const char* t = "Peasants";

	const char* r = utf8seek(t, t, 15, SEEK_CUR);

	EXPECT_EQ(t + strlen(t), r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = 0;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(SeekForward, AsciiAtEnd)
{
	const char* t = "Shipping";

	const char* r = utf8seek(t + strlen(t), t, 2, SEEK_CUR);

	EXPECT_EQ(t + strlen(t), r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = 0;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(SeekForward, AsciiIllegalByteFE)
{
	const char* t = "Stuff\xFE happens";

	const char* r = utf8seek(t, t, 7, SEEK_CUR);

	EXPECT_EQ(t + 7, r);
	EXPECT_STREQ("happens", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('h', o);
}

TEST(SeekForward, AsciiIllegalByteFF)
{
	const char* t = "Mag\xFF KKD";

	const char* r = utf8seek(t, t, 6, SEEK_CUR);

	EXPECT_EQ(t + 6, r);
	EXPECT_STREQ("KD", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('K', o);
}

TEST(SeekForward, AsciiMalformedContinuationByte)
{
	const char* t = "Player: \x87" "bob";

	const char* r = utf8seek(t, t, 10, SEEK_CUR);

	EXPECT_EQ(t + 10, r);
	EXPECT_STREQ("ob", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('o', o);
}

TEST(SeekForward, TwoBytes)
{
	const char* t = "\xD0\xBB\xD0\xBE\xD0\xBA\xD0\xB0\xD0\xBB\xD0\xB8\xD0\xB7\xD0\xB0\xD1\x86\xD0\xB8\xD0\xB8";

	const char* r = utf8seek(t, t, 3, SEEK_CUR);

	EXPECT_EQ(t + 6, r);
	EXPECT_STREQ("\xD0\xB0\xD0\xBB\xD0\xB8\xD0\xB7\xD0\xB0\xD1\x86\xD0\xB8\xD0\xB8", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x430, o);
}

TEST(SeekForward, TwoBytesFromMiddle)
{
	const char* t = "\xD0\xBB\xD0\xBE\xD0\xBA\xD0\xB0\xD0\xBB\xD0\xB8\xD0\xB7\xD0\xB0\xD1\x86\xD0\xB8\xD0\xB8";

	const char* r = utf8seek(t + 6, t, 4, SEEK_CUR);

	EXPECT_EQ(t + 14, r);
	EXPECT_STREQ("\xD0\xB0\xD1\x86\xD0\xB8\xD0\xB8", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x430, o);
}

TEST(SeekForward, TwoBytesPastEnd)
{
	const char* t = "\xD0\xBB\xD0\xBE\xD0\xBA\xD0\xB0\xD0\xBB\xD0\xB8\xD0\xB7\xD0\xB0\xD1\x86\xD0\xB8\xD0\xB8";

	const char* r = utf8seek(t, t, 50, SEEK_CUR);

	EXPECT_EQ(t + strlen(t), r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = 0;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(SeekForward, TwoBytesAtEnd)
{
	const char* t = "\xD0\xBB\xD0\xBE\xD0\xBA\xD0\xB0\xD0\xBB\xD0\xB8\xD0\xB7\xD0\xB0\xD1\x86\xD0\xB8\xD0\xB8";

	const char* r = utf8seek(t + strlen(t), t, 2, SEEK_CUR);

	EXPECT_EQ(t + strlen(t), r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = 0;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(SeekForward, TwoBytesOverlong)
{
	const char* t = "\xC0\xAF\xC1\xBF\xC0\xAF";

	const char* r = utf8seek(t, t, 2, SEEK_CUR);

	EXPECT_EQ(t + 4, r);
	EXPECT_STREQ("\xC0\xAF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(SeekForward, TwoBytesOverlongFromMiddle)
{
	const char* t = "\xC0\xAF\xC1\xBF\xC0\xAF\xC0\xAF";

	const char* r = utf8seek(t + 4, t, 1, SEEK_CUR);

	EXPECT_EQ(t + 6, r);
	EXPECT_STREQ("\xC0\xAF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(SeekForward, TwoBytesOverlongPastEnd)
{
	const char* t = "\xC0\xAF\xC1\xBF\xC0\xAF";

	const char* r = utf8seek(t, t, 4, SEEK_CUR);

	EXPECT_EQ(t + strlen(t), r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = 0;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(SeekForward, TwoBytesLonelyStart)
{
	const char* t = "Gone\xC4" "Fishin'";

	const char* r = utf8seek(t, t, 5, SEEK_CUR);

	EXPECT_EQ(t + 6, r);
	EXPECT_STREQ("ishin'", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('i', o);
}

TEST(SeekForward, TwoBytesLonelyStartAtEnd)
{
	const char* t = "Megalodon\xDC";

	const char* r = utf8seek(t, t, 10, SEEK_CUR);

	EXPECT_EQ(t + 10, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = 0;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(SeekForward, ThreeBytes)
{
	const char* t = "\xE0\xA4\x81\xE0\xA4\x8B\xE0\xA4\xB4\xE0\xA4\xBD";

	const char* r = utf8seek(t, t, 2, SEEK_CUR);

	EXPECT_EQ(t + 6, r);
	EXPECT_STREQ("\xE0\xA4\xB4\xE0\xA4\xBD", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x934, o);
}

TEST(SeekForward, ThreeBytesFromMiddle)
{
	const char* t = "\xE0\xA4\x81\xE0\xA4\x8B\xE0\xA4\xB4\xE0\xA4\xBD";

	const char* r = utf8seek(t + 3, t, 2, SEEK_CUR);

	EXPECT_EQ(t + 9, r);
	EXPECT_STREQ("\xE0\xA4\xBD", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x93d, o);
}

TEST(SeekForward, ThreeBytesPastEnd)
{
	const char* t = "\xE0\xA4\x81\xE0\xA4\x8B\xE0\xA4\xB4\xE0\xA4\xBD";

	const char* r = utf8seek(t + 3, t, 4, SEEK_CUR);

	EXPECT_EQ(t + 12, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = 0;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(SeekForward, ThreeBytesAtEnd)
{
	const char* t = "\xE0\xA4\x81\xE0\xA4\x8B\xE0\xA4\xB4\xE0\xA4\xBD";

	const char* r = utf8seek(t + strlen(t), t, 2, SEEK_CUR);

	EXPECT_EQ(t + strlen(t), r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = 0;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(SeekForward, ThreeBytesOverlong)
{
	const char* t = "\xE0\x80\xAF\xE0\x9F\xBF\xE0\x80\xAF";

	const char* r = utf8seek(t, t, 1, SEEK_CUR);

	EXPECT_EQ(t + 3, r);
	EXPECT_STREQ("\xE0\x9F\xBF\xE0\x80\xAF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(SeekForward, ThreeBytesOverlongFromMiddle)
{
	const char* t = "\xE0\x80\xAF\xE0\x9F\xBF\xE0\x80\xAF\xE0\x80\xAF";

	const char* r = utf8seek(t + 6, t, 1, SEEK_CUR);

	EXPECT_EQ(t + 9, r);
	EXPECT_STREQ("\xE0\x80\xAF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(SeekForward, ThreeBytesOverlongPastEnd)
{
	const char* t = "\xE0\x80\xAF\xE0\x80\xAF\xE0\x80\xAF";

	const char* r = utf8seek(t, t, 4, SEEK_CUR);

	EXPECT_EQ(t + strlen(t), r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = 0;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(SeekForward, ThreeBytesLonelyStart)
{
	const char* t = "Rocker\xE4" "coaster";

	const char* r = utf8seek(t, t, 8, SEEK_CUR);

	EXPECT_EQ(t + 10, r);
	EXPECT_STREQ("ster", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('s', o);
}

TEST(SeekForward, ThreeBytesLonelyStartAtEnd)
{
	const char* t = "Submarine\xED";

	const char* r = utf8seek(t, t, 10, SEEK_CUR);

	EXPECT_EQ(t + 10, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = 0;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(SeekForward, ThreeBytesNotEnoughData)
{
	const char* t = "Compan\xEF\xBFions";

	const char* r = utf8seek(t, t, 8, SEEK_CUR);

	EXPECT_EQ(t + 10, r);
	EXPECT_STREQ("ns", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('n', o);
}

TEST(SeekForward, ThreeBytesNotEnoughDataAtEnd)
{
	const char* t = "BBBark\xEF\xBF";

	const char* r = utf8seek(t, t, 7, SEEK_CUR);

	EXPECT_EQ(t + 8, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = 0;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(SeekForward, FourBytes)
{
	const char* t = "\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80";

	const char* r = utf8seek(t, t, 2, SEEK_CUR);

	EXPECT_EQ(t + 8, r);
	EXPECT_STREQ("\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x10480, o);
}

TEST(SeekForward, FourBytesFromMiddle)
{
	const char* t = "\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80";

	const char* r = utf8seek(t + 4, t, 3, SEEK_CUR);

	EXPECT_EQ(t + 16, r);
	EXPECT_STREQ("\xF0\x90\x92\x80", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x10480, o);
}

TEST(SeekForward, FourBytesPastEnd)
{
	const char* t = "\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80";

	const char* r = utf8seek(t, t, 25, SEEK_CUR);

	EXPECT_EQ(t + strlen(t), r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = 0;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(SeekForward, FourBytesAtEnd)
{
	const char* t = "\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80";

	const char* r = utf8seek(t + strlen(t), t, 2, SEEK_CUR);

	EXPECT_EQ(t + strlen(t), r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = 0;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(SeekForward, FourBytesOverlong)
{
	const char* t = "\xF0\x80\x80\xAF\xF0\x80\x80\xAF\xF4\x90\x80\x80";

	const char* r = utf8seek(t, t, 1, SEEK_CUR);

	EXPECT_EQ(t + 4, r);
	EXPECT_STREQ("\xF0\x80\x80\xAF\xF4\x90\x80\x80", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(SeekForward, FourBytesOverlongFromMiddle)
{
	const char* t = "\xF0\x80\x80\xAF\xF0\x80\x80\xAF\xF7\xBF\xBF\xBF\xF4\x90\x80\x80";

	const char* r = utf8seek(t + 8, t, 1, SEEK_CUR);

	EXPECT_EQ(t + 12, r);
	EXPECT_STREQ("\xF4\x90\x80\x80", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(SeekForward, FourBytesOverlongPastEnd)
{
	const char* t = "\xF0\x80\x80\xAF\xF0\x80\x80\xAF\xF7\xBF\xBF\xBF\xF4\x90\x80\x80";

	const char* r = utf8seek(t, t, 6, SEEK_CUR);

	EXPECT_EQ(t + strlen(t), r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = 0;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(SeekForward, FourBytesLonelyStart)
{
	const char* t = "Clam\xF4shellpower";

	const char* r = utf8seek(t, t, 8, SEEK_CUR);

	EXPECT_EQ(t + 11, r);
	EXPECT_STREQ("ower", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('o', o);
}

TEST(SeekForward, FourBytesLonelyStartAtEnd)
{
	const char* t = "Magic\xF6";

	const char* r = utf8seek(t, t, 6, SEEK_CUR);

	EXPECT_EQ(t + 6, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = 0;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(SeekForward, FourBytesNotEnoughData)
{
	const char* t = "Brilli\xF0\x90\x80" "ant";

	const char* r = utf8seek(t, t, 8, SEEK_CUR);

	EXPECT_EQ(t + 11, r);
	EXPECT_STREQ("t", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('t', o);
}

TEST(SeekForward, FourBytesNotEnoughDataAtEnd)
{
	const char* t = "Night\xF0\x90\x80";

	const char* r = utf8seek(t, t, 6, SEEK_CUR);

	EXPECT_EQ(t + 8, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = 0;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(SeekForward, FiveBytesOverlong)
{
	const char* t = "\xF8\x88\x80\x80\x80\xF8\x80\x80\x80\xAF\xFB\xBF\xBF\xBF\xBF";

	const char* r = utf8seek(t, t, 1, SEEK_CUR);

	EXPECT_EQ(t + 5, r);
	EXPECT_STREQ("\xF8\x80\x80\x80\xAF\xFB\xBF\xBF\xBF\xBF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(SeekForward, FiveBytesOverlongFromMiddle)
{
	const char* t = "\xF8\x88\x80\x80\x80\xF8\x80\x80\x80\xAF\xFB\xBF\xBF\xBF\xBF";

	const char* r = utf8seek(t + 5, t, 1, SEEK_CUR);

	EXPECT_EQ(t + 10, r);
	EXPECT_STREQ("\xFB\xBF\xBF\xBF\xBF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(SeekForward, FiveBytesOverlongPastEnd)
{
	const char* t = "\xF8\x88\x80\x80\x80\xF8\x80\x80\x80\xAF\xFB\xBF\xBF\xBF\xBF";

	const char* r = utf8seek(t, t, 13, SEEK_CUR);

	EXPECT_EQ(t + strlen(t), r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = 0;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(SeekForward, FiveBytesLonelyStart)
{
	const char* t = "Beyond\xFAMegalodon";

	const char* r = utf8seek(t, t, 7, SEEK_CUR);

	EXPECT_EQ(t + 11, r);
	EXPECT_STREQ("lodon", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('l', o);
}

TEST(SeekForward, FiveBytesLonelyStartAtEnd)
{
	const char* t = "Dinos\xFA";

	const char* r = utf8seek(t, t, 6, SEEK_CUR);

	EXPECT_EQ(t + 6, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = 0;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(SeekForward, FiveBytesNotEnoughData)
{
	const char* t = "Central\xF8\x80\x80\x80Plaza baker";

	const char* r = utf8seek(t, t, 8, SEEK_CUR);

	EXPECT_EQ(t + 12, r);
	EXPECT_STREQ("laza baker", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('l', o);
}

TEST(SeekForward, FiveBytesNotEnoughDataAtEnd)
{
	const char* t = "Apple\xF8\x80\x80\x80";

	const char* r = utf8seek(t, t, 6, SEEK_CUR);

	EXPECT_EQ(t + 9, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = 0;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(SeekForward, SixBytesOverlong)
{
	const char* t = "\xFC\x84\x80\x80\x80\x80\xFC\x80\x80\x80\x80\x80\xFC\x84\x80\x80\x80\x80\xFD\xBF\xBF\xBF\xBF\xBF";

	const char* r = utf8seek(t, t, 2, SEEK_CUR);

	EXPECT_EQ(t + 12, r);
	EXPECT_STREQ("\xFC\x84\x80\x80\x80\x80\xFD\xBF\xBF\xBF\xBF\xBF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(SeekForward, SixBytesOverlongFromMiddle)
{
	const char* t = "\xFC\x84\x80\x80\x80\x80\xFC\x80\x80\x80\x80\x80\xFC\x84\x80\x80\x80\x80\xFD\xBF\xBF\xBF\xBF\xBF";

	const char* r = utf8seek(t + 6, t, 2, SEEK_CUR);

	EXPECT_EQ(t + 18, r);
	EXPECT_STREQ("\xFD\xBF\xBF\xBF\xBF\xBF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(SeekForward, SixBytesOverlongPastEnd)
{
	const char* t = "\xFC\x84\x80\x80\x80\x80\xFC\x80\x80\x80\x80\x80\xFC\x84\x80\x80\x80\x80\xFD\xBF\xBF\xBF\xBF\xBF";

	const char* r = utf8seek(t, t, 81, SEEK_CUR);

	EXPECT_EQ(t + strlen(t), r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = 0;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(SeekForward, SixBytesLonelyStart)
{
	const char* t = "FC\xFCKnudde festival";

	const char* r = utf8seek(t, t, 8, SEEK_CUR);

	EXPECT_EQ(t + 13, r);
	EXPECT_STREQ("tival", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('t', o);
}

TEST(SeekForward, SixBytesLonelyStartAtEnd)
{
	const char* t = "Stuff\xFD";

	const char* r = utf8seek(t, t, 6, SEEK_CUR);

	EXPECT_EQ(t + 6, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = 0;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(SeekForward, SixBytesNotEnoughData)
{
	const char* t = "Hint\xFC\x80\x80\x80\x80machine";

	const char* r = utf8seek(t, t, 6, SEEK_CUR);

	EXPECT_EQ(t + 11, r);
	EXPECT_STREQ("chine", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('c', o);
}

TEST(SeekForward, SixBytesNotEnoughDataAtEnd)
{
	const char* t = "Dull\xFC\x80\x80\x80\x80";

	const char* r = utf8seek(t, t, 5, SEEK_CUR);

	EXPECT_EQ(t + 9, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = 0;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(SeekForward, SixBytesSwappedParameters)
{
	const char* t = "\xFC\x84\x80\x80\x80\x80\xFC\x80\x80\x80\x80\x80\xFC\x84\x80\x80\x80\x80\xFD\xBF\xBF\xBF\xBF\xBF";

	const char* r = utf8seek(t, t + strlen(t), 2, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xFC\x84\x80\x80\x80\x80\xFC\x80\x80\x80\x80\x80\xFC\x84\x80\x80\x80\x80\xFD\xBF\xBF\xBF\xBF\xBF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(SeekBackward, EndsWithAscii)
{
	const char* t = "Tree\xD1\x86j";

	const char* r = utf8seek(t + strlen(t), t, -3, SEEK_CUR);

	EXPECT_EQ(t + 3, r);
	EXPECT_STREQ("e\xD1\x86j", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('e', o);
}

TEST(SeekBackward, Ascii)
{
	const char* t = "Paper boat";

	const char* r = utf8seek(t + strlen(t), t, -2, SEEK_CUR);

	EXPECT_EQ(t + 8, r);
	EXPECT_STREQ("at", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('a', o);
}

TEST(SeekBackward, AsciiFromMiddle)
{
	const char* t = "Banananana";

	const char* r = utf8seek(t + 6, t, -4, SEEK_CUR);

	EXPECT_EQ(t + 2, r);
	EXPECT_STREQ("nananana", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('n', o);
}

TEST(SeekBackward, AsciiZeroOffset)
{
	const char* t = "Apple";

	const char* r = utf8seek(t + strlen(t) - 2, t, 0, SEEK_CUR);

	EXPECT_EQ(t + strlen(t) - 2, r);
	EXPECT_STREQ("le", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('l', o);
}

TEST(SeekBackward, AsciiIllegalByteFE)
{
	const char* t = "Stuff\xFE happens";

	const char* r = utf8seek(t + 14, t, -11, SEEK_CUR);

	EXPECT_EQ(t + 3, r);
	EXPECT_STREQ("ff\xFE happens", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('f', o);
}

TEST(SeekBackward, AsciiIllegalByteFF)
{
	const char* t = "Mag\xFF KKD";

	const char* r = utf8seek(t + 8, t, -7, SEEK_CUR);

	EXPECT_EQ(t + 1, r);
	EXPECT_STREQ("ag\xFF KKD", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('a', o);
}

TEST(SeekBackward, AsciiMalformedContinuationByte)
{
	const char* t = "Player: \x87" "bob";

	const char* r = utf8seek(t + 12, t, -8, SEEK_CUR);

	EXPECT_EQ(t + 3, r);
	EXPECT_STREQ("yer: \x87" "bob", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('y', o);
}

TEST(SeekBackward, AsciiPastStart)
{
	const char* t = "Beautiful day.";

	const char* r = utf8seek(t + strlen(t), t, -50, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("Beautiful day.", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('B', o);
}

TEST(SeekBackward, AsciiAtStart)
{
	const char* t = "Blogspam";

	const char* r = utf8seek(t, t, -4, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("Blogspam", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('B', o);
}

TEST(SeekBackward, AsciiSwappedParameters)
{
	const char* t = "100 km/h";

	const char* r = utf8seek(t, t + strlen(t), -2, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("100 km/h", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('1', o);
}

TEST(SeekBackward, AsciiEndsInMiddle)
{
	const char* t = "Massive\0Pony";

	const char* r = utf8seek(t + 12, t, -5, SEEK_CUR);

	EXPECT_EQ(t + 7, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = 0;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(SeekBackward, TwoBytes)
{
	const char* t = "\xD0\xBB\xD0\xBE\xD0\xBA\xD0\xB0\xD0\xBB\xD0\xB8\xD0\xB7\xD0\xB0\xD1\x86\xD0\xB8\xD0\xB8";

	const char* r = utf8seek(t + strlen(t), t, -3, SEEK_CUR);

	EXPECT_EQ(t + 16, r);
	EXPECT_STREQ("\xD1\x86\xD0\xB8\xD0\xB8", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x446, o);
}

TEST(SeekBackward, TwoBytesFromMiddle)
{
	const char* t = "\xD0\xBB\xD0\xBE\xD0\xBA\xD0\xB0\xD0\xBB\xD0\xB8\xD0\xB7\xD0\xB0\xD1\x86\xD0\xB8\xD0\xB8";

	const char* r = utf8seek(t + 8, t, -3, SEEK_CUR);

	EXPECT_EQ(t + 2, r);
	EXPECT_STREQ("\xD0\xBE\xD0\xBA\xD0\xB0\xD0\xBB\xD0\xB8\xD0\xB7\xD0\xB0\xD1\x86\xD0\xB8\xD0\xB8", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x43E, o);
}

TEST(SeekBackward, TwoBytesZeroOffset)
{
	const char* t = "\xD0\xB0\xD1\x86\xD0\xB8";

	const char* r = utf8seek(t + strlen(t), t, 0, SEEK_CUR);

	EXPECT_EQ(t + strlen(t), r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = 0;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(SeekBackward, TwoBytesPastStart)
{
	const char* t = "\xD0\xBE\xD0\xBA\xD0\xB0\xD0\xBB";

	const char* r = utf8seek(t + 6, t, -16, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xD0\xBE\xD0\xBA\xD0\xB0\xD0\xBB", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x43E, o);
}

TEST(SeekBackward, TwoBytesAtStart)
{
	const char* t = "\xD0\xBE\xD0\xBA\xD0\xB0\xD0\xBB";

	const char* r = utf8seek(t, t, -2, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xD0\xBE\xD0\xBA\xD0\xB0\xD0\xBB", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x43E, o);
}

TEST(SeekBackward, TwoBytesOverlong)
{
	const char* t = "\xC0\xAF\xC1\xBF\xC0\xAF";

	const char* r = utf8seek(t + 6, t, -1, SEEK_CUR);

	EXPECT_EQ(t + 4, r);
	EXPECT_STREQ("\xC0\xAF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(SeekBackward, TwoBytesOverlongFromMiddle)
{
	const char* t = "\xC0\xAF\xC1\xBF\xC0\xAF\xC0\xAF";

	const char* r = utf8seek(t + 6, t, -1, SEEK_CUR);

	EXPECT_EQ(t + 4, r);
	EXPECT_STREQ("\xC0\xAF\xC0\xAF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(SeekBackward, TwoBytesOverlongPastStart)
{
	const char* t = "\xC0\xAF\xC1\xBF\xC0\xAF";

	const char* r = utf8seek(t + strlen(t), t, -6, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xC0\xAF\xC1\xBF\xC0\xAF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(SeekBackward, TwoBytesLonelyStart)
{
	const char* t = "Gone\xC4" "Fishin'";

	const char* r = utf8seek(t + strlen(t), t, -8, SEEK_CUR);

	EXPECT_EQ(t + 4, r);
	EXPECT_STREQ("\xC4" "Fishin'", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(SeekBackward, TwoBytesLonelyStartAtStart)
{
	const char* t = "\xDCMegalodon";

	const char* r = utf8seek(t + strlen(t), t, -10, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xDCMegalodon", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(SeekBackward, TwoBytesSwappedParameters)
{
	const char* t = "\xD0\xBE\xD0\xBA\xD0\xB0\xD0\xBB";

	const char* r = utf8seek(t, t + strlen(t), -3, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xD0\xBE\xD0\xBA\xD0\xB0\xD0\xBB", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x43E, o);
}

TEST(SeekBackward, TwoBytesEndsInMiddle)
{
	const char* t = "\xD0\xBE\xD0\xBA\0\xD0\xB0\xD0\xBB";

	const char* r = utf8seek(t + 9, t, -4, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xD0\xBE\xD0\xBA", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x43E, o);
}

TEST(SeekBackward, ThreeBytes)
{
	const char* t = "\xE0\xA4\x81\xE0\xA4\x8B\xE0\xA4\xB4\xE0\xA4\xBD";

	const char* r = utf8seek(t + strlen(t), t, -2, SEEK_CUR);

	EXPECT_EQ(t + 6, r);
	EXPECT_STREQ("\xE0\xA4\xB4\xE0\xA4\xBD", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x934, o);
}

TEST(SeekBackward, ThreeBytesFromMiddle)
{
	const char* t = "\xE0\xA4\x81\xE0\xA4\x8B\xE0\xA4\xB4\xE0\xA4\xBD";

	const char* r = utf8seek(t + 9, t, -2, SEEK_CUR);

	EXPECT_EQ(t + 3, r);
	EXPECT_STREQ("\xE0\xA4\x8B\xE0\xA4\xB4\xE0\xA4\xBD", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x90B, o);
}

TEST(SeekBackward, ThreeBytesZeroOffset)
{
	const char* t = "\xE0\xA4\x81\xE0\xA4\x8B\xE0\xA4\xB4\xE0\xA4\xBD";

	const char* r = utf8seek(t + 9, t, 0, SEEK_CUR);

	EXPECT_EQ(t + 9, r);
	EXPECT_STREQ("\xE0\xA4\xBD", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x93D, o);
}

TEST(SeekBackward, ThreeBytesPastStart)
{
	const char* t = "\xE0\xA4\x81\xE0\xA4\x8B\xE0\xA4\xB4\xE0\xA4\xBD";

	const char* r = utf8seek(t + strlen(t), t, -26, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xE0\xA4\x81\xE0\xA4\x8B\xE0\xA4\xB4\xE0\xA4\xBD", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x901, o);
}

TEST(SeekBackward, ThreeBytesAtStart)
{
	const char* t = "\xE0\xA4\x81\xE0\xA4\x8B\xE0\xA4\xB4\xE0\xA4\xBD";

	const char* r = utf8seek(t, t, -2, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xE0\xA4\x81\xE0\xA4\x8B\xE0\xA4\xB4\xE0\xA4\xBD", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x901, o);
}

TEST(SeekBackward, ThreeBytesOverlong)
{
	const char* t = "\xE0\x80\xAF\xE0\x9F\xBF\xE0\x80\xAF";

	const char* r = utf8seek(t + strlen(t), t, -1, SEEK_CUR);

	EXPECT_EQ(t + 6, r);
	EXPECT_STREQ("\xE0\x80\xAF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(SeekBackward, ThreeBytesOverlongFromMiddle)
{
	const char* t = "\xE0\x80\xAF\xE0\x9F\xBF\xE0\x80\xAF\xE0\x80\xAF";

	const char* r = utf8seek(t + 9, t, -1, SEEK_CUR);

	EXPECT_EQ(t + 6, r);
	EXPECT_STREQ("\xE0\x80\xAF\xE0\x80\xAF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(SeekBackward, ThreeBytesOverlongPastStart)
{
	const char* t = "\xE0\x80\xAF\xE0\x80\xAF\xE0\x80\xAF";

	const char* r = utf8seek(t + strlen(t), t, -4, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xE0\x80\xAF\xE0\x80\xAF\xE0\x80\xAF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(SeekBackward, ThreeBytesLonelyStart)
{
	const char* t = "Rocker\xE4" "coaster";

	const char* r = utf8seek(t + strlen(t), t, -10, SEEK_CUR);

	EXPECT_EQ(t + 4, r);
	EXPECT_STREQ("er\xE4" "coaster", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('e', o);
}

TEST(SeekBackward, ThreeBytesLonelyStartAtStart)
{
	const char* t = "\xEDSubmarine";

	const char* r = utf8seek(t + strlen(t), t, -10, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xEDSubmarine", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(SeekBackward, ThreeBytesNotEnoughData)
{
	const char* t = "Compan\xEF\xBFions";

	const char* r = utf8seek(t + strlen(t), t, -6, SEEK_CUR);

	EXPECT_EQ(t + 5, r);
	EXPECT_STREQ("n\xEF\xBFions", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('n', o);
}

TEST(SeekBackward, ThreeBytesNotEnoughDataAtStart)
{
	const char* t = "\xEF\xBF" "BBBark";

	const char* r = utf8seek(t + strlen(t), t, -7, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xEF\xBF" "BBBark", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(SeekBackward, ThreeBytesSwappedParameters)
{
	const char* t = "\xE0\xA4\x81\xE0\xA4\x8B\xE0\xA4\xB4\xE0\xA4\xBD";

	const char* r = utf8seek(t, t + strlen(t), -2, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xE0\xA4\x81\xE0\xA4\x8B\xE0\xA4\xB4\xE0\xA4\xBD", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x901, o);
}

TEST(SeekBackward, ThreeBytesEndsInMiddle)
{
	const char* t = "\xE0\xA4\x81\xE0\xA4\x8B\0\xE0\xA4\xB4\xE0\xA4\xBD";

	const char* r = utf8seek(t + 13, t, -5, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xE0\xA4\x81\xE0\xA4\x8B", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x901, o);
}

TEST(SeekBackward, FourBytes)
{
	const char* t = "\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80";

	const char* r = utf8seek(t + strlen(t), t, -2, SEEK_CUR);

	EXPECT_EQ(t + 12, r);
	EXPECT_STREQ("\xF0\x90\x92\x80\xF0\x90\x92\x80", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x10480, o);
}

TEST(SeekBackward, FourBytesFromMiddle)
{
	const char* t = "\xF0\x9F\x88\xB8\xF0\x9F\x89\x90\xF0\x9F\x89\x81\xF0\x9F\x89\x9A\xF0\x9F\x89\x9E";

	const char* r = utf8seek(t + 16, t, -3, SEEK_CUR);

	EXPECT_EQ(t + 4, r);
	EXPECT_STREQ("\xF0\x9F\x89\x90\xF0\x9F\x89\x81\xF0\x9F\x89\x9A\xF0\x9F\x89\x9E", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x1F250, o);
}

TEST(SeekBackward, FourBytesZeroOffset)
{
	const char* t = "\xF0\x9F\x88\xB8\xF0\x9F\x89\x90\xF0\x9F\x89\x81\xF0\x9F\x89\x9A\xF0\x9F\x89\x9E";

	const char* r = utf8seek(t + 16, t, 0, SEEK_CUR);

	EXPECT_EQ(t + 16, r);
	EXPECT_STREQ("\xF0\x9F\x89\x9E", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x1F25E, o);
}

TEST(SeekBackward, FourBytesPastStart)
{
	const char* t = "\xF0\x9F\x88\xB8\xF0\x9F\x89\x90\xF0\x9F\x89\x81\xF0\x9F\x89\x9A\xF0\x9F\x89\x9E";

	const char* r = utf8seek(t + strlen(t), t, -12, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xF0\x9F\x88\xB8\xF0\x9F\x89\x90\xF0\x9F\x89\x81\xF0\x9F\x89\x9A\xF0\x9F\x89\x9E", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x1F238, o);
}

TEST(SeekBackward, FourBytesAtStart)
{
	const char* t = "\xF0\x9F\x88\xB8\xF0\x9F\x89\x90\xF0\x9F\x89\x81\xF0\x9F\x89\x9A\xF0\x9F\x89\x9E";

	const char* r = utf8seek(t, t, -2, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xF0\x9F\x88\xB8\xF0\x9F\x89\x90\xF0\x9F\x89\x81\xF0\x9F\x89\x9A\xF0\x9F\x89\x9E", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x1F238, o);
}

TEST(SeekBackward, FourBytesOverlong)
{
	const char* t = "\xF0\x80\x80\xAF\xF0\x80\x80\xAF\xF4\x90\x80\x80";

	const char* r = utf8seek(t + strlen(t), t, -1, SEEK_CUR);

	EXPECT_EQ(t + 8, r);
	EXPECT_STREQ("\xF4\x90\x80\x80", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(SeekBackward, FourBytesOverlongFromMiddle)
{
	const char* t = "\xF0\x80\x80\xAF\xF0\x80\x80\xAF\xF7\xBF\xBF\xBF\xF4\x90\x80\x80";

	const char* r = utf8seek(t + 12, t, -2, SEEK_CUR);

	EXPECT_EQ(t + 4, r);
	EXPECT_STREQ("\xF0\x80\x80\xAF\xF7\xBF\xBF\xBF\xF4\x90\x80\x80", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(SeekBackward, FourBytesOverlongPastStart)
{
	const char* t = "\xF0\x80\x80\xAF\xF0\x80\x80\xAF\xF7\xBF\xBF\xBF\xF4\x90\x80\x80";

	const char* r = utf8seek(t + strlen(t), t, -5, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xF0\x80\x80\xAF\xF0\x80\x80\xAF\xF7\xBF\xBF\xBF\xF4\x90\x80\x80", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(SeekBackward, FourBytesLonelyStart)
{
	const char* t = "Clam\xF4shellpower";

	const char* r = utf8seek(t + strlen(t), t, -12, SEEK_CUR);

	EXPECT_EQ(t + 3, r);
	EXPECT_STREQ("m\xF4shellpower", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('m', o);
}

TEST(SeekBackward, FourBytesLonelyStartAtStart)
{
	const char* t = "\xF6Magic";

	const char* r = utf8seek(t + strlen(t), t, -6, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xF6Magic", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(SeekBackward, FourBytesNotEnoughData)
{
	const char* t = "Brilli\xF0\x90\x80" "ant";

	const char* r = utf8seek(t + strlen(t), t, -4, SEEK_CUR);

	EXPECT_EQ(t + 6, r);
	EXPECT_STREQ("\xF0\x90\x80" "ant", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(SeekBackward, FourBytesNotEnoughDataAtStart)
{
	const char* t = "\xF0\x90\x80Night";

	const char* r = utf8seek(t + strlen(t), t, -6, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xF0\x90\x80Night", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(SeekBackward, FourBytesSwappedParameters)
{
	const char* t = "\xF0\x9F\x88\xB8\xF0\x9F\x89\x90\xF0\x9F\x89\x81\xF0\x9F\x89\x9A\xF0\x9F\x89\x9E";

	const char* r = utf8seek(t, t + strlen(t), -8, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xF0\x9F\x88\xB8\xF0\x9F\x89\x90\xF0\x9F\x89\x81\xF0\x9F\x89\x9A\xF0\x9F\x89\x9E", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x1F238, o);
}

TEST(SeekBackward, FourBytesEndsInMiddle)
{
	const char* t = "\xF0\x9F\x88\xB8\xF0\x9F\x89\x90\xF0\x9F\x89\x81\0\xF0\x9F\x89\x9A\xF0\x9F\x89\x9E";

	const char* r = utf8seek(t + 21, t, -4, SEEK_CUR);

	EXPECT_EQ(t + 8, r);
	EXPECT_STREQ("\xF0\x9F\x89\x81", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x1F241, o);
}

TEST(SeekBackward, FiveBytesOverlong)
{
	const char* t = "\xF8\x88\x80\x80\x80\xF8\x80\x80\x80\xAF\xFB\xBF\xBF\xBF\xBF";

	const char* r = utf8seek(t + strlen(t), t, -2, SEEK_CUR);

	EXPECT_EQ(t + 5, r);
	EXPECT_STREQ("\xF8\x80\x80\x80\xAF\xFB\xBF\xBF\xBF\xBF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(SeekBackward, FiveBytesOverlongFromMiddle)
{
	const char* t = "\xF8\x88\x80\x80\x80\xF8\x80\x80\x80\xAF\xFB\xBF\xBF\xBF\xBF";

	const char* r = utf8seek(t + 10, t, -1, SEEK_CUR);

	EXPECT_EQ(t + 5, r);
	EXPECT_STREQ("\xF8\x80\x80\x80\xAF\xFB\xBF\xBF\xBF\xBF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(SeekBackward, FiveBytesOverlongPastStart)
{
	const char* t = "\xF8\x88\x80\x80\x80\xF8\x80\x80\x80\xAF\xFB\xBF\xBF\xBF\xBF";

	const char* r = utf8seek(t + strlen(t), t, -13, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xF8\x88\x80\x80\x80\xF8\x80\x80\x80\xAF\xFB\xBF\xBF\xBF\xBF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(SeekBackward, FiveBytesLonelyStart)
{
	const char* t = "Beyond\xFAMegalodon";

	const char* r = utf8seek(t + strlen(t), t, -12, SEEK_CUR);

	EXPECT_EQ(t + 4, r);
	EXPECT_STREQ("nd\xFAMegalodon", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('n', o);
}

TEST(SeekBackward, FiveBytesLonelyStartAtStart)
{
	const char* t = "\xFA" "Brontos";

	const char* r = utf8seek(t, t, -7, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xFA" "Brontos", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(SeekBackward, FiveBytesNotEnoughData)
{
	const char* t = "Central\xF8\x80\x80\x80Plaza baker";

	const char* r = utf8seek(t + strlen(t), t, -13, SEEK_CUR);

	EXPECT_EQ(t + 6, r);
	EXPECT_STREQ("l\xF8\x80\x80\x80Plaza baker", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('l', o);
}

TEST(SeekBackward, FiveBytesNotEnoughDataAtStart)
{
	const char* t = "\xF8\x80\x80\x80" "Apple";

	const char* r = utf8seek(t + strlen(t), t, -6, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xF8\x80\x80\x80" "Apple", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(SeekBackward, FiveBytesSwappedParameters)
{
	const char* t = "\xF8\x88\x80\x80\x80\xF8\x80\x80\x80\xAF\xFB\xBF\xBF\xBF\xBF";

	const char* r = utf8seek(t, t + strlen(t), -2, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xF8\x88\x80\x80\x80\xF8\x80\x80\x80\xAF\xFB\xBF\xBF\xBF\xBF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(SeekBackward, FiveBytesEndsInMiddle)
{
	const char* t = "\xF8\x88\x80\x80\x80\xF8\x80\x80\x80\xAF\0\xFB\xBF\xBF\xBF\xBF";

	const char* r = utf8seek(t + 16, t, -2, SEEK_CUR);

	EXPECT_EQ(t + 10, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = 0;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(SeekBackward, SixBytesOverlong)
{
	const char* t = "\xFC\x84\x80\x80\x80\x80\xFC\x80\x80\x80\x80\x80\xFC\x84\x80\x80\x80\x80\xFD\xBF\xBF\xBF\xBF\xBF";

	const char* r = utf8seek(t + strlen(t), t, -2, SEEK_CUR);

	EXPECT_EQ(t + 12, r);
	EXPECT_STREQ("\xFC\x84\x80\x80\x80\x80\xFD\xBF\xBF\xBF\xBF\xBF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(SeekBackward, SixBytesOverlongFromMiddle)
{
	const char* t = "\xFC\x84\x80\x80\x80\x80\xFC\x80\x80\x80\x80\x80\xFC\x84\x80\x80\x80\x80\xFD\xBF\xBF\xBF\xBF\xBF";

	const char* r = utf8seek(t + 12, t, -1, SEEK_CUR);

	EXPECT_EQ(t + 6, r);
	EXPECT_STREQ("\xFC\x80\x80\x80\x80\x80\xFC\x84\x80\x80\x80\x80\xFD\xBF\xBF\xBF\xBF\xBF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(SeekBackward, SixBytesOverlongPastStart)
{
	const char* t = "\xFC\x84\x80\x80\x80\x80\xFC\x80\x80\x80\x80\x80\xFC\x84\x80\x80\x80\x80\xFD\xBF\xBF\xBF\xBF\xBF";

	const char* r = utf8seek(t + strlen(t), t, -8, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xFC\x84\x80\x80\x80\x80\xFC\x80\x80\x80\x80\x80\xFC\x84\x80\x80\x80\x80\xFD\xBF\xBF\xBF\xBF\xBF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(SeekBackward, SixBytesLonelyStart)
{
	const char* t = "FC\xFCKnudde festival";

	const char* r = utf8seek(t + strlen(t), t, -16, SEEK_CUR);

	EXPECT_EQ(t + 2, r);
	EXPECT_STREQ("\xFCKnudde festival", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(SeekBackward, SixBytesLonelyStartAtStart)
{
	const char* t = "\xFDStuff";

	const char* r = utf8seek(t + strlen(t), t, -6, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xFDStuff", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(SeekBackward, SixBytesNotEnoughData)
{
	const char* t = "Hint\xFC\x80\x80\x80\x80machine";

	const char* r = utf8seek(t + strlen(t), t, -9, SEEK_CUR);

	EXPECT_EQ(t + 3, r);
	EXPECT_STREQ("t\xFC\x80\x80\x80\x80machine", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('t', o);
}

TEST(SeekBackward, SixBytesNotEnoughDataAtStart)
{
	const char* t = "\xFC\x80\x80\x80\x80" "Dull";

	const char* r = utf8seek(t + strlen(t), t, -5, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xFC\x80\x80\x80\x80" "Dull", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(SeekBackward, SixBytesEndsInMiddle)
{
	const char* t = "\xFC\x84\x80\x80\x80\x80\xFC\x80\x80\x80\x80\x80\xFC\x84\x80\x80\x80\x80\0\xFD\xBF\xBF\xBF\xBF\xBF";

	const char* r = utf8seek(t + 25, t, -2, SEEK_CUR);

	EXPECT_EQ(t + 18, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = 0;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}