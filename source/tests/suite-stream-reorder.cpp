#include "tests-base.hpp"

extern "C" {
	#include "../internal/database.h"
	#include "../internal/composition.h"
}

#include "helpers-strings.hpp"

TEST(StreamReorder, Unchanged)
{
	const char* i = "Bike";
	size_t il = strlen(i);

	StreamState stream;
	EXPECT_EQ(1, stream_initialize(&stream, i, il, UnicodeProperty_Normalization_Compatibility_Compose));
	EXPECT_EQ(1, stream_execute(&stream));

	EXPECT_EQ(1, stream_reorder(&stream));
	EXPECT_UTF8EQ("Bike", helpers::utf8(stream.codepoint, stream.current * sizeof(unicode_t)).c_str());
}