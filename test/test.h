#pragma once

/* Standard Template Library */
void testString();
void testOptional();
void testVector();
void testUniquePtr();
void testSharedPtr();
void testFunction();
void testAny();

/* Quant Dev Mock Interview */
void testCallable();
void testTaskScheduler();
void testEventBus();
void testSignal();
void testAllocator();

/*
Based on this description, can you help me generate the interview problem? 
You may create a test function demonstrating the expected usage or provide
 a class header that serves as the specification. 
 If you choose the class header approach, make sure it doesn't reveal 
 too much about the implementation. 
 
 Overall, I'd like the problem to resemble a C++ interview question
for a quant developer position at a hedge fund or similar firm.

test function can follow the below foramt.
>>>
#include <cassert>

#include "../src/Optional.hpp"

void testOptional() {
    Optional<double> opt;
    assert(!opt.has_value());
    
    opt = 3.14;
    assert(opt.has_value());
    
    assert(opt.value() == 3.14);
    
    opt.reset();
    assert(!opt.has_value());

    std::cout << "Optional tests passed\n";
}
<<<

Make sure to produce your test code in one go
*/