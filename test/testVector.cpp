#include <cassert>
#include <iostream>
#include <string>

#include "../src/Vector.hpp"

void testVector() {
    Vector<std::string> a;

    a.push_back("Alice");
    a.push_back("Bob");

    assert(a.size() == 2);
    assert(a[0] == "Alice");
    assert(a[1] == "Bob");

    // copy constructor
    Vector<std::string> b = a;
    assert(b.size() == 2);
    assert(b[0] == "Alice");

    // copy assignment
    Vector<std::string> c;
    c = a;
    assert(c.size() == 2);
    assert(c[1] == "Bob");


    // fill constructor
    Vector<std::string> d(5);
    for (int i = 0; i < 5; i++)
    {
        d[i] = "abc";
    }
    for (int i = 0; i < 5; i++)
    {
        assert(d[i] == "abc");
    }
    Vector<std::string> e(5, "def");
    for (int i = 0; i < 5; i++)
    {
        assert(e[i] == "def");
    }
    std::cout << "Vector tests passed\n";
}

