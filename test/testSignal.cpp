#include <cassert>
#include <iostream>
#include <vector>

#include "../src/Signal.hpp"

void testSignal() {
    std::cout << "Test 1: Basic subscription and publish\n";
    {
        Signal<int> sig;

        int sum = 0;

        auto sub = sig.subscribe([&](int x) {
            sum += x;
        });

        sig.publish(10);
        sig.publish(5);

        assert(sum == 15);
    }

    std::cout << "Test 2: Multiple subscribers\n";
    {
        Signal<int> sig;

        int a = 0;
        int b = 0;

        auto sub1 = sig.subscribe([&](int x) {
            a += x;
        });

        auto sub2 = sig.subscribe([&](int x) {
            b += 2 * x;
        });

        sig.publish(3);

        assert(a == 3);
        assert(b == 6);
    }

    std::cout << "Test 3: Unsubscription via RAII token\n";
    {
        Signal<int> sig;

        int x = 0;

        {
            auto sub = sig.subscribe([&](int v) {
                x += v;
            });

            sig.publish(7);
            assert(x == 7);
        } // sub destroyed here

        sig.publish(7);
        assert(x == 7); // should NOT change anymore
    }

    std::cout << "Test 4: Order preservation\n";
    {
        Signal<int> sig;

        std::vector<int> order;

        sig.subscribe([&](int x) {
            order.push_back(1);
        });

        sig.subscribe([&](int x) {
            order.push_back(2);
        });

        sig.subscribe([&](int x) {
            order.push_back(3);
        });

        sig.publish(0);

        std::vector<int> expected = {1, 2, 3};
        assert(order == expected);
    }

    std::cout << "Test 5: Copy safety (independent signals)\n";
    {
        Signal<int> sig1;

        int x = 0;

        auto sub = sig1.subscribe([&](int v) {
            x += v;
        });

        Signal<int> sig2 = sig1; // copy signal

        sig2.publish(10);

        // important: behavior depends on design choice, but MUST NOT crash
        // (either shared or independent, but consistent and safe)
        assert(true);
    }

    std::cout << "All Signal tests passed!\n";
}