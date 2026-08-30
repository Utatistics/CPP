#include <cassert>
#include <iostream>

#include "../src/OrderBook.hpp"

void testOrderBook() {

    {
        OrderBook ob;

        assert(ob.empty());
        assert(ob.bidVolume() == 0);
        assert(ob.askVolume() == 0);
    }

    {
        OrderBook ob;

        assert(ob.addOrder(1, Side::Buy, 100, 10));
        assert(ob.remainingQuantity(1) == 10);

        assert(ob.bidVolume() == 10);
        assert(ob.askVolume() == 0);

        assert(ob.lastTrades().empty());
    }

    {
        OrderBook ob;

        assert(ob.addOrder(1, Side::Sell, 105, 7));

        assert(ob.remainingQuantity(1) == 7);
        assert(ob.bidVolume() == 0);
        assert(ob.askVolume() == 7);

        assert(ob.lastTrades().empty());
    }

    /* Full fill */
    {
        OrderBook ob;

        assert(ob.addOrder(1, Side::Buy, 100, 10));
        assert(ob.addOrder(2, Side::Sell, 100, 10));

        assert(ob.remainingQuantity(1) == 0);
        assert(ob.remainingQuantity(2) == 0);

        assert(ob.bidVolume() == 0);
        assert(ob.askVolume() == 0);
        const auto& trades = ob.lastTrades();

        assert(trades.size() == 1);
        assert(trades[0].buyOrderId == 1);
        assert(trades[0].sellOrderId == 2);
        assert(trades[0].price == 100);
        assert(trades[0].quantity == 10);
        assert(!trades[0].buyerIsAggressor);
    }

    /* Partial fill (incoming smaller) */
    {
        OrderBook ob;

        assert(ob.addOrder(1, Side::Buy, 100, 20));
        assert(ob.addOrder(2, Side::Sell, 99, 5));

        assert(ob.remainingQuantity(1) == 15);
        assert(ob.remainingQuantity(2) == 0);
        assert(ob.bidVolume() == 15);
        assert(ob.askVolume() == 0);
        const auto& trades = ob.lastTrades();

        assert(trades.size() == 1);
        assert(trades[0].quantity == 5);
    }

    /* Partial fill (incoming larger) */
    {
        OrderBook ob;

        assert(ob.addOrder(1, Side::Buy, 100, 5));
        assert(ob.addOrder(2, Side::Sell, 99, 20));

        assert(ob.remainingQuantity(1) == 0);
        assert(ob.remainingQuantity(2) == 15);
        assert(ob.bidVolume() == 0);
        assert(ob.askVolume() == 15);
        const auto& trades = ob.lastTrades();

        assert(trades.size() == 1);
        assert(trades[0].quantity == 5);
    }


    /* One incoming order matches multiple resting orders */
    {
        OrderBook ob;

        assert(ob.addOrder(1, Side::Sell, 100, 3));
        assert(ob.addOrder(2, Side::Sell, 100, 4));
        assert(ob.addOrder(3, Side::Sell, 100, 5));
        assert(ob.addOrder(10, Side::Buy, 100, 10));

        const auto& trades = ob.lastTrades();
        assert(trades.size() == 3);
        assert(trades[0].sellOrderId == 1);
        assert(trades[1].sellOrderId == 2);
        assert(trades[2].sellOrderId == 3);
        assert(trades[0].quantity == 3);
        assert(trades[1].quantity == 4);
        assert(trades[2].quantity == 3);

        assert(ob.remainingQuantity(3) == 2);
    }

    /* FIFO at same price */
    {
        OrderBook ob;

        assert(ob.addOrder(1, Side::Sell, 100, 5));
        assert(ob.addOrder(2, Side::Sell, 100, 5));
        assert(ob.addOrder(3, Side::Buy, 100, 7));

        const auto& trades = ob.lastTrades();
        assert(trades.size() == 2);
        assert(trades[0].sellOrderId == 1);
        assert(trades[1].sellOrderId == 2);

        assert(ob.remainingQuantity(1) == 0);
        assert(ob.remainingQuantity(2) == 3);
    }

    /* Better price matched first */
    {
        OrderBook ob;

        assert(ob.addOrder(1, Side::Sell, 101, 5));
        assert(ob.addOrder(2, Side::Sell, 99, 5));
        assert(ob.addOrder(3, Side::Buy, 101, 6));

        const auto& trades = ob.lastTrades();

        assert(trades.size() == 2);
        assert(trades[0].sellOrderId == 2);
        assert(trades[1].sellOrderId == 1);
        assert(trades[0].quantity == 5);
        assert(trades[1].quantity == 1);
    }

    /* No crossing prices */
    {
        OrderBook ob;

        assert(ob.addOrder(1, Side::Buy, 99, 10));

        assert(ob.addOrder(2, Side::Sell, 100, 10));

        assert(ob.lastTrades().empty());

        assert(ob.remainingQuantity(1) == 10);
        assert(ob.remainingQuantity(2) == 10);
    }

    /* Cancel */
    {
        OrderBook ob;

        assert(ob.addOrder(1, Side::Buy, 100, 10));

        assert(ob.cancelOrder(1));

        assert(ob.remainingQuantity(1) == 0);

        assert(ob.bidVolume() == 0);

        assert(!ob.cancelOrder(1));
    }

    /* Duplicate order id */
    {
        OrderBook ob;

        assert(ob.addOrder(1, Side::Buy, 100, 5));

        assert(!ob.addOrder(1, Side::Sell, 100, 5));
    }

    /* Invalid orders */ 
    {
        OrderBook ob;

        assert(!ob.addOrder(1, Side::Buy, 0, 10));
        assert(!ob.addOrder(2, Side::Buy, 100, 0));
        assert(!ob.addOrder(3, Side::Buy, -1, 5));
        assert(!ob.addOrder(4, Side::Buy, 100, -5));
    }

    /* Buyer aggressor flag */
    {
        OrderBook ob;

        assert(ob.addOrder(1, Side::Sell, 100, 5));

        assert(ob.addOrder(2, Side::Buy, 101, 5));

        const auto& trades = ob.lastTrades();

        assert(trades.size() == 1);
        assert(trades[0].buyerIsAggressor);
    }

    //
    // Seller aggressor flag
    //
    {
        OrderBook ob;

        assert(ob.addOrder(1, Side::Buy, 100, 5));

        assert(ob.addOrder(2, Side::Sell, 99, 5));

        const auto& trades = ob.lastTrades();

        assert(trades.size() == 1);
        assert(!trades[0].buyerIsAggressor);
    }

    std::cout << "OrderBook tests passed\n";
}