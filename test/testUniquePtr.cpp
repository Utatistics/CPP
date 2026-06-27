#include <cassert>
#include <iostream>
#include <string>

#include "../src/UniquePtr.hpp"

void testUniquePtr() {
    UniquePtr<std::string> u1(new std::string("Alice"));

    assert(*u1 == "Alice");
    assert(u1->size() == 5);
    assert((bool)u1 == true);

    // move constructor
    UniquePtr<std::string> u2 = std::move(u1);
    assert(*u2 == "Alice");
    assert((bool)u2 == true);
    assert((bool)u1 == false);


    // move assignment
    UniquePtr<std::string> u3;
    u3 = std::move(u2);
    assert(*u3 == "Alice");
    assert((bool)u3 == true);
    assert((bool)u2 == false);

    // reset
    u3.reset(new std::string("Bob"));
    assert(*u3 == "Bob");
    assert((bool)u3 == true);

    // release
    std::string* raw = u3.release();
    assert(*raw == "Bob");
    assert((bool)u3 == false);
    delete raw;


    std::cout << "UniquePtr tests passed\n";
}
