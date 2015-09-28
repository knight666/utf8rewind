#include "performance-base.hpp"

#include "../helpers/helpers-strings.hpp"

extern "C" {
	#include "../internal/codepoint.h"
	#include "../internal/database.h"
}

class Database
	: public performance::Suite
{

public:

	const char* m_output[MAX_LEGAL_UNICODE];

};

PERF_TEST_F(Database, QueryDecomposeNFD)
{
	uint8_t length = 0;

	for (unicode_t i = 0; i <= MAX_LEGAL_UNICODE; ++i)
	{
		m_output[i] = database_querydecomposition(i, NFDDataPtr, NFDIndex1Ptr, NFDIndex2Ptr, &length);
	}
}

PERF_TEST_F(Database, QueryDecomposeNFKD)
{
	uint8_t length = 0;

	for (unicode_t i = 0; i <= MAX_LEGAL_UNICODE; ++i)
	{
		m_output[i] = database_querydecomposition(i, NFKDDataPtr, NFKDIndex1Ptr, NFKDIndex2Ptr, &length);
	}
}

PERF_TEST_F(Database, QueryDecomposeUppercase)
{
	uint8_t length = 0;

	for (unicode_t i = 0; i <= MAX_LEGAL_UNICODE; ++i)
	{
		m_output[i] = database_querydecomposition(i, UppercaseDataPtr, UppercaseIndex1Ptr, UppercaseIndex2Ptr, &length);
	}
}

PERF_TEST_F(Database, QueryDecomposeLowercase)
{
	uint8_t length = 0;

	for (unicode_t i = 0; i <= MAX_LEGAL_UNICODE; ++i)
	{
		m_output[i] = database_querydecomposition(i, LowercaseDataPtr, LowercaseIndex1Ptr, LowercaseIndex2Ptr, &length);
	}
}

PERF_TEST_F(Database, QueryDecomposeTitlecase)
{
	uint8_t length = 0;

	for (unicode_t i = 0; i <= MAX_LEGAL_UNICODE; ++i)
	{
		m_output[i] = database_querydecomposition(i, TitlecaseDataPtr, TitlecaseIndex1Ptr, TitlecaseIndex2Ptr, &length);
	}
}