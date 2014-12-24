#pragma once

#include "tests-base.hpp"

#include "utf8rewind.h"

#define CHECK_NORMALIZE(_codepoint, _decomposed, _composed, _name) { \
	::helpers::NormalizationEntry e; \
	e.codepoint = _codepoint; \
	e.name = _name; \
	e.decomposed = _decomposed; \
	e.composed = _composed; \
	::helpers::NormalizationEntry a; \
	a.decomposed = helpers::nfd(_codepoint); \
	a.composed = helpers::nfc(helpers::nfd(_codepoint)); \
	EXPECT_PRED_FORMAT2(::helpers::CompareNormalization, e, a); \
}

#define CHECK_NORMALIZE_COMPATIBILITY(_codepoint, _decomposed, _composed, _name) { \
	::helpers::NormalizationEntry e; \
	e.codepoint = _codepoint; \
	e.name = _name; \
	e.compatibility = true; \
	e.decomposed = _decomposed; \
	e.composed = _composed; \
	::helpers::NormalizationEntry a; \
	a.decomposed = helpers::nfkd(_codepoint); \
	a.composed = helpers::nfkc(helpers::nfkd(_codepoint)); \
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
			, compatibility(false)
		{
		}

		unicode_t codepoint;
		std::string name;
		bool compatibility;
		std::string decomposed;
		std::string composed;
	};

	::testing::AssertionResult CompareNormalization(
		const char* expressionExpected, const char* expressionActual,
		const NormalizationEntry& entryExpected, const NormalizationEntry& entryActual);

};