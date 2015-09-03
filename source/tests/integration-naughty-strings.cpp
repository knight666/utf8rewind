/*
	DO NOT MODIFY, AUTO-GENERATED

	Generated on:
		2015-09-03T09:58:40

	Command line:
		E:\Projects\utf8rewind\tools\converter\blns.py
*/

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
	EXPECT_STREQ(L"`\x2044\x20AC\x2039\x203A\xFB01\xFB02\x2021\xB0\xB7\x201A\x2014\xB1", helpers::wide(ReadSection(1033, 34)).c_str());
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

TEST_F(NaughtyStrings, QuotationMarks)
{
	EXPECT_STREQ(L"'", helpers::wide(ReadSection(1519, 1)).c_str());
	EXPECT_STREQ(L"\"", helpers::wide(ReadSection(1521, 1)).c_str());
	EXPECT_STREQ(L"''", helpers::wide(ReadSection(1523, 2)).c_str());
	EXPECT_STREQ(L"\"\"", helpers::wide(ReadSection(1526, 2)).c_str());
	EXPECT_STREQ(L"'\"'", helpers::wide(ReadSection(1529, 3)).c_str());
	EXPECT_STREQ(L"\"''''\"'\"", helpers::wide(ReadSection(1533, 8)).c_str());
	EXPECT_STREQ(L"\"'\"'\"''''\"", helpers::wide(ReadSection(1542, 10)).c_str());
}

TEST_F(NaughtyStrings, TwoByteCharacters)
{
#if UTF8_WCHAR_UTF16
	EXPECT_STREQ(L"\x7530\x4E2D\x3055\x3093\x306B\x3042\x3052\x3066\x4E0B\x3055\x3044", helpers::wide(ReadSection(1678, 33)).c_str());
	EXPECT_STREQ(L"\x30D1\x30FC\x30C6\x30A3\x30FC\x3078\x884C\x304B\x306A\x3044\x304B", helpers::wide(ReadSection(1712, 33)).c_str());
	EXPECT_STREQ(L"\x548C\x88FD\x6F22\x8A9E", helpers::wide(ReadSection(1746, 12)).c_str());
	EXPECT_STREQ(L"\x90E8\x843D\x683C", helpers::wide(ReadSection(1759, 9)).c_str());
	EXPECT_STREQ(L"\xC0AC\xD68C\xACFC\xD559\xC6D0 \xC5B4\xD559\xC5F0\xAD6C\xC18C", helpers::wide(ReadSection(1769, 31)).c_str());
	EXPECT_STREQ(L"\xCC26\xCC28\xB97C \xD0C0\xACE0 \xC628 \xD3B2\xC2DC\xB9E8\xACFC \xC45B\xB2E4\xB9AC \xB620\xBC29\xAC01\xD558", helpers::wide(ReadSection(1801, 56)).c_str());
	EXPECT_STREQ(L"\x793E\x6703\x79D1\x5B78\x9662\x8A9E\x5B78\x7814\x7A76\x6240", helpers::wide(ReadSection(1858, 30)).c_str());
	EXPECT_STREQ(L"\xC6B8\xB780\xBC14\xD1A0\xB974", helpers::wide(ReadSection(1889, 15)).c_str());
	EXPECT_STREQ(L"\xD841\xDF0E\xD841\xDF31\xD841\xDF79\xD843\xDC53\xD843\xDC78\xD843\xDC96\xD843\xDCCF", helpers::wide(ReadSection(1905, 28)).c_str());
#elif UTF8_WCHAR_UTF32
	EXPECT_STREQ(L"\x7530\x4E2D\x3055\x3093\x306B\x3042\x3052\x3066\x4E0B\x3055\x3044", helpers::wide(ReadSection(1678, 33)).c_str());
	EXPECT_STREQ(L"\x30D1\x30FC\x30C6\x30A3\x30FC\x3078\x884C\x304B\x306A\x3044\x304B", helpers::wide(ReadSection(1712, 33)).c_str());
	EXPECT_STREQ(L"\x548C\x88FD\x6F22\x8A9E", helpers::wide(ReadSection(1746, 12)).c_str());
	EXPECT_STREQ(L"\x90E8\x843D\x683C", helpers::wide(ReadSection(1759, 9)).c_str());
	EXPECT_STREQ(L"\xC0AC\xD68C\xACFC\xD559\xC6D0 \xC5B4\xD559\xC5F0\xAD6C\xC18C", helpers::wide(ReadSection(1769, 31)).c_str());
	EXPECT_STREQ(L"\xCC26\xCC28\xB97C \xD0C0\xACE0 \xC628 \xD3B2\xC2DC\xB9E8\xACFC \xC45B\xB2E4\xB9AC \xB620\xBC29\xAC01\xD558", helpers::wide(ReadSection(1801, 56)).c_str());
	EXPECT_STREQ(L"\x793E\x6703\x79D1\x5B78\x9662\x8A9E\x5B78\x7814\x7A76\x6240", helpers::wide(ReadSection(1858, 30)).c_str());
	EXPECT_STREQ(L"\xC6B8\xB780\xBC14\xD1A0\xB974", helpers::wide(ReadSection(1889, 15)).c_str());
	EXPECT_STREQ(L"\x2070E\x20731\x20779\x20C53\x20C78\x20C96\x20CCF", helpers::wide(ReadSection(1905, 28)).c_str());
#endif
}

