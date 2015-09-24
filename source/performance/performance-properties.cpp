#include "performance-base.hpp"

#include "../helpers/helpers-strings.hpp"

extern "C" {
	#include "../internal/codepoint.h"
	#include "../internal/database.h"
}

class Properties
	: public performance::Suite
{

public:

	uint8_t m_output[MAX_LEGAL_UNICODE];
	const char* m_outputString[MAX_LEGAL_UNICODE];

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

PERF_TEST_F(Properties, QueryDecomposeNFD)
{
	for (unicode_t i = 0; i <= MAX_LEGAL_UNICODE; ++i)
	{
		uint8_t length;
		m_outputString[i] = database_querydecomposition2(i, NFDIndex1Ptr, NFDIndex2Ptr, NFDDataPtr, &length);
	}
}

PERF_TEST_F(Properties, QueryDecomposeNFKD)
{
	for (unicode_t i = 0; i <= MAX_LEGAL_UNICODE; ++i)
	{
		uint8_t length;
		m_outputString[i] = database_querydecomposition2(i, NFKDIndex1Ptr, NFKDIndex2Ptr, NFKDDataPtr, &length);
	}
}

PERF_TEST_F(Properties, QueryDecomposeUppercase)
{
	for (unicode_t i = 0; i <= MAX_LEGAL_UNICODE; ++i)
	{
		uint8_t length;
		m_outputString[i] = database_querydecomposition2(i, UppercaseIndex1Ptr, UppercaseIndex2Ptr, UppercaseDataPtr, &length);
	}
}

PERF_TEST_F(Properties, QueryDecomposeLowercase)
{
	for (unicode_t i = 0; i <= MAX_LEGAL_UNICODE; ++i)
	{
		uint8_t length;
		m_outputString[i] = database_querydecomposition2(i, LowercaseIndex1Ptr, LowercaseIndex2Ptr, LowercaseDataPtr, &length);
	}
}

PERF_TEST_F(Properties, QueryDecomposeTitlecase)
{
	for (unicode_t i = 0; i <= MAX_LEGAL_UNICODE; ++i)
	{
		uint8_t length;
		m_outputString[i] = database_querydecomposition2(i, TitlecaseIndex1Ptr, TitlecaseIndex2Ptr, TitlecaseDataPtr, &length);
	}
}