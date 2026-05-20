#include <gtest/gtest.h>

#include <string>

#include "../src/Utilities/helper.hpp"

TEST(HelperEdges, ProcessObjectUpdatesBothSidesFromJson) {
    json::object obj;

    json::array bids;
    bids.emplace_back(json::array{boost::json::value("100.50"), boost::json::value("1.25000000")});
    bids.emplace_back(json::array{boost::json::value("99.75"), boost::json::value("2.00000000")});
    obj["b"] = bids;

    json::array asks;
    asks.emplace_back(json::array{boost::json::value("101.00"), boost::json::value("3.50000000")});
    asks.emplace_back(json::array{boost::json::value("101.50"), boost::json::value("4.00000000")});
    obj["a"] = asks;

    OrderBook book;
    processObject(obj, book);

    EXPECT_EQ(book.get_best_bid(), (OrderLevel{10050, 125000000}));
    EXPECT_EQ(book.get_best_ask(), (OrderLevel{10100, 350000000}));
}

TEST(HelperEdges, MissingSidesLeaveExistingBookStateUntouched) {
    json::object obj;
    json::array bids;
    bids.emplace_back(json::array{boost::json::value("100.00"), boost::json::value("1.00000000")});
    obj["b"] = bids;

    OrderBook book;
    processObject(obj, book);

    EXPECT_EQ(book.get_best_bid(), (OrderLevel{10000, 100000000}));
    EXPECT_EQ(book.get_best_ask(), (OrderLevel{0, 0}));
}

TEST(HelperEdges, EmptyObjectDoesNotChangeAnEmptyBook) {
    json::object obj;
    OrderBook book;

    processObject(obj, book);

    EXPECT_EQ(book.get_best_bid(), (OrderLevel{0, 0}));
    EXPECT_EQ(book.get_best_ask(), (OrderLevel{0, 0}));
}

TEST(HelperEdges, InvalidLevelDataIsReportedAndSkipped) {
    json::object obj;

    json::array bids;
    bids.emplace_back(json::array{boost::json::value("100.00"), boost::json::value("1.00000000")});
    bids.emplace_back(json::array{boost::json::value("99.00")});
    obj["b"] = bids;

    json::array asks;
    asks.emplace_back(json::array{boost::json::value("101.00"), boost::json::value("2.00000000")});
    obj["a"] = asks;

    OrderBook book;

    testing::internal::CaptureStderr();
    processObject(obj, book);
    std::string errors = testing::internal::GetCapturedStderr();

    EXPECT_EQ(book.get_best_bid(), (OrderLevel{10000, 100000000}));
    EXPECT_EQ(book.get_best_ask(), (OrderLevel{10100, 200000000}));
    EXPECT_NE(errors.find("Invalid bid data"), std::string::npos);
}

TEST(HelperEdges, MalformedJsonTypeThrows) {
    json::object obj;
    obj["b"] = boost::json::value("not-an-array");

    OrderBook book;

    EXPECT_THROW(processObject(obj, book), boost::system::system_error);
}

TEST(HelperEdges, EmptyBookPrintsZeroValues) {
    std::pair<OrderLevel, OrderLevel> levels{OrderLevel{0, 0}, OrderLevel{0, 0}};

    testing::internal::CaptureStdout();
    print_order_book(levels);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "0.00 0.00000000 - 0.00 0.00000000\n");
}
