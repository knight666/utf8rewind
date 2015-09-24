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

PERF_TEST_F(Database, Decompose)
{
	char scratch[128] = { 0 };

	for (unicode_t i = 0; i <= MAX_LEGAL_UNICODE; ++i)
	{
		const char* decomposition = database_querydecomposition(i, UnicodeProperty_Normalization_Decompose);
		if (decomposition != nullptr)
		{
			memcpy(scratch, decomposition, strlen(decomposition));
		}
	}
}

PERF_TEST_F(Database, DecomposeCompatibility)
{
	char scratch[128] = { 0 };

	for (unicode_t i = 0; i <= MAX_LEGAL_UNICODE; ++i)
	{
		const char* decomposition = database_querydecomposition(i, UnicodeProperty_Normalization_Compatibility_Decompose);
		if (decomposition != nullptr)
		{
			memcpy(scratch, decomposition, strlen(decomposition));
		}
	}
}