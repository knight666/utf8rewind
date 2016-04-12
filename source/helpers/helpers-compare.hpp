#pragma once

/*!
	\file
	\brief Comparison helper functions.

	\cond INTERNAL
*/

#include "helpers-base.hpp"

#define EXPECT_CMP_EQ(_left, _right, _size, _flags, _expected) { \
	::helpers::CompareEntry e; \
	e.left = _left; \
	e.right = _right; \
	e.result = _expected; \
	::helpers::CompareEntry a; \
	a.result = utf8cmp(_left, _right, _size, _flags); \
	a.expression = "utf8cmp(" #_left ", " #_right ", " #_size ", " #_flags ")"; \
	EXPECT_PRED_FORMAT2(::helpers::CompareText, e, a); \
}

namespace helpers {

	struct CompareEntry
	{
		std::string left;
		std::string right;
		size_t result;
		std::string expression;
	};

	::testing::AssertionResult CompareText(
		const char* expressionExpected, const char* expressionActual,
		const CompareEntry& entryExpected, const CompareEntry& entryActual);

};

/*! \endcond */