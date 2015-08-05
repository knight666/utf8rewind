#include "tests-base.hpp"

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);

	int result = RUN_ALL_TESTS();

	if (result != 0)
	{
		std::cout << "Press any key to continue.";

		std::cin.get();
	}

	return result;
}