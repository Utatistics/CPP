#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include "../src/TaskScheduler.hpp"

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
