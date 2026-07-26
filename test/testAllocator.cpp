#include <cassert>
#include <iostream>

#include "../src/Allocator.hpp"

int testAllocator() {
    /* constroctor  */
    Allocator alloc(128);

    /* basic alloc and dealloc  */
    auto* a = alloc.allocate(16);
    auto* b = alloc.allocate(24);
    auto* c = alloc.allocate(8);

    assert(alloc.owns(a));
    assert(alloc.owns(b));
    assert(alloc.owns(c));

    alloc.deallocate(b);

    auto* d = alloc.allocate(20);

    assert(alloc.owns(d));
    assert(!alloc.owns(b));

    /*allocaotr needs to merge adj blocks */ 
    alloc.deallocate(a);
    alloc.deallocate(c);
    alloc.deallocate(d);
    auto* e = alloc.allocate(120); 
    assert(e != nullptr);

    alloc.deallocate(e);

    std::cout << "Passed\n";
}