TEST_F(NaughtyStrings, JapaneseEmoticons)
{
	EXPECT_STREQ(L"\x30FD\xF3C\xE88\x644\x35C\xE88\xF3D\xFF89 \x30FD\xF3C\xE88\x644\x35C\xE88\xF3D\xFF89 ", helpers::wide(ReadSection(2041, 46)).c_str());
	EXPECT_STREQ(L"(\xFF61\x25D5 \x2200 \x25D5\xFF61)", helpers::wide(ReadSection(2088, 19)).c_str());
	EXPECT_STREQ(L"\xFF40\xFF68(\xB4\x2200\xFF40\x2229", helpers::wide(ReadSection(2108, 18)).c_str());
	EXPECT_STREQ(L"__\xFF9B(,_,*)", helpers::wide(ReadSection(2127, 11)).c_str());
	EXPECT_STREQ(L"\x30FB(\xFFE3\x2200\xFFE3)\x30FB:*:", helpers::wide(ReadSection(2139, 20)).c_str());
	EXPECT_STREQ(L"\xFF9F\xFF65\x273F\x30FE\x2572(\xFF61\x25D5\x203F\x25D5\xFF61)\x2571\x273F\xFF65\xFF9F", helpers::wide(ReadSection(2160, 44)).c_str());
	EXPECT_STREQ(L",\x3002\x30FB:*:\x30FB\x309C\x2019( \x263B \x3C9 \x263B )\x3002\x30FB:*:\x30FB\x309C\x2019", helpers::wide(ReadSection(2205, 51)).c_str());
	EXPECT_STREQ(L"(\x256F\xB0\x25A1\xB0\xFF09\x256F\xFE35 \x253B\x2501\x253B)  ", helpers::wide(ReadSection(2257, 33)).c_str());
	EXPECT_STREQ(L"(\xFF89\xCA5\x76CA\xCA5\xFF09\xFF89\xFEFF \x253B\x2501\x253B", helpers::wide(ReadSection(2291, 32)).c_str());
}

TEST_F(NaughtyStrings, Emoji)
{
#if UTF8_WCHAR_UTF16
	EXPECT_STREQ(L"\xD83D\xDE0D", helpers::wide(ReadSection(2434, 4)).c_str());
	EXPECT_STREQ(L"\xD83D\xDC69\xD83C\xDFFD", helpers::wide(ReadSection(2439, 8)).c_str());
	EXPECT_STREQ(L"\xD83D\xDC7E \xD83D\xDE47 \xD83D\xDC81 \xD83D\xDE45 \xD83D\xDE46 \xD83D\xDE4B \xD83D\xDE4E \xD83D\xDE4D ", helpers::wide(ReadSection(2448, 40)).c_str());
	EXPECT_STREQ(L"\xD83D\xDC35 \xD83D\xDE48 \xD83D\xDE49 \xD83D\xDE4A", helpers::wide(ReadSection(2489, 19)).c_str());
	EXPECT_STREQ(L"\x2764\xFE0F \xD83D\xDC94 \xD83D\xDC8C \xD83D\xDC95 \xD83D\xDC9E \xD83D\xDC93 \xD83D\xDC97 \xD83D\xDC96 \xD83D\xDC98 \xD83D\xDC9D \xD83D\xDC9F \xD83D\xDC9C \xD83D\xDC9B \xD83D\xDC9A \xD83D\xDC99", helpers::wide(ReadSection(2509, 76)).c_str());
	EXPECT_STREQ(L"\x270B\xD83C\xDFFF \xD83D\xDCAA\xD83C\xDFFF \xD83D\xDC50\xD83C\xDFFF \xD83D\xDE4C\xD83C\xDFFF \xD83D\xDC4F\xD83C\xDFFF \xD83D\xDE4F\xD83C\xDFFF", helpers::wide(ReadSection(2586, 52)).c_str());
	EXPECT_STREQ(L"\xD83D\xDEBE \xD83C\xDD92 \xD83C\xDD93 \xD83C\xDD95 \xD83C\xDD96 \xD83C\xDD97 \xD83C\xDD99 \xD83C\xDFE7", helpers::wide(ReadSection(2639, 39)).c_str());
	EXPECT_STREQ(L"0\xFE0F\x20E3 1\xFE0F\x20E3 2\xFE0F\x20E3 3\xFE0F\x20E3 4\xFE0F\x20E3 5\xFE0F\x20E3 6\xFE0F\x20E3 7\xFE0F\x20E3 8\xFE0F\x20E3 9\xFE0F\x20E3 \xD83D\xDD1F", helpers::wide(ReadSection(2679, 84)).c_str());
#elif UTF8_WCHAR_UTF32
	EXPECT_STREQ(L"\x1F60D", helpers::wide(ReadSection(2434, 4)).c_str());
	EXPECT_STREQ(L"\x1F469\x1F3FD", helpers::wide(ReadSection(2439, 8)).c_str());
	EXPECT_STREQ(L"\x1F47E \x1F647 \x1F481 \x1F645 \x1F646 \x1F64B \x1F64E \x1F64D ", helpers::wide(ReadSection(2448, 40)).c_str());
	EXPECT_STREQ(L"\x1F435 \x1F648 \x1F649 \x1F64A", helpers::wide(ReadSection(2489, 19)).c_str());
	EXPECT_STREQ(L"\x2764\xFE0F \x1F494 \x1F48C \x1F495 \x1F49E \x1F493 \x1F497 \x1F496 \x1F498 \x1F49D \x1F49F \x1F49C \x1F49B \x1F49A \x1F499", helpers::wide(ReadSection(2509, 76)).c_str());
	EXPECT_STREQ(L"\x270B\x1F3FF \x1F4AA\x1F3FF \x1F450\x1F3FF \x1F64C\x1F3FF \x1F44F\x1F3FF \x1F64F\x1F3FF", helpers::wide(ReadSection(2586, 52)).c_str());
	EXPECT_STREQ(L"\x1F6BE \x1F192 \x1F193 \x1F195 \x1F196 \x1F197 \x1F199 \x1F3E7", helpers::wide(ReadSection(2639, 39)).c_str());
	EXPECT_STREQ(L"0\xFE0F\x20E3 1\xFE0F\x20E3 2\xFE0F\x20E3 3\xFE0F\x20E3 4\xFE0F\x20E3 5\xFE0F\x20E3 6\xFE0F\x20E3 7\xFE0F\x20E3 8\xFE0F\x20E3 9\xFE0F\x20E3 \x1F51F", helpers::wide(ReadSection(2679, 84)).c_str());
#endif
}

