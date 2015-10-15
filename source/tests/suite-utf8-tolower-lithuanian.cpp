#include "tests-base.hpp"

#include "../helpers/helpers-locale.hpp"
#include "../helpers/helpers-strings.hpp"

class Utf8ToLowerLithuanian
	: public ::testing::Test
{

protected:

	void SetUp()
	{
		SET_LOCALE_LITHUANIAN();
	}

	void TearDown()
	{
		RESET_LOCALE();
	}

};

TEST_F(Utf8ToLowerLithuanian, SingleLatinSmallLetterI)
{
	// 0069
	// 0069

	const char* c = "i";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(1, utf8tolower(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("i", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToLowerLithuanian, SingleLatinSmallLetterIAndCombiningDotAbove)
{
	// 0069 0307
	// 0069 0307

	const char* c = "i\xCC\x87";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8tolower(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("i\xCC\x87", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToLowerLithuanian, SingleLatinSmallLetterIWithGrave)
{
	// 00EC
	// 00EC

	const char* c = "\xC3\xAC";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8tolower(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC3\xAC", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToLowerLithuanian, SingleLatinSmallLetterIAndCombiningGraveAccent)
{
	// 0069 0300
	// 0069 0300

	const char* c = "i\xCC\x80";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8tolower(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("i\xCC\x80", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToLowerLithuanian, SingleLatinSmallLetterICombiningDotAboveAndCombiningGraveAccent)
{
	// 0069 0307 0300
	// 0069 0307 0300

	const char* c = "i\xCC\x87\xCC\x80";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(5, utf8tolower(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("i\xCC\x87\xCC\x80", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToLowerLithuanian, SingleLatinSmallLetterIWithAcute)
{
	// 00ED
	// 00ED

	const char* c = "\xC3\xAD";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8tolower(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC3\xAD", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToLowerLithuanian, SingleLatinSmallLetterIAndCombiningAcuteAccent)
{
	// 0069 0301
	// 0069 0301

	const char* c = "i\xCC\x81";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8tolower(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("i\xCC\x81", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToLowerLithuanian, SingleLatinSmallLetterICombiningDotAboveAndCombiningAcuteAccent)
{
	// 0069 0307 0301
	// 0069 0307 0301

	const char* c = "i\xCC\x87\xCC\x81";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(5, utf8tolower(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("i\xCC\x87\xCC\x81", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToLowerLithuanian, SingleLatinSmallLetterIWithTilde)
{
	// 0129
	// 0129

	const char* c = "\xC4\xA9";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8tolower(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC4\xA9", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToLowerLithuanian, SingleLatinSmallLetterIAndCombiningTildeAccent)
{
	// 0069 0303
	// 0069 0303

	const char* c = "i\xCC\x83";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8tolower(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("i\xCC\x83", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToLowerLithuanian, SingleLatinSmallLetterICombiningDotAboveAndCombiningTildeAccent)
{
	// 0069 0307 0303
	// 0069 0307 0303

	const char* c = "i\xCC\x87\xCC\x83";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(5, utf8tolower(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("i\xCC\x87\xCC\x83", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToLowerLithuanian, SingleLatinSmallLetterIAndCombiningOgonekAccent)
{
	// 0069 0328
	// 0069 0328

	const char* c = "i\xCC\xA8";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8tolower(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("i\xCC\xA8", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToLowerLithuanian, SingleLatinSmallLetterICombiningDotAboveAndCombiningOgonekAccent)
{
	// 0069 0307 0328
	// 0069 0307 0328

	const char* c = "i\xCC\x87\xCC\xA8";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(5, utf8tolower(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("i\xCC\x87\xCC\xA8", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToLowerLithuanian, SingleLatinSmallLetterIMoreAboveOutOfOrder)
{
	// 0069 0670 20EF 0328
	//    0   35  220  216

	// 0069 0670 20EF 0328
	//    0   35  220  216

	const char* c = "i\xD9\xB0\xE2\x83\xAF\xCC\xA8";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(8, utf8tolower(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("i\xD9\xB0\xE2\x83\xAF\xCC\xA8", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToLowerLithuanian, SingleLatinSmallLetterIWithOgonek)
{
	// 012F
	// 012F

	const char* c = "\xC4\xAF";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8tolower(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC4\xAF", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToLowerLithuanian, SingleLatinSmallLetterIWithOgonekAndCombiningDotAbove)
{
	// 012F 0307
	// 012F 0307

	const char* c = "\xC4\xAF\xCC\x87";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8tolower(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC4\xAF\xCC\x87", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToLowerLithuanian, SingleLatinSmallLetterIWithOgonekAndCombiningGraveAccent)
{
	// 012F 0300
	// 012F 0300

	const char* c = "\xC4\xAF\xCC\x80";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8tolower(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC4\xAF\xCC\x80", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToLowerLithuanian, SingleLatinSmallLetterIWithOgonekAndCombiningAcuteAccent)
{
	// 012F 0301
	// 012F 0301

	const char* c = "\xC4\xAF\xCC\x81";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8tolower(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC4\xAF\xCC\x81", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToLowerLithuanian, SingleLatinSmallLetterIWithOgonekAndCombiningTildeAccent)
{
	// 012F 0303
	// 012F 0303

	const char* c = "\xC4\xAF\xCC\x83";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8tolower(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC4\xAF\xCC\x83", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToLowerLithuanian, SingleLatinSmallLetterIWithOgonekAndCombiningOgonekAccent)
{
	// 012F 0328
	// 012F 0328

	const char* c = "\xC4\xAF\xCC\xA8";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8tolower(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC4\xAF\xCC\xA8", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToLowerLithuanian, SingleLatinCapitalLetterI)
{
	// 0049
	// 0069 0307

	const char* c = "I";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8tolower(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("i\xCC\x87", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToLowerLithuanian, SingleLatinCapitalLetterIAndCombingDotAbove)
{
	// 0049 0307
	// 0069 0307

	const char* c = "I\xCC\x87";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8tolower(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("i\xCC\x87", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToLowerLithuanian, SingleLatinCapitalLetterIWithGrave)
{
	// 00CC
	// 0069 0307 0300

	const char* c = "\xC3\x8C";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(5, utf8tolower(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("i\xCC\x87\xCC\x80", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToLowerLithuanian, SingleLatinCapitalLetterIAndCombiningGraveAccent)
{
	// 0049 0300
	// 0069 0307 0300

	const char* c = "I\xCC\x80";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(5, utf8tolower(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("i\xCC\x87\xCC\x80", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToLowerLithuanian, SingleLatinCapitalLetterICombiningDotAboveAndCombiningGraveAccent)
{
	// 0049 0307 0300
	// 0069 0307 0300

	const char* c = "I\xCC\x87\xCC\x80";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(5, utf8tolower(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("i\xCC\x87\xCC\x80", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToLowerLithuanian, SingleLatinCapitalLetterIWithAcute)
{
	// 00CD
	// 0069 0307 0301

	const char* c = "\xC3\x8D";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(5, utf8tolower(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("i\xCC\x87\xCC\x81", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToLowerLithuanian, SingleLatinCapitalLetterIAndCombiningAcuteAccent)
{
	// 0049 0301
	// 0069 0307 0301

	const char* c = "I\xCC\x81";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(5, utf8tolower(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("i\xCC\x87\xCC\x81", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToLowerLithuanian, SingleLatinCapitalLetterICombiningDotAboveAndCombiningAcuteAccent)
{
	// 0049 0307 0301
	// 0069 0307 0301

	const char* c = "I\xCC\x87\xCC\x81";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(5, utf8tolower(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("i\xCC\x87\xCC\x81", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToLowerLithuanian, SingleLatinCapitalLetterIWithTilde)
{
	// 0128
	// 0069 0307 0303

	const char* c = "\xC4\xA8";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(5, utf8tolower(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("i\xCC\x87\xCC\x83", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToLowerLithuanian, SingleLatinCapitalLetterIAndCombiningTildeAccent)
{
	// 0049 0303
	// 0069 0307 0303

	const char* c = "I\xCC\x83";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(5, utf8tolower(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("i\xCC\x87\xCC\x83", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToLowerLithuanian, SingleLatinCapitalLetterICombiningDotAboveAndCombiningTildeAccent)
{
	// 0049 0307 0303
	// 0069 0307 0303

	const char* c = "I\xCC\x87\xCC\x83";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(5, utf8tolower(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("i\xCC\x87\xCC\x83", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToLowerLithuanian, SingleLatinCapitalLetterIAndCombiningOgnonekAccent)
{
	// 0049 0328
	// 0069 0307 0328

	const char* c = "I\xCC\xA8";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(5, utf8tolower(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("i\xCC\x87\xCC\xA8", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToLowerLithuanian, SingleLatinCapitalLetterICombiningDotAboveAndCombiningOgnonekAccent)
{
	// 0049 0307 0328
	// 0069 0307 0328

	const char* c = "I\xCC\x87\xCC\xA8";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(5, utf8tolower(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("i\xCC\x87\xCC\xA8", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToLowerLithuanian, SingleLatinCapitalLetterIMoreAboveOutOfOrder)
{
	// 0049 0313 0323 031B
	//    0  230  220  216

	// 0069 0307 0313 0323 031B
	//    0  230  230  220  216

	const char* c = "I\xCC\x93\xCC\xA3\xCC\x9B";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(9, utf8tolower(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("i\xCC\x87\xCC\x93\xCC\xA3\xCC\x9B", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToLowerLithuanian, SingleLatinCapitalLetterIWithOgonek)
{
	// 012E
	// 012F

	const char* c = "\xC4\xAE";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8tolower(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC4\xAF", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToLowerLithuanian, SingleLatinCapitalLetterIWithOgonekAndCombiningDotAbove)
{
	// 012E 0307
	// 012F 0307

	const char* c = "\xC4\xAE\xCC\x87";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8tolower(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC4\xAE\xCC\x87", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToLowerLithuanian, SingleLatinCapitalLetterIWithOgonekAndCombiningGraveAccent)
{
	// 012E 0300
	// 012F 0307 0300

	const char* c = "\xC4\xAE\xCC\x80";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(6, utf8tolower(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC4\xAF\xCC\x87\xCC\x80", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToLowerLithuanian, SingleLatinCapitalLetterIWithOgonekAndCombiningAcuteAccent)
{
	// 012E 0301
	// 012F 0307 0301

	const char* c = "\xC4\xAE\xCC\x81";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(6, utf8tolower(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC4\xAF\xCC\x87\xCC\x81", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToLowerLithuanian, SingleLatinCapitalLetterIWithOgonekAndCombiningTildeAccent)
{
	// 012E 0303
	// 012F 0307 0303

	const char* c = "\xC4\xAF\xCC\x83";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(6, utf8tolower(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC4\xAF\xCC\x87\xCC\x83", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToLowerLithuanian, SingleLatinCapitalLetterIWithOgonekAndCombiningOgonekAccent)
{
	// 012E 0328
	// 012F 0328

	const char* c = "\xC4\xAE\xCC\xA8";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8tolower(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC4\xAF\xCC\xA8", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToLowerLithuanian, SingleLatinCapitalLetterIWithOgonekMoreAboveOutOfOrder)
{
	// 012E 0327 1DCE 0335 0346
	//    0  202  214    1  230

	// 012F 0327 1DCE 0335 0346
	//    0  202  214    1  230

	const char* c = "\xC4\xAE\xCC\xA7\xE1\xB7\x8E\xCC\xB5\xCD\x86";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(11, utf8tolower(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC4\xAF\xCC\xA7\xE1\xB7\x8E\xCC\xB5\xCD\x86", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}