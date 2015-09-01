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

TEST_F(NaughtyStrings, TwoByteCharacters)
{
	EXPECT_STREQ(L"\x7530\x4E2D\x3055\x3093\x306B\x3042\x3052\x3066\x4E0B\x3055\x3044", helpers::wide(ReadSection(1678, 33)).c_str());
	EXPECT_STREQ(L"\x30D1\x30FC\x30C6\x30A3\x30FC\x3078\x884C\x304B\x306A\x3044\x304B", helpers::wide(ReadSection(1712, 33)).c_str());
	EXPECT_STREQ(L"\x548C\x88FD\x6F22\x8A9E", helpers::wide(ReadSection(1746, 12)).c_str());
	EXPECT_STREQ(L"\x90E8\x843D\x683C", helpers::wide(ReadSection(1759, 9)).c_str());
	EXPECT_STREQ(L"\xC0AC\xD68C\xACFC\xD559\xC6D0 \xC5B4\xD559\xC5F0\xAD6C\xC18C", helpers::wide(ReadSection(1769, 31)).c_str());
	EXPECT_STREQ(L"\xCC26\xCC28\xB97C \xD0C0\xACE0 \xC628 \xD3B2\xC2DC\xB9E8\xACFC \xC45B\xB2E4\xB9AC \xB620\xBC29\xAC01\xD558", helpers::wide(ReadSection(1801, 56)).c_str());
	EXPECT_STREQ(L"\x793E\x6703\x79D1\x5B78\x9662\x8A9E\x5B78\x7814\x7A76\x6240", helpers::wide(ReadSection(1858, 30)).c_str());
	EXPECT_STREQ(L"\xC6B8\xB780\xBC14\xD1A0\xB974", helpers::wide(ReadSection(1889, 15)).c_str());
	EXPECT_STREQ(L"\xD841\xDF0E\xD841\xDF31\xD841\xDF79\xD843\xDC53\xD843\xDC78\xD843\xDC96\xD843\xDCCF", helpers::wide(ReadSection(1905, 28)).c_str());
}

TEST_F(NaughtyStrings, JapaneseEmoticons)
{
	EXPECT_STREQ(L"\x30FD\x0F3C\x0E88\x0644\x035C\x0E88\x0F3D\xFF89 \x30FD\x0F3C\x0E88\x0644\x035C\x0E88\x0F3D\xFF89 ", helpers::wide(ReadSection(2041, 46)).c_str());
	EXPECT_STREQ(L"(\xFF61\x25D5 \x2200 \x25D5\xFF61)", helpers::wide(ReadSection(2088, 19)).c_str());
	EXPECT_STREQ(L"\xFF40\xFF68(\x00B4\x2200\xFF40\x2229", helpers::wide(ReadSection(2108, 18)).c_str());
	EXPECT_STREQ(L"__\xFF9B(,_,*)", helpers::wide(ReadSection(2127, 11)).c_str());
	EXPECT_STREQ(L"\x30FB(\xFFE3\x2200\xFFE3)\x30FB:*:", helpers::wide(ReadSection(2139, 20)).c_str());
	EXPECT_STREQ(L"\xFF9F\xFF65\x273F\x30FE\x2572(\xFF61\x25D5\x203F\x25D5\xFF61)\x2571\x273F\xFF65\xFF9F", helpers::wide(ReadSection(2160, 44)).c_str());
	EXPECT_STREQ(L",\x3002\x30FB:*:\x30FB\x309C\x2019( \x263B \x03C9 \x263B )\x3002\x30FB:*:\x30FB\x309C\x2019", helpers::wide(ReadSection(2205, 51)).c_str());
	EXPECT_STREQ(L"(\x256F\x00B0\x25A1\x00B0\xFF09\x256F\xFE35 \x253B\x2501\x253B)  ", helpers::wide(ReadSection(2257, 33)).c_str());
	EXPECT_STREQ(L"(\xFF89\x0CA5\x76CA\x0CA5\xFF09\xFF89\xFEFF \x253B\x2501\x253B", helpers::wide(ReadSection(2291, 32)).c_str());
}

