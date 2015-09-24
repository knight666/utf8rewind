#include "performance-base.hpp"

int main(int argc, char** argv)
{
	int result = PERF_RUN_ALL(argc, argv);

	std::cout << "Press any key to continue.";

	std::cin.get();

	return result;
}