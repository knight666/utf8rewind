#include "tests-base.hpp"

TEST(Utf8Sort, Same)
{
	const char* l = "amazing";
	size_t ls = strlen(l);
	const char* r = "amazing";
	size_t rs = strlen(r);
	int32_t errors = -4;

	EXPECT_EQ(0, utf8sort(l, ls, r, rs, UTF8_LOCALE_DEFAULT, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8Sort, Lesser)
{
	const char* l = "book";
	size_t ls = strlen(l);
	const char* r = "rack";
	size_t rs = strlen(r);
	int32_t errors = 1877;

	EXPECT_EQ(-1, utf8sort(l, ls, r, rs, UTF8_LOCALE_DEFAULT, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8Sort, Greater)
{
	const char* l = "medical";
	size_t ls = strlen(l);
	const char* r = "branch";
	size_t rs = strlen(r);
	int32_t errors = 33;

	EXPECT_EQ(1, utf8sort(l, ls, r, rs, UTF8_LOCALE_DEFAULT, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8Sort, Longer)
{
	const char* l = "bambi";
	size_t ls = strlen(l);
	const char* r = "bamb";
	size_t rs = strlen(r);
	int32_t errors = 3;

	EXPECT_EQ(1, utf8sort(l, ls, r, rs, UTF8_LOCALE_DEFAULT, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8Sort, Shorter)
{
	const char* l = "ha";
	size_t ls = strlen(l);
	const char* r = "ham";
	size_t rs = strlen(r);
	int32_t errors = 3;

	EXPECT_EQ(-1, utf8sort(l, ls, r, rs, UTF8_LOCALE_DEFAULT, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8Sort, CaseDifference)
{
	const char* l = "cave";
	size_t ls = strlen(l);
	const char* r = "CAVE";
	size_t rs = strlen(r);
	int32_t errors = 3;

	EXPECT_EQ(-1, utf8sort(l, ls, r, rs, UTF8_LOCALE_DEFAULT, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
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
		return utf8sort(left.c_str(), left.length(), right.c_str(), right.length(), UTF8_LOCALE_DEFAULT, nullptr) == -1;
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