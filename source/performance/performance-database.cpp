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
	for (unicode_t i = 0; i <= MAX_LEGAL_UNICODE; ++i)
	{
		m_output[i] = database_querydecomposition(i, UnicodeProperty_Normalization_Decompose);
	}
}

PERF_TEST_F(Database, QueryDecomposeNFKD)
{
	for (unicode_t i = 0; i <= MAX_LEGAL_UNICODE; ++i)
	{
		m_output[i] = database_querydecomposition(i, UnicodeProperty_Normalization_Compatibility_Decompose);
	}
}

PERF_TEST_F(Database, QueryDecomposeUppercase)
{
	for (unicode_t i = 0; i <= MAX_LEGAL_UNICODE; ++i)
	{
		m_output[i] = database_querydecomposition(i, UnicodeProperty_Uppercase);
	}
}

PERF_TEST_F(Database, QueryDecomposeLowercase)
{
	for (unicode_t i = 0; i <= MAX_LEGAL_UNICODE; ++i)
	{
		m_output[i] = database_querydecomposition(i, UnicodeProperty_Lowercase);
	}
}

PERF_TEST_F(Database, QueryDecomposeTitlecase)
{
	for (unicode_t i = 0; i <= MAX_LEGAL_UNICODE; ++i)
	{
		m_output[i] = database_querydecomposition(i, UnicodeProperty_Titlecase);
	}
}