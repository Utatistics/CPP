#pragma once

#include <bits/stdint-uintn.h>
#include <cstdint>
#include <vector>
#include <queue>
#include <map>

enum class Side {
    Buy, // 0 origin
    Sell 
};

enum class FilledStatus {
    Full,
    Partial, 
    Nothing
};

struct Order {
    Order(std::uint64_t id, Side side, int price, int amt) :
        orderId(id), side(side), price(price), amount(amt) {}
    std::uint64_t orderId;
    Side side;
    int price; 
    int amount;
};

class OrderBook
{ // primary class implementation
    using OrderQueue = std::map<int, std::deque<Order>>;

public:
    /* special member functions */
    OrderBook(/* args */);
    ~OrderBook();

    /* member functions */
    void addOrder(std::uint64_t id, Side side, int price, int amt) {
        Order order(id, side, price, amt);

        FilledStatus status = matchMaker(order);
        if (status != FilledStatus::Nothing) {  // immediate match, no need to push
            m_orderBalanace[id] = 0;
            return;
        }

        /* if no match, add to queue */
        m_orderBalanace[id] = amt;
        if (side == Side::Buy) 
            m_orderQueueBuy[price].push_back(order);
        else 
            m_orderQueueSell[price].push_back(order);
    }

    int remainingQuantity(uint64_t id) {
        return m_orderBalanace[id];
    }

    int bidVolume() {};
    int askVolume() {};
    // lastTrades() {};
    bool empty();

private:
    FilledStatus matchMaker(const Order& order) {
        FilledStatus status = FilledStatus::Nothing;
        if (order.side == Side::Buy) 
            status = matchMakerImpl(order, m_orderQueueBuy);
        else 
            status = matchMakerImpl(order, m_orderQueueSell);
        
        return status;           
    }

    FilledStatus matchMakerImpl (const Order& order, OrderQueue& orderQueue) {
        auto amtToFill = order.amount;
        const auto& price = order.price;

        if (orderQueue.count(price) > 0) {
            while (amtToFill && !orderQueue[price].empty()) {
                auto toq = orderQueue[price].front(); orderQueue[price].pop_front();

                if (amtToFill < toq.amount) {
                    auto toqUpdated = std::move(toq);
                    toqUpdated.amount = toqUpdated.amount - amtToFill;
                    orderQueue[price].push_front(toqUpdated);
                    m_orderBalanace[toqUpdated.orderId] = toqUpdated.amount;

                    return FilledStatus::Full;
                }
                else {
                    amtToFill -= toq.amount;
                    m_orderBalanace[toq.orderId] = 0;
                }
            }

            return amtToFill == 0 ? FilledStatus::Full : FilledStatus::Partial;
        }
        else return FilledStatus::Nothing;
    }
    
private: /* data memebr */
    OrderQueue m_orderQueueBuy;
    OrderQueue m_orderQueueSell;
    
    std::map<std::uint64_t, int> m_orderBalanace;

};

