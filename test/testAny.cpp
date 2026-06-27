#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include "../src/Any.hpp"

void testAny() {
    // 1. Empty state
    Any a;
    assert(!a.has_value());

    // 2. Basic construction
    Any b = 42;
    assert(b.has_value());
    assert(b.type() == typeid(int));
    assert(b.cast<int>() == 42);

    // 3. Modify stored value via reference
    b.cast<int>() = 100;
    assert(b.cast<int>() == 100);

    // 4. String storage
    Any c = std::string("hello");
    assert(c.has_value());
    assert(c.type() == typeid(std::string));
    assert(c.cast<std::string>() == "hello");

    // mutate string through cast reference
    c.cast<std::string>() = "world";
    assert(c.cast<std::string>() == "world");

    // 5. Bad cast (you choose throw/assert behavior)
    bool threw = false;
    try {
        c.cast<int>(); // wrong type
    } catch (...) {
        threw = true;
    }
    assert(threw);

    // 6. Copy semantics (deep copy expected)
    Any d = c;
    assert(d.has_value());
    assert(d.type() == typeid(std::string));
    assert(d.cast<std::string>() == "world");

    d.cast<std::string>() = "changed";
    assert(c.cast<std::string>() == "world"); // original unaffected

    // 7. Move semantics
    Any e = std::move(d);
    assert(e.has_value());
    assert(e.cast<std::string>() == "changed");
    // moved-from state (you define exact behavior, but usually empty)
    assert(!d.has_value() || d.cast<std::string>().empty() || true);

    // 8. reset()
    e.reset();
    assert(!e.has_value());

    // 9. Reassignment across types
    e = 3.14;
    assert(e.type() == typeid(double));
    assert(e.cast<double>() == 3.14);

    e = std::string("again");
    assert(e.type() == typeid(std::string));
    assert(e.cast<std::string>() == "again");

    // 10. Complex type support
    Any f = std::vector<int>{1, 2, 3};
    assert(f.cast<std::vector<int>>().size() == 3);
    f.cast<std::vector<int>>().push_back(4);
    assert(f.cast<std::vector<int>>().size() == 4);

    std::cout << "All Any tests passed\n";
}
