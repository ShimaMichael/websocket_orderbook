#include <gtest/gtest.h>

#include <string>

#include "../src/Utilities/helper.hpp"

TEST(HelperTest, ParseStringToInt64HandlesCommonValues) {
    EXPECT_EQ(parse_string_to_int64("123.45", 2), 12345);
    EXPECT_EQ(parse_string_to_int64("1.2", 2), 120);
    EXPECT_EQ(parse_string_to_int64("42", 2), 4200);
    EXPECT_EQ(parse_string_to_int64("0.12345678", 8), 12345678);
}

TEST(HelperTest, PrintOrderBookFormatsBestLevels) {
    std::pair<OrderLevel, OrderLevel> levels{
        OrderLevel{10050, 125000000},
        OrderLevel{10100, 350000000}
    };

    testing::internal::CaptureStdout();
    print_order_book(levels);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "100.50 1.25000000 - 101.00 3.50000000\n");
}
