#pragma once

#include "tests-base.hpp"

#include "utf8rewind.h"

extern "C" {
	#include "../internal/database.h"
	#include "../internal/streaming.h"
}

#define CHECK_STREAM(_stream, _index, _codepoint, _qc, _ccc) { \
	::helpers::StreamEntry e; \
	e.codepoint = _codepoint; \
	e.quick_check = QuickCheckResult_ ## _qc; \
	e.canonical_combining_class = _ccc; \
	::helpers::StreamEntry a; \
	a.codepoint = _stream.codepoint[_index]; \
	a.quick_check = _stream.quick_check[_index]; \
	a.canonical_combining_class = _stream.canonical_combining_class[_index]; \
	EXPECT_PRED_FORMAT2(::helpers::CompareStream, e, a); \
}

#define CHECK_STREAM_END(_stream, _index) { \
	::helpers::StreamEntry e; \
	e.codepoint = 0; \
	e.quick_check = QuickCheckResult_Yes; \
	e.canonical_combining_class = 0; \
	::helpers::StreamEntry a; \
	a.codepoint = _stream.codepoint[_index]; \
	a.quick_check = _stream.quick_check[_index]; \
	a.canonical_combining_class = _stream.canonical_combining_class[_index]; \
	EXPECT_PRED_FORMAT2(::helpers::CompareStream, e, a); \
}

namespace helpers {

	StreamState createStream(const std::string& text);

	std::string quickCheckToString(uint8_t quickCheck);

	struct StreamEntry
	{
		unicode_t codepoint;
		uint8_t quick_check;
		uint8_t canonical_combining_class;
	};

	::testing::AssertionResult CompareStream(
		const char* expressionExpected, const char* expressionActual,
		const StreamEntry& entryExpected, const StreamEntry& entryActual);

};