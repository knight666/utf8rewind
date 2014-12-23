#pragma once

#include "tests-base.hpp"

#include "utf8rewind.h"

#define CHECK_NORMALIZE(_source, _nfd, _nfc) { \
	EXPECT_UTF8EQ(_nfd, helpers::nfd(_source).c_str()); \
	EXPECT_UTF8EQ(_nfc, helpers::nfc(helpers::nfd(_source)).c_str()); \
}

#define CHECK_NORMALIZE_COMPATIBILITY(_source, _nfkd, _nfkc) { \
	EXPECT_UTF8EQ(_nfkd, helpers::nfkd(_source).c_str()); \
	EXPECT_UTF8EQ(_nfkc, helpers::nfkc(helpers::nfkd(_source)).c_str()); \
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

};