TEST_F(NaughtyStrings, UnicodeNumbers)
{
	EXPECT_STREQ(L"\xFF11\xFF12\xFF13", helpers::wide(ReadSection(2888, 9)).c_str());
	EXPECT_STREQ(L"\x661\x662\x663", helpers::wide(ReadSection(2898, 6)).c_str());
}

TEST_F(NaughtyStrings, RightToLeftStrings)
{
	EXPECT_STREQ(L"\x62B\x645 \x646\x641\x633 \x633\x642\x637\x62A \x648\x628\x627\x644\x62A\x62D\x62F\x64A\x62F\x60C, \x62C\x632\x64A\x631\x62A\x64A \x628\x627\x633\x62A\x62E\x62F\x627\x645 \x623\x646 \x62F\x646\x648. \x625\x630 \x647\x646\x627\x61F \x627\x644\x633\x62A\x627\x631 \x648\x62A\x646\x635\x64A\x628 \x643\x627\x646. \x623\x647\x651\x644 \x627\x64A\x637\x627\x644\x64A\x627\x60C \x628\x631\x64A\x637\x627\x646\x64A\x627-\x641\x631\x646\x633\x627 \x642\x62F \x623\x62E\x630. \x633\x644\x64A\x645\x627\x646\x60C \x625\x62A\x641\x627\x642\x64A\x629 \x628\x64A\x646 \x645\x627, \x64A\x630\x643\x631 \x627\x644\x62D\x62F\x648\x62F \x623\x64A \x628\x639\x62F, \x645\x639\x627\x645\x644\x629 \x628\x648\x644\x646\x62F\x627\x60C \x627\x644\x625\x637\x644\x627\x642 \x639\x644 \x625\x64A\x648.", helpers::wide(ReadSection(3024, 334)).c_str());
	EXPECT_STREQ(L"\x5D1\x5BC\x5B0\x5E8\x5B5\x5D0\x5E9\x5C1\x5B4\x5D9\x5EA, \x5D1\x5BC\x5B8\x5E8\x5B8\x5D0 \x5D0\x5B1\x5DC\x5B9\x5D4\x5B4\x5D9\x5DD, \x5D0\x5B5\x5EA \x5D4\x5B7\x5E9\x5BC\x5C1\x5B8\x5DE\x5B7\x5D9\x5B4\x5DD, \x5D5\x5B0\x5D0\x5B5\x5EA \x5D4\x5B8\x5D0\x5B8\x5E8\x5B6\x5E5", helpers::wide(ReadSection(3359, 111)).c_str());
	EXPECT_STREQ(L"\x5D4\x5B8\x5D9\x5B0\x5EA\x5B8\x5D4" L"test\x627\x644\x635\x641\x62D\x627\x62A \x627\x644\x62A\x651\x62D\x648\x644", helpers::wide(ReadSection(3471, 47)).c_str());
}

TEST_F(NaughtyStrings, UnicodeSpaces)
{
	EXPECT_STREQ(L"\x200B", helpers::wide(ReadSection(3670, 3)).c_str());
	EXPECT_STREQ(L"\x1680", helpers::wide(ReadSection(3674, 3)).c_str());
	EXPECT_STREQ(L"\x180E", helpers::wide(ReadSection(3678, 3)).c_str());
	EXPECT_STREQ(L"\x3000", helpers::wide(ReadSection(3682, 3)).c_str());
	EXPECT_STREQ(L"\xFEFF", helpers::wide(ReadSection(3686, 3)).c_str());
	EXPECT_STREQ(L"\x2423", helpers::wide(ReadSection(3690, 3)).c_str());
	EXPECT_STREQ(L"\x2422", helpers::wide(ReadSection(3694, 3)).c_str());
	EXPECT_STREQ(L"\x2421", helpers::wide(ReadSection(3698, 3)).c_str());
}

TEST_F(NaughtyStrings, TrickUnicode)
{
	EXPECT_STREQ(L"\x202A\x202A" L"test\x202A", helpers::wide(ReadSection(3859, 13)).c_str());
	EXPECT_STREQ(L"\x202B" L"test\x202B", helpers::wide(ReadSection(3873, 10)).c_str());
	EXPECT_STREQ(L"\x2029" L"test\x2029", helpers::wide(ReadSection(3884, 10)).c_str());
	EXPECT_STREQ(L"test\x2060" L"test\x202B", helpers::wide(ReadSection(3895, 14)).c_str());
	EXPECT_STREQ(L"\x2066" L"test\x2067", helpers::wide(ReadSection(3910, 10)).c_str());
}

