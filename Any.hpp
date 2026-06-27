#pragma once

#include <typeinfo>

class Any {
public:
    /* constructor */
    Any() : _m_ptr(nullptr) {}

    template <typename T>
    Any(T value) {
        _m_ptr = new Holder<T>(value);
    }

    Any(const Any& other) { // copy constructor
        delete _m_ptr;
        _m_ptr = other._m_ptr; // NEED CLONE!!
    }

    /* destructor */
    ~Any() {
        delete _m_ptr;
    }

    /* operator overload */
    Any& operator=(const Any& other) { // copy assignment 
        if (this == &other) return *this;

        delete _m_ptr;
        _m_ptr = other._m_ptr; // NEED CLONE!!

        return *this;
    }

    /* method */
    bool has_value() {
        return _m_ptr != nullptr;
    }

    const std::type_info& type() const {
        return _m_ptr->type();
    }

    template <typename T>
    T& cast() {
        Holder<T>* ptr = dynamic_cast<Holder<T>*>(_m_ptr);

        if (!ptr) {
            throw std::bad_cast();
        }
        return ptr->_m_data;
    } 

    void reset() {

    }


private:
    struct Base { // abstruct class for type erasure
        virtual ~Base() {}
        virtual const std::type_info& type() const = 0;
    };

    template <typename T>
    struct Holder : Base {
        ~Holder() override {
            delete _m_ptr;
        }

        const std::type_info& type() const override {
            return typeid(T);
        }

        T _m_data;

    };

    Base* _m_ptr = nullptr; // can't do Base<T>

};
