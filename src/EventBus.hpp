#include <iostream>
#include <map>
#include <vector>

template <typename Event>
class EventBus {
/*
You are asked to implement a lightweight EventBus system, 
similar to a simplified version of what trading systems
use for market data / signals distribution. The goal is to
allow different parts of a system to subscribe to events of
arbitrary types and get notified when those events are published.

it must support:
    - multiple event types (PriceUpdate, OrderBook, Signal, etc.)
    - type-safe subscriptions
    - efficient dispatch
    - safe lifetime management of subscriptions
 */
public:
    /* constructor */
    EventBus() {}

    /* method */
    template <typename Callback>
    Subscription subscribe(Callback&& cb) { // returns a subscription handle
        Subscription sub;
        return sub;
    }

    void publish(const Event& event) { // Calls all registered callbacks for that event type
        for (Callback cb : _m_event_callbacks[event])
        {
            /* code */
            cb();
        }
    }

private:
    class Subscription {
    public:
        Subscription();
        Subscription(const Subscription&) = delete;
        Subscription(Subscription&&);

        Subscription& operator=(const Subscription&) = delete;
        Subscription& operator=(Subscription&&);

        void unsubscribe();
        bool active() const;
    };

    class Callback {

    };

    std::map<Event, std::vector<Callback>> _m_event_callbacks;
};

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