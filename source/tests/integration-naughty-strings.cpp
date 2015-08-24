#include "tests-base.hpp"

#include "../helpers/helpers-strings.hpp"

#define NAUGHTY_STRINGS_LENGTH 10370

class NaughtyStrings
	: public ::testing::Test
{

protected:

	void SetUp()
	{
		file.open("testdata/big-list-of-naughty-strings-master/blns.txt", std::ios_base::in);
		ASSERT_TRUE(file.is_open());
	}

	void TearDown()
	{
		file.close();
	}

	std::string ReadSection(size_t position, size_t length)
	{
		std::string result;

		file.seekg(position, std::ios::beg);
		if (file.eof())
		{
			return result;
		}

		result.resize(length + 1);
		file.read(&result[0], length);

		return result;
	}

	std::fstream file;

};

TEST_F(NaughtyStrings, ReservedStrings)
{
	EXPECT_STREQ(L"undefined", helpers::wide(ReadSection(68, 9)).c_str());
	EXPECT_STREQ(L"undef", helpers::wide(ReadSection(78, 5)).c_str());
	EXPECT_STREQ(L"null", helpers::wide(ReadSection(84, 4)).c_str());
	EXPECT_STREQ(L"NULL", helpers::wide(ReadSection(89, 4)).c_str());
	EXPECT_STREQ(L"nil", helpers::wide(ReadSection(94, 3)).c_str());
	EXPECT_STREQ(L"NIL", helpers::wide(ReadSection(98, 3)).c_str());
	EXPECT_STREQ(L"true", helpers::wide(ReadSection(102, 4)).c_str());
	EXPECT_STREQ(L"false", helpers::wide(ReadSection(107, 5)).c_str());
	EXPECT_STREQ(L"True", helpers::wide(ReadSection(113, 4)).c_str());
	EXPECT_STREQ(L"False", helpers::wide(ReadSection(118, 5)).c_str());
	EXPECT_STREQ(L"None", helpers::wide(ReadSection(124, 4)).c_str());
	EXPECT_STREQ(L"\\", helpers::wide(ReadSection(129, 1)).c_str());
	EXPECT_STREQ(L"\\\\", helpers::wide(ReadSection(131, 2)).c_str());
}

TEST_F(NaughtyStrings, NumericStrings)
{
	EXPECT_STREQ(L"0", helpers::wide(ReadSection(202, 1)).c_str());
	EXPECT_STREQ(L"1", helpers::wide(ReadSection(204, 1)).c_str());
	EXPECT_STREQ(L"1.00", helpers::wide(ReadSection(206, 4)).c_str());
	EXPECT_STREQ(L"$1.00", helpers::wide(ReadSection(211, 5)).c_str());
	EXPECT_STREQ(L"1/2", helpers::wide(ReadSection(217, 3)).c_str());
	EXPECT_STREQ(L"1E2", helpers::wide(ReadSection(221, 3)).c_str());
	EXPECT_STREQ(L"1E02", helpers::wide(ReadSection(225, 4)).c_str());
	EXPECT_STREQ(L"1E+02", helpers::wide(ReadSection(230, 5)).c_str());
	EXPECT_STREQ(L"-1", helpers::wide(ReadSection(236, 2)).c_str());
	EXPECT_STREQ(L"-1.00", helpers::wide(ReadSection(239, 5)).c_str());
	EXPECT_STREQ(L"-$1.00", helpers::wide(ReadSection(245, 6)).c_str());
	EXPECT_STREQ(L"-1/2", helpers::wide(ReadSection(252, 4)).c_str());
	EXPECT_STREQ(L"-1E2", helpers::wide(ReadSection(257, 4)).c_str());
	EXPECT_STREQ(L"-1E02", helpers::wide(ReadSection(262, 5)).c_str());
	EXPECT_STREQ(L"-1E+02", helpers::wide(ReadSection(268, 6)).c_str());
	EXPECT_STREQ(L"1/0", helpers::wide(ReadSection(275, 3)).c_str());
	EXPECT_STREQ(L"0/0", helpers::wide(ReadSection(279, 3)).c_str());
	EXPECT_STREQ(L"-2147483648/-1", helpers::wide(ReadSection(283, 14)).c_str());
	EXPECT_STREQ(L"-9223372036854775808/-1", helpers::wide(ReadSection(298, 23)).c_str());
	EXPECT_STREQ(L"0.00", helpers::wide(ReadSection(322, 4)).c_str());
	EXPECT_STREQ(L"0..0", helpers::wide(ReadSection(327, 4)).c_str());
	EXPECT_STREQ(L".", helpers::wide(ReadSection(332, 1)).c_str());
	EXPECT_STREQ(L"0.0.0", helpers::wide(ReadSection(334, 5)).c_str());
	EXPECT_STREQ(L"0,00", helpers::wide(ReadSection(340, 4)).c_str());
	EXPECT_STREQ(L"0,,0", helpers::wide(ReadSection(345, 4)).c_str());
	EXPECT_STREQ(L",", helpers::wide(ReadSection(350, 1)).c_str());
	EXPECT_STREQ(L"0,0", helpers::wide(ReadSection(352, 3)).c_str());
	EXPECT_STREQ(L"0,0,0", helpers::wide(ReadSection(352, 5)).c_str());
	EXPECT_STREQ(L"0.0/0", helpers::wide(ReadSection(358, 5)).c_str());
	EXPECT_STREQ(L"1.0/0.0", helpers::wide(ReadSection(364, 7)).c_str());
	EXPECT_STREQ(L"0.0/0.0", helpers::wide(ReadSection(372, 7)).c_str());
	EXPECT_STREQ(L"1,0/0,0", helpers::wide(ReadSection(380, 7)).c_str());
	EXPECT_STREQ(L"0,0/0,0", helpers::wide(ReadSection(388, 7)).c_str());
	EXPECT_STREQ(L"--1", helpers::wide(ReadSection(396, 3)).c_str());
	EXPECT_STREQ(L"-", helpers::wide(ReadSection(400, 1)).c_str());
	EXPECT_STREQ(L"-.", helpers::wide(ReadSection(402, 2)).c_str());
	EXPECT_STREQ(L"-,", helpers::wide(ReadSection(405, 2)).c_str());
	EXPECT_STREQ(L"999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999", helpers::wide(ReadSection(408, 96)).c_str());
	EXPECT_STREQ(L"NaN", helpers::wide(ReadSection(505, 3)).c_str());
	EXPECT_STREQ(L"Infinity", helpers::wide(ReadSection(509, 8)).c_str());
	EXPECT_STREQ(L"-Infinity", helpers::wide(ReadSection(518, 9)).c_str());
	EXPECT_STREQ(L"0x0", helpers::wide(ReadSection(528, 3)).c_str());
	EXPECT_STREQ(L"0xffffffff", helpers::wide(ReadSection(532, 10)).c_str());
	EXPECT_STREQ(L"0xffffffffffffffff", helpers::wide(ReadSection(543, 18)).c_str());
	EXPECT_STREQ(L"0xabad1dea", helpers::wide(ReadSection(562, 10)).c_str());
	EXPECT_STREQ(L"123456789012345678901234567890123456789", helpers::wide(ReadSection(573, 39)).c_str());
}

