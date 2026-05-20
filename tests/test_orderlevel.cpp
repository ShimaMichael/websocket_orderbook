#include <gtest/gtest.h>

#include "../src/orderbook/orderbook.hpp"

TEST(OrderLevelTest, EqualityWorksForIdenticalAndDifferentLevels) {
    OrderLevel level{12345, 987654321};

    EXPECT_TRUE(level == (OrderLevel{12345, 987654321}));
    EXPECT_FALSE(level == (OrderLevel{12346, 987654321}));
    EXPECT_FALSE(level == (OrderLevel{12345, 987654322}));
}

TEST(OrderLevelTest, ConvertsToDecimalValues) {
    OrderLevel level{12345, 987654321};

    EXPECT_DOUBLE_EQ(level.get_price_double(), 123.45);
    EXPECT_DOUBLE_EQ(level.get_quantity_double(), 9.87654321);
}
