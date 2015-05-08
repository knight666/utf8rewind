#pragma once

/*!
	\file
	\brief Seeking helper functions.

	\cond INTERNAL
*/

#include "tests-base.hpp"

#include "utf8rewind.h"

#include "helpers-strings.hpp"

#define EXPECT_SEEKEQ(_expectedString, _expectedOffset, _actual) \
	EXPECT_UTF8EQ(_expectedString, _actual); \
	EXPECT_EQ(t + _expectedOffset, _actual);

/*! \endcond */