#pragma once

#include <typeinfo>
#include <stdexcept>

class Any {
/*
*   - Any class does not know T at compile time
*   - only stores Base* that enables type erasure
*   - has template function as method to create an object of type T
*/
public:
    /* constructor */
    Any() : _m_ptr(nullptr) {}

    template <typename T>
    Any(T data) {
        _m_ptr = new Holder<T>(std::move(data)); // explicit cast, move semanctics 
    }

    Any(const Any& other) { // copy constructor
        _m_ptr = other._m_ptr ? other._m_ptr->clone() : nullptr; // deep copy

    }

    Any(Any&& other) noexcept { // move constructor
        _m_ptr = other._m_ptr; // shallow copy        
        other._m_ptr = nullptr;
    }

    /* destructor */
    ~Any() {
        delete _m_ptr;
    }

    /* operator overload */
    Any& operator=(const Any& other) { // copy assignment 
        if (this == &other) return *this;

        delete _m_ptr;
        _m_ptr = other._m_ptr ? other._m_ptr->clone() : nullptr; // deep copy

        return *this;
    }

    Any& operator=(Any&& other) noexcept { // move assignment 
        if (this == &other) return *this;
        if (_m_ptr)
            delete _m_ptr;
        
        _m_ptr = other._m_ptr; // shallow copy        
        other._m_ptr = nullptr;

        return *this;
    }

    /* method */
    bool has_value() const { // can be used for const
        return _m_ptr != nullptr;
    }

    const std::type_info& type() const {
        if (_m_ptr)
            return _m_ptr->type();
        else
            throw std::runtime_error("Any is empty");
    }

    template <typename T>
    T& cast() {
        if (_m_ptr) {
            Holder<T>* ptr = dynamic_cast<Holder<T>*>(_m_ptr); // i.e. Base* to Holder*
            if (ptr) return ptr->_m_data; // validate dynamic cast
            else throw std::bad_cast();
        }
        else 
            throw std::bad_cast();        
    } 

    void reset() { // Throw away the currently held object and become empty.
        delete _m_ptr;
        _m_ptr = nullptr;
    }

private:
    struct Base { // abstruct class for type erasure
        virtual ~Base() {}
        virtual const std::type_info& type() const = 0;
        virtual Base* clone() const = 0;
    };

    template <typename T>
    struct Holder : Base {
        Holder(T data) : _m_data(std::move(data)){} 

        ~Holder() override = default;

        const std::type_info& type() const override {
            return typeid(T);
        }

        Base* clone() const override {
            Base* ptr = new Holder<T>(_m_data); // deep copy
            return ptr;
        }

        T _m_data;
    };

    Base* _m_ptr = nullptr; // can't do Base<T>

};
