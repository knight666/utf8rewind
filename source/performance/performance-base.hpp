#pragma once

// C

#include <stdint.h>

// STL

#include <chrono>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

// utf8rewind

#include "utf8rewind.h"

// Google Test

#include "gtest/internal/gtest-port.h"
#include "gtest/gtest.h"

namespace testing {
namespace internal {

	extern bool ParseBoolFlag(
		const char* str,
		const char* flag,
		bool* value);

	extern bool ParseInt32Flag(
		const char* str,
		const char* flag,
		Int32* value);

	extern bool ParseStringFlag(
		const char* str,
		const char* flag,
		std::string* value);

	static bool PatternMatchesString(
		const char* pattern,
		const char* text)
	{
		switch (*pattern)
		{
			case '\0':
			case ':':  // Either ':' or '\0' marks the end of the pattern.
				return *text == '\0';
			case '?':  // Matches any single character.
				return
					*text != '\0'
					&& PatternMatchesString(pattern + 1, text + 1);
			case '*':  // Matches any string (possibly empty) of characters.
				return (
					*text != '\0' &&
					PatternMatchesString(pattern, text + 1)) ||
					PatternMatchesString(pattern + 1, text);
			default:  // Non-special character.  Matches itself.
				return
					*pattern == *text &&
					PatternMatchesString(pattern + 1, text + 1);
		}
	}

	static bool MatchesFilter(
		const std::string& name,
		const std::string& filter)
	{
		const char *cur_pattern = filter.c_str();
		while (1)
		{
			if (PatternMatchesString(cur_pattern, name.c_str()))
			{
				return true;
			}

			cur_pattern = strchr(cur_pattern, ':');
			if (cur_pattern == NULL)
			{
				return false;
			}

			// Skip the pattern separator (the ':' character).
			cur_pattern++;
		}
	}

};
};


namespace performance {

	class Suite
	{

	public:

		virtual void setup() { }
		virtual void body() = 0;
		virtual void tearDown() { }

	};

	class BaseSuiteFactory
	{

	public:

		virtual Suite* create() = 0;

	};

	template <class SuiteType>
	class SuiteFactory
		: public BaseSuiteFactory
	{

	public:

		virtual Suite* create() override { return new SuiteType(); }

	};

	class Collection
	{

	public:

		static Collection& get()
		{
			static Collection instance;
			return instance;
		}

		void addFactory(
			const std::string& name,
			BaseSuiteFactory* factory)
		{
			m_factories.push_back(std::make_pair(name, factory));
		}

