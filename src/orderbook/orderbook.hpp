#ifndef ORDERBOOK_HPP
#define ORDERBOOK_HPP

#include <map>
#include <string>
#include <vector>

using bids_t = std::map<double, double, std::greater<double>>;
using asks_t = std::map<double, double>;
class OrderBook {
    public:
    OrderBook() = default;
    ~OrderBook() = default;

    void load_snapshot(const std::vector<std::pair<std::string, std::string>>& bids,
                       const std::vector<std::pair<std::string, std::string>>& asks);

    void update_bid(const std::string& price, const std::string& quantity);

    void update_ask(const std::string& price, const std::string& quantity);

    const bids_t& get_bids() const {
        return bids_;
    }
    const asks_t& get_asks() const {
        return asks_;
    }
    bids_t bids_;
    asks_t asks_;
    private:
        
};

#endif // ORDERBOOK_HPP