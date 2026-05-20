#include <gtest/gtest.h>

#include "../src/orderbook/orderbook.hpp"

TEST(OrderBookEdges, RemovingLevelsFallsBackToNextValidPrice) {
    OrderBook book;

    book.update_bid(10000, 1);
    book.update_bid(10100, 2);
    book.update_bid(10200, 3);
    book.update_ask(10300, 1);
    book.update_ask(10200, 2);
    book.update_ask(10100, 3);

    book.update_bid(10200, 0);
    book.update_bid(10100, 0);
    book.update_ask(10100, 0);
    book.update_ask(10200, 0);

    EXPECT_EQ(book.get_best_bid(), (OrderLevel{10000, 1}));
    EXPECT_EQ(book.get_best_ask(), (OrderLevel{10300, 1}));
}

TEST(OrderBookEdges, RemovingNonexistentLevelsIsSafe) {
    OrderBook book;

    book.update_bid(10000, 0);
    book.update_ask(10100, 0);

    EXPECT_EQ(book.get_best_bid(), (OrderLevel{0, 0}));
    EXPECT_EQ(book.get_best_ask(), (OrderLevel{0, 0}));
}

TEST(OrderBookEdges, DuplicatePriceUpdatesDoNotCreateWrongBestLevel) {
    OrderBook book;

    book.update_bid(10000, 1);
    book.update_bid(10000, 5);
    book.update_ask(10100, 2);
    book.update_ask(10100, 7);

    EXPECT_EQ(book.get_best_bid(), (OrderLevel{10000, 5}));
    EXPECT_EQ(book.get_best_ask(), (OrderLevel{10100, 7}));
}
