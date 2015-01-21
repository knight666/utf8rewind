#include "tests-base.hpp"

extern "C" {
	#include "../internal/database.h"
	#include "../internal/composition.h"
}

#include "helpers-strings.hpp"

StreamState CreateStream(const char* text)
{
	StreamState stream = { 0 };

	std::vector<unicode_t> converted = helpers::utf32(text);

	for (std::vector<unicode_t>::iterator it = converted.begin(); it != converted.end(); ++it)
	{
		stream.codepoint[stream.current] = *it;
		stream.canonical_combining_class[stream.current] = database_queryproperty(*it, UnicodeProperty_CanonicalCombiningClass);
		stream.current++;
	}

	return stream;
}

TEST(StreamReorder, Unchanged)
{
	StreamState stream = CreateStream("Bike");

	EXPECT_EQ(1, stream_reorder(&stream));
	EXPECT_UTF8EQ("Bike", helpers::utf8(stream.codepoint, stream.current * sizeof(unicode_t)).c_str());
}