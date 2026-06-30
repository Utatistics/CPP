#include <cassert>
#include <iostream>
#include <functional>
#include <string>

#include "../src/Callable.hpp"

// The test function
void testCallable() {
    // Test 1: Free function
    int freeFuncCallCount = 0;
    auto freeFunction = [&freeFuncCallCount]() { freeFuncCallCount++; };
    
    Callable cb1(freeFunction);
    cb1.call();
    assert(freeFuncCallCount == 1);  // Ensure it ran once

    // Test 2: Lambda with a string
    std::string result;
    auto lambda = [&result]() { result = "Hello, World!"; };
    
    Callable cb2(lambda);
    cb2.call();
    assert(result == "Hello, World!");  // Check that the lambda ran

    // Test 3: Member function
    class TestClass {
    public:
        int value = 0;
        void memberFunc() { value++; }
    };

    TestClass obj;
    Callable cb3([&obj]() { obj.memberFunc(); });
    cb3.call();
    assert(obj.value == 1);  // Ensure member function updated the value

    std::cout << "All tests passed!" << std::endl;
}

int main() {
    testCallable();
    return 0;
}