TEST_F(NaughtyStrings, SpecialCharacters)
{
	EXPECT_STREQ(L",./;'[]\\-=", helpers::wide(ReadSection(719, 10)).c_str());
	EXPECT_STREQ(L"<>?:\"{}|_+", helpers::wide(ReadSection(730, 10)).c_str());
	EXPECT_STREQ(L"!@#$%^&*()`~", helpers::wide(ReadSection(741, 12)).c_str());
}

TEST_F(NaughtyStrings, UnicodeSymbols)
{
	EXPECT_STREQ(L"\x3A9\x2248\xE7\x221A\x222B\x2DC\xB5\x2264\x2265\xF7", helpers::wide(ReadSection(843, 25)).c_str());
	EXPECT_STREQ(L"\xE5\xDF\x2202\x192\xA9\x2D9\x2206\x2DA\xAC\x2026\xE6", helpers::wide(ReadSection(869, 25)).c_str());
	EXPECT_STREQ(L"\x153\x2211\xB4\xAE\x2020\xA5\xA8\x2C6\xF8\x3C0\x201C\x2018", helpers::wide(ReadSection(895, 28)).c_str());
	EXPECT_STREQ(L"\xA1\x2122\xA3\xA2\x221E\xA7\xB6\x2022\xAA\xBA\x2013\x2260", helpers::wide(ReadSection(924, 29)).c_str());
	EXPECT_STREQ(L"\xB8\x2DB\xC7\x25CA\x131\x2DC\xC2\xAF\x2D8\xBF", helpers::wide(ReadSection(954, 21)).c_str());
	EXPECT_STREQ(L"\xC5\xCD\xCE\xCF\x2DD\xD3\xD4\xF8FF\xD2\xDA\xC6\x2603", helpers::wide(ReadSection(976, 26)).c_str());
	EXPECT_STREQ(L"\x152\x201E\xB4\x2030\x2C7\xC1\xA8\x2C6\xD8\x220F\x201D\x2019", helpers::wide(ReadSection(1003, 29)).c_str());
	EXPECT_STREQ(L"\x60\x2044\x20AC\x2039\x203A\xFB01\xFB02\x2021\xB0\xB7\x201A\x2014\xB1", helpers::wide(ReadSection(1033, 34)).c_str());
	EXPECT_STREQ(L"\x215B\x215C\x215D\x215E", helpers::wide(ReadSection(1068, 12)).c_str());
	EXPECT_STREQ(L"\x401\x402\x403\x404\x405\x406\x407\x408\x409\x40A\x40B\x40C\x40D\x40E\x40F\x410\x411\x412\x413\x414\x415\x416\x417\x418\x419\x41A\x41B\x41C\x41D\x41E\x41F\x420\x421\x422\x423\x424\x425\x426\x427\x428\x429\x42A\x42B\x42C\x42D\x42E\x42F\x430\x431\x432\x433\x434\x435\x436\x437\x438\x439\x43A\x43B\x43C\x43D\x43E\x43F\x440\x441\x442\x443\x444\x445\x446\x447\x448\x449\x44A\x44B\x44C\x44D\x44E\x44F", helpers::wide(ReadSection(1081, 158)).c_str());
	EXPECT_STREQ(L"\x660\x661\x662\x663\x664\x665\x666\x667\x668\x669", helpers::wide(ReadSection(1240, 20)).c_str());
}

TEST_F(NaughtyStrings, UnicodeSubscriptSuperscript)
{
	EXPECT_STREQ(L"\x2070\x2074\x2075", helpers::wide(ReadSection(1381, 9)).c_str());
	EXPECT_STREQ(L"\x2080\x2081\x2082", helpers::wide(ReadSection(1391, 9)).c_str());
	EXPECT_STREQ(L"\x2070\x2074\x2075\x2080\x2081\x2082", helpers::wide(ReadSection(1401, 18)).c_str());
}