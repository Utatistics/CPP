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
