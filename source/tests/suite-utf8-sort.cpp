#include "tests-base.hpp"

TEST(Utf8Sort, Equal)
{
	const char* l = "amazing";
	size_t ls = strlen(l);
	const char* r = "amazing";
	size_t rs = strlen(r);

	EXPECT_EQ(UTF8_RANK_EQUAL, utf8sort(l, ls, r, rs, UTF8_LOCALE_DEFAULT));
}

TEST(Utf8Sort, Above)
{
	const char* l = "book";
	size_t ls = strlen(l);
	const char* r = "rack";
	size_t rs = strlen(r);

	EXPECT_EQ(UTF8_RANK_ABOVE, utf8sort(l, ls, r, rs, UTF8_LOCALE_DEFAULT));
}

TEST(Utf8Sort, Below)
{
	const char* l = "medical";
	size_t ls = strlen(l);
	const char* r = "branch";
	size_t rs = strlen(r);

	EXPECT_EQ(UTF8_RANK_BELOW, utf8sort(l, ls, r, rs, UTF8_LOCALE_DEFAULT));
}

TEST(Utf8Sort, Longer)
{
	const char* l = "bambi";
	size_t ls = strlen(l);
	const char* r = "bamb";
	size_t rs = strlen(r);

	EXPECT_EQ(UTF8_RANK_BELOW, utf8sort(l, ls, r, rs, UTF8_LOCALE_DEFAULT));
}

TEST(Utf8Sort, Shorter)
{
	const char* l = "ha";
	size_t ls = strlen(l);
	const char* r = "ham";
	size_t rs = strlen(r);

	EXPECT_EQ(UTF8_RANK_ABOVE, utf8sort(l, ls, r, rs, UTF8_LOCALE_DEFAULT));
}

TEST(Utf8Sort, CaseDifference)
{
	const char* l = "cave";
	size_t ls = strlen(l);
	const char* r = "CAVE";
	size_t rs = strlen(r);

	EXPECT_EQ(UTF8_RANK_ABOVE, utf8sort(l, ls, r, rs, UTF8_LOCALE_DEFAULT));
}

TEST(Utf8Sort, Dictionary)
{
	std::vector<std::string> dictionary = {
		"CAT",
		"CAVE",
		"Cat",
		"cAT",
		"cAt",
		"car",
		"caT",
		"cat"
	};
	std::sort(dictionary.begin(), dictionary.end(), [](const std::string& left, const std::string& right) {
		return utf8sort(left.c_str(), left.length(), right.c_str(), right.length(), UTF8_LOCALE_DEFAULT) == UTF8_RANK_ABOVE;
	});

	EXPECT_STREQ("car", dictionary[0].c_str());
	EXPECT_STREQ("cat", dictionary[1].c_str());
	EXPECT_STREQ("caT", dictionary[2].c_str());
	EXPECT_STREQ("cAt", dictionary[3].c_str());
	EXPECT_STREQ("cAT", dictionary[4].c_str());
	EXPECT_STREQ("Cat", dictionary[5].c_str());
	EXPECT_STREQ("CAT", dictionary[6].c_str());
	EXPECT_STREQ("CAVE", dictionary[7].c_str());
}