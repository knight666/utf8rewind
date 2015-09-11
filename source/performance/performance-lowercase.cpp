#include "performance-base.hpp"

#include "../helpers/helpers-strings.hpp"
#include "../internal/codepoint.h"

class LowercaseBasicLatin
	: public performance::Suite
{

public:

	virtual void setup() override
	{
		std::vector<unicode_t> codepoints;

		for (unicode_t i = 0; i <= MAX_BASIC_LATIN; ++i)
		{
			codepoints.push_back(i);
		}

		m_input = helpers::utf8(codepoints);
	}

	std::string m_input;

};

PERF_TEST_F(LowercaseBasicLatin, Static)
{
	char o[1024] = { 0 };
	size_t ol = 1023;
	int32_t e;

	utf8tolower(m_input.c_str(), m_input.length(), o, ol, &e);
}

PERF_TEST_F(LowercaseBasicLatin, Dynamic)
{
	int32_t e;

	size_t ol = utf8tolower(m_input.c_str(), m_input.length(), nullptr, 0, &e);
	if (ol > 0 &&
		e == UTF8_ERR_NONE)
	{
		char* o = new char[ol + 1];
		memset(o, 0, ol + 1);

		utf8tolower(m_input.c_str(), m_input.length(), o, ol, nullptr);

		delete [] o;
	}
}

class LowercaseLatin1
	: public performance::Suite
{

public:

	virtual void setup() override
	{
		std::vector<unicode_t> codepoints;

		for (unicode_t i = 0; i <= MAX_LATIN_1; ++i)
		{
			codepoints.push_back(i);
		}

		m_input = helpers::utf8(codepoints);
	}

	std::string m_input;

};

PERF_TEST_F(LowercaseLatin1, Static)
{
	char o[1024] = { 0 };
	size_t ol = 1023;
	int32_t e;

	utf8tolower(m_input.c_str(), m_input.length(), o, ol, &e);
}

PERF_TEST_F(LowercaseLatin1, Dynamic)
{
	int32_t e;

	size_t ol = utf8tolower(m_input.c_str(), m_input.length(), nullptr, 0, &e);
	if (ol > 0 &&
		e == UTF8_ERR_NONE)
	{
		char* o = new char[ol + 1];
		memset(o, 0, ol + 1);

		utf8tolower(m_input.c_str(), m_input.length(), o, ol, nullptr);

		delete [] o;
	}
}

class LowercaseBasicMultilingualPlane
	: public performance::Suite
{

public:

	virtual void setup() override
	{
		std::vector<unicode_t> codepoints;

		for (unicode_t i = 0; i <= MAX_BASIC_MULTILINGUAL_PLANE; ++i)
		{
			if (i < SURROGATE_HIGH_START ||
				i > SURROGATE_LOW_END)
			{
				codepoints.push_back(i);
			}
		}

		m_input = helpers::utf8(codepoints);
	}

	std::string m_input;

};

PERF_TEST_F(LowercaseBasicMultilingualPlane, Static)
{
	char o[1024] = { 0 };
	size_t ol = 1023;
	int32_t e;

	utf8tolower(m_input.c_str(), m_input.length(), o, ol, &e);
}

PERF_TEST_F(LowercaseBasicMultilingualPlane, Dynamic)
{
	int32_t e;

	size_t ol = utf8tolower(m_input.c_str(), m_input.length(), nullptr, 0, &e);
	if (ol > 0 &&
		e == UTF8_ERR_NONE)
	{
		char* o = new char[ol + 1];
		memset(o, 0, ol + 1);

		utf8tolower(m_input.c_str(), m_input.length(), o, ol, nullptr);

		delete [] o;
	}
}