#include "tests-base.hpp"

#include "utf8rewind.h"

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);

	int result = RUN_ALL_TESTS();

	return result;
}