#include <cassert>

#include <../src/EventBus.hpp>

void testEventBus() {

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