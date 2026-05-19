#ifndef ORDERBOOK_HPP
#define ORDERBOOK_HPP

#include <unordered_map>
#include <queue>
#include <string>
#include <vector>
#include <iostream>

#include <map>


/*** Stores one price level from the order book using fixed-point integers.
@param price Price scaled by 100.
@param quantity Quantity scaled by 100000000.
@return No return value.
***/
struct OrderLevel {
    int64_t price;
    int64_t quantity;

    /*** Compares two order levels for matching price and quantity.
    @param other Order level to compare against.
    @return True when both price and quantity are equal, otherwise false.
    ***/
    bool operator==(const OrderLevel& other) const {
        return price == other.price && quantity == other.quantity;
    }

    /*** Converts the fixed-point price into a decimal value for display.
    @param None.
    @return Price as a double with two decimal places of precision.
    ***/
    double get_price_double() const {
        return static_cast<double>(price) / 100.0;
    }

    /*** Converts the fixed-point quantity into a decimal value for display.
    @param None.
    @return Quantity as a double with eight decimal places of precision.
    ***/
    double get_quantity_double() const {
        return static_cast<double>(quantity) / 100000000.0; 
    }
};

/*** Maintains bid and ask sides of an order book and exposes the best levels.
@param None.
@return No return value.
***/
class OrderBook {
    public:
    /*** Creates an empty order book.
    @param None.
    @return No return value.
    ***/
    OrderBook() = default;

    /*** Destroys the order book and releases owned containers.
    @param None.
    @return No return value.
    ***/
    ~OrderBook() = default;

    /*** Inserts, updates, or removes a bid level.
    @param price Bid price scaled by 100.
    @param quantity Bid quantity scaled by 100000000. A value of 0 removes the level.
    @return No return value.
    ***/
    void update_bid(int64_t price, int64_t quantity);

    /*** Inserts, updates, or removes an ask level.
    @param price Ask price scaled by 100.
    @param quantity Ask quantity scaled by 100000000. A value of 0 removes the level.
    @return No return value.
    ***/
    void update_ask(int64_t price, int64_t quantity);

    /*** Gets the highest active bid level currently stored in the order book.
    @param None.
    @return Best bid as an OrderLevel, or {0, 0} when no bid is available.
    ***/
    OrderLevel get_best_bid();

    /*** Gets the lowest active ask level currently stored in the order book.
    @param None.
    @return Best ask as an OrderLevel, or {0, 0} when no ask is available.
    ***/
    OrderLevel get_best_ask();
    
    private:
        std::unordered_map<int64_t, int64_t> bids_;
        std::unordered_map<int64_t, int64_t> asks_;
        std::priority_queue<int64_t> best_bids_heap_;
        std::priority_queue<int64_t, std::vector<int64_t>, std::greater<int64_t>> best_asks_heap_;

        /*** Applies a price level update to one side of the book and its heap index.
        @param price Price level scaled by 100.
        @param quantity Quantity scaled by 100000000. A value of 0 removes the level.
        @param side_heap Heap used to track candidate best prices for this side.
        @param order_side Map containing the active price levels for this side.
        @return No return value.
        ***/
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

        /*** Finds the best active price level for one side of the book.
        @param side_heap Heap containing candidate best prices for this side.
        @param order_side Map containing the active price levels for this side.
        @return Best active OrderLevel, or {0, 0} when no level is available.
        ***/
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