TEST_F(NaughtyStrings, ZalgoText)
{
	EXPECT_STREQ(L"T\x315\x32D\x33A\x33A" L"o\x35E \x337" L"i\x332\x32C\x347\x32A\x359" L"n\x31D\x317\x355" L"v\x35F\x31F\x31C\x318\x326" L"o\x336\x319\x330\x320" L"ke\x340\x35A\x32E\x33A\x32A\x339\x331\x324 \x316" L"t\x35E\x31D\x355\x333\x323\x33B\x32A" L"h\x33C\x353\x332\x326\x333\x318\x332" L"e\x347\x323\x330\x326\x32C\x34E \x322\x33C\x33B\x331\x318" L"h\x345\x35A\x34E\x359\x31C\x323\x332" L"i\x326\x332\x323\x330\x324" L"v\x33B\x34D" L"e\x33A\x32D\x333\x32A\x330-m\x322" L"i\x345" L"n\x316\x33A\x31E\x332\x32F\x330" L"d\x335\x33C\x31F\x359\x329\x33C\x318\x333 \x31E\x325\x331\x333\x32D" L"r\x31B\x317\x318" L"e\x359" L"p\x360" L"r\x33C\x31E\x33B\x32D\x317" L"e\x35F\x33A\x320\x323" L"s\x318\x347\x333\x34D\x31D\x349" L"e\x35C\x349\x325\x32F\x31E\x332\x35A\x32C" L"n\x340\x32C\x34E\x34E\x31F\x316\x347\x324" L"t\x358\x34D\x32C\x345\x324\x353\x33C\x32D" L"i\x32A\x331" L"n\x360" L"g\x334\x349 \x34F\x349\x345" L"c\x32C\x31F" L"h\x361" L"a\x358\x32B\x33B\x32F" L"o\x32B\x31F\x316\x34D\x319\x31D\x349" L"s\x317\x326\x332.\x328\x339\x348\x323", helpers::wide(ReadSection(4065, 383)).c_str());
	EXPECT_STREQ(L"\x321\x353\x345\x31EI\x35D\x317\x318\x326" L"n\x347\x347\x359" L"v\x32E\x32B" L"ok\x332\x32B\x319\x348" L"i\x316\x359\x32D\x339\x320\x31E" L"n\x321\x33B\x32E\x323\x33A" L"g\x332\x348\x359\x32D\x359\x32C\x34E \x330" L"t\x354\x326" L"h\x31E\x332" L"e\x322\x324 \x34D\x32C\x332\x356" L"f\x334\x318\x355\x323" L"e\x340\x356" L"e\x323\x325\x329" L"l\x356\x354\x35A" L"i\x360\x353\x35A\x326" L"n\x356\x34D\x317\x353\x333\x32E" L"g\x34D \x328" L"o\x361\x35A\x32A" L"f\x318\x323\x32C \x316\x318\x356\x31F\x359\x32E" L"c\x489\x354\x32B\x356\x353\x347\x356\x345" L"h\x335\x324\x323\x35A\x354" L"a\x341\x317\x33C\x345\x355" L"o\x33C\x323\x325" L"s\x362\x331\x348\x33A\x316\x326\x33B.\x31B\x316\x31E\x320\x32B\x330", helpers::wide(ReadSection(4449, 262)).c_str());
	EXPECT_STREQ(L"\x317\x33A\x356\x339\x32F\x353T\x331\x324\x34D\x325\x347\x348" L"h\x341\x332" L"e\x34F\x353\x33C\x317\x319\x33C\x323\x354 \x347\x31C\x331\x320\x353\x34D\x345N\x360\x355" L"e\x317\x331z\x318\x31D\x31C\x33A\x359" L"p\x324\x33A\x339\x34D\x32F\x35A" L"e\x35C\x320\x33B\x320" L"r\x328\x324\x34D\x33A\x316\x354\x316\x316" L"d\x35F\x320\x31F\x32D\x32C\x31D" L"i\x326\x356\x329\x353\x354\x324" L"a\x320\x317\x32C\x349\x319" L"n\x35C\x35A \x33B\x31E\x330\x35A\x345" L"h\x335\x349" L"i\x333\x31E" L"v\x322\x347" L"e\x35F\x32D\x34E-\x489\x32D\x329\x33C\x354" L"m\x324\x32D\x32B" L"i\x355\x347\x31D\x326" L"n\x317\x359" L"d\x323\x31F \x35E\x32F\x332\x355" L"o\x328\x31F\x32F\x330\x332\x359\x33B\x31D" L"f \x358\x32A\x330\x330\x317\x316\x32D\x318" L"c\x326\x34D\x332\x31E\x34D\x329\x319" L"h\x323\x35A" L"a\x35C\x32E\x34E\x31F\x319" L"o\x31B\x329\x339\x34E" L"s\x324.\x31D\x31D \x489Z\x321\x316\x31C\x356\x330\x323\x349\x31C" L"a\x361\x356\x330\x359\x32C" L"l\x332\x32B\x333\x34D\x329" L"g\x321\x31F\x33C\x331\x35A\x31E\x32C\x345" L"o\x35C\x317.\x31F", helpers::wide(ReadSection(4712, 393)).c_str());
	EXPECT_STREQ(L"\x326H\x35D\x32C\x324\x317\x324" L"e\x35C \x341\x31C\x325\x31D\x33B\x34D\x31Fw\x315" L"h\x316\x32F\x353" L"o\x31D\x359\x316\x34E\x331\x32E \x489\x33A\x319\x31E\x31F\x348W\x337\x33C\x32D" L"a\x33A\x32A\x34D" L"i\x328\x348\x355\x32D\x359\x32F\x31C" L"t\x336\x33C\x32E" L"s\x315\x318\x359\x356 \x320\x32B\x320" L"B\x33B\x34D\x345\x359\x349\x333" L"e\x335" L"h\x335\x32C\x347\x32B\x359" L"i\x315\x339\x353\x333\x333\x32E\x34E\x32B" L"n\x35F" L"d\x334\x32A\x31C\x316 \x35E\x330\x349\x329\x345\x347\x359\x332T\x362\x356\x33C\x353\x32A" L"h\x34F\x353\x32E\x33B" L"e\x32C\x31D\x345\x31F \x324\x339\x31DW\x35D\x345\x359\x31E\x31D\x354\x347" L"a\x34F\x353\x354\x339\x33C\x323" L"l\x334\x354\x330\x324\x31F\x354" L"l\x32D\x32B.\x355", helpers::wide(ReadSection(5106, 277)).c_str());
	EXPECT_STREQ(L"Z\x32E\x31E\x345\x320\x359\x354" L"A\x325\x317\x31E\x348\x33B\x317L\x323\x359\x34E\x32F\x339\x31E\x353G\x33BO\x32D\x317\x32E", helpers::wide(ReadSection(5384, 51)).c_str());
}

