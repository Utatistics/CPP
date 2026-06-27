#include <iostream>
#include <stdexcept>

template<typename T>
class Optional {
public :
    /* constructor */
    Optional() = default;
    Optional(const T& data) : _data(data), _hasValue(true) {};

    
    /* oprator overload */
    explicit operator bool() const { // conversion operator *only explict, i.e. if(), while(), bool()
        return _hasValue;
    };
    
    Optional<T>& operator=(const T& value) { // assign operator 
        _data = value;
        _hasValue = true;
        return *this; // return refference (chainable)
    };
    
    T& operator*() { // access operator
        if (!_hasValue)
            throw std::runtime_error("Optional is Empty.");

        return _data;
    };
    const T& operator*() const { // access operator (second const allows * on const object)
        if (!_hasValue)
            throw std::runtime_error("Optional is Empty.");
        return _data;
    };

    /* member functions */
    T& value() {
        return _data;
    }

    void reset() {
        _hasValue = false;
    }

    bool has_value() {
        return _hasValue;
    };
   
private:
    T _data;
    bool _hasValue = false;
};

template<typename T> // operator overload defined outside
std::ostream& operator<<(std::ostream& os, const Optional<T>& opt) {
    if (opt) os << *opt;
    else os << "nullopt";
    return os;
}
template<typename T>
void printOpt(const Optional<T> &opt) {
    if (!opt) { // the compiler tries to convert opt into 'bool'
        std::cout << "No value yet" << std::endl;
    }
    else {
        std::cout << "Value: " << *opt << std::endl;
    }
}
