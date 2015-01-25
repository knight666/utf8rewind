#pragma once

#include "tests-base.hpp"

#include "utf8rewind.h"

#define CHECK_NORMALIZE_CODEPOINT(_codepoint, _decomposed, _composed, _decomposedCompatibility, _composedCompatibility, _name) { \
	::helpers::NormalizationEntry e; \
	e.codepoint = _codepoint; \
	e.name = _name; \
	e.decomposed = _decomposed; \
	e.composed = _composed; \
	e.decomposedCompatibility = _decomposedCompatibility; \
	e.composedCompatibility = _composedCompatibility; \
	::helpers::NormalizationEntry a; \
	a.decomposed = helpers::nfd(_codepoint); \
	a.composed = helpers::nfc(helpers::nfd(_codepoint)); \
	a.decomposedCompatibility = helpers::nfkd(_codepoint); \
	a.composedCompatibility = helpers::nfkc(helpers::nfkd(_codepoint)); \
	EXPECT_PRED_FORMAT2(::helpers::CompareNormalization, e, a); \
}

#define CHECK_NORMALIZE_SEQUENCE(_sequence, _decomposed, _composed, _decomposedCompatibility, _composedCompatibility) { \
	::helpers::NormalizationEntry e; \
	e.sequence = _sequence; \
	e.decomposed = _decomposed; \
	e.composed = _composed; \
	e.decomposedCompatibility = _decomposedCompatibility; \
	e.composedCompatibility = _composedCompatibility; \
	::helpers::NormalizationEntry a; \
	a.decomposed = helpers::nfd(_sequence); \
	a.composed = helpers::nfc(helpers::nfd(_sequence)); \
	a.decomposedCompatibility = helpers::nfkd(_sequence); \
	a.composedCompatibility = helpers::nfkc(helpers::nfkd(_sequence)); \
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
		NormalizationEntry()
			: codepoint(0)
		{
		}

		unicode_t codepoint;
		std::string sequence;
		std::string name;
		std::string decomposed;
		std::string composed;
		std::string decomposedCompatibility;
		std::string composedCompatibility;
	};

	::testing::AssertionResult CompareNormalization(
		const char* expressionExpected, const char* expressionActual,
		const NormalizationEntry& entryExpected, const NormalizationEntry& entryActual);

};