#pragma once

template<typename T>
class UniquePtr {
public:
    /* constructor */  
    UniquePtr(T* ptr = nullptr) : _size(sizeof(T)), _ptr(ptr) {}
    
    UniquePtr(const UniquePtr& other) = delete; // copy constructor disabled.

    UniquePtr(UniquePtr&& other) { // move constructor
        _size = other._size;
        _ptr = other._ptr;

        other._size = 0;
        other._ptr = nullptr;
    }

    /* destructor */
    ~UniquePtr() {
        delete _ptr; // free resource
    }

    /* oprator overload */
    UniquePtr& operator=(const UniquePtr& other) = delete; // copy assignment operator *disabled
    
    UniquePtr& operator=(UniquePtr&& other) { // move assignment operator
        if (this == &other) return *this; // address check
        delete _ptr;

        _size = other._size;
        _ptr = other._ptr;

        other._size = 0;
        other._ptr = nullptr;

        return *this;
    } 

    T& operator*() { // dereference operator
        return *_ptr; 
    }
    const T& operator*() const { 
        return *_ptr; 
    }

    T* operator->() const { // arrow operator, must be a member
         return _ptr;
    }

    explicit operator bool() const {
        return _ptr != nullptr; 
    }

    // methods
    void reset(T* ptr = nullptr) { // replace
        if (_ptr) delete _ptr;

        if (ptr) _ptr = ptr;
        else _ptr = nullptr;
    }

    T* release () { // give away an ownership    
        T* ptr = _ptr;
        
        _ptr = nullptr;
        _size = 0;

        return ptr;
    }


private:
    size_t _size;
    T* _ptr;
};
