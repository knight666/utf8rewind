#include "tests-base.hpp"

#include "utf8rewind.h"

TEST(SeekEnd, Empty)
{
	const char* t = "";

	const char* r = utf8seek(t, t, -5, SEEK_END);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = 0;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(SeekEnd, Ascii)
{
	const char* t = "Farmer";

	const char* r = utf8seek(t, t, 2, SEEK_END);

	EXPECT_EQ(t + 4, r);
	EXPECT_STREQ("er", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('e', o);
}

TEST(SeekEnd, AsciiOffset)
{
	const char* t = "Waterbringer";

	const char* r = utf8seek(t + 5, t, 7, SEEK_END);

	EXPECT_EQ(t + 5, r);
	EXPECT_STREQ("bringer", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('b', o);
}

TEST(SeekEnd, AsciiPastStart)
{
	const char* t = "Moonshine";

	const char* r = utf8seek(t, t, 16, SEEK_END);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("Moonshine", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('M', o);
}

TEST(SeekEnd, AsciiNegative)
{
	const char* t = "Alternative";

	const char* r = utf8seek(t, t, -2, SEEK_END);

	EXPECT_EQ(t + 11, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = 0;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(SeekEnd, TwoBytes)
{
	const char* t = "\xD4\x9A\xD4\x9C\xD4\x86\xD4\x8A\xD4\xB6";

	const char* r = utf8seek(t, t, 1, SEEK_END);

	EXPECT_EQ(t + 8, r);
	EXPECT_STREQ("\xD4\xB6", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x536, o);
}

TEST(SeekEnd, TwoBytesOffset)
{
	const char* t = "\xD4\x9A\xD4\x9C\xD4\x86\xD4\x8A\xD4\xB6";

	const char* r = utf8seek(t + 6, t, 2, SEEK_END);

	EXPECT_EQ(t + 6, r);
	EXPECT_STREQ("\xD4\x8A\xD4\xB6", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x50A, o);
}

TEST(SeekEnd, TwoBytesPastStart)
{
	const char* t = "\xD4\x9A\xD4\x9C\xD4\x86\xD4\x8A\xD4\xB6";

	const char* r = utf8seek(t, t, 16, SEEK_END);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xD4\x9A\xD4\x9C\xD4\x86\xD4\x8A\xD4\xB6", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x51A, o);
}

TEST(SeekEnd, TwoBytesNegative)
{
	const char* t = "\xD4\x9A\xD4\x9C\xD4\x86\xD4\x8A\xD4\xB6";

	const char* r = utf8seek(t, t, -4, SEEK_END);

	EXPECT_EQ(t + 10, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = 0;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(SeekEnd, ThreeBytes)
{
	const char* t = "\xE2\xB7\xB0\xE2\xB8\x97\xE2\xB8\xBA\xE2\xB8\xAF\xE2\xB9\x8F";

	const char* r = utf8seek(t, t, 2, SEEK_END);

	EXPECT_EQ(t + 9, r);
	EXPECT_STREQ("\xE2\xB8\xAF\xE2\xB9\x8F", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x2E2F, o);
}

TEST(SeekEnd, ThreeBytesOffset)
{
	const char* t = "\xE2\xB7\xB0\xE2\xB8\x97\xE2\xB8\xBA\xE2\xB8\xAF\xE2\xB9\x8F";

	const char* r = utf8seek(t + 4, t, 1, SEEK_END);

	EXPECT_EQ(t + 12, r);
	EXPECT_STREQ("\xE2\xB9\x8F", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x2E4F, o);
}

TEST(SeekEnd, ThreeBytesPastStart)
{
	const char* t = "\xE2\xB7\xB0\xE2\xB8\x97\xE2\xB8\xBA\xE2\xB8\xAF\xE2\xB9\x8F";

	const char* r = utf8seek(t + 4, t, 24, SEEK_END);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xE2\xB7\xB0\xE2\xB8\x97\xE2\xB8\xBA\xE2\xB8\xAF\xE2\xB9\x8F", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x2DF0, o);
}

TEST(SeekEnd, ThreeBytesNegative)
{
	const char* t = "\xE2\xB7\xB0\xE2\xB8\x97\xE2\xB8\xBA\xE2\xB8\xAF\xE2\xB9\x8F";

	const char* r = utf8seek(t, t, -3, SEEK_END);

	EXPECT_EQ(t + 15, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = 0;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(SeekEnd, FourBytes)
{
	const char* t = "\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80";

	const char* r = utf8seek(t, t, 4, SEEK_END);

	EXPECT_EQ(t + 4, r);
	EXPECT_STREQ("\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x10480, o);
}

TEST(SeekEnd, FourBytesOffset)
{
	const char* t = "\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80";

	const char* r = utf8seek(t + 12, t, 2, SEEK_END);

	EXPECT_EQ(t + 12, r);
	EXPECT_STREQ("\xF0\x90\x92\x80\xF0\x90\x92\x80", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x10480, o);
}

TEST(SeekEnd, FourBytesPastStart)
{
	const char* t = "\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80";

	const char* r = utf8seek(t, t, 128, SEEK_END);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x10480, o);
}

TEST(SeekEnd, FourBytesNegative)
{
	const char* t = "\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80";

	const char* r = utf8seek(t, t, -12, SEEK_END);

	EXPECT_EQ(t + 20, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = 0;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}