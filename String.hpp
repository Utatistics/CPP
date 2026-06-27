#include <iostream>
#include <string.h>

class String {
private:
    static constexpr size_t SSO_SIZE = 16; // set in compile timme

public:
    /* constructors */
    String() : _size(0), _isSSO(true) {  // default constructor
        _sso[0] = '\0';
    };

    String(const char* cstr) { // *constructor with 1 arg can be used for implicit conversion
        _size = strlen(cstr);

        if (_size <= SSO_SIZE) {
            _isSSO = true;
            memcpy(_sso, cstr, _size + 1);
        } else {
            _data = new char[_size + 1];
            memcpy(_data, cstr, _size);
        }
    }

    String(const String& other) { // copy constructor
        // std::cout << "copy constructor called." << std::endl;
        _size = other._size;
        _data = new char[_size];
        memcpy(_data, other._data, _size);
    }

    String(String&& other) noexcept { // move constructor
        std::cout << "move constructor called." << std::endl;
        _size = other._size;
        _data = other._data;

        // hollow object
        other._size = 0;
        other._data = nullptr;
    }

    /* destructor */
    ~String() {
        delete[] _data;
    }

    /* operator overload*/
    String& operator=(const String& other) { // copy assignment operator
        if (this == &other) return *this;
        delete[] _data;
        
        _size = other._size;
        _data = new char[_size];
        memcpy(_data, other._data, _size);

        return *this;
    }

    String& operator=(String&& other) noexcept { // move assignment operator
        if (this == &other) return *this;
        delete[] _data;        

        _size = other._size;
        _data = other._data;

        other._size = 0;
        other._data = nullptr;

        return *this;
    }
    
    bool operator==(const String& other) const {
        if (this == &other) return true;
        if (_size != other._size) return false;

        bool res = true;
        for (size_t i = 0; i < _size; i++) {
            if (_data[i] != other._data[i]) {
                res = false;
                break;
            }
        }
        return res;
    }

    bool operator==(const std::string& string) const {
        if (_size != string.size()) return false;

        bool res = true;
        for (size_t i = 0; i < _size; i++) {
            if (_data[i] != string[i]) {
                res = false;
                break;
            }
        }
        return res;
    }
    
    bool operator==(const char* cstr) {
        if (!cstr) return false;
        if (_size != strlen(cstr)) return false;

        bool res = true;
        for (size_t i = 0; i < _size; i++) {
            if (_data[i] != cstr[i]) {
                res = false;
                break;
            }
        }
        return res;
        
    }

    /* methods */
    const char* c_str() const { 
        return _data;
    }

    void printString() {
        for (size_t i = 0; i < _size; i++) {
            printf("%c", _data[i]);
        }
        printf("\n");
        
    }

private:
    union {
        char* _data = nullptr; // allocated on heap
        char _sso[SSO_SIZE]; // part of object memory layout
    };

    size_t _size = 0;
    bool _isSSO;

};

