#ifndef ORDERBOOK_HPP
#define ORDERBOOK_HPP

#include <unordered_map>
#include <queue>
#include <string>
#include <vector>
#include <iostream>

#include <map>


struct OrderLevel {
    int64_t price;
    int64_t quantity;

    bool operator==(const OrderLevel& other) const {
        return price == other.price && quantity == other.quantity;
    }

    double get_price_double() const {
        return static_cast<double>(price) / 100.0;
    }

    double get_quantity_double() const {
        return static_cast<double>(quantity) / 100000000.0; 
    }
};


class OrderBook {
    public:
    OrderBook() = default;
    ~OrderBook() = default;

    
    void update_bid(int64_t price, int64_t quantity);

    void update_ask(int64_t price, int64_t quantity);

    OrderLevel get_best_bid();

    OrderLevel get_best_ask();
    
    private:
        std::unordered_map<int64_t, int64_t> bids_;
        std::unordered_map<int64_t, int64_t> asks_;
        std::priority_queue<int64_t> best_bids_heap_;
        std::priority_queue<int64_t, std::vector<int64_t>, std::greater<int64_t>> best_asks_heap_;

        template <typename HeapType>
        void update_order(
            int64_t price,
            int64_t quantity,
            HeapType& side_heap, 
            std::unordered_map<int64_t, int64_t>& order_side
        ) {
            if (quantity == 0) {
                order_side.erase(price);
            } 
            else {
                auto it = order_side.find(price);
                if (it == order_side.end()){
                    side_heap.push(price);
                    order_side[price] = quantity;
                }
                else{
                    it->second = quantity;
                }
                
            }
        }

        template<typename HeapType>
        OrderLevel get_best_level(
            HeapType& side_heap, 
            std::unordered_map<int64_t, int64_t>& order_side
        ) {
            if (order_side.empty()) {
                return {0, 0};
            }
            while (!side_heap.empty()){
                auto best_ask = side_heap.top();
                auto it = order_side.find(best_ask);

                if(it == order_side.end() || it->second == 0){
                    side_heap.pop(); 
                }
                else{
                    return {it->first, it->second};
                }
            }
            return {0,0};
        }
        
};

#endif // ORDERBOOK_HPP