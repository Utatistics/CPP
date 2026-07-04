class Callable {
/*
    - function signature
        - number and type of parameters
        - return type
        - cv/ reference (e.g. const, reference) * for class member function
    - NOT function signature
        - lambda capture list
        - function name, .etc.
*/
public:
    /* memmber strucct */
    struct Base {
        Base() = default;
        virtual ~Base() = default;
        virtual void call() = 0;
    };

    template <typename T>
    struct Derived : public Base{
    public:
        Derived() = default;
        Derived(T callable) : _m_callable(callable) {};
        
        void call() override {
            _m_callable();
        }

    private:
        T _m_callable;
    };

    /* constructors */
    Callable() = default; // default constructor
    
    Callable(const Callable& other) { // copy constructor
        _m_ptr = other._m_ptr;
    }
    
    template <typename F>
    Callable(F callable) {
        _m_ptr = new Derived<F>(callable);
    }

    ~Callable() { // destructor
        delete _m_ptr;
    }

    /* operator overload */
    Callable& operator=(const Callable& other) { // copy assignment
        if (&other == this) return *this;
        
        delete _m_ptr;
        _m_ptr = other._m_ptr;
    }
    
    /* method */
    void call() {
        _m_ptr->call();
    }

private:
    Base* _m_ptr = nullptr;
};