TEST_F(NaughtyStrings, UnicodeUpsidedown)
{
	EXPECT_STREQ(L"\x2D9\x250" L"nb\x1D09" L"l\x250 \x250" L"u\x183\x250\x26F \x1DD\x279" L"olop \x287\x1DD \x1DD\x279" L"oq\x250" L"l \x287" L"n \x287" L"unp\x1D09" L"p\x1D09\x254" L"u\x1D09 \x279" L"od\x26F\x1DD\x287 po\x26F" L"sn\x1D09\x1DD op p\x1DD" L"s '\x287\x1D09" L"l\x1DD \x183" L"u\x1D09\x254" L"s\x1D09" L"d\x1D09" L"p\x250 \x279" L"n\x287\x1DD\x287\x254\x1DD" L"suo\x254 '\x287\x1DD\x26F\x250 \x287\x1D09" L"s \x279" L"olop \x26F" L"nsd\x1D09 \x26F\x1DD\x279" L"o\x2E5", helpers::wide(ReadSection(5557, 192)).c_str());
	EXPECT_STREQ(L"00\x2D9\x196$-", helpers::wide(ReadSection(5750, 8)).c_str());
}

TEST_F(NaughtyStrings, UnicodeFont)
{
#if UTF8_WCHAR_UTF16
	EXPECT_STREQ(L"\xFF34\xFF48\xFF45 \xFF51\xFF55\xFF49\xFF43\xFF4B \xFF42\xFF52\xFF4F\xFF57\xFF4E \xFF46\xFF4F\xFF58 \xFF4A\xFF55\xFF4D\xFF50\xFF53 \xFF4F\xFF56\xFF45\xFF52 \xFF54\xFF48\xFF45 \xFF4C\xFF41\xFF5A\xFF59 \xFF44\xFF4F\xFF47", helpers::wide(ReadSection(5849, 113)).c_str());
	EXPECT_STREQ(L"\xD835\xDC13\xD835\xDC21\xD835\xDC1E \xD835\xDC2A\xD835\xDC2E\xD835\xDC22\xD835\xDC1C\xD835\xDC24 \xD835\xDC1B\xD835\xDC2B\xD835\xDC28\xD835\xDC30\xD835\xDC27 \xD835\xDC1F\xD835\xDC28\xD835\xDC31 \xD835\xDC23\xD835\xDC2E\xD835\xDC26\xD835\xDC29\xD835\xDC2C \xD835\xDC28\xD835\xDC2F\xD835\xDC1E\xD835\xDC2B \xD835\xDC2D\xD835\xDC21\xD835\xDC1E \xD835\xDC25\xD835\xDC1A\xD835\xDC33\xD835\xDC32 \xD835\xDC1D\xD835\xDC28\xD835\xDC20", helpers::wide(ReadSection(5963, 148)).c_str());
	EXPECT_STREQ(L"\xD835\xDD7F\xD835\xDD8D\xD835\xDD8A \xD835\xDD96\xD835\xDD9A\xD835\xDD8E\xD835\xDD88\xD835\xDD90 \xD835\xDD87\xD835\xDD97\xD835\xDD94\xD835\xDD9C\xD835\xDD93 \xD835\xDD8B\xD835\xDD94\xD835\xDD9D \xD835\xDD8F\xD835\xDD9A\xD835\xDD92\xD835\xDD95\xD835\xDD98 \xD835\xDD94\xD835\xDD9B\xD835\xDD8A\xD835\xDD97 \xD835\xDD99\xD835\xDD8D\xD835\xDD8A \xD835\xDD91\xD835\xDD86\xD835\xDD9F\xD835\xDD9E \xD835\xDD89\xD835\xDD94\xD835\xDD8C", helpers::wide(ReadSection(6112, 148)).c_str());
	EXPECT_STREQ(L"\xD835\xDC7B\xD835\xDC89\xD835\xDC86 \xD835\xDC92\xD835\xDC96\xD835\xDC8A\xD835\xDC84\xD835\xDC8C \xD835\xDC83\xD835\xDC93\xD835\xDC90\xD835\xDC98\xD835\xDC8F \xD835\xDC87\xD835\xDC90\xD835\xDC99 \xD835\xDC8B\xD835\xDC96\xD835\xDC8E\xD835\xDC91\xD835\xDC94 \xD835\xDC90\xD835\xDC97\xD835\xDC86\xD835\xDC93 \xD835\xDC95\xD835\xDC89\xD835\xDC86 \xD835\xDC8D\xD835\xDC82\xD835\xDC9B\xD835\xDC9A \xD835\xDC85\xD835\xDC90\xD835\xDC88", helpers::wide(ReadSection(6261, 148)).c_str());
	EXPECT_STREQ(L"\xD835\xDCE3\xD835\xDCF1\xD835\xDCEE \xD835\xDCFA\xD835\xDCFE\xD835\xDCF2\xD835\xDCEC\xD835\xDCF4 \xD835\xDCEB\xD835\xDCFB\xD835\xDCF8\xD835\xDD00\xD835\xDCF7 \xD835\xDCEF\xD835\xDCF8\xD835\xDD01 \xD835\xDCF3\xD835\xDCFE\xD835\xDCF6\xD835\xDCF9\xD835\xDCFC \xD835\xDCF8\xD835\xDCFF\xD835\xDCEE\xD835\xDCFB \xD835\xDCFD\xD835\xDCF1\xD835\xDCEE \xD835\xDCF5\xD835\xDCEA\xD835\xDD03\xD835\xDD02 \xD835\xDCED\xD835\xDCF8\xD835\xDCF0", helpers::wide(ReadSection(6410, 148)).c_str());
	EXPECT_STREQ(L"\xD835\xDD4B\xD835\xDD59\xD835\xDD56 \xD835\xDD62\xD835\xDD66\xD835\xDD5A\xD835\xDD54\xD835\xDD5C \xD835\xDD53\xD835\xDD63\xD835\xDD60\xD835\xDD68\xD835\xDD5F \xD835\xDD57\xD835\xDD60\xD835\xDD69 \xD835\xDD5B\xD835\xDD66\xD835\xDD5E\xD835\xDD61\xD835\xDD64 \xD835\xDD60\xD835\xDD67\xD835\xDD56\xD835\xDD63 \xD835\xDD65\xD835\xDD59\xD835\xDD56 \xD835\xDD5D\xD835\xDD52\xD835\xDD6B\xD835\xDD6A \xD835\xDD55\xD835\xDD60\xD835\xDD58", helpers::wide(ReadSection(6559, 148)).c_str());
	EXPECT_STREQ(L"\xD835\xDE83\xD835\xDE91\xD835\xDE8E \xD835\xDE9A\xD835\xDE9E\xD835\xDE92\xD835\xDE8C\xD835\xDE94 \xD835\xDE8B\xD835\xDE9B\xD835\xDE98\xD835\xDEA0\xD835\xDE97 \xD835\xDE8F\xD835\xDE98\xD835\xDEA1 \xD835\xDE93\xD835\xDE9E\xD835\xDE96\xD835\xDE99\xD835\xDE9C \xD835\xDE98\xD835\xDE9F\xD835\xDE8E\xD835\xDE9B \xD835\xDE9D\xD835\xDE91\xD835\xDE8E \xD835\xDE95\xD835\xDE8A\xD835\xDEA3\xD835\xDEA2 \xD835\xDE8D\xD835\xDE98\xD835\xDE90", helpers::wide(ReadSection(6708, 148)).c_str());
	EXPECT_STREQ(L"\x24AF\x24A3\x24A0 \x24AC\x24B0\x24A4\x249E\x24A6 \x249D\x24AD\x24AA\x24B2\x24A9 \x24A1\x24AA\x24B3 \x24A5\x24B0\x24A8\x24AB\x24AE \x24AA\x24B1\x24A0\x24AD \x24AF\x24A3\x24A0 \x24A7\x249C\x24B5\x24B4 \x249F\x24AA\x24A2", helpers::wide(ReadSection(6857, 113)).c_str());
#elif UTF8_WCHAR_UTF32
	EXPECT_STREQ(L"\xFF34\xFF48\xFF45 \xFF51\xFF55\xFF49\xFF43\xFF4B \xFF42\xFF52\xFF4F\xFF57\xFF4E \xFF46\xFF4F\xFF58 \xFF4A\xFF55\xFF4D\xFF50\xFF53 \xFF4F\xFF56\xFF45\xFF52 \xFF54\xFF48\xFF45 \xFF4C\xFF41\xFF5A\xFF59 \xFF44\xFF4F\xFF47", helpers::wide(ReadSection(5849, 113)).c_str());
	EXPECT_STREQ(L"\x1D413\x1D421\x1D41E \x1D42A\x1D42E\x1D422\x1D41C\x1D424 \x1D41B\x1D42B\x1D428\x1D430\x1D427 \x1D41F\x1D428\x1D431 \x1D423\x1D42E\x1D426\x1D429\x1D42C \x1D428\x1D42F\x1D41E\x1D42B \x1D42D\x1D421\x1D41E \x1D425\x1D41A\x1D433\x1D432 \x1D41D\x1D428\x1D420", helpers::wide(ReadSection(5963, 148)).c_str());
	EXPECT_STREQ(L"\x1D57F\x1D58D\x1D58A \x1D596\x1D59A\x1D58E\x1D588\x1D590 \x1D587\x1D597\x1D594\x1D59C\x1D593 \x1D58B\x1D594\x1D59D \x1D58F\x1D59A\x1D592\x1D595\x1D598 \x1D594\x1D59B\x1D58A\x1D597 \x1D599\x1D58D\x1D58A \x1D591\x1D586\x1D59F\x1D59E \x1D589\x1D594\x1D58C", helpers::wide(ReadSection(6112, 148)).c_str());
	EXPECT_STREQ(L"\x1D47B\x1D489\x1D486 \x1D492\x1D496\x1D48A\x1D484\x1D48C \x1D483\x1D493\x1D490\x1D498\x1D48F \x1D487\x1D490\x1D499 \x1D48B\x1D496\x1D48E\x1D491\x1D494 \x1D490\x1D497\x1D486\x1D493 \x1D495\x1D489\x1D486 \x1D48D\x1D482\x1D49B\x1D49A \x1D485\x1D490\x1D488", helpers::wide(ReadSection(6261, 148)).c_str());
	EXPECT_STREQ(L"\x1D4E3\x1D4F1\x1D4EE \x1D4FA\x1D4FE\x1D4F2\x1D4EC\x1D4F4 \x1D4EB\x1D4FB\x1D4F8\x1D500\x1D4F7 \x1D4EF\x1D4F8\x1D501 \x1D4F3\x1D4FE\x1D4F6\x1D4F9\x1D4FC \x1D4F8\x1D4FF\x1D4EE\x1D4FB \x1D4FD\x1D4F1\x1D4EE \x1D4F5\x1D4EA\x1D503\x1D502 \x1D4ED\x1D4F8\x1D4F0", helpers::wide(ReadSection(6410, 148)).c_str());
	EXPECT_STREQ(L"\x1D54B\x1D559\x1D556 \x1D562\x1D566\x1D55A\x1D554\x1D55C \x1D553\x1D563\x1D560\x1D568\x1D55F \x1D557\x1D560\x1D569 \x1D55B\x1D566\x1D55E\x1D561\x1D564 \x1D560\x1D567\x1D556\x1D563 \x1D565\x1D559\x1D556 \x1D55D\x1D552\x1D56B\x1D56A \x1D555\x1D560\x1D558", helpers::wide(ReadSection(6559, 148)).c_str());
	EXPECT_STREQ(L"\x1D683\x1D691\x1D68E \x1D69A\x1D69E\x1D692\x1D68C\x1D694 \x1D68B\x1D69B\x1D698\x1D6A0\x1D697 \x1D68F\x1D698\x1D6A1 \x1D693\x1D69E\x1D696\x1D699\x1D69C \x1D698\x1D69F\x1D68E\x1D69B \x1D69D\x1D691\x1D68E \x1D695\x1D68A\x1D6A3\x1D6A2 \x1D68D\x1D698\x1D690", helpers::wide(ReadSection(6708, 148)).c_str());
	EXPECT_STREQ(L"\x24AF\x24A3\x24A0 \x24AC\x24B0\x24A4\x249E\x24A6 \x249D\x24AD\x24AA\x24B2\x24A9 \x24A1\x24AA\x24B3 \x24A5\x24B0\x24A8\x24AB\x24AE \x24AA\x24B1\x24A0\x24AD \x24AF\x24A3\x24A0 \x24A7\x249C\x24B5\x24B4 \x249F\x24AA\x24A2", helpers::wide(ReadSection(6857, 113)).c_str());
#endif
}

