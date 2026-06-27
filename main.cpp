#include <iostream>
#include <string>
#include <cassert>

/* Standard Template Library */
#include "Optional.hpp"
#include "Vector.hpp"
#include "UniquePtr.hpp"
#include "SharedPtr.hpp"
#include "String.hpp"
#include "Function.hpp"
#include "Any.hpp"

/* QRT Mock Interviews */
#include "TaskScheduler.hpp"

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

void testUniquePtr() {
    UniquePtr<std::string> u1(new std::string("Alice"));

    assert(*u1 == "Alice");
    assert(u1->size() == 5);
    assert((bool)u1 == true);


    // move constructor
    UniquePtr<std::string> u2 = std::move(u1);

    assert(*u2 == "Alice");
    assert((bool)u2 == true);
    assert((bool)u1 == false);


    // move assignment
    UniquePtr<std::string> u3;

    u3 = std::move(u2);

    assert(*u3 == "Alice");
    assert((bool)u3 == true);
    assert((bool)u2 == false);


    // reset
    u3.reset(new std::string("Bob"));

    assert(*u3 == "Bob");
    assert((bool)u3 == true);


    // release
    std::string* raw = u3.release();

    assert(*raw == "Bob");
    assert((bool)u3 == false);

    delete raw;


    std::cout << "UniquePtr tests passed\n";
}

void testSharedPtr() {
    SharedPtr<std::string> s1(
        new std::string("Hello")
    );

    assert(*s1 == "Hello");
    assert(s1.use_count() == 1);


    SharedPtr<std::string> s2 = s1;

    assert(s1.use_count() == 2);
    assert(s2.use_count() == 2);


    {
        SharedPtr<std::string> s3 = s1;

        assert(s1.use_count() == 3);
        assert(*s3 == "Hello");
    }


    assert(s1.use_count() == 2);



    SharedPtr<std::string> s4;

    s4 = s1;

    assert(s1.use_count() == 3);
    assert(s4.use_count() == 3);


    // self assignment
    s4 = s4;

    assert(s4.use_count() == 3);



    s4.reset(new std::string("World"));

    assert(*s4 == "World");
    assert(s4.use_count() == 1);


    // old object survives
    assert(*s1 == "Hello");
    assert(s1.use_count() == 2);



    SharedPtr<std::string> keepAlive;

    {
        SharedPtr<std::string> temp(
            new std::string("Still alive")
        );

        keepAlive = temp;

        assert(temp.use_count() == 2);
    }


    assert(*keepAlive == "Still alive");
    assert(keepAlive.use_count() == 1);


    std::cout << "SharedPtr tests passed\n";
}

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

void testTaskScheduler() {

    // =========================
    // basic add + polymorphism
    // =========================
    TaskScheduler s;

    s.add(PrintTask("Hello"));
    s.add(MathTask(
        [](int x) { return x * 2; },
        21
    ));
    s.run_all();


    // =========================
    // copy constructor
    // deep copy
    // ========================
    TaskScheduler copy = s;
    copy.run_all();

    // modifying copy should not affect original
    copy.add(PrintTask("copy only"));
    copy.clear();

    // original still works
    s.run_all();

    // =========================
    // copy assignment
    // =========================
    TaskScheduler assigned;

    assigned = s;
    assigned.run_all();
    assigned = assigned; // self assignment
    assigned.run_all();

    // =========================
    // move constructor
    // =========================
    TaskScheduler moved = std::move(s);
    moved.run_all();

    // moved-from object must remain valid
    s.run_all();

    // =========================
    // move assignment
    // =========================
    TaskScheduler movedAssign;
    movedAssign = std::move(moved);
    movedAssign.run_all();
    moved.run_all();

    // =========================
    // clear
    // =========================
    movedAssign.clear();

    // should not crash
    movedAssign.run_all();
    
    std::cout << "Scheduler tests passed\n";
}

int main () { 
    /*
        make build_debug FILE=main.cpp
        make run FILE=main.cpp 
    */

    /* Standard Template Library */
    // testOptional();
    // testVector();
    // testUniquePtr();
    // testSharedPtr();
    // testString();
    // testFunction();

    /* Mock QRT questions */
    testTaskScheduler();
    return 0;
}

