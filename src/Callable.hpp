// Minimal Callable class (for demo purposes)
class Callable {
public:
    // Base class for type erasure
    struct Base {
        virtual ~Base() = default;
        virtual void call() = 0;
    };

    template <typename F>
    struct Derived : Base {
        F func;
        Derived(F f) : func(std::move(f)) {}
        void call() override { func(); }
    };

    template <typename F>
    Callable(F f) : impl(new Derived<F>(std::move(f))) {}

    void call() {
        impl->call();
    }

private:
    std::unique_ptr<Base> impl;
};