TEST_F(NaughtyStrings, ScriptInjection)
{
	EXPECT_STREQ(L"<script>alert(123)</script>", helpers::wide(ReadSection(7082, 27)).c_str());
	EXPECT_STREQ(L"<img src=x onerror=alert(123) />", helpers::wide(ReadSection(7110, 32)).c_str());
	EXPECT_STREQ(L"<svg><script>123<1>alert(123)</script> ", helpers::wide(ReadSection(7143, 39)).c_str());
	EXPECT_STREQ(L"\"><script>alert(123)</script>", helpers::wide(ReadSection(7183, 29)).c_str());
	EXPECT_STREQ(L"'><script>alert(123)</script>", helpers::wide(ReadSection(7213, 29)).c_str());
	EXPECT_STREQ(L"><script>alert(123)</script>", helpers::wide(ReadSection(7243, 28)).c_str());
	EXPECT_STREQ(L"</script><script>alert(123)</script>", helpers::wide(ReadSection(7272, 36)).c_str());
	EXPECT_STREQ(L"< / script >< script >alert(123)< / script >", helpers::wide(ReadSection(7309, 44)).c_str());
	EXPECT_STREQ(L" onfocus=JaVaSCript:alert(123) autofocus ", helpers::wide(ReadSection(7354, 41)).c_str());
	EXPECT_STREQ(L"\" onfocus=JaVaSCript:alert(123) autofocus ", helpers::wide(ReadSection(7396, 42)).c_str());
	EXPECT_STREQ(L"' onfocus=JaVaSCript:alert(123) autofocus ", helpers::wide(ReadSection(7439, 42)).c_str());
	EXPECT_STREQ(L"\xFF1C" L"script\xFF1E" L"alert(123)\xFF1C/script\xFF1E", helpers::wide(ReadSection(7482, 35)).c_str());
	EXPECT_STREQ(L"<sc<script>ript>alert(123)</sc</script>ript>", helpers::wide(ReadSection(7518, 44)).c_str());
	EXPECT_STREQ(L"--><script>alert(123)</script>", helpers::wide(ReadSection(7563, 30)).c_str());
	EXPECT_STREQ(L"\";alert(123);t=\"", helpers::wide(ReadSection(7594, 16)).c_str());
	EXPECT_STREQ(L"';alert(123);t='", helpers::wide(ReadSection(7611, 16)).c_str());
	EXPECT_STREQ(L"JavaSCript:alert(123)", helpers::wide(ReadSection(7628, 21)).c_str());
	EXPECT_STREQ(L";alert(123);", helpers::wide(ReadSection(7650, 12)).c_str());
	EXPECT_STREQ(L"src=JaVaSCript:prompt(132)", helpers::wide(ReadSection(7663, 26)).c_str());
	EXPECT_STREQ(L"\"><script>alert(123);</script x=\"", helpers::wide(ReadSection(7690, 33)).c_str());
	EXPECT_STREQ(L"'><script>alert(123);</script x='", helpers::wide(ReadSection(7724, 33)).c_str());
	EXPECT_STREQ(L"><script>alert(123);</script x=", helpers::wide(ReadSection(7758, 31)).c_str());
	EXPECT_STREQ(L"\" autofocus onkeyup=\"javascript:alert(123)", helpers::wide(ReadSection(7790, 42)).c_str());
	EXPECT_STREQ(L"' autofocus onkeyup='javascript:alert(123)", helpers::wide(ReadSection(7833, 42)).c_str());
}

