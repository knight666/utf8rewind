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

			size_t offset = std::max<size_t>(entryActual.result, 1) - 1;

			const char* left = entryExpected.left.c_str();
			const char* leftCurrent = left + offset;
			const char* leftNext = utf8seek(leftCurrent, entryExpected.left.length(), left, 1, SEEK_CUR);
			size_t leftDifferenceStart = offset;
			size_t leftDifferenceEnd = leftDifferenceStart + (leftNext - leftCurrent);
			std::stringstream printLeft;
			std::stringstream markLeft;

			const char* right = entryExpected.right.c_str();
			const char* rightCurrent = right + offset;
			const char* rightNext = utf8seek(rightCurrent, entryExpected.right.length(), right, 1, SEEK_CUR);
			size_t rightDifferenceStart = offset;
			size_t rightDifferenceEnd = rightDifferenceStart + (rightNext - rightCurrent);
			std::stringstream printRight;
			std::stringstream markRight;

			for (size_t i = 0; i < std::max(entryExpected.left.length(), entryExpected.right.length()); ++i)
			{
				if (i < entryExpected.left.length())
				{
					uint8_t left = (uint8_t)entryExpected.left[i];

					printLeft << "\\x" << std::uppercase << std::setfill('0') << std::hex << std::setw(2) << ((unicode_t)left & 0xFF);
					markLeft << ((i >= leftDifferenceStart && i <= leftDifferenceEnd) ? "~~~~" : "    ");
				}

				if (i < entryExpected.right.length())
				{
					uint8_t right = (uint8_t)entryExpected.right[i];

					printRight << "\\x" << std::uppercase << std::setfill('0') << std::hex << std::setw(2) << ((unicode_t)right & 0xFF);
					markRight << ((i >= rightDifferenceStart && i <= rightDifferenceEnd) ? "~~~~" : "    ");
				}
			}

			result << "[Text]" << std::endl;
			result << "Left:      \"" << printLeft.str() << "\"" << std::endl;
			result << "            " << markLeft.str() << std::endl;
			result << "Right:     \"" << printRight.str() << "\"" << std::endl;
			result << "            " << markRight.str() << std::endl;

			result << "[Result]" << std::endl;
			result << "    Actual: " << entryActual.result << std::endl;
			result << "  Expected: " << entryExpected.result << std::endl;

			return result;
		}
	}

};