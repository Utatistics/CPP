template<typename T>
class Vector {
public:
    // constructor
    Vector() : _size(0), _capacity(0), _data(nullptr) {} // default constructor 
    
    Vector(const Vector& other) : _size(other._size), _capacity(other._capacity) { // copy constructor
        _data = new T[_capacity];
        for (int i = 0; i < (int)_size; i++) {
            _data[i] = other._data[i];
        }
    }

    Vector(int N) : _size(N), _capacity(2 * N), _data() {
        _data = new T[2 * N]();  // zero intialization
        for (int i = 0; i < N; i++) {
            _data[i] = "";
        }
    }
    
    Vector(int N, const T& value) : _size(N), _capacity(2 * N) {
        _data = new T[2 * N];
        for (int i = 0; i < N; i++) {
            _data[i] = value;
        }
    }

    // destructor
    ~Vector() {
        delete[] _data;
    }

    // operator overload
    Vector& operator=(const Vector& other) { // copy assignment operator
        if (this != &other) { // guard against self-assignment * would lead to undefined behaviour        
            delete[] _data;

            _size = other._size;
            _capacity = other._capacity;
            _data = new T[_capacity];

            for (int i = 0; i < (int)_size; i++) {
                _data[i] = other._data[i];
            }
        }

        return *this; // dereference this,  retrun ref to allow chaining
    }

    T& operator[](int i) { // access operator
        return _data[i];
    }

    const T& operator[](int i) const { // access operator *const
        return _data[i];
    }

    // util method
    size_t size() const {
        return _size;
    }

    void push_back(T value) {
        if (_size == _capacity) { // copy and move
            size_t _new_capacity = (_capacity == 0) ? 1 : _capacity * 2;
            T* _new_data = new T[_new_capacity];
            
            for (int i = 0; i < (int)_size; i++) {
                _new_data[i] = _data[i];
            }
            // update members
            delete[] _data;
            _data = _new_data;
            _capacity = _new_capacity;
        }
        _data[_size] = value;
        _size++;
    }

private : 
    // data memeber
    size_t _size;
    size_t _capacity;

    T* _data;
};