TEST_F(NaughtyStrings, Emoji)
{
#if UTF8_WCHAR_UTF16
	EXPECT_STREQ(L"\xD83D\xDE0D", helpers::wide(ReadSection(2434, 4)).c_str());
	EXPECT_STREQ(L"\xD83D\xDC69\xD83C\xDFFD", helpers::wide(ReadSection(2439, 8)).c_str());
	EXPECT_STREQ(L"\xD83D\xDC7E \xD83D\xDE47\x0020\xD83D\xDC81\x0020\xD83D\xDE45\x0020\xD83D\xDE46\x0020\xD83D\xDE4B\x0020\xD83D\xDE4E\x0020\xD83D\xDE4D\x0020", helpers::wide(ReadSection(2448, 40)).c_str());
	EXPECT_STREQ(L"\xD83D\xDC35\x0020\xD83D\xDE48\x0020\xD83D\xDE49\x0020\xD83D\xDE4A", helpers::wide(ReadSection(2489, 19)).c_str());
	EXPECT_STREQ(L"\x2764\xFE0F\x0020\xD83D\xDC94\x0020\xD83D\xDC8C\x0020\xD83D\xDC95\x0020\xD83D\xDC9E\x0020\xD83D\xDC93\x0020\xD83D\xDC97\x0020\xD83D\xDC96\x0020\xD83D\xDC98\x0020\xD83D\xDC9D\x0020\xD83D\xDC9F\x0020\xD83D\xDC9C\x0020\xD83D\xDC9B\x0020\xD83D\xDC9A\x0020\xD83D\xDC99", helpers::wide(ReadSection(2509, 76)).c_str());
	EXPECT_STREQ(L"\x270B\xD83C\xDFFF\x0020\xD83D\xDCAA\xD83C\xDFFF\x0020\xD83D\xDC50\xD83C\xDFFF\x0020\xD83D\xDE4C\xD83C\xDFFF\x0020\xD83D\xDC4F\xD83C\xDFFF\x0020\xD83D\xDE4F\xD83C\xDFFF", helpers::wide(ReadSection(2586, 52)).c_str());
	EXPECT_STREQ(L"\xD83D\xDEBE\x0020\xD83C\xDD92\x0020\xD83C\xDD93\x0020\xD83C\xDD95\x0020\xD83C\xDD96\x0020\xD83C\xDD97\x0020\xD83C\xDD99\x0020\xD83C\xDFE7", helpers::wide(ReadSection(2639, 39)).c_str());
	EXPECT_STREQ(L"\x0030\xFE0F\x20E3\x0020\x0031\xFE0F\x20E3\x0020\x0032\xFE0F\x20E3\x0020\x0033\xFE0F\x20E3\x0020\x0034\xFE0F\x20E3\x0020\x0035\xFE0F\x20E3\x0020\x0036\xFE0F\x20E3\x0020\x0037\xFE0F\x20E3\x0020\x0038\xFE0F\x20E3\x0020\x0039\xFE0F\x20E3\x0020\xD83D\xDD1F", helpers::wide(ReadSection(2679, 84)).c_str());
#elif UTF8_WCHAR_UTF32
	EXPECT_STREQ(L"\x1F60D", helpers::wide(ReadSection(2434, 4)).c_str());
	EXPECT_STREQ(L"\x1F469\x1F3FD", helpers::wide(ReadSection(2439, 8)).c_str());
	EXPECT_STREQ(L"\x1F47E\x0020\x1F647\x0020\x1F481\x0020\x1F645\x0020\x1F646\x0020\x1F64B\x0020\x1F64E\x0020\x1F64D\0020", helpers::wide(ReadSection(2448, 40)).c_str());
	EXPECT_STREQ(L"\x1F435\x0020\x1F648\x0020\x1F649\x0020\x1F64A", helpers::wide(ReadSection(2489, 19)).c_str());
	EXPECT_STREQ(L"\x2764\x0FE0F\x0020\x1F494 \x1F48C\x0020\x1F495 \x1F49E \x1F493 \x1F497 \x1F496 \x1F498 \x1F49D \x1F49F \x1F49C \x1F49B \x1F49A \x1F499", helpers::wide(ReadSection(2509, 76)).c_str());
	EXPECT_STREQ(L"\x0270B\x1F3FF \x1F4AA\x1F3FF \x1F450\x1F3FF \x1F64C\x1F3FF \x1F44F\x1F3FF \x1F64F\x1F3FF", helpers::wide(ReadSection(2586, 52)).c_str());
	EXPECT_STREQ(L"\x1F6BE \x1F192 \x1F193 \x1F195 \x1F196 \x1F197 \x1F199 \x1F3E7", helpers::wide(ReadSection(2639, 39)).c_str());
	EXPECT_STREQ(L"\x30\x0FE0F\x020E3 \x31\x0FE0F\x020E3 \x32\x0FE0F\x020E3 \x33\x0FE0F\x020E3 \x34\x0FE0F\x020E3 \x35\x0FE0F\x020E3 \x36\x0FE0F\x020E3 \x37\x0FE0F\x020E3 \x38\x0FE0F\x020E3 \x39\x0FE0F\x020E3 \x1F51F", helpers::wide(ReadSection(2679, 84)).c_str());
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
	EXPECT_STREQ(L"\x5D4\x5B8\x5D9\x5B0\x5EA\x5B8\x5D4\x74\x65\x73\x74\x627\x644\x635\x641\x62D\x627\x62A \x627\x644\x62A\x651\x62D\x648\x644", helpers::wide(ReadSection(3471, 47)).c_str());
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
	EXPECT_STREQ(L"\x202A\x202Atest\x202A", helpers::wide(ReadSection(3859, 13)).c_str());
	EXPECT_STREQ(L"\x202Btest\x202B", helpers::wide(ReadSection(3873, 10)).c_str());
	EXPECT_STREQ(L"\x2029test\x2029", helpers::wide(ReadSection(3884, 10)).c_str());
	EXPECT_STREQ(L"test\x2060test\x202B", helpers::wide(ReadSection(3895, 14)).c_str());
	EXPECT_STREQ(L"\x2066test\x2067", helpers::wide(ReadSection(3910, 10)).c_str());
}

