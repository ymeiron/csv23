#include <sstream>
#include <gtest/gtest.h>

#include "csv.h"

TEST(Cast, Double) {
    double result;
    std::string string {"1.2345"};
    csv::detail::cast(string, result);
    EXPECT_EQ(result, 1.2345);

    char cstyle[] = "5.4321";
    csv::detail::cast(cstyle, result);
    EXPECT_EQ(result, 5.4321);

    std::string bad_string {"9.876abcd"};
    EXPECT_ANY_THROW(
        csv::detail::cast(bad_string, result)
    );
}

TEST(Cast, Int) {
    double result;
    std::string string {"123"};
    csv::detail::cast(string, result);
    EXPECT_EQ(result, 123);

    std::string bad_string {"abcd"};
    EXPECT_ANY_THROW(
        csv::detail::cast(bad_string, result)
    );
}

TEST(StringToTuple, WithSpaces) {
    std::string string {"  test, 123 ,1.23   "};
    std::tuple<std::string,int,double> result, expected {"test", 123, 1.23};
    csv::detail::string_to_tuple(string, result);
    EXPECT_EQ(result, expected);
}

TEST(StringToTuple, CustomDelim) {
    std::string string {"test|123|1.23"};
    std::tuple<std::string,int,double> result, expected {"test", 123, 1.23};
    csv::detail::string_to_tuple(string, result, "|");
    EXPECT_EQ(result, expected);
}

TEST(Parse, FromFile) {
    auto data {csv::parse<std::string,double,double,double,double>("iris.csv")};
    EXPECT_EQ(data.size(), 150);
    constexpr size_t line_num = 99;
    EXPECT_EQ(std::get<0>(data[line_num]), "setosa");
    EXPECT_EQ(std::get<1>(data[line_num]), 5.1);
    EXPECT_EQ(std::get<2>(data[line_num]), 3.5);
    EXPECT_EQ(std::get<3>(data[line_num]), 1.4);
    EXPECT_EQ(std::get<4>(data[line_num]), 0.3);
}

TEST(Parse, BadFile) {
    EXPECT_ANY_THROW(
        csv::parse<int>("abcdefg.blah")
    );
}

TEST(Parse, FromStringStream) {
    std::stringstream ss {"hello,1\nbye,2"};
    auto data {csv::parse<std::string,int>(ss)};
    EXPECT_EQ(std::get<0>(data[0]), "hello");
    EXPECT_EQ(std::get<1>(data[0]), 1);
    EXPECT_EQ(std::get<0>(data[1]), "bye");
    EXPECT_EQ(std::get<1>(data[1]), 2);
}