TEST_F(NaughtyStrings, SqlInjection)
{
	EXPECT_STREQ(L"1;DROP TABLE users", helpers::wide(ReadSection(7966, 18)).c_str());
	EXPECT_STREQ(L"1'; DROP TABLE users--", helpers::wide(ReadSection(7985, 22)).c_str());
}

TEST_F(NaughtyStrings, ServerCodeInjection)
{
	EXPECT_STREQ(L"-", helpers::wide(ReadSection(8162, 1)).c_str());
	EXPECT_STREQ(L"--", helpers::wide(ReadSection(8164, 2)).c_str());
	EXPECT_STREQ(L"--version", helpers::wide(ReadSection(8167, 9)).c_str());
	EXPECT_STREQ(L"--help", helpers::wide(ReadSection(8177, 6)).c_str());
	EXPECT_STREQ(L"$USER", helpers::wide(ReadSection(8184, 5)).c_str());
	EXPECT_STREQ(L"/dev/null; touch /tmp/blns.fail ; echo", helpers::wide(ReadSection(8190, 38)).c_str());
	EXPECT_STREQ(L"`touch /tmp/blns.fail`", helpers::wide(ReadSection(8229, 22)).c_str());
	EXPECT_STREQ(L"$(touch /tmp/blns.fail)", helpers::wide(ReadSection(8252, 23)).c_str());
	EXPECT_STREQ(L"@{[system \"touch /tmp/blns.fail\"]}", helpers::wide(ReadSection(8276, 34)).c_str());
}