TEST_F(NaughtyStrings, ZalgoText)
{
	EXPECT_STREQ(L"\x54\x315\x32D\x33A\x33A\x6F\x35E \x337\x69\x332\x32C\x347\x32A\x359\x6E\x31D\x317\x355\x76\x35F\x31F\x31C\x318\x326\x6F\x336\x319\x330\x320\x6B\x65\x340\x35A\x32E\x33A\x32A\x339\x331\x324 \x316\x74\x35E\x31D\x355\x333\x323\x33B\x32A\x68\x33C\x353\x332\x326\x333\x318\x332\x65\x347\x323\x330\x326\x32C\x34E \x322\x33C\x33B\x331\x318\x68\x345\x35A\x34E\x359\x31C\x323\x332\x69\x326\x332\x323\x330\x324\x76\x33B\x34D\x65\x33A\x32D\x333\x32A\x330\x2D\x6D\x322\x69\x345\x6E\x316\x33A\x31E\x332\x32F\x330\x64\x335\x33C\x31F\x359\x329\x33C\x318\x333 \x31E\x325\x331\x333\x32D\x72\x31B\x317\x318\x65\x359\x70\x360\x72\x33C\x31E\x33B\x32D\x317\x65\x35F\x33A\x320\x323\x73\x318\x347\x333\x34D\x31D\x349\x65\x35C\x349\x325\x32F\x31E\x332\x35A\x32C\x6E\x340\x32C\x34E\x34E\x31F\x316\x347\x324\x74\x358\x34D\x32C\x345\x324\x353\x33C\x32D\x69\x32A\x331\x6E\x360\x67\x334\x349 \x34F\x349\x345\x63\x32C\x31F\x68\x361\x61\x358\x32B\x33B\x32F\x6F\x32B\x31F\x316\x34D\x319\x31D\x349\x73\x317\x326\x332\x2E\x328\x339\x348\x323", helpers::wide(ReadSection(4065, 383)).c_str());
	EXPECT_STREQ(L"\x321\x353\x345\x31E\x49\x35D\x317\x318\x326\x6E\x347\x347\x359\x76\x32E\x32B\x6F\x6B\x332\x32B\x319\x348\x69\x316\x359\x32D\x339\x320\x31E\x6E\x321\x33B\x32E\x323\x33A\x67\x332\x348\x359\x32D\x359\x32C\x34E \x330\x74\x354\x326\x68\x31E\x332\x65\x322\x324 \x34D\x32C\x332\x356\x66\x334\x318\x355\x323\x65\x340\x356\x65\x323\x325\x329\x6C\x356\x354\x35A\x69\x360\x353\x35A\x326\x6E\x356\x34D\x317\x353\x333\x32E\x67\x34D \x328\x6F\x361\x35A\x32A\x66\x318\x323\x32C \x316\x318\x356\x31F\x359\x32E\x63\x489\x354\x32B\x356\x353\x347\x356\x345\x68\x335\x324\x323\x35A\x354\x61\x341\x317\x33C\x345\x355\x6F\x33C\x323\x325\x73\x362\x331\x348\x33A\x316\x326\x33B\x2E\x31B\x316\x31E\x320\x32B\x330", helpers::wide(ReadSection(4449, 262)).c_str());
	EXPECT_STREQ(L"\x317\x33A\x356\x339\x32F\x353\x54\x331\x324\x34D\x325\x347\x348\x68\x341\x332\x65\x34F\x353\x33C\x317\x319\x33C\x323\x354 \x347\x31C\x331\x320\x353\x34D\x345\x4E\x360\x355\x65\x317\x331\x7A\x318\x31D\x31C\x33A\x359\x70\x324\x33A\x339\x34D\x32F\x35A\x65\x35C\x320\x33B\x320\x72\x328\x324\x34D\x33A\x316\x354\x316\x316\x64\x35F\x320\x31F\x32D\x32C\x31D\x69\x326\x356\x329\x353\x354\x324\x61\x320\x317\x32C\x349\x319\x6E\x35C\x35A \x33B\x31E\x330\x35A\x345\x68\x335\x349\x69\x333\x31E\x76\x322\x347\x65\x35F\x32D\x34E\x2D\x489\x32D\x329\x33C\x354\x6D\x324\x32D\x32B\x69\x355\x347\x31D\x326\x6E\x317\x359\x64\x323\x31F \x35E\x32F\x332\x355\x6F\x328\x31F\x32F\x330\x332\x359\x33B\x31D\x66 \x358\x32A\x330\x330\x317\x316\x32D\x318\x63\x326\x34D\x332\x31E\x34D\x329\x319\x68\x323\x35A\x61\x35C\x32E\x34E\x31F\x319\x6F\x31B\x329\x339\x34E\x73\x324\x2E\x31D\x31D \x489\x5A\x321\x316\x31C\x356\x330\x323\x349\x31C\x61\x361\x356\x330\x359\x32C\x6C\x332\x32B\x333\x34D\x329\x67\x321\x31F\x33C\x331\x35A\x31E\x32C\x345\x6F\x35C\x317\x2E\x31F", helpers::wide(ReadSection(4712, 393)).c_str());
	EXPECT_STREQ(L"\x326\x48\x35D\x32C\x324\x317\x324\x65\x35C \x341\x31C\x325\x31D\x33B\x34D\x31F\x77\x315\x68\x316\x32F\x353\x6F\x31D\x359\x316\x34E\x331\x32E \x489\x33A\x319\x31E\x31F\x348\x57\x337\x33C\x32D\x61\x33A\x32A\x34D\x69\x328\x348\x355\x32D\x359\x32F\x31C\x74\x336\x33C\x32E\x73\x315\x318\x359\x356 \x320\x32B\x320\x42\x33B\x34D\x345\x359\x349\x333\x65\x335\x68\x335\x32C\x347\x32B\x359\x69\x315\x339\x353\x333\x333\x32E\x34E\x32B\x6E\x35F\x64\x334\x32A\x31C\x316 \x35E\x330\x349\x329\x345\x347\x359\x332\x54\x362\x356\x33C\x353\x32A\x68\x34F\x353\x32E\x33B\x65\x32C\x31D\x345\x31F \x324\x339\x31D\x57\x35D\x345\x359\x31E\x31D\x354\x347\x61\x34F\x353\x354\x339\x33C\x323\x6C\x334\x354\x330\x324\x31F\x354\x6C\x32D\x32B\x2E\x355", helpers::wide(ReadSection(5106, 277)).c_str());
	EXPECT_STREQ(L"\x5A\x32E\x31E\x345\x320\x359\x354\x41\x325\x317\x31E\x348\x33B\x317\x4C\x323\x359\x34E\x32F\x339\x31E\x353\x47\x33B\x4F\x32D\x317\x32E", helpers::wide(ReadSection(5384, 51)).c_str());
}

