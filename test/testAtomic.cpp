#include <cassert>
#include <iostream>

 #include "../src/Atomic.hpp"

void testAtomic() {
    // -------------------------------------------------
    // default construction
    // -------------------------------------------------
    Atomic<int> a;
    assert(a.load() == 0);

    // -------------------------------------------------
    // value construction
    // -------------------------------------------------
    Atomic<int> b(42);
    assert(b.load() == 42);

    // -------------------------------------------------
    // store / load
    // -------------------------------------------------
    b.store(100);
    assert(b.load() == 100);

    b.store(-7);
    assert(b.load() == -7);

    // -------------------------------------------------
    // assignment from value
    // -------------------------------------------------
    b = 55;
    assert(b.load() == 55);

    // -------------------------------------------------
    // implicit conversion operator
    // -------------------------------------------------
    int x = b;
    assert(x == 55);

    // -------------------------------------------------
    // exchange
    // -------------------------------------------------
    int old = b.exchange(99);

    assert(old == 55);
    assert(b.load() == 99);

    // -------------------------------------------------
    // compare_exchange_strong (success)
    // -------------------------------------------------
    int expected = 99;

    bool success = b.compare_exchange_strong(expected, 123);

    assert(success);
    assert(b.load() == 123);

    // expected should remain unchanged on success
    assert(expected == 99);

    // -------------------------------------------------
    // compare_exchange_strong (failure)
    // -------------------------------------------------
    expected = 50;

    success = b.compare_exchange_strong(expected, 777);

    assert(!success);

    // expected should be overwritten with actual value
    assert(expected == 123);

    assert(b.load() == 123);

    // -------------------------------------------------
    // fetch_add
    // returns old value
    // -------------------------------------------------
    Atomic<int> c(10);

    old = c.fetch_add(5);

    assert(old == 10);
    assert(c.load() == 15);

    old = c.fetch_add(1);

    assert(old == 15);
    assert(c.load() == 16);

    // -------------------------------------------------
    // fetch_sub
    // -------------------------------------------------
    old = c.fetch_sub(6);

    assert(old == 16);
    assert(c.load() == 10);

    // -------------------------------------------------
    // prefix increment
    // -------------------------------------------------
    assert(++c == 11);
    assert(c.load() == 11);

    // -------------------------------------------------
    // postfix increment
    // -------------------------------------------------
    assert(c++ == 11);
    assert(c.load() == 12);

    // -------------------------------------------------
    // prefix decrement
    // -------------------------------------------------
    assert(--c == 11);
    assert(c.load() == 11);

    // -------------------------------------------------
    // postfix decrement
    // -------------------------------------------------
    assert(c-- == 11);
    assert(c.load() == 10);

    // -------------------------------------------------
    // multiple stores
    // -------------------------------------------------
    c.store(1);
    c.store(2);
    c.store(3);

    assert(c.load() == 3);

    // -------------------------------------------------
    // bool specialization (should work automatically)
    // -------------------------------------------------
    Atomic<bool> flag(false);

    assert(flag.load() == false);

    flag.store(true);

    assert(flag.load() == true);

    bool previous = flag.exchange(false);

    assert(previous == true);
    assert(flag.load() == false);

    // -------------------------------------------------
    // exchange on bool
    // -------------------------------------------------
    previous = flag.exchange(true);

    assert(previous == false);
    assert(flag.load() == true);

    // -------------------------------------------------
    // compare_exchange on bool
    // -------------------------------------------------
    bool expectedFlag = true;

    success = flag.compare_exchange_strong(expectedFlag, false);

    assert(success);
    assert(flag.load() == false);

    expectedFlag = true;

    success = flag.compare_exchange_strong(expectedFlag, true);

    assert(!success);

    assert(expectedFlag == false);

    // -------------------------------------------------
    // const load
    // -------------------------------------------------
    const Atomic<int> d(500);

    assert(d.load() == 500);

    // -------------------------------------------------
    // compile-time API expectations
    // (these lines should compile)
    // -------------------------------------------------

    Atomic<int> e(1);

    e.store(2);
    e.load();
    e.exchange(5);

    int expectedValue = 5;
    e.compare_exchange_strong(expectedValue, 6);

    e.fetch_add(3);
    e.fetch_sub(2);

    ++e;
    e++;
    --e;
    e--;

    int value = e;
    (void)value;

    std::cout << "Atomic tests passed\n";
}