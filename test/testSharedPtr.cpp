#include <cassert>
#include <iostream>
#include <string>

#include "../src/SharedPtr.hpp"

void testSharedPtr() {
    SharedPtr<std::string> s1(
        new std::string("Hello")
    );

    assert(*s1 == "Hello");
    assert(s1.use_count() == 1);
    SharedPtr<std::string> s2 = s1;

    assert(s1.use_count() == 2);
    assert(s2.use_count() == 2);

    {
        SharedPtr<std::string> s3 = s1;
        assert(s1.use_count() == 3);
        assert(*s3 == "Hello");
    }


    assert(s1.use_count() == 2);
    SharedPtr<std::string> s4;
    s4 = s1;
    assert(s1.use_count() == 3);
    assert(s4.use_count() == 3);

    // self assignment
    s4 = s4;
    assert(s4.use_count() == 3);
    s4.reset(new std::string("World"));
    assert(*s4 == "World");
    assert(s4.use_count() == 1);


    // old object survives
    assert(*s1 == "Hello");
    assert(s1.use_count() == 2);
    SharedPtr<std::string> keepAlive;
    {
        SharedPtr<std::string> temp(
            new std::string("Still alive")
        );
        keepAlive = temp;
        assert(temp.use_count() == 2);
    }

    assert(*keepAlive == "Still alive");
    assert(keepAlive.use_count() == 1);


    std::cout << "SharedPtr tests passed\n";
}