		int run(int argc, char** argv)
		{
			using testing::internal::ParseBoolFlag;
			using testing::internal::ParseInt32Flag;
			using testing::internal::ParseStringFlag;

			testing::internal::Int32 repeat_count = 10;
			bool display_individual = false;
			std::string filter = "*";
			bool show_help = false;

			for (int i = 1; i < argc; ++i)
			{
				std::string arg_help = argv[i];

				if (arg_help == "--help" ||
					arg_help == "-h" ||
					arg_help == "-?" ||
					arg_help == "/?")
				{
					show_help = true;

					break;
				}

				if (!ParseInt32Flag(argv[i], "repeat_count", &repeat_count) &&
					!ParseBoolFlag(argv[i], "display_individual", &display_individual) &&
					!ParseStringFlag(argv[i], "filter", &filter))
				{
					show_help = true;

					break;
				}
			}

			if (show_help)
			{
				std::cout
					<< "--" GTEST_FLAG_PREFIX_ "repeat_count=[COUNT]" << std::endl
					<< "    How many times to repeat the performance tests. The default is " << std::endl
					<< "    100 times." << std::endl
					<< "--" GTEST_FLAG_PREFIX_ "display_invidual" << std::endl
					<< "    Display individual timings, instead of just the total time, worst," << std::endl
					<< "    best and average case." << std::endl
					<< "--" GTEST_FLAG_PREFIX_ "filter=POSITIVE_PATTERNS[-NEGATIVE_PATTERNS]" << std::endl
					<< "    Run only the tests whose name matches one of the positive patterns but" << std::endl
					<< "    none of the negative patterns. '?' matches any single character; '*'" << std::endl
					<< "    matches any substring; ':' separates two patterns." << std::endl;

				return 0;
			}

			if (filter != "*")
			{
				std::cout
					<< "NOTE: Filter is \"" << filter << "\""
					<< std::endl;
			}

			std::string positive_filter;
			std::string negative_filter;
			const char* dash = strchr(filter.c_str(), '-');
			if (dash != nullptr)
			{
				positive_filter = std::string(filter.c_str(), dash);
				if (positive_filter.empty())
				{
					positive_filter = "*";
				}
				negative_filter = std::string(dash + 1);
			}
			else
			{
				positive_filter = filter;
			}

			std::cout
				<< "Running " << repeat_count << " iterations."
				<< std::endl;

			typedef std::chrono::steady_clock clock;
			typedef std::chrono::microseconds ms;

			clock::time_point time_start = clock::now();

			typedef
				std::vector<std::pair<std::string, BaseSuiteFactory*>>::iterator
				factory_it;

			for (factory_it it = m_factories.begin();
				it != m_factories.end();
				++it)
			{
				using testing::internal::MatchesFilter;

				Suite* suite = it->second->create();

				if (!MatchesFilter(it->first, positive_filter) ||
					MatchesFilter(it->first, negative_filter))
				{
					continue;
				}

				std::cout
					<< "[" << it->first << "]"
					<< std::endl;

				std::vector<uint32_t> timings;

				clock::time_point total_start = clock::now();

				suite->setup();

				for (size_t i = 0; i < repeat_count; ++i)
				{
					clock::time_point test_start = clock::now();

					suite->body();

					uint32_t test_duration = (uint32_t)(
						std::chrono::duration_cast<ms>(
							clock::now() - test_start
						)).count() / 1000;

					if (display_individual)
					{
						std::cout
							<< std::setw(10) << i << ": "
							<< std::setw(8) << test_duration << " ms"
							<< std::endl;
					}

					timings.push_back(test_duration);
				}

				suite->tearDown();

				uint32_t total_duration = (uint32_t)(
						std::chrono::duration_cast<ms>(
							clock::now() - total_start
						)).count() / 1000;

				uint32_t worst_case = 0;
				uint32_t best_case = std::numeric_limits<uint32_t>::max();
				double average = 0.0;

				for (std::vector<uint32_t>::iterator it = timings.begin();
					it != timings.end();
					++it)
				{
					worst_case = std::max(worst_case, *it);
					best_case = std::min(best_case, *it);
					average += (double)*it;
				}

				average /= (double)timings.size();

				std::cout
					<< "     Total: "
					<< std::setw(8) << total_duration << " ms"
					<< std::endl;

				std::cout
					<< " Best case: "
					<< std::setw(8) << best_case << " ms"
					<< std::endl;

				std::cout
					<< "Worst case: "
					<< std::setw(8) << worst_case << " ms"
					<< std::endl;

				std::cout
					<< "   Average: "
					<< std::setw(8) << average << " ms"
					<< std::endl;

				std::cout << std::endl;
			}

			return 0;
		}

	private:

		std::vector<std::pair<std::string, BaseSuiteFactory*>> m_factories;

	};

	inline bool registerTest(
		const char* caseName,
		const char* testName,
		BaseSuiteFactory* factory)
	{
		std::string name = caseName;
		name += ".";
		name += testName;

		Collection::get().addFactory(name, factory);

		return true;
	}

}

#define PERF_RUN_ALL(_argc, _argv) \
	performance::Collection::get().run(_argc, _argv)

#define PERF_TEST_CLASS_NAME(_caseName, _testName) \
	_caseName ## _ ## _testName ## _Test

#define PERF_TEST_IMPL(_caseName, _testName, _parentClass) \
	class PERF_TEST_CLASS_NAME(_caseName, _testName) \
		: public _parentClass { \
	private: \
		virtual void body() override; \
		static bool m_registered; \
	}; \
	bool PERF_TEST_CLASS_NAME(_caseName, _testName)::m_registered = \
		performance::registerTest( \
			#_caseName, #_testName, \
			new performance::SuiteFactory< \
				PERF_TEST_CLASS_NAME(_caseName, _testName)>); \
	void PERF_TEST_CLASS_NAME(_caseName, _testName)::body()

#define PERF_TEST(_caseName, _testName) \
	PERF_TEST_IMPL(_caseName, _testName, performance::Suite)

#define PERF_TEST_F(_caseName, _testName) \
	PERF_TEST_IMPL(_caseName, _testName, _caseName)