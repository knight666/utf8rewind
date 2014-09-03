#include "tests-base.hpp"

#include "utf8rewind.h"

TEST(SeekStart, Valid)
{
	const char* t = "\xE0\xA4\x81\xE0\xA4\x8B\xE0\xA4\xB4\xE0\xA4\xBD";

	const char* r = utf8seek(t, t, 2, SEEK_SET);

	EXPECT_EQ(t + 6, r);
	EXPECT_STREQ("\xE0\xA4\xB4\xE0\xA4\xBD", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x934, o);
}

TEST(SeekSet, Offset)
{
	const char* t = "The Doctor";

	const char* r = utf8seek(t + 4, t, 4, SEEK_SET);

	EXPECT_EQ(t + 4, r);
	EXPECT_STREQ("Doctor", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('D', o);
}

TEST(SeekSet, ZeroOffset)
{
	const char* t = "Magic powered";

	const char* r = utf8seek(t, t, 0, SEEK_SET);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("Magic powered", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('M', o);
}

TEST(SeekSet, NegativeOffset)
{
	const char* t = "Dreaming";

	const char* r = utf8seek(t, t, -12, SEEK_SET);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("Dreaming", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('D', o);
}

TEST(SeekSet, PastEnd)
{
	const char* t = "\xD0\xBB\xD0\xBE\xD0\xBA\xD0\xB0\xD0\xBB\xD0\xB8\xD0\xB7\xD0\xB0\xD1\x86\xD0\xB8\xD0\xB8";

	const char* r = utf8seek(t, t, 33, SEEK_SET);

	EXPECT_EQ(t + strlen(t), r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = 0;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(SeekSet, EndsInMiddle)
{
	const char* t = "\xE0\xA4\x81\xE0\xA4\x8B\0\xE0\xA4\xB4\xE0\xA4\xBD";

	const char* r = utf8seek(t, t, 5, SEEK_SET);

	EXPECT_EQ(t + 6, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = 0;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(SeekSet, SwappedParameters)
{
	const char* t = "\xD0\xBB\xD0\xBE\xD0\xBA\xD0\xB0\xD0\xBB\xD0\xB8\xD0\xB7\xD0\xB0\xD1\x86\xD0\xB8\xD0\xB8";

	const char* r = utf8seek(t, t + strlen(t), 3, SEEK_SET);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xD0\xBB\xD0\xBE\xD0\xBA\xD0\xB0\xD0\xBB\xD0\xB8\xD0\xB7\xD0\xB0\xD1\x86\xD0\xB8\xD0\xB8", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x43B, o);
}

TEST(SeekSet, Empty)
{
	const char* t = "";

	const char* r = utf8seek(t, t, 3, SEEK_SET);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = 0;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(SeekSet, Ascii)
{
	const char* t = "Riverbed";

	const char* r = utf8seek(t, t, 4, SEEK_SET);

	EXPECT_EQ(t + 4, r);
	EXPECT_STREQ("rbed", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('r', o);
}

TEST(SeekSet, AsciiIllegalByteFE)
{
	const char* t = "The time\xFE" "box";

	const char* r = utf8seek(t, t, 10, SEEK_SET);

	EXPECT_EQ(t + 10, r);
	EXPECT_STREQ("ox", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('o', o);
}

TEST(SeekSet, AsciiIllegalByteFF)
{
	const char* t = "Mag\xFF KKD";

	const char* r = utf8seek(t, t, 4, SEEK_SET);

	EXPECT_EQ(t + 4, r);
	EXPECT_STREQ(" KKD", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(' ', o);
}

TEST(SeekSet, AsciiMalformedContinuationByte)
{
	const char* t = "Player: \x87" "bob";

	const char* r = utf8seek(t, t, 10, SEEK_SET);

	EXPECT_EQ(t + 10, r);
	EXPECT_STREQ("ob", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('o', o);
}

TEST(SeekSet, TwoBytes)
{
	const char* t = "\xD0\xBB\xD0\xBE\xD0\xBA\xD0\xB0\xD0\xBB\xD0\xB8\xD0\xB7\xD0\xB0\xD1\x86\xD0\xB8\xD0\xB8";

	const char* r = utf8seek(t, t, 3, SEEK_SET);

	EXPECT_EQ(t + 6, r);
	EXPECT_STREQ("\xD0\xB0\xD0\xBB\xD0\xB8\xD0\xB7\xD0\xB0\xD1\x86\xD0\xB8\xD0\xB8", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x430, o);
}

TEST(SeekSet, TwoBytesOverlong)
{
	const char* t = "\xC0\xAF\xC1\xBF\xC0\xAF";

	const char* r = utf8seek(t, t, 1, SEEK_SET);

	EXPECT_EQ(t + 2, r);
	EXPECT_STREQ("\xC1\xBF\xC0\xAF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(SeekSet, TwoBytesOverlongPastEnd)
{
	const char* t = "\xC0\xAF\xC1\xBF\xC0\xAF";

	const char* r = utf8seek(t, t, 6, SEEK_SET);

	EXPECT_EQ(t + 6, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = 0;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(SeekSet, TwoBytesLonelyStart)
{
	const char* t = "Gone\xC4" "Fishin'";

	const char* r = utf8seek(t, t, 8, SEEK_SET);

	EXPECT_EQ(t + 9, r);
	EXPECT_STREQ("in'", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('i', o);
}

TEST(SeekSet, TwoBytesLonelyStartAtEnd)
{
	const char* t = "Megalodon\xDC";

	const char* r = utf8seek(t, t, 10, SEEK_SET);

	EXPECT_EQ(t + 10, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = 0;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(SeekSet, ThreeBytes)
{
	const char* t = "\xE0\xA4\x81\xE0\xA4\x8B\xE0\xA4\xB4\xE0\xA4\xBD";

	const char* r = utf8seek(t, t, 2, SEEK_SET);

	EXPECT_EQ(t + 6, r);
	EXPECT_STREQ("\xE0\xA4\xB4\xE0\xA4\xBD", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x934, o);
}

TEST(SeekSet, ThreeBytesOverlong)
{
	const char* t = "\xE0\x80\xAF\xE0\x9F\xBF\xE0\x80\xAF";

	const char* r = utf8seek(t, t, 1, SEEK_SET);

	EXPECT_EQ(t + 3, r);
	EXPECT_STREQ("\xE0\x9F\xBF\xE0\x80\xAF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(SeekSet, ThreeBytesOverlongPastEnd)
{
	const char* t = "\xE0\x80\xAF\xE0\x80\xAF\xE0\x80\xAF";

	const char* r = utf8seek(t, t, 12, SEEK_SET);

	EXPECT_EQ(t + 9, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = 0;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(SeekSet, ThreeBytesLonelyStart)
{
	const char* t = "Rocker\xE4" "coaster";

	const char* r = utf8seek(t, t, 10, SEEK_SET);

	EXPECT_EQ(t + 12, r);
	EXPECT_STREQ("er", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('e', o);
}

TEST(SeekSet, ThreeBytesLonelyStartAtEnd)
{
	const char* t = "Submarine\xED";

	const char* r = utf8seek(t, t, 10, SEEK_SET);

	EXPECT_EQ(t + 10, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = 0;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(SeekSet, ThreeBytesNotEnoughData)
{
	const char* t = "Compan\xEF\xBFions";

	const char* r = utf8seek(t, t, 7, SEEK_SET);

	EXPECT_EQ(t + 9, r);
	EXPECT_STREQ("ons", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('o', o);
}

TEST(SeekSet, ThreeBytesNotEnoughDataAtEnd)
{
	const char* t = "BBBark\xEF\xBF";

	const char* r = utf8seek(t, t, 7, SEEK_SET);

	EXPECT_EQ(t + 8, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = 0;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(SeekSet, FourBytes)
{
	const char* t = "\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80";

	const char* r = utf8seek(t, t, 3, SEEK_SET);

	EXPECT_EQ(t + 12, r);
	EXPECT_STREQ("\xF0\x90\x92\x80\xF0\x90\x92\x80", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x10480, o);
}