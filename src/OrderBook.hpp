#pragma once

#include <algorithm>
#include <cstdint>
#include <map>
#include <queue>
#include <vector>

enum class Side {
    Buy,
    Sell
};

enum class FilledStatus {
    Full,
    Partial,
    Nothing
};

struct Order {
    Order(std::uint64_t id, Side side, int price, int qty, std::uint64_t sequence)
        : orderId(id), side(side), price(price), quantity(qty), sequence(sequence) {}

    std::uint64_t orderId;
    Side side;
    int price;
    int quantity;
    std::uint64_t sequence;

    bool operator<(const Order& rhs) const {
        if (price != rhs.price) {
            if (side == Side::Buy)
                return price < rhs.price;   // higher buy price is better
            else
                return price > rhs.price;   // lower sell price is better
        }
        return sequence > rhs.sequence;     // older order is better
    }
};

struct Trade {
    Trade(const Order& aggressor, const Order& resting, int fillQty)
        : price(resting.price), quantity(fillQty)
    {
        if (aggressor.side == Side::Buy) {
            buyOrderId = aggressor.orderId;
            sellOrderId = resting.orderId;
            buyerIsAggressor = true;
        }
        else {
            buyOrderId = resting.orderId;
            sellOrderId = aggressor.orderId;
            buyerIsAggressor = false;
        }
    }

    std::uint64_t buyOrderId;
    std::uint64_t sellOrderId;
    int price;
    int quantity;
    bool buyerIsAggressor;
};

class OrderBook {
    using OrderQueue = std::priority_queue<Order, std::vector<Order>>;

public:
    OrderBook() = default;
    ~OrderBook() = default;

    bool addOrder(std::uint64_t id, Side side, int price, int qty) {
        if (m_orderBalance.count(id) > 0)
            return false;

        if (price <= 0 || qty <= 0)
            return false;

        Order order(id, side, price, qty, m_nextSequence++);

        matchMaker(order);

        return true;
    }

    bool cancelOrder(std::uint64_t id) {
        if (m_orderBalance.count(id) == 0)
            return false;

        m_orderBalance.erase(id);
        return true;
    }

    int remainingQuantity(std::uint64_t id) const {
        auto it = m_orderBalance.find(id);

        if (it == m_orderBalance.end())
            return 0;

        return it->second;
    }

    int bidVolume() const {
        return m_bidVolume;
    }

    int askVolume() const {
        return m_askVolume;
    }

    bool empty() const {
        return m_bidVolume == 0 && m_askVolume == 0;
    }

    const std::vector<Trade>& lastTrades() const {
        return m_trades;
    }

private:
    FilledStatus matchMaker(Order& order) {
        FilledStatus status = FilledStatus::Nothing;

        if (order.side == Side::Buy)
            status = matchMakerImpl(order, m_orderQueueSell);
        else
            status = matchMakerImpl(order, m_orderQueueBuy);

        return status;
    }

    FilledStatus matchMakerImpl(Order& order, OrderQueue& orderQueue) {
        int qtyToFill = order.quantity;
        const int originalQty = qtyToFill;

        while (!orderQueue.empty() && qtyToFill > 0) {
            Order other = orderQueue.top();

            if (m_orderBalance.count(other.orderId) == 0) { // Lazily remove cancelled orders.
                orderQueue.pop();
                continue;
            }
            if (other < order) // Prices do not cross.
                break;

            orderQueue.pop();

            const int fillQty = std::min(qtyToFill, other.quantity);
            m_trades.emplace_back(order, other, fillQty);

            qtyToFill -= fillQty;
            other.quantity -= fillQty;

            if (other.side == Side::Buy) m_bidVolume -= fillQty;
            else m_askVolume -= fillQty;
            
            if (other.quantity > 0) {
                orderQueue.push(other);
                m_orderBalance[other.orderId] = other.quantity;
            }
            else {
                m_orderBalance[other.orderId] = 0;
            }
        }

        if (qtyToFill > 0) {  // Remaining incoming quantity becomes a resting order.
            m_orderBalance[order.orderId] = qtyToFill;
            if (order.side == Side::Buy)
                m_bidVolume += qtyToFill;
            else
                m_askVolume += qtyToFill;
        }

        if (qtyToFill == 0) return FilledStatus::Full;
        else if (qtyToFill < originalQty) return FilledStatus::Partial;
        else return FilledStatus::Nothing;
    }

private:
    OrderQueue m_orderQueueBuy;
    OrderQueue m_orderQueueSell;

    int m_bidVolume = 0;
    int m_askVolume = 0;

    std::vector<Trade> m_trades;
    std::map<std::uint64_t, int> m_orderBalance;

    std::uint64_t m_nextSequence = 0;
};