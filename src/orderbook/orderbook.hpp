#ifndef ORDERBOOK_HPP
#define ORDERBOOK_HPP

#include <map>
#include <string>
#include <vector>

class OrderBook {
    public:
    OrderBook() = default;
    ~OrderBook() = default;

    void load_snapshot(const std::vector<std::pair<std::string, std::string>>& bids,
                       const std::vector<std::pair<std::string, std::string>>& asks);

    void update_bid(const std::string& price, const std::string& quantity);

    void update_ask(const std::string& price, const std::string& quantity);

    private:
        std::map<double, double, std::greater<double>> bids_;
        std::map<double, double> asks_;
};

#endif // ORDERBOOK_HPP