#include "helpers-streams.hpp"

#include "helpers-strings.hpp"

namespace helpers {

	StreamState createStream(const std::string& text)
	{
		StreamState stream;
		memset(&stream, 0, sizeof(stream));

		std::vector<unicode_t> converted = helpers::utf32(text);

		for (std::vector<unicode_t>::iterator it = converted.begin(); it != converted.end(); ++it)
		{
			stream.codepoint[stream.current] = *it;
			stream.quick_check[stream.current] = QuickCheckResult_Yes;
		#if UTF8_VERSION_GUARD(1, 3, 0)
			stream.canonical_combining_class[stream.current] = PROPERTY_GET_CCC(*it);
		#else
			stream.canonical_combining_class[stream.current] = database_queryproperty(*it, UnicodeProperty_CanonicalCombiningClass);
		#endif
			stream.current++;
		}

		return stream;
	}

	std::string quickCheckToString(uint8_t value)
	{
		std::stringstream ss;

		switch (value)
		{

	#define MAKE_CASE(_name) case QuickCheckResult_ ## _name: ss << #_name; break

		MAKE_CASE(Yes);
		MAKE_CASE(Maybe);
		MAKE_CASE(No);

	#undef MAKE_CASE

		default:
			ss << "<invalid>";
			break;

		}

		ss << " (" << (int)value << ")";

		return ss.str();
	}

	std::string canonicalCombiningClassToString(uint8_t value)
	{
		std::stringstream ss;

		if (value >= CCC_FIXED_POSITION_START &&
			value <= CCC_FIXED_POSITION_END)
		{
			ss << "FIXED_POSITION";
		}
		else
		{
			switch (value)
			{

		#define MAKE_CASE(_name) case CCC_ ## _name: ss << #_name; break

			MAKE_CASE(NOT_REORDERED);
			MAKE_CASE(OVERLAY);
			MAKE_CASE(NUKTA);
			MAKE_CASE(KANA_VOICING);
			MAKE_CASE(VIRAMA);
			MAKE_CASE(FIXED_POSITION_START);
			MAKE_CASE(FIXED_POSITION_END);
			MAKE_CASE(ATTACHED_BELOW_LEFT);
			MAKE_CASE(ATTACHED_BELOW);
			MAKE_CASE(ATTACHED_BOTTOM_RIGHT);
			MAKE_CASE(ATTACHED_LEFT);
			MAKE_CASE(ATTACHED_RIGHT);
			MAKE_CASE(ATTACHED_TOP_LEFT);
			MAKE_CASE(ATTACHED_ABOVE);
			MAKE_CASE(ATTACHED_ABOVE_RIGHT);
			MAKE_CASE(BELOW_LEFT);
			MAKE_CASE(BELOW);
			MAKE_CASE(BELOW_RIGHT);
			MAKE_CASE(LEFT);
			MAKE_CASE(RIGHT);
			MAKE_CASE(ABOVE_LEFT);
			MAKE_CASE(ABOVE);
			MAKE_CASE(ABOVE_RIGHT);
			MAKE_CASE(DOUBLE_BELOW);
			MAKE_CASE(DOUBLE_ABOVE);
			MAKE_CASE(IOTA_SUBSCRIPT);
			MAKE_CASE(INVALID);

		#undef MAKE_CASE

			default:
				ss << "<invalid>";
				break;

			}
		}

		ss << " (" << (int)value << ")";

		return ss.str();
	}

	::testing::AssertionResult CompareStream(
		const char* expressionExpected GTEST_ATTRIBUTE_UNUSED_, const char* expressionActual GTEST_ATTRIBUTE_UNUSED_,
		const StreamEntry& entryExpected, const StreamEntry& entryActual)
	{
		if (entryActual.codepoint == entryExpected.codepoint &&
			entryActual.quick_check == entryExpected.quick_check &&
			entryActual.canonical_combining_class == entryExpected.canonical_combining_class)
		{
			return ::testing::AssertionSuccess();
		}
		else
		{
			::testing::AssertionResult result = ::testing::AssertionFailure();

			result << "Unexpected entry in stream at index " << (int)entryActual.index << std::endl;

			if (entryActual.codepoint != entryExpected.codepoint)
			{
				result << "[Codepoint]" << std::endl;
				result << "  Actual:  " << identifiable(entryActual.codepoint) << " \"" << printable(entryActual.codepoint) << "\"" << std::endl;
				result << "Expected:  " << identifiable(entryExpected.codepoint) << " \"" << printable(entryExpected.codepoint) << "\"" << std::endl;
			}
			else
			{
				result << "[Codepoint]  " << identifiable(entryActual.codepoint) << " \"" << printable(entryActual.codepoint) << "\"" << std::endl;
			}

			if (entryActual.quick_check != entryExpected.quick_check)
			{
				result << "[QuickCheck]" << std::endl;
				result << "  Actual:  " << quickCheckToString(entryActual.quick_check) << std::endl;
				result << "Expected:  " << quickCheckToString(entryExpected.quick_check) << std::endl;
			}
			else
			{
				result << "[QuickCheck]  " << quickCheckToString(entryActual.quick_check) << std::endl;
			}

			if (entryActual.canonical_combining_class != entryExpected.canonical_combining_class)
			{
				result << "[CanonicalCombiningClass]" << std::endl;
				result << "  Actual:  " << canonicalCombiningClassToString(entryActual.canonical_combining_class) << std::endl;
				result << "Expected:  " << canonicalCombiningClassToString(entryExpected.canonical_combining_class);
			}
			else
			{
				result << "[CanonicalCombiningClass]  " << canonicalCombiningClassToString(entryActual.canonical_combining_class);
			}

			return result;
		}
	}

};