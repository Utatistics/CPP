#include <cassert>
#include <iostream>
#include <string>

#include "../src/EventBus.hpp"

struct PriceUpdate {
    std::string symbol;
    double price;
};

struct OrderBookUpdate {
    std::string symbol;
    int bidSize;
    int askSize;
};

struct Trade {
    std::string symbol;
    int qty;
    double price;
};


void testEventBus() {

    EventBus<PriceUpdate> priceBus;
    EventBus<OrderBookUpdate> bookBus;
    EventBus<Trade> tradeBus;

    int priceTicks = 0;
    int bookTicks = 0;
    int tradeTicks = 0;

    /*
        subscribe returns a RAII handle.

        The callback should be stored inside the bus.
        The Subscription should only manage lifetime.
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
        test unsubscribe
    */
    priceSub.unsubscribe();
    priceBus.publish(
        PriceUpdate{"AAPL", 102.0}
    );
    // callback should not fire
    assert(priceTicks == 2);

    /*
        other event buses should be unaffected
    */
    bookBus.publish(
        OrderBookUpdate{"AAPL", 700, 800}
    );
    assert(bookTicks == 2);



    /*
        test move semantics

        after move:
            moved-from subscription should be inactive
            new subscription owns it
    */
    auto tradeSub2 = std::move(tradeSub);
    tradeBus.publish(
        Trade{"AAPL", 200, 102.0}
    );
    assert(tradeTicks == 3);


    /*
        destructor test

        leaving scope should automatically unsubscribe
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

    } // tempSub destroyed here
    priceBus.publish(
        PriceUpdate{"AAPL", 104.0}
    );

    // tempSub should have disappeared
    assert(priceTicks == 3);

    /*
        self-unsubscribe / double unsubscribe safety
    */

    tradeSub2.unsubscribe();
    tradeSub2.unsubscribe(); // should be safe


    tradeBus.publish(
        Trade{"AAPL", 300, 105.0}
    );


    assert(tradeTicks == 3);



    std::cout << "EventBus tests passed\n";
}

    EventBus<PriceUpdate> priceBus;
    EventBus<OrderBookUpdate> bookBus;
    EventBus<Trade> tradeBus;

    int priceTicks = 0;
    int bookTicks = 0;
    int tradeTicks = 0;

    /*
    If some template parameters are not explicitly given, 
    and if it can be deduced from function arguments, 
    C++ will do so. 
    */
    auto priceSub = priceBus.subscribe(
        [&](const PriceUpdate& p) { priceTicks++; }
    );

    auto bookSub = bookBus.subscribe(
        [&](const OrderBookUpdate& b) { bookTicks++; }
    );

    auto tradeSub = tradeBus.subscribe(
        [&](const Trade& t) { tradeTicks++;}
    );

    // simulate market activity
    priceBus.publish({"AAPL", 101.0});
    priceBus.publish({"AAPL", 102.0});

    bookBus.publish({"AAPL", 500, 600});

    tradeBus.publish({"AAPL", 100, 101.5});
    tradeBus.publish({"AAPL", 50, 101.7});

    assert(priceTicks == 2);
    assert(bookTicks == 1);
    assert(tradeTicks == 2);

    // unsubscribe one stream and verify isolation
    priceSub.unsubscribe();

    priceBus.publish({"AAPL", 103.0});
    assert(priceTicks == 2);

    // other buses unaffected
    bookBus.publish({"AAPL", 700, 800});
    assert(bookTicks == 2);

    std::cout << "Rich EventBus tests passed\n";
}