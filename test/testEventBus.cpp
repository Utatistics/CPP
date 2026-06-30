#include <cassert>
#include <iostream>
#include <string>

#include "../src/EventBus.hpp"

void testEventBus() {
    EventBus<PriceUpdate> priceBus;
    EventBus<OrderBookUpdate> bookBus;
    EventBus<Trade> tradeBus;

    int priceTicks = 0;
    int bookTicks = 0;
    int tradeTicks = 0;

    /*
        subscribe returns RAII handle
    */
    auto priceSub = priceBus.subscribe(
        [&](const PriceUpdate& p)
        {
            assert(p.symbol == "AAPL");
            priceTicks++;
        }
    );
    auto bookSub = bookBus.subscribe(
        [&](const OrderBookUpdate& b)
        {
            assert(b.symbol == "AAPL");
            assert(b.bidSize > 0);
            assert(b.askSize > 0);

            bookTicks++;
        }
    );
    auto tradeSub = tradeBus.subscribe(
        [&](const Trade& t)
        {
            assert(t.qty > 0);
            tradeTicks++;
        }
    );


    /*
        normal publishing
    */
    priceBus.publish(
        PriceUpdate{"AAPL", 100.0}
    );
    priceBus.publish(
        PriceUpdate{"AAPL", 101.0}
    );
    bookBus.publish(
        OrderBookUpdate{"AAPL", 500, 600}
    );
    tradeBus.publish(
        Trade{"AAPL", 100, 101.5}
    );
    tradeBus.publish(
        Trade{"AAPL", 50, 101.7}
    );
    assert(priceTicks == 2);
    assert(bookTicks == 1);
    assert(tradeTicks == 2);

    /*
        unsubscribe test
    */
    priceSub.unsubscribe();
    priceBus.publish(
        PriceUpdate{"AAPL", 102.0}
    );
    // price callback removed
    assert(priceTicks == 2);

    /*
        other event buses unaffected
    */
    bookBus.publish(
        OrderBookUpdate{"AAPL", 700, 800}
    );
    assert(bookTicks == 2);

    /*
        move semantics test
    */

    auto tradeSub2 = std::move(tradeSub);
    tradeBus.publish(
        Trade{"AAPL", 200, 102.0}
    );
    assert(tradeTicks == 3);

    /*
        destructor automatically unsubscribes
    */
    {
        auto tempSub = priceBus.subscribe(
            [&](const PriceUpdate&)
            {
                priceTicks++;
            }
        );
        priceBus.publish(
            PriceUpdate{"AAPL", 103.0}
        );
        assert(priceTicks == 3);

    } // tempSub destroyed
    priceBus.publish(
        PriceUpdate{"AAPL", 104.0}
    );

    // tempSub was removed
    assert(priceTicks == 3);

    /*
        double unsubscribe safety
    */

    tradeSub2.unsubscribe();
    tradeSub2.unsubscribe();
    tradeBus.publish(
        Trade{"AAPL", 300, 105.0}
    );
    assert(tradeTicks == 3);

    std::cout << "EventBus tests passed\n";
}