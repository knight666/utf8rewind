#include "tests-base.hpp"

#include "../helpers/helpers-locale.hpp"
#include "../helpers/helpers-strings.hpp"

class Utf8ToUpperLithuanian
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

TEST_F(Utf8ToUpperLithuanian, SingleLatinSmallLetterI)
{
	// 0069
	// 0049

	const char* c = "i";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(1, utf8toupper(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperLithuanian, SingleLatinSmallLetterIAndCombiningDotAbove)
{
	// 0069 0307
	// 0049

	const char* c = "i\xCC\x87";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(1, utf8toupper(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperLithuanian, SingleLatinSmallLetterIWithGrave)
{
	// 00EC
	// 00CC

	const char* c = "\xC3\xAC";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toupper(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC3\x8C", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperLithuanian, SingleLatinSmallLetterIAndCombiningGraveAccent)
{
	// 0069 0300
	// 0049 0300

	const char* c = "i\xCC\x80";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8toupper(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I\xCC\x80", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperLithuanian, SingleLatinSmallLetterICombiningDotAboveAndCombiningGraveAccent)
{
	// 0069 0307 0300
	// 0049 0300

	const char* c = "i\xCC\x87\xCC\x80";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8toupper(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I\xCC\x80", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperLithuanian, SingleLatinSmallLetterIWithAcute)
{
	// 00ED
	// 00CD

	const char* c = "\xC3\xAD";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toupper(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC3\x8D", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperLithuanian, SingleLatinSmallLetterIAndCombiningAcuteAccent)
{
	// 0069 0301
	// 0049 0301

	const char* c = "i\xCC\x81";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8toupper(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I\xCC\x81", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperLithuanian, SingleLatinSmallLetterICombiningDotAboveAndCombiningAcuteAccent)
{
	// 0069 0307 0301
	// 0049 0301

	const char* c = "i\xCC\x87\xCC\x81";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8toupper(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I\xCC\x81", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperLithuanian, SingleLatinSmallLetterIWithTilde)
{
	// 0129
	// 0128

	const char* c = "\xC4\xA9";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toupper(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC4\xA8", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperLithuanian, SingleLatinSmallLetterIAndCombiningTildeAccent)
{
	// 0069 0303
	// 0049 0303

	const char* c = "i\xCC\x83";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8toupper(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I\xCC\x83", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperLithuanian, SingleLatinSmallLetterICombiningDotAboveAndCombiningTildeAccent)
{
	// 0069 0307 0303
	// 0049 0303

	const char* c = "i\xCC\x87\xCC\x83";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8toupper(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I\xCC\x83", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperLithuanian, SingleLatinSmallLetterIAndCombiningOgonekAccent)
{
	// 0069 0328
	// 0049 0328

	const char* c = "i\xCC\xA8";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8toupper(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I\xCC\xA8", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperLithuanian, SingleLatinSmallLetterICombiningDotAboveAndCombiningOgonekAccent)
{
	// 0069 0307 0328
	// 0049 0307 0328

	const char* c = "i\xCC\x87\xCC\xA8";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(5, utf8toupper(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I\xCC\x87\xCC\xA8", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperLithuanian, SingleLatinSmallLetterIMoreAboveOutOfOrder)
{
	// 0069 1939 034E 0747
	//    0  222  220  230

	// 0049 1939 034E 0747
	//    0  222  220  230

	const char* c = "i\xE1\xA4\xB9\xCD\x8E\xDD\x87";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(8, utf8toupper(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I\xE1\xA4\xB9\xCD\x8E\xDD\x87", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperLithuanian, SingleLatinSmallLetterIWithOgonek)
{
	// 012F
	// 012E

	const char* c = "\xC4\xAF";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toupper(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC4\xAE", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperLithuanian, SingleLatinSmallLetterIWithOgonekAndCombiningDotAbove)
{
	// 012F 0307
	// 012E 0307

	const char* c = "\xC4\xAF\xCC\x87";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toupper(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC4\xAE\xCC\x87", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperLithuanian, SingleLatinSmallLetterIWithOgonekAndCombiningGraveAccent)
{
	// 012F 0300
	// 012E 0300

	const char* c = "\xC4\xAF\xCC\x80";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toupper(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC4\xAE\xCC\x80", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperLithuanian, SingleLatinSmallLetterIWithOgonekAndCombiningAcuteAccent)
{
	// 012F 0301
	// 012E 0301

	const char* c = "\xC4\xAF\xCC\x81";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toupper(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC4\xAE\xCC\x81", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperLithuanian, SingleLatinSmallLetterIWithOgonekAndCombiningTildeAccent)
{
	// 012F 0303
	// 012E 0303

	const char* c = "\xC4\xAF\xCC\x83";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toupper(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC4\xAE\xCC\x83", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperLithuanian, SingleLatinSmallLetterIWithOgonekAndCombiningOgonekAccent)
{
	// 012F 0328
	// 012E 0328

	const char* c = "\xC4\xAF\xCC\xA8";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toupper(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC4\xAE\xCC\xA8", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperLithuanian, SingleLatinCapitalLetterI)
{
	// 0049
	// 0049

	const char* c = "I";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(1, utf8toupper(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperLithuanian, SingleLatinCapitalLetterIAndCombingDotAbove)
{
	// 0049 0307
	// 0049 0307

	const char* c = "I\xCC\x87";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8toupper(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I\xCC\x87", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperLithuanian, SingleLatinCapitalLetterIWithGrave)
{
	// 00CC
	// 00CC

	const char* c = "\xC3\x8C";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toupper(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC3\x8C", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperLithuanian, SingleLatinCapitalLetterIAndCombiningGraveAccent)
{
	// 0049 0300
	// 0049 0300

	const char* c = "I\xCC\x80";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8toupper(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I\xCC\x80", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperLithuanian, SingleLatinCapitalLetterICombiningDotAboveAndCombiningGraveAccent)
{
	// 0049 0307 0300
	// 0049 0307 0300

	const char* c = "I\xCC\x87\xCC\x80";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(5, utf8toupper(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I\xCC\x87\xCC\x80", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperLithuanian, SingleLatinCapitalLetterIWithAcute)
{
	// 00CD
	// 00CD

	const char* c = "\xC3\x8D";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toupper(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC3\x8D", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperLithuanian, SingleLatinCapitalLetterIAndCombiningAcuteAccent)
{
	// 0049 0301
	// 0049 0301

	const char* c = "I\xCC\x81";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8toupper(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I\xCC\x81", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperLithuanian, SingleLatinCapitalLetterICombiningDotAboveAndCombiningAcuteAccent)
{
	// 0049 0307 0301
	// 0049 0307 0301

	const char* c = "I\xCC\x87\xCC\x81";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(5, utf8toupper(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I\xCC\x87\xCC\x81", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperLithuanian, SingleLatinCapitalLetterIWithTilde)
{
	// 0128
	// 0128

	const char* c = "\xC4\xA8";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toupper(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC4\xA8", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperLithuanian, SingleLatinCapitalLetterIAndCombiningTildeAccent)
{
	// 0049 0303
	// 0049 0303

	const char* c = "I\xCC\x83";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8toupper(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I\xCC\x83", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperLithuanian, SingleLatinCapitalLetterICombiningDotAboveAndCombiningTildeAccent)
{
	// 0049 0307 0303
	// 0049 0307 0303

	const char* c = "I\xCC\x87\xCC\x83";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(5, utf8toupper(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I\xCC\x87\xCC\x83", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperLithuanian, SingleLatinCapitalLetterIAndCombiningOgnonekAccent)
{
	// 0049 0328
	// 0049 0328

	const char* c = "I\xCC\xA8";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8toupper(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I\xCC\xA8", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperLithuanian, SingleLatinCapitalLetterICombiningDotAboveAndCombiningOgnonekAccent)
{
	// 0049 0307 0328
	// 0049 0307 0328

	const char* c = "I\xCC\x87\xCC\xA8";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(5, utf8toupper(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I\xCC\x87\xCC\xA8", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperLithuanian, SingleLatinCapitalLetterIMoreAboveOutOfOrder)
{
	// 0049 302D 0EB8 0F7B 0350
	//    0  222  118  130  230

	// 0049 302D 0EB8 0F7B 0350
	//    0  222  118  130  230

	const char* c = "I\xE3\x80\xAD\xE0\xBA\xB8\xE0\xBD\xBB\xCD\x90";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(12, utf8toupper(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I\xE3\x80\xAD\xE0\xBA\xB8\xE0\xBD\xBB\xCD\x90", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperLithuanian, SingleLatinCapitalLetterIWithOgonek)
{
	// 012E
	// 012E

	const char* c = "\xC4\xAE";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8toupper(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC4\xAE", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperLithuanian, SingleLatinCapitalLetterIWithOgonekAndCombiningDotAbove)
{
	// 012E 0307
	// 012E 0307

	const char* c = "\xC4\xAE\xCC\x87";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toupper(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC4\xAE\xCC\x87", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperLithuanian, SingleLatinCapitalLetterIWithOgonekAndCombiningGraveAccent)
{
	// 012E 0300
	// 012E 0300

	const char* c = "\xC4\xAE\xCC\x80";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toupper(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC4\xAE\xCC\x80", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperLithuanian, SingleLatinCapitalLetterIWithOgonekAndCombiningAcuteAccent)
{
	// 012E 0301
	// 012E 0301

	const char* c = "\xC4\xAE\xCC\x81";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toupper(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC4\xAE\xCC\x81", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperLithuanian, SingleLatinCapitalLetterIWithOgonekAndCombiningTildeAccent)
{
	// 012E 0303
	// 012E 0303

	const char* c = "\xC4\xAE\xCC\x83";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toupper(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC4\xAE\xCC\x83", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperLithuanian, SingleLatinCapitalLetterIWithOgonekAndCombiningOgonekAccent)
{
	// 012E 0328
	// 012E 0328

	const char* c = "\xC4\xAE\xCC\xA8";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8toupper(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC4\xAE\xCC\xA8", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToUpperLithuanian, SingleLatinCapitalLetterIWithOgonekMoreAboveOutOfOrder)
{
	// 012E 302E 1DCA AAC1
	//    0  224  220  230

	// 012E 302E 1DCA AAC1
	//    0  224  220  230

	const char* c = "\xC4\xAE\xE3\x80\xAE\xE1\xB7\x8A\xEA\xAB\x81";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(11, utf8toupper(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC4\xAE\xE3\x80\xAE\xE1\xB7\x8A\xEA\xAB\x81", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}