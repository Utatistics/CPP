
template <typename T>
class Atomic {
/*
    - think of std::atomic as a variable that guarantees each individual operation is atomic (indivisible)
    - test your understanding of concurrency API
*/
public:
    /* constructors */
    Atomic() ()  // default constructor
    Atomic(T value) : _m_value(value) {}

    /* member functions */
    T load() const { // should return copy
        std::lock_guard<std::mutex> lock(_m_lock);
        return _m_value;
    }

private:
    std::mutex _m_lock;
    T _m_value;
};
