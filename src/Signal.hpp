#include <functional>
#include <map>

template <typename T>
class Signal {
    // friend struct Subscription; *EXPLAIN WHY THIS IS NOT NEEDED...
    using Function = std::function<void(int)>;
public:
    /* member struct */
    struct Subscription {
    public:
        Subscription() = default; // default constructor
        Subscription(size_t id,  Function callable, Signal* sig) 
            : _m_id(id), _m_callable(callable),  _m_signal(sig) {}

        ~Subscription() {
            _m_signal->_m_subscriptions.erase(_m_id);
        }

        void execute(int val) {
            _m_callable(val);
        }

    private:
        size_t _m_id;
        Function _m_callable;
        Signal* _m_signal;
    };

    /* constructor */
    Signal() : _m_id_counter(0) {} // default constructor

    Signal(const Signal& other) :_m_id_counter(other._m_id_counter) { // copy constructor
        release();
        for (auto [id, sub] : other._m_subscriptions)
        {
            _m_subscriptions[id] = sub;
        }
    }

    ~Signal() { // destructor
        release();
    }

    /* operator overload */
    Signal& operator=(const Signal& other) { // copy assignment
        if (this == &other) return *this;

        release();
        for (auto [id, sub] : other._m_subscriptions)
        {
            _m_subscriptions[id] = sub;
        }
        
        return *this;
    }

    /* method */
    Subscription subscribe(Function&& callable) {
        size_t id = _m_id_counter++;
        Subscription* sub = new Subscription(id, callable, this);
        _m_subscriptions[id] = sub;

        return *sub;
    }

    void publish(int val) {
        for (auto [id, sub] : _m_subscriptions)
        {
            sub->execute(val);
        }        
    }

    void release() {
        for (auto [id, sub] : _m_subscriptions)
            {
                _m_subscriptions.erase(id);        
            }
    }

private:
    size_t _m_id_counter = 0;
    std::map<size_t, Subscription*> _m_subscriptions;
};
