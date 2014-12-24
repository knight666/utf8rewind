#pragma once

#include "tests-base.hpp"

#include "utf8rewind.h"

#define CHECK_NORMALIZE(_source, _decomposed, _composed) { \
	::helpers::NormalizationEntry e; \
	e.source = _source; \
	e.decomposed = _decomposed; \
	e.composed = _composed; \
	::helpers::NormalizationEntry a; \
	a.source = _source; \
	a.decomposed = helpers::nfd(_source); \
	a.composed = helpers::nfc(helpers::nfd(_source)); \
	EXPECT_PRED_FORMAT2(::helpers::CompareNormalization, e, a); \
}

#define CHECK_NORMALIZE_COMPATIBILITY(_source, _decomposed, _composed) { \
	::helpers::NormalizationEntry e; \
	e.source = _source; \
	e.decomposed = _decomposed; \
	e.composed = _composed; \
	::helpers::NormalizationEntry a; \
	a.source = _source; \
	a.decomposed = helpers::nfkd(_source); \
	a.composed = helpers::nfkc(helpers::nfkd(_source)); \
	EXPECT_PRED_FORMAT2(::helpers::CompareNormalization, e, a); \
}

namespace helpers {

	std::string nfc(unicode_t codepointLeft, unicode_t codepointRight);
	std::string nfc(const std::string& text);

	std::string nfd(unicode_t codepoint);
	std::string nfd(const std::string& text);

	std::string nfkc(unicode_t codepointLeft, unicode_t codepointRight);
	std::string nfkc(const std::string& text);

	std::string nfkd(unicode_t codepoint);
	std::string nfkd(const std::string& text);

	struct NormalizationEntry
	{
		std::string source;
		std::string decomposed;
		std::string composed;
	};

	::testing::AssertionResult CompareNormalization(
		const char* expressionExpected, const char* expressionActual,
		const NormalizationEntry& entryExpected, const NormalizationEntry& entryActual);

};