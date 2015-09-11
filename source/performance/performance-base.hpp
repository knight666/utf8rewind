#pragma once

// C

#include <stdint.h>

// STL

#include <chrono>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <vector>

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
			m_factories[name] = factory;
		}

		int run(size_t repeatCount)
		{
			std::cout
				<< "Running " << repeatCount << " iterations."
				<< std::endl;

			typedef std::chrono::steady_clock clock;
			typedef std::chrono::microseconds ms;

			clock::time_point time_start = clock::now();

			typedef
				std::map<std::string, BaseSuiteFactory*>::iterator
				factory_it;

			for (factory_it it = m_factories.begin();
				it != m_factories.end();
				++it)
			{
				Suite* suite = it->second->create();

				std::cout
					<< "[" << it->first << "]"
					<< std::endl;

				std::vector<uint32_t> timings;

				clock::time_point total_start = clock::now();

				suite->setup();

				for (size_t i = 0; i < repeatCount; ++i)
				{
					clock::time_point test_start = clock::now();

					suite->body();

					uint32_t test_duration = (uint32_t)(
						std::chrono::duration_cast<ms>(
							clock::now() - test_start
						)).count() / 1000;

					std::cout
						<< std::setw(10) << i << ": "
						<< std::setw(8) << test_duration << " ms"
						<< std::endl;

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
					<< "Worst case: "
					<< std::setw(8) << worst_case << " ms"
					<< std::endl;

				std::cout
					<< " Best case: "
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

		std::map<std::string, BaseSuiteFactory*> m_factories;

	};

	bool registerTest(
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

#define PERF_RUN_ALL(_repeatCount) \
	performance::Collection::get().run(_repeatCount)

#define PERF_TEST_CLASS_NAME(_caseName, _testName) \
	_caseName ## _ ## _testName ## _Test

#define PERF_TEST(_caseName, _testName) \
	class PERF_TEST_CLASS_NAME(_caseName, _testName) \
		: public performance::Suite { \
	private: \
		virtual void body() override; \
		static bool m_registered; \
	}; \
	bool PERF_TEST_CLASS_NAME(_caseName, _testName)::m_registered = \
		performance::registerTest( \
			#_caseName, #_testName, \
			new performance::SuiteFactory<PERF_TEST_CLASS_NAME(_caseName, _testName)>); \
	void PERF_TEST_CLASS_NAME(_caseName, _testName)::body()