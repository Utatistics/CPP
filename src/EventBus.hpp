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
    using Callable = std::function<void(const Event&)>; // upgrade: custom type erasure

public: 
    class Subscription {
    public:
        /* constructor */
        Subscription() : _m_id(0), _m_bus(nullptr) {};
        Subscription(size_t id, EventBus* bus) : _m_id(id), _m_bus(bus) {};
        Subscription(const Subscription&) = delete; // copy constructor *disabled
        Subscription(Subscription&&) = default;

        /* destructor */
        ~Subscription() {
            unsubscribe();
        }

        /* operator overload */
        Subscription& operator=(const Subscription&) = delete; // copy assignment *disabled
        Subscription& operator=(Subscription&&) = default;

        /* method */
        void unsubscribe() {
            if (_m_bus) {
                _m_bus->remove(_m_id);
                _m_bus = nullptr;
            }
        }

        bool active() const;

    private:
        size_t _m_id;
        EventBus* _m_bus;
    };

private:
    class Callback {
        friend class EventBus;

    public:
        /* constructor */
        Callback() = default;
        Callback(Callable callable, size_t id) : _m_id(id), _m_callback(std::move(callable)) {}

        /* method */
        void execute (const Event& event) {
            _m_callback(event);
        }
    private:
        size_t _m_id;
        Callable _m_callback; // actual callbale (e.g. lamda, functor)
    };

public:
    /* constructor */
    EventBus() = default;
    EventBus(const EventBus&) = delete; // copy constructor
    EventBus(EventBus&&) = default;

    /* destructor */
    ~EventBus() {
        for (auto [id, cb]: _m_callbacks)
        {
            delete cb;
        }
    }

    /* operator oveload */
    EventBus& operator=(const EventBus& other) {}
    EventBus& operator=(EventBus&& other) {}

    /* method */
    Subscription subscribe(Callable&& callable) { // returns a subscription handle
        Callback* cb = new Callback(callable, _m_handles++); 
        
        _m_callbacks[cb->_m_id] = cb; // cb gets copied?        
        
        Subscription sub(cb->_m_id, this); 
        return sub;
    }

    void remove(size_t id) {
        auto it = _m_callbacks.find(id);
        if (it != _m_callbacks.end()) {
            delete it->second;
            _m_callbacks.erase(it);
        }
    }

    void publish(const Event& event) { // Calls all registered callbacks for that event type
        for (auto [id, cb] : _m_callbacks)
        {
            cb->execute(event);
        }
    }

/* event struct */
private:
    std::map<size_t, Callback*> _m_callbacks;
    size_t _m_handles = 0;
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