TEST_F(NaughtyStrings, UnicodeUpsidedown)
{
	EXPECT_STREQ(L"\x2D9\x250\x6E\x62\x1D09\x6C\x250 \x250\x75\x183\x250\x26F \x1DD\x279\x6F\x6C\x6F\x70 \x287\x1DD \x1DD\x279\x6F\x71\x250\x6C \x287\x6E \x287\x75\x6E\x70\x1D09\x70\x1D09\x254\x75\x1D09 \x279\x6F\x64\x26F\x1DD\x287 \x70\x6F\x26F\x73\x6E\x1D09\x1DD \x6F\x70 \x70\x1DD\x73 \x27\x287\x1D09\x6C\x1DD \x183\x75\x1D09\x254\x73\x1D09\x64\x1D09\x70\x250 \x279\x6E\x287\x1DD\x287\x254\x1DD\x73\x75\x6F\x254 \x27\x287\x1DD\x26F\x250 \x287\x1D09\x73 \x279\x6F\x6C\x6F\x70 \x26F\x6E\x73\x64\x1D09 \x26F\x1DD\x279\x6F\x2E5", helpers::wide(ReadSection(5557, 192)).c_str());
	EXPECT_STREQ(L"\x30\x30\x2D9\x196\x24\x2D", helpers::wide(ReadSection(5750, 8)).c_str());
}

