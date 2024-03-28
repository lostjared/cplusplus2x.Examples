/* Use GCC 13.2  or greater */
// Help from Chapter 15 C++ 20 The Complete Guide
#include <coroutine>
#include <cstdlib>
#include <iostream>

class Task {
public:
	template <typename Type>
	struct Promise {
		int value = 0;
		auto get_return_object() { return std::coroutine_handle<Promise<Type>>::from_promise(*this); }
		auto initial_suspend() { return std::suspend_always{}; }
		auto final_suspend() noexcept { return std::suspend_always{}; }
		void unhandled_exception() { std::terminate(); }
		void return_void() {}
		std::suspend_always yield_value(const int &val) {
			value = val;
			return {};
		}
	};
	using promise_type = Promise<Task>;
	Task(auto h) : coro{h} {}
	~Task() {
		if (coro) {
			coro.destroy();
		}
	}
	Task(const Task &) = delete;
	Task &operator=(const Task &) = delete;

	bool resume() const {
		if (!coro) {
			return false;
		}
		coro.resume();
		return !coro.done();
	}

	int getValue() { return coro.promise().value; }

private:
	std::coroutine_handle<promise_type> coro;
};

Task count_to(int n) {
	for (int i = 0; i < n; ++i) {
		co_yield (i);
	}
}

int main() {
	auto i = count_to(5);
	while (i.resume()) {
		std::cout << "call -> " << i.getValue() << "\n";
	}
}