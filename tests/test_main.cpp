#include <gtest/gtest.h>

#include "../src/Utilities/helper.hpp"

TEST(IntegrationTest, SequentialUpdatesKeepBestLevelsStable) {
    OrderBook book;
    json::object snapshot_one;
    json::object snapshot_two;

    json::array bids_one;
    bids_one.emplace_back(json::array{boost::json::value("100.00"), boost::json::value("1.00000000")});
    bids_one.emplace_back(json::array{boost::json::value("101.00"), boost::json::value("2.00000000")});
    snapshot_one["b"] = bids_one;

    json::array asks_one;
    asks_one.emplace_back(json::array{boost::json::value("103.00"), boost::json::value("3.00000000")});
    asks_one.emplace_back(json::array{boost::json::value("102.00"), boost::json::value("4.00000000")});
    snapshot_one["a"] = asks_one;

    processObject(snapshot_one, book);
    EXPECT_EQ(book.get_best_bid(), (OrderLevel{10100, 200000000}));
    EXPECT_EQ(book.get_best_ask(), (OrderLevel{10200, 400000000}));

    json::array bids_two;
    bids_two.emplace_back(json::array{boost::json::value("101.00"), boost::json::value("5.00000000")});
    bids_two.emplace_back(json::array{boost::json::value("102.00"), boost::json::value("6.00000000")});
    snapshot_two["b"] = bids_two;

    json::array asks_two;
    asks_two.emplace_back(json::array{boost::json::value("102.00"), boost::json::value("0.00000000")});
    asks_two.emplace_back(json::array{boost::json::value("104.00"), boost::json::value("1.00000000")});
    snapshot_two["a"] = asks_two;

    processObject(snapshot_two, book);
    EXPECT_EQ(book.get_best_bid(), (OrderLevel{10200, 600000000}));
    EXPECT_EQ(book.get_best_ask(), (OrderLevel{10300, 300000000}));
}