TEST_F(NaughtyStrings, CommandInjectionRuby)
{
	EXPECT_STREQ(L"eval(\"puts 'hello world'\")", helpers::wide(ReadSection(8414, 26)).c_str());
	EXPECT_STREQ(L"System(\"ls -al /\")", helpers::wide(ReadSection(8441, 18)).c_str());
	EXPECT_STREQ(L"`ls -al /`", helpers::wide(ReadSection(8460, 10)).c_str());
	EXPECT_STREQ(L"Kernel.exec(\"ls -al /\")", helpers::wide(ReadSection(8471, 23)).c_str());
	EXPECT_STREQ(L"Kernel.exit(1)", helpers::wide(ReadSection(8495, 14)).c_str());
	EXPECT_STREQ(L"%x('ls -al /')", helpers::wide(ReadSection(8510, 14)).c_str());
}

TEST_F(NaughtyStrings, XxeInjectionXml)
{
	EXPECT_STREQ(L"<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?><!DOCTYPE foo [ <!ELEMENT foo ANY ><!ENTITY xxe SYSTEM \"file:///etc/passwd\" >]><foo>&xxe;</foo>", helpers::wide(ReadSection(8640, 138)).c_str());
}

TEST_F(NaughtyStrings, UnwantedInterpolation)
{
	EXPECT_STREQ(L"$HOME", helpers::wide(ReadSection(9086, 5)).c_str());
	EXPECT_STREQ(L"$ENV{'HOME'}", helpers::wide(ReadSection(9092, 12)).c_str());
	EXPECT_STREQ(L"%d", helpers::wide(ReadSection(9105, 2)).c_str());
	EXPECT_STREQ(L"%s", helpers::wide(ReadSection(9108, 2)).c_str());
	EXPECT_STREQ(L"%*.*s", helpers::wide(ReadSection(9111, 5)).c_str());
}

TEST_F(NaughtyStrings, FileInclusion)
{
	EXPECT_STREQ(L"../../../../../../../../../../../etc/passwd%00", helpers::wide(ReadSection(9228, 46)).c_str());
	EXPECT_STREQ(L"../../../../../../../../../../../etc/hosts", helpers::wide(ReadSection(9275, 42)).c_str());
}

TEST_F(NaughtyStrings, KnownCvesAndVulnerabilities)
{
	EXPECT_STREQ(L"() { 0; }; touch /tmp/blns.shellshock1.fail;", helpers::wide(ReadSection(9401, 44)).c_str());
	EXPECT_STREQ(L"() { _; } >_[$($())] { touch /tmp/blns.shellshock2.fail; }", helpers::wide(ReadSection(9446, 58)).c_str());
}

TEST_F(NaughtyStrings, MsdosWindowsSpecialFilenames)
{
	EXPECT_STREQ(L"CON", helpers::wide(ReadSection(9600, 3)).c_str());
	EXPECT_STREQ(L"PRN", helpers::wide(ReadSection(9604, 3)).c_str());
	EXPECT_STREQ(L"AUX", helpers::wide(ReadSection(9608, 3)).c_str());
	EXPECT_STREQ(L"CLOCK$", helpers::wide(ReadSection(9612, 6)).c_str());
	EXPECT_STREQ(L"NUL", helpers::wide(ReadSection(9619, 3)).c_str());
	EXPECT_STREQ(L"A:", helpers::wide(ReadSection(9623, 2)).c_str());
	EXPECT_STREQ(L"ZZ:", helpers::wide(ReadSection(9626, 3)).c_str());
	EXPECT_STREQ(L"COM1", helpers::wide(ReadSection(9630, 4)).c_str());
	EXPECT_STREQ(L"LPT1", helpers::wide(ReadSection(9635, 4)).c_str());
	EXPECT_STREQ(L"LPT2", helpers::wide(ReadSection(9640, 4)).c_str());
	EXPECT_STREQ(L"LPT3", helpers::wide(ReadSection(9645, 4)).c_str());
	EXPECT_STREQ(L"COM2", helpers::wide(ReadSection(9650, 4)).c_str());
	EXPECT_STREQ(L"COM3", helpers::wide(ReadSection(9655, 4)).c_str());
	EXPECT_STREQ(L"COM4", helpers::wide(ReadSection(9660, 4)).c_str());
}

TEST_F(NaughtyStrings, HumanInjection)
{
	EXPECT_STREQ(L"If you're reading this, you've been in a coma for almost 20 years now. We're trying a new technique. We don't know where this message will end up in your dream, but we hope it works. Please wake up, we miss you.", helpers::wide(ReadSection(9744, 211)).c_str());
}

TEST_F(NaughtyStrings, TerminalEscapeCodes)
{
	EXPECT_STREQ(L"Roses are \x1B[0;31mred\x1B[0m, violets are \x1B[0;34mblue. Hope you enjoy terminal hue", helpers::wide(ReadSection(10047, 78)).c_str());
	EXPECT_STREQ(L"But now...\x1B[20Cfor my greatest trick...\x1B[8m", helpers::wide(ReadSection(10126, 43)).c_str());
	EXPECT_STREQ(L"The quic\b\b\b\b\b\bk brown fo\a\a\a\a\a\a\a\a\a\a\ax... [Beeeep]", helpers::wide(ReadSection(10170, 48)).c_str());
}

TEST_F(NaughtyStrings, IosVulnerability)
{
	EXPECT_STREQ(L"Power\x644\x64F\x644\x64F\x635\x651\x628\x64F\x644\x64F\x644\x635\x651\x628\x64F\x631\x631\x64B \x963 \x963" L"h \x963 \x963\x5197", helpers::wide(ReadSection(10308, 61)).c_str());
}