TEST_F(NaughtyStrings, UnicodeFont)
{
	EXPECT_STREQ(L"\xFF34\xFF48\xFF45 \xFF51\xFF55\xFF49\xFF43\xFF4B \xFF42\xFF52\xFF4F\xFF57\xFF4E \xFF46\xFF4F\xFF58 \xFF4A\xFF55\xFF4D\xFF50\xFF53 \xFF4F\xFF56\xFF45\xFF52 \xFF54\xFF48\xFF45 \xFF4C\xFF41\xFF5A\xFF59 \xFF44\xFF4F\xFF47", helpers::wide(ReadSection(5849, 113)).c_str());
	EXPECT_STREQ(L"\xD835\xDC13\xD835\xDC21\xD835\xDC1E \xD835\xDC2A\xD835\xDC2E\xD835\xDC22\xD835\xDC1C\xD835\xDC24 \xD835\xDC1B\xD835\xDC2B\xD835\xDC28\xD835\xDC30\xD835\xDC27 \xD835\xDC1F\xD835\xDC28\xD835\xDC31 \xD835\xDC23\xD835\xDC2E\xD835\xDC26\xD835\xDC29\xD835\xDC2C \xD835\xDC28\xD835\xDC2F\xD835\xDC1E\xD835\xDC2B \xD835\xDC2D\xD835\xDC21\xD835\xDC1E \xD835\xDC25\xD835\xDC1A\xD835\xDC33\xD835\xDC32 \xD835\xDC1D\xD835\xDC28\xD835\xDC20", helpers::wide(ReadSection(5963, 148)).c_str());
	EXPECT_STREQ(L"\xD835\xDD7F\xD835\xDD8D\xD835\xDD8A \xD835\xDD96\xD835\xDD9A\xD835\xDD8E\xD835\xDD88\xD835\xDD90 \xD835\xDD87\xD835\xDD97\xD835\xDD94\xD835\xDD9C\xD835\xDD93 \xD835\xDD8B\xD835\xDD94\xD835\xDD9D \xD835\xDD8F\xD835\xDD9A\xD835\xDD92\xD835\xDD95\xD835\xDD98 \xD835\xDD94\xD835\xDD9B\xD835\xDD8A\xD835\xDD97 \xD835\xDD99\xD835\xDD8D\xD835\xDD8A \xD835\xDD91\xD835\xDD86\xD835\xDD9F\xD835\xDD9E \xD835\xDD89\xD835\xDD94\xD835\xDD8C", helpers::wide(ReadSection(6112, 148)).c_str());
	EXPECT_STREQ(L"\xD835\xDC7B\xD835\xDC89\xD835\xDC86 \xD835\xDC92\xD835\xDC96\xD835\xDC8A\xD835\xDC84\xD835\xDC8C \xD835\xDC83\xD835\xDC93\xD835\xDC90\xD835\xDC98\xD835\xDC8F \xD835\xDC87\xD835\xDC90\xD835\xDC99 \xD835\xDC8B\xD835\xDC96\xD835\xDC8E\xD835\xDC91\xD835\xDC94 \xD835\xDC90\xD835\xDC97\xD835\xDC86\xD835\xDC93 \xD835\xDC95\xD835\xDC89\xD835\xDC86 \xD835\xDC8D\xD835\xDC82\xD835\xDC9B\xD835\xDC9A \xD835\xDC85\xD835\xDC90\xD835\xDC88", helpers::wide(ReadSection(6261, 148)).c_str());
	EXPECT_STREQ(L"\xD835\xDCE3\xD835\xDCF1\xD835\xDCEE \xD835\xDCFA\xD835\xDCFE\xD835\xDCF2\xD835\xDCEC\xD835\xDCF4 \xD835\xDCEB\xD835\xDCFB\xD835\xDCF8\xD835\xDD00\xD835\xDCF7 \xD835\xDCEF\xD835\xDCF8\xD835\xDD01 \xD835\xDCF3\xD835\xDCFE\xD835\xDCF6\xD835\xDCF9\xD835\xDCFC \xD835\xDCF8\xD835\xDCFF\xD835\xDCEE\xD835\xDCFB \xD835\xDCFD\xD835\xDCF1\xD835\xDCEE \xD835\xDCF5\xD835\xDCEA\xD835\xDD03\xD835\xDD02 \xD835\xDCED\xD835\xDCF8\xD835\xDCF0", helpers::wide(ReadSection(6410, 148)).c_str());
	EXPECT_STREQ(L"\xD835\xDD4B\xD835\xDD59\xD835\xDD56 \xD835\xDD62\xD835\xDD66\xD835\xDD5A\xD835\xDD54\xD835\xDD5C \xD835\xDD53\xD835\xDD63\xD835\xDD60\xD835\xDD68\xD835\xDD5F \xD835\xDD57\xD835\xDD60\xD835\xDD69 \xD835\xDD5B\xD835\xDD66\xD835\xDD5E\xD835\xDD61\xD835\xDD64 \xD835\xDD60\xD835\xDD67\xD835\xDD56\xD835\xDD63 \xD835\xDD65\xD835\xDD59\xD835\xDD56 \xD835\xDD5D\xD835\xDD52\xD835\xDD6B\xD835\xDD6A \xD835\xDD55\xD835\xDD60\xD835\xDD58", helpers::wide(ReadSection(6559, 148)).c_str());
	EXPECT_STREQ(L"\xD835\xDE83\xD835\xDE91\xD835\xDE8E \xD835\xDE9A\xD835\xDE9E\xD835\xDE92\xD835\xDE8C\xD835\xDE94 \xD835\xDE8B\xD835\xDE9B\xD835\xDE98\xD835\xDEA0\xD835\xDE97 \xD835\xDE8F\xD835\xDE98\xD835\xDEA1 \xD835\xDE93\xD835\xDE9E\xD835\xDE96\xD835\xDE99\xD835\xDE9C \xD835\xDE98\xD835\xDE9F\xD835\xDE8E\xD835\xDE9B \xD835\xDE9D\xD835\xDE91\xD835\xDE8E \xD835\xDE95\xD835\xDE8A\xD835\xDEA3\xD835\xDEA2 \xD835\xDE8D\xD835\xDE98\xD835\xDE90", helpers::wide(ReadSection(6708, 148)).c_str());
	EXPECT_STREQ(L"\x24AF\x24A3\x24A0 \x24AC\x24B0\x24A4\x249E\x24A6 \x249D\x24AD\x24AA\x24B2\x24A9 \x24A1\x24AA\x24B3 \x24A5\x24B0\x24A8\x24AB\x24AE \x24AA\x24B1\x24A0\x24AD \x24AF\x24A3\x24A0 \x24A7\x249C\x24B5\x24B4 \x249F\x24AA\x24A2", helpers::wide(ReadSection(6857, 113)).c_str());
}