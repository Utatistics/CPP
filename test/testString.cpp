#include <cassert>
#include <iostream>

#include "../src/String.hpp"

void testString() {
    String s1("Hello");
    assert(strcmp(s1.c_str(), "Hello") == 0);
    
    // copy constructor
    String s2 = s1;
    assert(s2 == s1);

    // self assignment
    s1 = s1;
    assert(s1 == s2);

    // copy assignment
    String s3("World");
    s3 = s1;
    assert(s3 == s1);

    // move constructor
    String s4 = String("Temporary");
    assert(s4 == "Temporary");

    // move assignment
    String s5("A");
    s5 = String("BBBBB");
    assert(s5 == "BBBBB");

    // move
    String s6 = std::move(s5);
    assert(s6 == "BBBBB");
    std::cout << "String tests passed\n";
}
