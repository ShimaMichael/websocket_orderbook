#include "helper.hpp"

#include <string>
#include <cmath>
#include <iostream>
#include <iomanip>

/*** Applies bid and ask updates from a parsed websocket JSON object to the order book.
@param obj Parsed JSON object containing bid updates under "b" and ask updates under "a".
@param order_book OrderBook instance to update with parsed price levels.
@return No return value.
***/
void processObject(const json::object& obj, OrderBook& order_book) {
    auto process_side = [&](const std::string& key, const std::string& label, auto update_func) {
        if (!obj.contains(key)) {
            return;
        }

        const auto& levels = obj.at(key).as_array();
        for (const auto& level_element : levels) {
            const auto& level = level_element.as_array();
            
            if (level.size() < 2) {
                std::cerr << "Invalid " << label << " data: " << level_element << std::endl;
                continue; 
            }

            auto price = parse_string_to_int64(level[0].as_string(), 2);
            auto quantity = parse_string_to_int64(level[1].as_string(), 8);
            
            (order_book.*update_func)(price, quantity);
        }
    };

    process_side("b", "bid", &OrderBook::update_bid);
    process_side("a", "ask", &OrderBook::update_ask);
}

/*** Prints the current best bid and ask in human-readable decimal format.
@param level Pair containing the best bid as first and best ask as second.
@return No return value.
***/
void print_order_book(std::pair<OrderLevel, OrderLevel>& level) {
    OrderLevel best_bid = level.first;
    OrderLevel best_ask =level.second;

    std::cout << std::fixed;

    std::cout << std::setprecision(2) << best_bid.get_price_double() << " "
              << std::setprecision(8) << best_bid.get_quantity_double()
              << " - "
              << std::setprecision(2) << best_ask.get_price_double() << " "
              << std::setprecision(8) << best_ask.get_quantity_double()
              << std::endl;
}

/*** Converts a decimal string into a fixed-point signed integer.
@param value JSON string containing a non-negative decimal number.
@param target_decimals Number of fractional decimal places to preserve.
@return Fixed-point integer representation of the input value.
***/
int64_t parse_string_to_int64(const boost::json::string& price_str, int target_decimals) {
    int64_t whole_part = 0;
    int64_t frac_part = 0;
    size_t i = 0;
    size_t len = std::string(price_str).length();

    while (i < len && price_str[i] != '.') {
        whole_part = whole_part * 10 + (price_str[i] - '0');
        i++;
    }

    if (i < len && price_str[i] == '.') {
        i++;
    }

    int digits_parsed = 0;
    while (i < len && digits_parsed < target_decimals) {
        frac_part = frac_part * 10 + (price_str[i] - '0');
        i++;
        digits_parsed++;
    }

    while (digits_parsed < target_decimals) {
        frac_part *= 10;
        digits_parsed++;
    }

    int64_t multiplier = 1;
    for (int d = 0; d < target_decimals; ++d) {
        multiplier *= 10;
    }

    return (whole_part * multiplier) + frac_part;
}