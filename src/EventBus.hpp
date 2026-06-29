#include <iostream>
#include <map>
#include <functional>

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
private:
    /* forward declaration */
    class Subscription;
    class Callback;
    using Callable = std::function<void(const Event&)>;

public:
    /* constructor */
    EventBus() = default;
    EventBus(const EventBus&) = delete; // copy constructor
    EventBus(EventBus&&) = default;  

    /* operator oveload */
    EventBus& operator=(const EventBus& other) {}
    EventBus& operator=(EventBus&& other) {}

    /* method */
    template <typename T>
    Subscription subscribe(T&& callable) { // returns a subscription handle
        Callback* cb(callable);
        _m_event_callbacks[cb->_m_id](cb);
        
        Subscription sub(cb->_m_id);        
        return sub;
    }

    void publish(const Event& event) { // Calls all registered callbacks for that event type
        for (auto [id, cb] : _m_event_callbacks)
        {
            /* code */
            cb->execute();
        }
    }

private:
    class Subscription {
    public:
        /* constructor */
        Subscription() = default;
        Subscription(size_t id) : _m_id(id);
        Subscription(const Subscription&) = delete; // copy constructor *disabled
        Subscription(Subscription&&);

        /* operator overload */
        Subscription& operator=(const Subscription&) = delete; // copy assignment *disabled
        Subscription& operator=(Subscription&&);

        /* method */
        void unsubscribe();
        bool active() const;

    private:
        size_t _m_id;
    };

    class Callback {
    public:
        /* constructor */
        Callback() = default;
        Callback(T callable) {
            _m_id = _m_callbacks.size();
            _m_callback = std::move(callable);
        }

        /* method */
        void execute () {
            _m_callback();
        }

    private:
        size_t _m_id;    
        Callable _m_callback; // manual type erasure?
    };

    std::map<size_t, Callback*> _m_callbacks;
};