#include "helpers-normalization.hpp"

#include "helpers-strings.hpp"

namespace helpers {

	std::string nfc(unicode_t codepointLeft, unicode_t codepointRight)
	{
		std::string converted;
		converted += utf8(codepointLeft);
		converted += utf8(codepointRight);

		return nfc(converted);
	}

	std::string nfc(const std::string& text)
	{
		int32_t errors = 0;

		std::string converted;

		size_t length = utf8transform(text.c_str(), text.size() - 1, nullptr, 0, UTF8_TRANSFORM_COMPOSED, &errors);
		if (length == 0 ||
			errors != 0)
		{
			return converted;
		}

		converted.resize(length + 1);
		utf8transform(text.c_str(), text.size() - 1, &converted[0], length, UTF8_TRANSFORM_COMPOSED, &errors);

		return converted;
	}

	std::string nfd(unicode_t codepoint)
	{
		return nfd(utf8(codepoint));
	}

	std::string nfd(const std::string& text)
	{
		int32_t errors = 0;

		std::string converted;

		size_t length = utf8transform(text.c_str(), text.size() - 1, nullptr, 0, UTF8_TRANSFORM_DECOMPOSED, &errors);
		if (length == 0 ||
			errors != 0)
		{
			return converted;
		}

		converted.resize(length + 1);
		utf8transform(text.c_str(), text.size() - 1, &converted[0], length, UTF8_TRANSFORM_DECOMPOSED, &errors);

		return converted;
	}

	std::string nfkc(unicode_t codepointLeft, unicode_t codepointRight)
	{
		std::string converted;
		converted += utf8(codepointLeft);
		converted += utf8(codepointRight);

		return nfkc(converted);
	}

	std::string nfkc(const std::string& text)
	{
		int32_t errors = 0;

		std::string converted;

		size_t length = utf8transform(text.c_str(), text.size() - 1, nullptr, 0, UTF8_TRANSFORM_COMPATIBILITY_COMPOSED, &errors);
		if (length == 0 ||
			errors != 0)
		{
			return converted;
		}

		converted.resize(length + 1);
		utf8transform(text.c_str(), text.size() - 1, &converted[0], length, UTF8_TRANSFORM_COMPATIBILITY_COMPOSED, &errors);

		return converted;
	}

	std::string nfkd(unicode_t codepoint)
	{
		return nfkd(utf8(codepoint));
	}

	std::string nfkd(const std::string& text)
	{
		int32_t errors = 0;

		std::string converted;

		size_t length = utf8transform(text.c_str(), text.size() - 1, nullptr, 0, UTF8_TRANSFORM_COMPATIBILITY_DECOMPOSED, &errors);
		if (length == 0 ||
			errors != 0)
		{
			return converted;
		}

		converted.resize(length + 1);
		utf8transform(text.c_str(), text.size() - 1, &converted[0], length, UTF8_TRANSFORM_COMPATIBILITY_DECOMPOSED, &errors);

		return converted;
	}

};