#include "performance-base.hpp"

#include "../helpers/helpers-strings.hpp"

extern "C" {
	#include "../internal/codepoint.h"
	#include "../internal/compressedproperties.h"
}

class Properties
	: public performance::Suite
{

public:

	uint8_t m_output[MAX_LEGAL_UNICODE];

};

PERF_TEST_F(Properties, QueryGeneralCategory)
{
	for (unicode_t i = 0; i <= MAX_LEGAL_UNICODE; ++i)
	{
		m_output[i] = PROPERTY_GET_GC(i);
	}
}

PERF_TEST_F(Properties, QueryCanonicalCombiningClass)
{
	for (unicode_t i = 0; i <= MAX_LEGAL_UNICODE; ++i)
	{
		m_output[i] = PROPERTY_GET_CCC(i);
	}
}

PERF_TEST_F(Properties, QueryNFC)
{
	for (unicode_t i = 0; i <= MAX_LEGAL_UNICODE; ++i)
	{
		m_output[i] = PROPERTY_GET_NFC(i);
	}
}

PERF_TEST_F(Properties, QueryNFD)
{
	for (unicode_t i = 0; i <= MAX_LEGAL_UNICODE; ++i)
	{
		m_output[i] = PROPERTY_GET_NFD(i);
	}
}

PERF_TEST_F(Properties, QueryNFKC)
{
	for (unicode_t i = 0; i <= MAX_LEGAL_UNICODE; ++i)
	{
		m_output[i] = PROPERTY_GET_NFKC(i);
	}
}

PERF_TEST_F(Properties, QueryNFKD)
{
	for (unicode_t i = 0; i <= MAX_LEGAL_UNICODE; ++i)
	{
		m_output[i] = PROPERTY_GET_NFKD(i);
	}
}