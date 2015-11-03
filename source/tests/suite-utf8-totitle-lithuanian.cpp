#include "tests-base.hpp"

#include "../helpers/helpers-locale.hpp"
#include "../helpers/helpers-strings.hpp"

class Utf8ToTitleLithuanian
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

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterI)
{
	// 0069
	// 0049

	const char* c = "i";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(1, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterINotEnoughSpace)
{
	const char* c = "i";
	const size_t s = 0;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterIAmountOfBytes)
{
	const char* c = "i";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(1, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterIAndCombiningDotAbove)
{
	// 0069 0307
	// 0049

	const char* c = "i\xCC\x87";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(1, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterIAndCombiningDotAboveNotEnoughSpace)
{
	const char* c = "i\xCC\x87";
	const size_t s = 0;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterIAndCombiningDotAboveAmountOfBytes)
{
	const char* c = "i\xCC\x87";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(1, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterIWithGrave)
{
	// 00EC
	// 00CC

	const char* c = "\xC3\xAC";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC3\x8C", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterIWithGraveNotEnoughSpace)
{
	const char* c = "\xC3\xAC";
	const size_t s = 1;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterIWithGraveAmountOfBytes)
{
	const char* c = "\xC3\xAC";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterIAndCombiningGrave)
{
	// 0069 0300
	// 0049 0300

	const char* c = "i\xCC\x80";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I\xCC\x80", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterIAndCombiningGraveNotEnoughSpace)
{
	const char* c = "i\xCC\x80";
	const size_t s = 2;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(1, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterIAndCombiningGraveAmountOfBytes)
{
	const char* c = "i\xCC\x80";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterICombiningDotAboveAndCombiningGrave)
{
	// 0069 0307 0300
	// 0049 0300

	const char* c = "i\xCC\x87\xCC\x80";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I\xCC\x80", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterICombiningDotAboveAndCombiningGraveNotEnoughSpace)
{
	const char* c = "i\xCC\x87\xCC\x80";
	const size_t s = 2;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(1, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterICombiningDotAboveAndCombiningGraveAmountOfBytes)
{
	const char* c = "i\xCC\x87\xCC\x80";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterIWithAcute)
{
	// 00ED
	// 00CD

	const char* c = "\xC3\xAD";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC3\x8D", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterIWithAcuteNotEnoughSpace)
{
	const char* c = "\xC3\xAD";
	const size_t s = 1;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterIWithAcuteAmountOfBytes)
{
	const char* c = "\xC3\xAD";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterIAndCombiningAcute)
{
	// 0069 0301
	// 0049 0301

	const char* c = "i\xCC\x81";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I\xCC\x81", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterIAndCombiningAcuteNotEnoughSpace)
{
	const char* c = "i\xCC\x81";
	const size_t s = 2;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(1, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterIAndCombiningAcuteAmountOfBytes)
{
	const char* c = "i\xCC\x81";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterICombiningDotAboveAndCombiningAcute)
{
	// 0069 0307 0301
	// 0049 0301

	const char* c = "i\xCC\x87\xCC\x81";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I\xCC\x81", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterICombiningDotAboveAndCombiningAcuteNotEnoughSpace)
{
	const char* c = "i\xCC\x87\xCC\x81";
	const size_t s = 1;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(1, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterICombiningDotAboveAndCombiningAcuteAmountOfBytes)
{
	const char* c = "i\xCC\x87\xCC\x81";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterIWithTilde)
{
	// 0129
	// 0128

	const char* c = "\xC4\xA9";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC4\xA8", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterIWithTildeNotEnoughSpace)
{
	const char* c = "\xC4\xA9";
	const size_t s = 1;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterIWithTildeAmountOfBytes)
{
	const char* c = "\xC4\xA9";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterIAndCombiningTilde)
{
	// 0069 0303
	// 0049 0303

	const char* c = "i\xCC\x83";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I\xCC\x83", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterIAndCombiningTildeNotEnoughSpace)
{
	const char* c = "i\xCC\x83";
	const size_t s = 2;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(1, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterIAndCombiningTildeAmountOfBytes)
{
	const char* c = "i\xCC\x83";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterICombiningDotAboveAndCombiningTilde)
{
	// 0069 0307 0303
	// 0049 0303

	const char* c = "i\xCC\x87\xCC\x83";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I\xCC\x83", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterICombiningDotAboveAndCombiningTildeNotEnoughSpace)
{
	const char* c = "i\xCC\x87\xCC\x83";
	const size_t s = 2;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(1, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterICombiningDotAboveAndCombiningTildeAmountOfBytes)
{
	const char* c = "i\xCC\x87\xCC\x83";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterIAndCombiningOgonek)
{
	// 0069 0328
	// 0049 0328

	const char* c = "i\xCC\xA8";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I\xCC\xA8", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterIAndCombiningOgonekNotEnoughSpace)
{
	const char* c = "i\xCC\xA8";
	const size_t s = 1;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(1, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterIAndCombiningOgonekAmountOfBytes)
{
	const char* c = "i\xCC\xA8";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterICombiningDotAboveAndCombiningOgonek)
{
	// 0069 0307 0328
	// 0049 0328

	const char* c = "i\xCC\x87\xCC\xA8";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I\xCC\xA8", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterICombiningDotAboveAndCombiningOgonekNotEnoughSpace)
{
	const char* c = "i\xCC\x87\xCC\xA8";
	const size_t s = 2;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(1, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterICombiningDotAboveAndCombiningOgonekAmountOfBytes)
{
	const char* c = "i\xCC\x87\xCC\xA8";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterIMoreAboveOutOfOrder)
{
	// 0069 1939 034E 0747
	//    0  222  220  230

	// 0049 1939 034E 0747
	//    0  222  220  230

	const char* c = "i\xE1\xA4\xB9\xCD\x8E\xDD\x87";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(8, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I\xE1\xA4\xB9\xCD\x8E\xDD\x87", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterJ)
{
	// 006A
	// 004A

	const char* c = "j";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(1, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("J", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterJNotEnoughSpace)
{
	const char* c = "j";
	const size_t s = 0;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterJAmountOfBytes)
{
	const char* c = "j";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(1, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterJAndCombiningDotAbove)
{
	// 006A 0307
	// 004A

	const char* c = "j\xCC\x87";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(1, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("J", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterJAndCombiningDotAboveNotEnoughSpace)
{
	const char* c = "j\xCC\x87";
	const size_t s = 0;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterJAndCombiningDotAboveAmountOfBytes)
{
	const char* c = "j\xCC\x87";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(1, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterJAndCombiningGrave)
{
	// 006A 0300
	// 004A 0300

	const char* c = "j\xCC\x80";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("J\xCC\x80", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterJAndCombiningGraveNotEnoughSpace)
{
	const char* c = "j\xCC\x80";
	const size_t s = 2;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(1, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("J", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterJAndCombiningGraveAmountOfBytes)
{
	const char* c = "j\xCC\x80";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterJCombiningDotAboveAndCombiningGrave)
{
	// 006A 0307 0300
	// 004A 0300

	const char* c = "j\xCC\x87\xCC\x80";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("J\xCC\x80", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterJCombiningDotAboveAndCombiningGraveNotEnoughSpace)
{
	const char* c = "j\xCC\x87\xCC\x80";
	const size_t s = 2;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(1, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("J", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterJCombiningDotAboveAndCombiningGraveAmountOfBytes)
{
	const char* c = "j\xCC\x87\xCC\x80";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterJAndCombiningAcute)
{
	// 006A 0301
	// 004A 0301

	const char* c = "j\xCC\x81";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("J\xCC\x81", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterJAndCombiningAcuteNotEnoughSpace)
{
	const char* c = "j\xCC\x81";
	const size_t s = 2;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(1, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("J", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterJAndCombiningAcuteAmountOfBytes)
{
	const char* c = "j\xCC\x81";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterJCombiningDotAboveAndCombiningAcute)
{
	// 006A 0307 0301
	// 004A 0301

	const char* c = "j\xCC\x87\xCC\x81";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("J\xCC\x81", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterJCombiningDotAboveAndCombiningAcuteNotEnoughSpace)
{
	const char* c = "j\xCC\x87\xCC\x81";
	const size_t s = 2;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(1, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("J", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterJCombiningDotAboveAndCombiningAcuteAmountOfBytes)
{
	const char* c = "j\xCC\x87\xCC\x81";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterJAndCombiningTilde)
{
	// 006A 0303
	// 004A 0303

	const char* c = "j\xCC\x83";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("J\xCC\x83", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterJAndCombiningTildeNotEnoughSpace)
{
	const char* c = "j\xCC\x83";
	const size_t s = 2;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(1, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("J", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterJAndCombiningTildeAmountOfBytes)
{
	const char* c = "j\xCC\x83";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterJCombiningDotAboveAndCombiningTilde)
{
	// 006A 0307 0303
	// 004A 0303

	const char* c = "j\xCC\x87\xCC\x83";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("J\xCC\x83", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterJCombiningDotAboveAndCombiningTildeNotEnoughSpace)
{
	const char* c = "j\xCC\x87\xCC\x83";
	const size_t s = 2;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(1, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("J", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterJCombiningDotAboveAndCombiningTildeAmountOfBytes)
{
	const char* c = "j\xCC\x87\xCC\x83";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterJAndCombiningOgonek)
{
	// 006A 0328
	// 004A 0328

	const char* c = "j\xCC\xA8";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("J\xCC\xA8", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterJAndCombiningOgonekNotEnoughSpace)
{
	const char* c = "j\xCC\xA8";
	const size_t s = 2;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(1, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("J", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterJAndCombiningOgonekAmountOfBytes)
{
	const char* c = "j\xCC\xA8";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterJCombiningDotAboveAndCombiningOgonek)
{
	// 006A 0307 0328
	// 004A 0328

	const char* c = "j\xCC\x87\xCC\xA8";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("J\xCC\xA8", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterJCombiningDotAboveAndCombiningOgonekNotEnoughSpace)
{
	const char* c = "j\xCC\x87\xCC\xA8";
	const size_t s = 2;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(1, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("J", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterJCombiningDotAboveAndCombiningOgonekAmountOfBytes)
{
	const char* c = "j\xCC\x87\xCC\xA8";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterJMoreAboveOutOfOrder)
{
	// 006A 1DCA 0595 0F74
	//    0  220  230  132

	// 004A 1DCA 0595 0F74
	//    0  220  230  132

	const char* c = "j\xE1\xB7\x8A\xD6\x95\xE0\xBD\xB4";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(9, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("J\xE1\xB7\x8A\xD6\x95\xE0\xBD\xB4", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterIWithOgonek)
{
	// 012F
	// 012E

	const char* c = "\xC4\xAF";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC4\xAE", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterIWithOgonekAndCombiningDotAbove)
{
	// 012F 0307
	// 012E 0307

	const char* c = "\xC4\xAF\xCC\x87";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC4\xAE\xCC\x87", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterIWithOgonekAndCombiningGrave)
{
	// 012F 0300
	// 012E 0300

	const char* c = "\xC4\xAF\xCC\x80";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC4\xAE\xCC\x80", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterIWithOgonekAndCombiningAcute)
{
	// 012F 0301
	// 012E 0301

	const char* c = "\xC4\xAF\xCC\x81";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC4\xAE\xCC\x81", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterIWithOgonekAndCombiningTilde)
{
	// 012F 0303
	// 012E 0303

	const char* c = "\xC4\xAF\xCC\x83";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC4\xAE\xCC\x83", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinSmallLetterIWithOgonekAndCombiningOgonek)
{
	// 012F 0328
	// 012E 0328

	const char* c = "\xC4\xAF\xCC\xA8";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC4\xAE\xCC\xA8", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinCapitalLetterI)
{
	// 0049
	// 0049

	const char* c = "I";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(1, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinCapitalLetterIAndCombingDotAbove)
{
	// 0049 0307
	// 0049 0307

	const char* c = "I\xCC\x87";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I\xCC\x87", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinCapitalLetterIWithGrave)
{
	// 00CC
	// 00CC

	const char* c = "\xC3\x8C";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC3\x8C", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinCapitalLetterIAndCombiningGrave)
{
	// 0049 0300
	// 0049 0300

	const char* c = "I\xCC\x80";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I\xCC\x80", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinCapitalLetterICombiningDotAboveAndCombiningGrave)
{
	// 0049 0307 0300
	// 0049 0307 0300

	const char* c = "I\xCC\x87\xCC\x80";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(5, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I\xCC\x87\xCC\x80", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinCapitalLetterIWithAcute)
{
	// 00CD
	// 00CD

	const char* c = "\xC3\x8D";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC3\x8D", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinCapitalLetterIAndCombiningAcute)
{
	// 0049 0301
	// 0049 0301

	const char* c = "I\xCC\x81";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I\xCC\x81", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinCapitalLetterICombiningDotAboveAndCombiningAcute)
{
	// 0049 0307 0301
	// 0049 0307 0301

	const char* c = "I\xCC\x87\xCC\x81";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(5, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I\xCC\x87\xCC\x81", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinCapitalLetterIWithTilde)
{
	// 0128
	// 0128

	const char* c = "\xC4\xA8";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC4\xA8", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinCapitalLetterIAndCombiningTilde)
{
	// 0049 0303
	// 0049 0303

	const char* c = "I\xCC\x83";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I\xCC\x83", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinCapitalLetterICombiningDotAboveAndCombiningTilde)
{
	// 0049 0307 0303
	// 0049 0307 0303

	const char* c = "I\xCC\x87\xCC\x83";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(5, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I\xCC\x87\xCC\x83", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinCapitalLetterIAndCombiningOgnonek)
{
	// 0049 0328
	// 0049 0328

	const char* c = "I\xCC\xA8";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I\xCC\xA8", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinCapitalLetterICombiningDotAboveAndCombiningOgnonek)
{
	// 0049 0307 0328
	// 0049 0307 0328

	const char* c = "I\xCC\x87\xCC\xA8";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(5, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I\xCC\x87\xCC\xA8", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinCapitalLetterIMoreAboveOutOfOrder)
{
	// 0049 302D 0EB8 0F7B 0350
	//    0  222  118  130  230

	// 0049 302D 0EB8 0F7B 0350
	//    0  222  118  130  230

	const char* c = "I\xE3\x80\xAD\xE0\xBA\xB8\xE0\xBD\xBB\xCD\x90";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(12, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I\xE3\x80\xAD\xE0\xBA\xB8\xE0\xBD\xBB\xCD\x90", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinCapitalLetterJ)
{
	// 004A
	// 004A

	const char* c = "J";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(1, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("J", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinCapitalLetterJAndCombiningDotAbove)
{
	// 004A 0307
	// 004A 0307

	const char* c = "J\xCC\x87";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("J\xCC\x87", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinCapitalLetterJAndCombiningGrave)
{
	// 004A 0300
	// 004A 0300

	const char* c = "J\xCC\x80";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("J\xCC\x80", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinCapitalLetterJCombiningDotAboveAndCombiningGrave)
{
	// 004A 0307 0300
	// 004A 0307 0300

	const char* c = "J\xCC\x87\xCC\x80";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(5, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("J\xCC\x87\xCC\x80", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinCapitalLetterJAndCombiningAcute)
{
	// 004A 0301
	// 004A 0301

	const char* c = "J\xCC\x81";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("J\xCC\x81", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinCapitalLetterJCombiningDotAboveAndCombiningAcute)
{
	// 004A 0307 0301
	// 004A 0307 0301

	const char* c = "J\xCC\x87\xCC\x81";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(5, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("J\xCC\x87\xCC\x81", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinCapitalLetterJAndCombiningTilde)
{
	// 004A 0303
	// 004A 0303

	const char* c = "J\xCC\x83";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("J\xCC\x83", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinCapitalLetterJCombiningDotAboveAndCombiningTilde)
{
	// 004A 0307 0303
	// 004A 0307 0303

	const char* c = "J\xCC\x87\xCC\x83";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(5, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("J\xCC\x87\xCC\x83", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinCapitalLetterJCombiningOgonek)
{
	// 004A 0328
	// 004A 0328

	const char* c = "J\xCC\xA8";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("J\xCC\xA8", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinCapitalLetterJCombiningDotAboveAndCombiningOgonek)
{
	// 004A 0307 0328
	// 004A 0307 0328

	const char* c = "J\xCC\x87\xCC\xA8";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(5, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("J\xCC\x87\xCC\xA8", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinCapitalLetterJMoreAboveOutOfOrder)
{
	// 004A AAB4 0619 0F72 0732
	//    0  220   31  130  230

	// 004A AAB4 0619 0F72 0732
	//    0  220   31  130  230

	const char* c = "J\xEA\xAA\xB4\xD8\x99\xE0\xBD\xB2\xDC\xB2";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(11, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("J\xEA\xAA\xB4\xD8\x99\xE0\xBD\xB2\xDC\xB2", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinCapitalLetterIWithOgonek)
{
	// 012E
	// 012E

	const char* c = "\xC4\xAE";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC4\xAE", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinCapitalLetterIWithOgonekAndCombiningDotAbove)
{
	// 012E 0307
	// 012E 0307

	const char* c = "\xC4\xAE\xCC\x87";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC4\xAE\xCC\x87", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinCapitalLetterIWithOgonekAndCombiningGrave)
{
	// 012E 0300
	// 012E 0300

	const char* c = "\xC4\xAE\xCC\x80";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC4\xAE\xCC\x80", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinCapitalLetterIWithOgonekAndCombiningAcute)
{
	// 012E 0301
	// 012E 0301

	const char* c = "\xC4\xAE\xCC\x81";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC4\xAE\xCC\x81", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinCapitalLetterIWithOgonekAndCombiningTilde)
{
	// 012E 0303
	// 012E 0303

	const char* c = "\xC4\xAE\xCC\x83";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC4\xAE\xCC\x83", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinCapitalLetterIWithOgonekAndCombiningOgonek)
{
	// 012E 0328
	// 012E 0328

	const char* c = "\xC4\xAE\xCC\xA8";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC4\xAE\xCC\xA8", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, SingleLatinCapitalLetterIWithOgonekMoreAboveOutOfOrder)
{
	// 012E 302E 1DCA AAC1
	//    0  224  220  230

	// 012E 302E 1DCA AAC1
	//    0  224  220  230

	const char* c = "\xC4\xAE\xE3\x80\xAE\xE1\xB7\x8A\xEA\xAB\x81";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(11, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC4\xAE\xE3\x80\xAE\xE1\xB7\x8A\xEA\xAB\x81", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterI)
{
	const char* c = "india";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(5, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("India", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterINotEnoughSpace)
{
	const char* c = "ixia";
	const size_t s = 3;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("Ixi", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterIAmountOfBytes)
{
	const char* c = "bind";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterIAndCombiningDotAbove)
{
	const char* c = "i\xCC\x87nter";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(5, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("Inter", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterIAndCombiningDotAboveNotEnoughSpace)
{
	const char* c = "Bli\xCC\x87ng";
	const size_t s = 4;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("Bli", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterIAndCombiningDotAboveAmountOfBytes)
{
	const char* c = "Bri\xCC\x87ng";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(7, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterIWithGrave)
{
	const char* c = "Wh\xC3\xACt";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(5, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("Wh\xC3\xACt", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterIWithGraveNotEnoughSpace)
{
	const char* c = "sp\xC3\xACkk";
	const size_t s = 3;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("Sp", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterIWithGraveAmountOfBytes)
{
	const char* c = "Bl\xC3\xAChhh";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(7, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterIAndCombiningGrave)
{
	const char* c = "Bri\xCC\x80lliant";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(11, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("Bri\xCC\x80lliant", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterIAndCombiningGraveNotEnoughSpace)
{
	const char* c = "Amazi\xCC\x80ng";
	const size_t s = 6;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(5, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("Amazi", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterIAndCombiningGraveAmountOfBytes)
{
	const char* c = "Bri\xCC\x80nk";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(7, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterICombiningDotAboveAndCombiningGrave)
{
	const char* c = "rapi\xCC\x87\xCC\x80" "d";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(9, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("Rapi\xCC\x87\xCC\x80" "d", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterICombiningDotAboveAndCombiningGraveNotEnoughSpace)
{
	const char* c = "Bori\xCC\x87\xCC\x80ng";
	const size_t s = 6;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(6, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("Bori\xCC\x87", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterICombiningDotAboveAndCombiningGraveAmountOfBytes)
{
	const char* c = "ki\xCC\x87\xCC\x80" "ds";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(8, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterIWithAcute)
{
	const char* c = "p\xC3\xADzza";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(6, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("P\xC3\xADzza", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterIWithAcuteNotEnoughSpace)
{
	const char* c = "\xC3\xADnvest";
	const size_t s = 4;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC3\x8Dnv", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterIWithAcuteAmountOfBytes)
{
	const char* c = "alm\xC3\xADnd";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(7, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterIAndCombiningAcute)
{
	const char* c = "bri\xCC\x81ng";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(7, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("Bri\xCC\x81ng", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterIAndCombiningAcuteNotEnoughSpace)
{
	const char* c = "Poi\xCC\x81nts";
	const size_t s = 5;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(5, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("Poi\xCC\x81", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterIAndCombiningAcuteAmountOfBytes)
{
	const char* c = "Arb\xCC\x81" "e";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(6, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterICombiningDotAboveAndCombiningAcute)
{
	const char* c = "brri\xCC\x87\xCC\x81p";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(9, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("Brri\xCC\x87\xCC\x81p", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterICombiningDotAboveAndCombiningAcuteNotEnoughSpace)
{
	const char* c = "i\xCC\x87\xCC\x81mminent";
	const size_t s = 6;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(6, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I\xCC\x81mmi", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterICombiningDotAboveAndCombiningAcuteAmountOfBytes)
{
	const char* c = "Mi\xCC\x87\xCC\x81ngle";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(10, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterIWithTilde)
{
	const char* c = "\xC4\xA9\xC7\x8E\xC7\xB4";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(6, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC4\xA8\xC7\x8E\xC7\xB5", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterIWithTildeNotEnoughSpace)
{
	const char* c = "\xC7\xAC\xCB\x97\xC7\xB9\xCA\xA0";
	const size_t s = 5;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC7\xAC\xCB\x97", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterIWithTildeAmountOfBytes)
{
	const char* c = "\xC9\xBF\xC4\xA9\xC9\xB9\xCC\x91";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(8, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterIAndCombiningTilde)
{
	const char* c = "Bri\xCC\x83llo";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(8, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("Bri\xCC\x83llo", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterIAndCombiningTildeNotEnoughSpace)
{
	const char* c = "Cli\xCC\x83" "ck";
	const size_t s = 5;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(5, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("Cli\xCC\x83", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterIAndCombiningTildeAmountOfBytes)
{
	const char* c = "Fi\xCC\x83ght";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(7, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterICombiningDotAboveAndCombiningTilde)
{
	const char* c = "\xC3\x8B" "i\xCC\x87\xCC\x83\xC3\x93\xC4\x8E";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(11, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC3\x8B" "i\xCC\x87\xCC\x83\xC3\xB3\xC4\x8F", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterICombiningDotAboveAndCombiningTildeNotEnoughSpace)
{
	const char* c = "\xC7\x8B\xC6\xBB" "i\xCC\x87\xCC\x83\xC5\x97";
	const size_t s = 8;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(7, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC7\x8B\xC6\xBBi\xCC\x87", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterICombiningDotAboveAndCombiningTildeAmountOfBytes)
{
	const char* c = "\xC7\x9F\xC5\xA1" "i\xCC\x87\xCC\x83";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(9, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterIAndCombiningOgonek)
{
	const char* c = "\xC9\xB4\xCA\x85" "i\xCC\xA8\xCD\x8B\xCA\x8C";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(11, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC9\xB4\xCA\x85" "i\xCC\xA8\xCD\x8B\xCA\x8C", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterIAndCombiningOgonekNotEnoughSpace)
{
	const char* c = "i\xCC\xA8\xCA\x98\xCE\x97\xE0\xA6\xA9";
	const size_t s = 6;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(5, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I\xCC\xA8\xCA\x98", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterIAndCombiningOgonekAmountOfBytes)
{
	const char* c = "\xD2\xAB" "i\xCC\xA8\xE0\xA8\x9F";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(8, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterICombiningDotAboveAndCombiningOgonek)
{
	const char* c = "\xC2\xA2\xCA\x8A" "i\xCC\x87\xCC\xA8\xD2\xA5";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(11, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC2\xA2\xC6\xB1" "i\xCC\x87\xCC\xA8\xD2\xA5", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterICombiningDotAboveAndCombiningOgonekNotEnoughSpace)
{
	const char* c = "i\xCC\x87\xCC\xA8\xCA\xA8\xC8\xB3\xD1\x9A";
	const size_t s = 4;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("I\xCC\xA8", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterICombiningDotAboveAndCombiningOgonekAmountOfBytes)
{
	const char* c = "\xC2\xA1" "i\xCC\x87\xCC\xA8\xC9\x8F\xCC\xBA\xCE\xA0";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(11, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterJ)
{
	const char* c = "j\xC2\xA8\xCA\x88\xC8\xB2";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(7, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("J\xC2\xA8\xC6\xAE\xC8\xB3", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterJNotEnoughSpace)
{
	const char* c = "\xCE\x8A\xC6\x8Aj\xCA\x88\xC8\xB3";
	const size_t s = 5;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(5, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xCE\x8A\xC9\x97j", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterJAmountOfBytes)
{
	const char* c = "oj\xCD\x85\xC8\xB3";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(6, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterJAndCombiningGrave)
{
	const char* c = "\xC9\xAA\xCE\xA8j\xCC\x80\xCE\xAA";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(9, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC9\xAA\xCF\x88j\xCC\x80\xCF\x8A", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterJAndCombiningGraveNotEnoughSpace)
{
	const char* c = "\x1A+j\xCC\x80\xCA\x8A\xC8\x91";
	const size_t s = 4;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\x1A+J", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterJAndCombiningGraveAmountOfBytes)
{
	const char* c = "\xCA\xA5\xC8\x90j\xCC\x80\xC5\xBA\xC3\xB2";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(11, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterJCombiningDotAboveAndCombiningGrave)
{
	const char* c = "z\xC4\x82j\xCC\x87\xCC\x80r\xCC\x81\xC4\xAA";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(13, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("Z\xC4\x83j\xCC\x87\xCC\x80r\xCC\x81\xC4\xAB", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterJCombiningDotAboveAndCombiningGraveNotEnoughSpace)
{
	const char* c = "j\xCC\x87\xCC\x80j\xC6\x8A";
	const size_t s = 5;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("J\xCC\x80j", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterJCombiningDotAboveAndCombiningGraveAmountOfBytes)
{
	const char* c = "\xDC\xA7j\xCC\x87\xCC\x80\xC4\x93";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(9, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterJAndCombiningAcute)
{
	const char* c = "\xCA\x87\xCC\x9Aj\xCC\x81\xC8\x93";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(10, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xEA\x9E\xB1\xCC\x9Aj\xCC\x81\xC8\x93", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterJAndCombiningAcuteNotEnoughSpace)
{
	const char* c = "j\xCC\x81\xD2\x8A\xCA\x89\xCE\x91";
	const size_t s = 6;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(5, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("J\xCC\x81\xD2\x8B", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterJAndCombiningAcuteAmountOfBytes)
{
	const char* c = "\xC6\xA5j\xCC\x81\xCC\xA9\xCC\x9E";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(9, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterJCombiningDotAboveAndCombiningAcute)
{
	const char* c = "\xC4\xAAj\xCC\x87\xCC\x81\xCA\x8A\xC9\x95";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(11, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC4\xAAj\xCC\x87\xCC\x81\xCA\x8A\xC9\x95", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterJCombiningDotAboveAndCombiningAcuteNotEnoughSpace)
{
	const char* c = "j\xCC\x87\xCC\x81\xC4\xAA\xC4\xB3";
	const size_t s = 5;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(5, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("J\xCC\x81\xC4\xAB", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterJCombiningDotAboveAndCombiningAcuteAmountOfBytes)
{
	const char* c = "\xC8\xA2j\xCC\x87\xCC\x81\xC4\xAA\xC4\xB3";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(11, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterJAndCombiningTilde)
{
	const char* c = "j\xCC\x83\xC2\xA9\xC4\xAB\xC4\xAA";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(9, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("J\xCC\x83\xC2\xA9\xC4\xAA\xC4\xAB", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterJAndCombiningTildeNotEnoughSpace)
{
	const char* c = "\xC9\x96j\xCC\x83\xC6\xB2\xC9\x9C";
	const size_t s = 4;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC6\x89j", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterJAndCombiningTildeAmountOfBytes)
{
	const char* c = "j\xCC\x83\xC9\xBB\xCD\x8A";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(7, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterJCombiningDotAboveAndCombiningTilde)
{
	const char* c = "z\xC9\x8Aj\xCC\x87\xCC\x83\xCD\x8F";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(10, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("Z\xC9\x8Bj\xCC\x87\xCC\x83\xCD\x8F", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterJCombiningDotAboveAndCombiningTildeNotEnoughSpace)
{
	const char* c = "j\xCC\x87\xCC\x83\xCA\x8A\xC9\x8A";
	const size_t s = 4;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("J\xCC\x83", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterJCombiningDotAboveAndCombiningTildeAmountOfBytes)
{
	const char* c = "\xC6\x9Aj\xCC\x87\xCC\x83\xC8\xA5\xC5\x9B";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(11, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterJAndCombiningOgonek)
{
	const char* c = "j\xCC\xA8\xC9\x95\xCA\xB7";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(7, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("J\xCC\xA8\xC9\x95\xCA\xB7", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterJAndCombiningOgonekNotEnoughSpace)
{
	const char* c = "\xC5\xAAj\xCC\xA8\xC8\xA8\xE0\xA2\x9A";
	const size_t s = 4;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC5\xAAj", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterJAndCombiningOgonekAmountOfBytes)
{
	const char* c = "j\xCC\xA8\xC9\xBB\xCD\x8A";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(7, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterJCombiningDotAboveAndCombiningOgonek)
{
	const char* c = "\xC4\xA7\xC6\x89j\xCC\x87\xCC\xA8\xCA\x8A";
	const size_t s = 255;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(11, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("\xC4\xA6\xC9\x96j\xCC\x87\xCC\xA8\xCA\x8A", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterJCombiningDotAboveAndCombiningOgonekNotEnoughSpace)
{
	const char* c = "j\xCC\x87\xCC\xA8\xC4\xA8\xC4\xA8";
	const size_t s = 4;
	char b[256] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), b, s, &errors));
	EXPECT_UTF8EQ("J\xCC\xA8", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST_F(Utf8ToTitleLithuanian, WordLatinSmallLetterJCombiningDotAboveAndCombiningOgonekAmountOfBytes)
{
	const char* c = "\xC6\xA5j\xCC\x87\xCC\xA8\xC6\xA6";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(9, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}