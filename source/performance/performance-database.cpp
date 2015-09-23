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

	uint8_t m_output[MAX_LEGAL_UNICODE];

};

PERF_TEST_F(Database, QueryGeneralCategory)
{
	for (unicode_t i = 0; i <= MAX_LEGAL_UNICODE; ++i)
	{
		m_output[i] = database_queryproperty(i, UnicodeProperty_GeneralCategory);
	}
}

PERF_TEST_F(Database, QueryCanonicalCombiningClass)
{
	for (unicode_t i = 0; i <= MAX_LEGAL_UNICODE; ++i)
	{
		m_output[i] = database_queryproperty(i, UnicodeProperty_CanonicalCombiningClass);
	}
}

PERF_TEST_F(Database, QueryNFC)
{
	for (unicode_t i = 0; i <= MAX_LEGAL_UNICODE; ++i)
	{
		m_output[i] = database_queryproperty(i, UnicodeProperty_Normalization_Compose);
	}
}

PERF_TEST_F(Database, QueryNFD)
{
	for (unicode_t i = 0; i <= MAX_LEGAL_UNICODE; ++i)
	{
		m_output[i] = database_queryproperty(i, UnicodeProperty_Normalization_Decompose);
	}
}

PERF_TEST_F(Database, QueryNFKC)
{
	for (unicode_t i = 0; i <= MAX_LEGAL_UNICODE; ++i)
	{
		m_output[i] = database_queryproperty(i, UnicodeProperty_Normalization_Compatibility_Compose);
	}
}

PERF_TEST_F(Database, QueryNFKD)
{
	for (unicode_t i = 0; i <= MAX_LEGAL_UNICODE; ++i)
	{
		m_output[i] = database_queryproperty(i, UnicodeProperty_Normalization_Compatibility_Decompose);
	}
}