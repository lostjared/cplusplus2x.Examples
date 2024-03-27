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
		std::suspend_always yield_value(T val) {
			value = val;
			return {};
		}
		void return_value(const auto &val) { value = val; }
	};
	using promise_type = Promise<Task<T>>;
	Task(auto h) : coro{h} {}
	~Task() {
		if (coro) {
			coro.destroy();
		}
	}
	Task(const Task<T> &) = delete;
	Task<T> &operator=(const Task<T> &) = delete;

	bool resume() const {
		if (!coro) {
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
		cur_token = "Bruni, ";
		co_await std::suspend_always();
		cur_token += " Jared";
		co_await std::suspend_always();
		cur_token += " is a Cool guy";
		co_return cur_token;
	}

private:
	T cur_token;
};

int main() {
	Scanner<std::string> scan;
	auto i{scan.getNext()};
	int index = 0;
	while (i.resume()) {
		++index;
		std::cout << "calling index: " << index << "\n";
	}
	std::cout << i.getValue() << " is the completed value..\n";
}