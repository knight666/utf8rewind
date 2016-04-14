#include "helpers-compare.hpp"

#include "helpers-strings.hpp"

namespace helpers {

	void printComparison(::testing::AssertionResult& output, const std::string& left, const std::string& right, size_t offset)
	{
		output << "   Result: " << offset << std::endl;

		if (offset == 0)
		{
			output << "     Left: \"" << printable(left) << "\"" << std::endl;
			output << "    Right: \"" << printable(right) << "\"" << std::endl;

			return;
		}

		offset--;

		const char* leftCurrent = left.c_str() + offset;
		const char* leftNext = utf8seek(leftCurrent, left.length(), left.c_str(), 1, SEEK_CUR);
		size_t leftDifferenceStart = offset;
		size_t leftDifferenceEnd = leftDifferenceStart + (leftNext - leftCurrent);
		std::stringstream printLeft;
		std::stringstream markLeft;
		bool hexLeft = false;

		const char* rightCurrent = right.c_str() + offset;
		const char* rightNext = utf8seek(rightCurrent, right.length(), right.c_str(), 1, SEEK_CUR);
		size_t rightDifferenceStart = offset;
		size_t rightDifferenceEnd = rightDifferenceStart + (rightNext - rightCurrent);
		std::stringstream printRight;
		std::stringstream markRight;
		bool hexRight = false;

		for (size_t i = 0; i < std::max(left.length(), right.length()); ++i)
		{
			if (i < left.length())
			{
				std::stringstream ss;
				printable(ss, hexLeft, (unicode_t)left[i] & 0xFF, 1);
				printLeft << ss.str();

				for (size_t j = 0; j < ss.str().length(); ++j)
				{
					markLeft << ((i >= leftDifferenceStart && i <= leftDifferenceEnd) ? "~" : " ");
				}
			}

			if (i < right.length())
			{
				std::stringstream ss;
				printable(ss, hexRight, (unicode_t)right[i] & 0xFF, 1);
				printRight << ss.str();

				for (size_t j = 0; j < ss.str().length(); ++j)
				{
					markRight << ((i >= rightDifferenceStart && i <= rightDifferenceEnd) ? "~" : " ");
				}
			}
		}

		output << "     Left: \"" << printLeft.str() << "\"" << std::endl;
		output << "            " << markLeft.str() << std::endl;
		output << "    Right: \"" << printRight.str() << "\"" << std::endl;
		output << "            " << markRight.str() << std::endl;
	}

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

			result << "[Actual]" << std::endl;
			printComparison(result, entryExpected.left, entryExpected.right, entryActual.result);

			result << "[Expected]" << std::endl;
			printComparison(result, entryExpected.left, entryExpected.right, entryExpected.result);

			return result;
		}
	}

};