#include <cassert>
#include <iostream>
#include <string>

#include "../src/Function.hpp"

int add(int a, int b) { return a + b; }

void testFunction() {
    // 1. empty state
    Function<int(int,int)> f1;
    assert(!f1); // should be false when empty

    // 2. lambda assignment
    Function<int(int,int)> f2;
    f2 = [](int a, int b) { return a + b; };

    assert(f2(2,3) == 5);
    assert(f2(10,20) == 30);
    assert(static_cast<bool>(f2) == true);

    // 3. copy construction (shared behavior expected)
    Function<int(int,int)> f3 = f2;
    assert(f3(3,4) == 7);
    assert(f3(100,200) == 300);

    // modify original, copy should remain independent
    f2 = [](int a, int b) { return a * b; };
    assert(f2(3,4) == 12);
    assert(f3(3,4) == 7);

    // 4. copy assignment
    Function<int(int,int)> f4;
    f4 = f3;
    assert(f4(5,6) == 11);

    // 5. move construction
    Function<int(int,int)> f5 = std::move(f4);
    assert(f5(7,8) == 15);
    assert(static_cast<bool>(f5) == true);

    // moved-from object should be empty (typical std::function behavior)
    assert(!f4);

    // // 6. move assignment
    Function<int(int,int)> f6;
    f6 = [](int a, int b) { return a - b; };

    Function<int(int,int)> f7;
    f7 = std::move(f6);

    assert(f7(10,3) == 7);
    assert(!f6);

    // 7. storing function pointer
    Function<int(int,int)> f8 = add;
    assert(f8(2,2) == 4);

    // 8. reset behavior
    f8 = nullptr;
    assert(!f8);

    std::cout << "Function tests passed\n";
}
