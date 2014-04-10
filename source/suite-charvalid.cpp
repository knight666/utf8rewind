#include "tests-base.hpp"

#include "utf8rewind.h"

TEST(CharValid, Valid)
{
	EXPECT_TRUE(utf8charvalid(0));
	EXPECT_TRUE(utf8charvalid(' '));
	EXPECT_TRUE(utf8charvalid('B'));
	EXPECT_TRUE(utf8charvalid('\x7F'));
	EXPECT_TRUE(utf8charvalid('\xE3'));
}

TEST(CharValid, Invalid)
{
	EXPECT_FALSE(utf8charvalid('\xC0'));
	EXPECT_FALSE(utf8charvalid('\xC1'));
	EXPECT_FALSE(utf8charvalid('\xF5'));
	EXPECT_FALSE(utf8charvalid('\xF6'));
	EXPECT_FALSE(utf8charvalid('\xF7'));
	EXPECT_FALSE(utf8charvalid('\xF8'));
	EXPECT_FALSE(utf8charvalid('\xF9'));
	EXPECT_FALSE(utf8charvalid('\xFA'));
	EXPECT_FALSE(utf8charvalid('\xFB'));
	EXPECT_FALSE(utf8charvalid('\xFC'));
	EXPECT_FALSE(utf8charvalid('\xFD'));
	EXPECT_FALSE(utf8charvalid('\xFE'));
	EXPECT_FALSE(utf8charvalid('\xFF'));
}