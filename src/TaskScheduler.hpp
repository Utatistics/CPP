#include <iostream>
#include <vector>
#include <functional>

class ITask {
public:
    virtual ~ITask() = default;
    virtual void execute() = 0;
    virtual ITask* clone() const = 0;
};

class PrintTask : public ITask {
/*
    Stores a string and prints it when executed.
*/
public:
    /*  constuctor */
    PrintTask(const char * str = nullptr) { // deafault if nullptr
        _m_size = 0;
        if (str) {
            while (str[_m_size] != '\0') _m_size++;
        }
        
        _m_string = new char[_m_size + 1];
        for (size_t i = 0; i < _m_size; i++) {
            _m_string[i] = str[i];
        }
    }  

    /* method */
    void execute () override {
        for (size_t i = 0; i < _m_size; i++)
        {
            std::cout << _m_string[i];
        }
        std::cout << '\n';
    }

    ITask* clone () const { // deep copy
        PrintTask* task = new PrintTask(nullptr);

        task->_m_size = _m_size;
        for (size_t i = 0; i < task->_m_size; i++) {
            task->_m_string[i] = _m_string[i];
        }
        
        return task;
    }

private:
    char* _m_string;
    size_t _m_size;

};

class MathTask : public ITask {
/*
    Stores:
     - a callable or logic (your choice of representation, but must be self-contained)
    - an integer input
    When executed, it prints or computes a result based on the stored logic.
*/
public:
    MathTask(std::function<int(int)> f, int x)
     : _m_callable(f), _m_input(x) {}

    void execute() override {
        std::cout << _m_callable(_m_input) << std::endl;
    }

    ITask* clone() const override { // deep copy
        MathTask* task = new MathTask(_m_callable, _m_input);
        return task;
    }

private:
    std::function<int(int)> _m_callable;
    int _m_input;
};

class TaskScheduler {
/*
    $ Storage Requirements
    The scheduler must store tasks polymorphically
    You may NOT store ITask objects directly, no slicing (to lose derived class part when copying)
    You must manage lifetime manually (no shared_ptr, unique_ptr unless explicitly justified and implemented correctly)

    You are expected to choose an internal representation such as:

    raw pointer array
    custom dynamic array
    or equivalent manual memory structure

    $ Semtantics
    add(task)
        Stores a deep copy of the provided task using clone()
    run_all()
        Executes all stored tasks in order
    clear()
        Frees all owned memory

*/
public:
    /* constructor */
    TaskScheduler() : _m_num_of_tasks(0), _m_tasks({}) {} // default constructor

    TaskScheduler(const TaskScheduler& other)  // copy constructor
    {
        _m_num_of_tasks = other._m_num_of_tasks;
        for (auto task : other._m_tasks)
        {
            _m_tasks.push_back(task);
        }
    }

    TaskScheduler(TaskScheduler&& other) noexcept  // move constructor
    { 
        _m_num_of_tasks = other._m_num_of_tasks;
        for (auto task : _m_tasks)
        {
            delete task;
        }
        for (auto& task : other._m_tasks)
        {
            _m_tasks.push_back(task);
        }

        other._m_num_of_tasks = 0;
        for (size_t i = 0; i < _m_num_of_tasks; i++)
        {
            other._m_tasks[i] = nullptr;
        }
    }

    /* destructor */
    ~TaskScheduler() {
        for (auto task : _m_tasks)
        {
            delete task;
        }
            
    }

    /* operator overload */
    TaskScheduler& operator=(const TaskScheduler& other) // copy assignment operator
    {
        if (this == &other) return *this;
        _m_num_of_tasks = other._m_num_of_tasks;

        for (auto task : other._m_tasks)
        {
            _m_tasks.push_back(task);
        }
        
        return *this;
    }

    TaskScheduler& operator=(TaskScheduler&& other) noexcept // move assignment operator
    {
        if (this == &other) return *this;
      
        for (auto task : _m_tasks)
        {
          delete task;
        }
    
        for (auto& task : other._m_tasks)
        {
          _m_tasks.push_back(task);    
        }
    
        other._m_num_of_tasks = 0;
        for (size_t i = 0; i < _m_num_of_tasks; i++)
        {
            other._m_tasks[i] = nullptr;
        }

        return *this;

    }

    /* method */
    void add(const ITask& task) {
        _m_tasks.push_back(task.clone());
    }

    void run_all() {
        for (ITask* task : _m_tasks) {
            task->execute();
        }
    }

    void clear() {
        for (auto task : _m_tasks)
        {
            delete task;
        }
        _m_num_of_tasks = 0;
        _m_tasks.clear();
    }

private: 
    std::vector<ITask*> _m_tasks;
    size_t _m_num_of_tasks;
};