#include <gtest/gtest.h>

#include "../src/orderbook/orderbook.hpp"

TEST(OrderBookBasic, EmptyBookHasNoBestLevels) {
    OrderBook book;

    EXPECT_EQ(book.get_best_bid(), (OrderLevel{0, 0}));
    EXPECT_EQ(book.get_best_ask(), (OrderLevel{0, 0}));
}

TEST(OrderBookBasic, BestBidAndAskReflectHighestAndLowestPrices) {
    OrderBook book;

    book.update_bid(10000, 5);
    book.update_bid(10100, 3);
    book.update_bid(9900, 9);
    book.update_ask(10300, 7);
    book.update_ask(10200, 4);
    book.update_ask(10400, 1);

    EXPECT_EQ(book.get_best_bid(), (OrderLevel{10100, 3}));
    EXPECT_EQ(book.get_best_ask(), (OrderLevel{10200, 4}));
}

TEST(OrderBookBasic, QuantityUpdatesReplaceExistingLevels) {
    OrderBook book;

    book.update_bid(10100, 3);
    book.update_bid(10100, 8);
    book.update_ask(10200, 6);
    book.update_ask(10200, 2);

    EXPECT_EQ(book.get_best_bid(), (OrderLevel{10100, 8}));
    EXPECT_EQ(book.get_best_ask(), (OrderLevel{10200, 2}));
}
