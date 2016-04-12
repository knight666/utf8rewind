#include "helpers-compare.hpp"

#include "helpers-strings.hpp"

namespace helpers {

	::testing::AssertionResult CompareText(
		const char* expressionExpected, const char* expressionActual,
		const CompareEntry& entryExpected, const CompareEntry& entryActual)
	{
		if (entryExpected.result == entryActual.result)
		{
			return ::testing::AssertionSuccess();
		}
		else
		{
			::testing::AssertionResult result = ::testing::AssertionFailure();

			result << entryActual.expression << std::endl;

			result << "[Result]" << std::endl;
			result << "    Actual: " << entryActual.result << std::endl;
			result << "  Expected: " << entryExpected.result << std::endl;

			result << "[Text]" << std::endl;

			std::string printLeft = printable(entryExpected.left);
			std::string printRight = printable(entryExpected.right);

			if (entryActual.result > 0)
			{
				const char* left = entryExpected.left.c_str();
				const char* leftOffset = left + entryActual.result - 1;
				const char* leftOffsetNext = utf8seek(leftOffset, entryExpected.left.length() - entryActual.result - 1, left, 1, SEEK_CUR);

				int i = 0;
			}

			result << printLeft << std::endl;
			result << printRight << std::endl;

			return result;
		}
	}

};