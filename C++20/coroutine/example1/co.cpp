/* Use GCC 13.2  or greater */
// Help from Chapter 15 C++ 20 The Complete Guide
#include <coroutine>
#include <cstdlib>
#include <iostream>
#include <string>

template <typename T>
class Task {
public:
	template <typename Type>
	struct Promise {
		T value;
		auto get_return_object() { return std::coroutine_handle<Promise<Type>>::from_promise(*this); }
		auto initial_suspend() { return std::suspend_always{}; }
		auto final_suspend() noexcept { return std::suspend_always{}; }
		void unhandled_exception() { std::terminate(); }
		void return_void() {}
		std::suspend_always yield_value(const T &val) {
			value = val;
			return {};
		}
	};
	using promise_type = Promise<Task<T>>;
	Task(auto h) : coro{h} {}
	~Task() {
		if(coro) {
			coro.destroy();
		}
	}
	Task(const Task<T> &) = delete;
	Task<T> &operator=(const Task<T> &) = delete;

	bool resume() const {
		if(!coro) {
			return false;
		}
		coro.resume();
		return !coro.done();
	}
	T getValue() { return coro.promise().value; }

private:
	std::coroutine_handle<promise_type> coro;
};

// coroutines would work perfect for a text scanner
template <typename T>
class Scanner {
public:
	Task<T> getNext() {
		while(cur_token.length() < 5) {
			cur_token += T{"x"};
			co_yield cur_token;
		}
	}

private:
	T cur_token;
};

int main() {
	Scanner<std::string> scan;
	auto i{scan.getNext()};
	while(i.resume()) {
		std::cout << "call -> " << i.getValue() << "\n";
	}
}