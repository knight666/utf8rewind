#pragma once

/*!
	\file
	\brief Seeking helper functions.

	\cond INTERNAL
*/

#include "tests-base.hpp"

#include "utf8rewind.h"

#define EXPECT_SEEKEQ(_expectedString, _expectedOffset, _actual) { \
	::helpers::SeekingParameters e; \
	e.text = _expectedString; \
	e.offset = _expectedOffset; \
	e.expression = ""; \
	::helpers::SeekingParameters a; \
	a.text = _actual; \
	a.offset = 0; \
	a.expression = #_actual; \
	::helpers::SeekingParameters i; \
	i.text = t; \
	i.offset = 0; \
	i.expression = ""; \
	EXPECT_PRED_FORMAT3(::helpers::CompareSeeking, e, a, i); \
}

namespace helpers {

	struct SeekingParameters
	{
		const char* text;
		size_t offset;
		const char* expression;
	};

	::testing::AssertionResult CompareSeeking(
		const char* expressionExpected, const char* expressionActual, const char* expressionInput,
		const SeekingParameters& paramsExpected, const SeekingParameters& paramsActual, const SeekingParameters& paramsInput);

};

/*! \endcond */