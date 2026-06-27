#pragma once

#include <functional>

template <typename Signature>
class Function;

template <typename R, typename... Args>
class Function<R(Args...)> {
/*
    more generic version of std::function
        - std::function is tied to single unique function signature
        - type erasure to accept any callable (i.e., function pointer, functor, lambda) 
*/
public:
    /* constructor */
    Function() : _m_callable(nullptr), _m_hasValue(false) {}

    Function(std::nullptr_t) : _m_callable(nullptr), _m_hasValue(false) {} // guard against nullptr

    template <typename F>
    Function(F f)
    {
        _m_callable = new Derived<F>(f);
        _m_hasValue = true;
    }

    Function(const Function& other) // copy constructor
    {
        if (other._m_callable)
            _m_callable = other._m_callable->clone();
        else
            _m_callable = nullptr;

        _m_hasValue = other._m_hasValue;
    }

    Function(Function&& other)  // move constructor
        : _m_callable(other._m_callable),
          _m_hasValue(other._m_hasValue)
    {
        other._m_callable = nullptr;
        other._m_hasValue = false;
    }

    /* destructor */
    ~Function()
    {
        delete _m_callable;
    }

    /* operator overload */
    Function& operator=(const Function& other) // copy assignment
    {
        if (this == &other)
            return *this;

        delete _m_callable;

        if (other._m_callable)
            _m_callable = other._m_callable->clone();
        else
            _m_callable = nullptr;

        _m_hasValue = other._m_hasValue;

        return *this;
    }

    Function& operator=(Function&& other) // move assignment
    {
        if (this == &other)
            return *this;

        delete _m_callable;

        _m_callable = other._m_callable;
        _m_hasValue = other._m_hasValue;

        other._m_callable = nullptr;
        other._m_hasValue = false;

        return *this;
    }

    template <typename F>
    Function& operator=(F f) // assignment overload
    {
        delete _m_callable;

        _m_callable = new Derived<F>(f);

        _m_hasValue = true;

        return *this;
    }

    Function& operator=(std::nullptr_t) // assignment overload * nullptr guard
    {
        delete _m_callable;
        _m_callable = nullptr;
        _m_hasValue = false;
        return *this;
    }

    R operator()(Args... args) // call operator
    {
        if (!_m_callable)
            throw std::bad_function_call();

        return _m_callable->call(std::forward<Args>(args)...);
    }

    explicit operator bool() const // conversion operator
    {
        return _m_hasValue;
    }

    bool operator!() const
    {
        return !_m_hasValue;
    }

private:

    struct Base {
        virtual ~Base() = default;
        virtual R call(Args... args) = 0;
        virtual Base* clone() = 0;
    };

    template <typename F>
    struct Derived : Base {
        Derived(F f) : _m_func(std::move(f)) {}

        R call(Args... args) override
        {
            return _m_func(std::forward<Args>(args)...);
        }

        Base* clone() override
        {
            return new Derived(_m_func);
        }

    private:
        F _m_func;
    };

    Base* _m_callable;
    bool _m_hasValue = false;
};