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