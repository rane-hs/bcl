//include gtest modules
#include <gtest/gtest.h>
#ifdef NDEBUG
//release
#pragma comment(lib, "gtest_main.lib")
#pragma comment(lib, "gtest.lib")
#else
//debug
#pragma comment(lib, "gtest_maind.lib")
#pragma comment(lib, "gtestd.lib")
#endif


//include bcl
#include <bcl/bclstr.h>
#include <bcl/ManagedFile.h>
#include <bcl/cast.h>
#include <bcl/FileSystem.h>

TEST(bclstr_test, format1)
{
	EXPECT_EQ(bcl::format(TEXT("%d,%4x,%s"), 5, 255, TEXT("abc")) , bcl::str_t(TEXT("5,  ff,abc")));
}
TEST(bclstr_test, replace1)
{
	bcl::str_t str1 = TEXT("abc1def1ghi");
	bcl::replace(str1, TEXT("1"), TEXT(","));
	EXPECT_EQ(str1 , bcl::str_t(TEXT("abc,def,ghi")));
	EXPECT_EQ(bcl::replace(TEXT("1,2,3,4,5"), TEXT(","), TEXT("|")) , bcl::str_t(TEXT("1|2|3|4|5")));
}
TEST(bclstr_test, narrow_widen)
{
	setlocale(LC_CTYPE, "");
	EXPECT_EQ(bcl::narrow(L"‚ ‚¢‚¤‚¦‚¨") , "‚ ‚¢‚¤‚¦‚¨");
	EXPECT_EQ(bcl::widen("‚ ‚¢‚¤‚¦‚¨") , L"‚ ‚¢‚¤‚¦‚¨");
}
TEST(bclstr_test, split1)
{
	bcl::basic_split<bcl::str_t>	sp(TEXT("1,2,3,4,5"), TEXT(","));
	EXPECT_EQ(sp.size(), 5);
	EXPECT_EQ(sp[0] , bcl::str_t(TEXT("1")));
	EXPECT_EQ(sp.c_at(3) , bcl::str_t(TEXT("4")));
	EXPECT_EQ(sp.c_at_s(4) , bcl::str_t(TEXT("5")));
	EXPECT_EQ(sp.c_at_s(5) , bcl::str_t(TEXT("")));
	sp.tokenize(TEXT("1/2,3/4,5"), TEXT("/"));
	EXPECT_EQ(sp.size(), 3);
	EXPECT_EQ(sp[1] , bcl::str_t(TEXT("2,3")));
}

TEST(bcl_time24, test1)
{
	bcl::time24 const checkTime(2010, 1, 2, 3, 4, 5);
	EXPECT_EQ(checkTime.year() , 2010);
	EXPECT_EQ(checkTime.month() , 1);
	EXPECT_EQ(checkTime.day() ,  2);
	EXPECT_EQ(checkTime.hour() , 3);
	EXPECT_EQ(checkTime.minute() , 4);
	EXPECT_EQ(checkTime.second() , 5);
	EXPECT_EQ(checkTime.year24() , 2010);
	EXPECT_EQ(checkTime.month24() , 1);
	EXPECT_EQ(checkTime.day24() ,  2);
	EXPECT_EQ(checkTime.hour24() , 3);
	EXPECT_EQ(checkTime.to_string24() , "2010/01/02 03:04:05");
	bcl::time24 checkTime2;
	EXPECT_EQ(checkTime2.is_valid(), false);
	checkTime2.from_string("2011/02/03 04:05:06");
	EXPECT_EQ(checkTime2.year() , 2011);
	EXPECT_EQ(checkTime2.month() , 2);
	EXPECT_EQ(checkTime2.day() ,  3);
	EXPECT_EQ(checkTime2.hour() , 4);
	EXPECT_EQ(checkTime2.minute() , 5);
	EXPECT_EQ(checkTime2.second() , 6);
	EXPECT_EQ(checkTime2.year24() , 2011);
	EXPECT_EQ(checkTime2.month24() , 2);
	EXPECT_EQ(checkTime2.day24() ,  3);
	EXPECT_EQ(checkTime2.hour24() , 4);
	EXPECT_EQ(checkTime2.is_valid(), true);

}
TEST(bcl_time24, test2)
{
	bcl::time24 const checkTime2(2010, 1, 1, 0, 0, 0);
	EXPECT_EQ(checkTime2.year() , 2010);
	EXPECT_EQ(checkTime2.month() , 1);
	EXPECT_EQ(checkTime2.day() ,  1);
	EXPECT_EQ(checkTime2.hour() , 0);
	EXPECT_EQ(checkTime2.minute() , 0);
	EXPECT_EQ(checkTime2.second() , 0);
	EXPECT_EQ(checkTime2.year24() , 2009);
	EXPECT_EQ(checkTime2.month24() , 12);
	EXPECT_EQ(checkTime2.day24() ,  31);
	EXPECT_EQ(checkTime2.hour24() , 24);
	EXPECT_EQ(checkTime2.to_string24() , "2009/12/31 24:00:00");
}
TEST(bcl_filesystem, test1)
{
	bcl::FilePath file_path1(TEXT("c:\\dir1\\dir2\\fname.ext"));
	EXPECT_EQ(file_path1.Drv(), bcl::str_t(TEXT("c:")));
	EXPECT_EQ(file_path1.Dir(), bcl::str_t(TEXT("\\dir1\\dir2\\")));
	EXPECT_EQ(file_path1.Fname(), bcl::str_t(TEXT("fname")));
	EXPECT_EQ(file_path1.Ext(), bcl::str_t(TEXT(".ext")));
	EXPECT_EQ(file_path1.DirPath(), bcl::str_t(TEXT("c:\\dir1\\dir2\\")));
}

TEST(bcl_cast, test1)
{
	EXPECT_EQ(bcl::hex_to_int("Ff"), 255);
	EXPECT_EQ(bcl::hex_char_to_int('5'), 5);
	EXPECT_EQ(bcl::hex_char_to_int('A'), char(10));
	EXPECT_EQ(bcl::hex_char_to_int('c'), char(12));
	EXPECT_EQ(bcl::to_string(255), "255");
	EXPECT_EQ(bcl::lexical_cast<bool>("1"), true);
	EXPECT_EQ(bcl::lexical_cast<bool>("2500"), true);
	EXPECT_EQ(bcl::lexical_cast<bool>("0"), false);
	EXPECT_EQ(bcl::lexical_cast<char>("25"), 25);
	EXPECT_EQ(bcl::lexical_cast<int>("-100"), -100);
	EXPECT_EQ(bcl::lexical_cast<long>("10000"), 10000);
	EXPECT_EQ(bcl::to_float("100.56"), 100.56);
}
#ifdef _UNICODE
int _tmain(int argc, wchar_t* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#else
int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#endif

