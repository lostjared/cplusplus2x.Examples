#include <coroutine>
#include <iostream>
#include <cstdlib>

template<typename T>
struct Counter {
    struct counter_promise {
        T value;
        auto yield_value(T val) -> std::suspend_always {
            value = val;
            return {};
        }
        auto initial_suspend() { return std::suspend_always{}; }
        auto final_suspend() noexcept { return std::suspend_always{}; }
        auto get_return_object() {
            // Corrected to return an instance of Counter
            return Counter{std::coroutine_handle<counter_promise>::from_promise(*this)};
        }
        void return_void() {}
        void unhandled_exception() {
            std::exit(EXIT_FAILURE);
        }
    };
    std::coroutine_handle<counter_promise> coro;
    explicit Counter(std::coroutine_handle<counter_promise> h) : coro{h} {}
    ~Counter() { if(coro) coro.destroy(); }
    Counter(const Counter &) = delete;
    Counter &operator=(const Counter &) = delete;
    Counter(Counter &&c) noexcept : coro{c.coro} { c.coro = nullptr; }
    Counter &operator=(Counter  &&c) noexcept {
        coro = c.coro;
        c.coro = nullptr;
        return *this;
    }

    T getValue() {
        return coro.promise().value;
    }

    bool next() {
        if(coro.done()) return false;
        coro.resume();
        return true;
    }
};

Counter<int> count_to(int n) {
    for(int i = 0; i < n; ++i)
        co_yield i;
}

int main() {
    Counter<int> cx = count_to(5); 
    while(cx.next()) {
        std::cout << "call: " << cx.getValue() << "\n";
    }
    return 0;
}