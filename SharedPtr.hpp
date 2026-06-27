#pragma once

template<typename T>
class SharedPtr {
private :

    struct ControlBlock {
        int cnt;
        ControlBlock() : cnt(1) {};
    };

    T* _ptr;
    ControlBlock* _ctrlb; // pointer to a central block

public:
    /* constructors */
    SharedPtr(T* ptr = nullptr) : _ptr(ptr) {
        if (ptr) _ctrlb = new ControlBlock();
        else _ctrlb  = nullptr;
    }

    SharedPtr(const SharedPtr& other) { // copy constructor
        _ctrlb = other._ctrlb;
        if (_ctrlb)
            _ctrlb->cnt++;
        
         _ptr = other._ptr;
    }

    /* destructor */
    ~SharedPtr () {
        if (_ctrlb) {
            _ctrlb->cnt--;
            if (_ctrlb->cnt == 0) {
                delete _ptr;
                delete _ctrlb;
            }
        }       
    }

    /* operator overload */
    SharedPtr& operator=(const SharedPtr& other) { // copy assignment operator
        if (this == &other) return *this;

        if (_ctrlb) {
            _ctrlb->cnt--;
            if (_ctrlb->cnt == 0) {
                delete _ptr;
                delete _ctrlb;
            }
        }

        _ptr = other._ptr;
        _ctrlb = other._ctrlb;
        if (_ctrlb) _ctrlb->cnt++;

        return *this;
    }

    T& operator*() { // dereference pointe
        return *_ptr;
    }
    const T& operator*() const { // dereference pointer
        return *_ptr;
    }

    /* member functions */
    int use_count() const {
        if (_ctrlb) return _ctrlb->cnt;
        else return 0;
    }

    void reset(T* ptr = nullptr) {
        if (_ctrlb) {
            _ctrlb->cnt--;
            if (_ctrlb->cnt == 0) {
                delete _ptr;
                delete _ctrlb;
            }
        }
        _ptr = ptr;
        if (ptr) 
            _ctrlb = new ControlBlock();
        else
            _ctrlb = nullptr;
    }

};
