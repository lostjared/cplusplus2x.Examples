/* Use GCC 13.2  or greater */
// Help from Chapter 15 C++ 20 The Complete Guide
#include <coroutine>
#include <cstdlib>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

template <typename T>
class Tokenizer {
public:
	template <typename Type>
	struct Promise {
		T value;
		auto get_return_object() { return std::coroutine_handle<Promise<Type>>::from_promise(*this); }
		auto initial_suspend() { return std::suspend_always{}; }
		auto final_suspend() noexcept { return std::suspend_always{}; }
		void unhandled_exception() { std::terminate(); }
		std::suspend_always yield_value(const T &val) {
			value = val;
			return {};
		}
		void return_value(const T &val) { value = val; }
	};
	using promise_type = Promise<Tokenizer<T>>;
	Tokenizer(auto h) : coro{h} {}
	~Tokenizer() {
		if (coro) {
			coro.destroy();
		}
	}
	Tokenizer(const Tokenizer<T> &) = delete;
	Tokenizer<T> &operator=(const Tokenizer<T> &) = delete;
	Tokenizer(Tokenizer<T> &&t) : coro{t.coro} {}
	Tokenizer<T> &operator=(Tokenizer<T> &&t) {
		coro = t.coro;
		return *this;
	}

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

// simple 'split' or like 'strtok'
template <typename T>
class Scanner {
public:
	explicit Scanner(const T &src, const T &seperator) : pos{}, source{src}, sep{seperator} {}
	void set(const T &src, const T &s) {
		source = src;
		sep = s;
		pos = 0;
	}
	Tokenizer<T> tokenizer() {
		auto s = source.find(sep, pos);
		while (s != std::string::npos) {
			std::string cur_token{source.substr(pos, s - pos)};
			pos = s + sep.length();
			if (cur_token.length() > 0)
				co_yield cur_token;
			s = source.find(sep, pos);
		}
		T last{source.substr(pos, source.length())};
		if (last.length() > 0)
			co_yield last;
	}

private:
	T::size_type pos;
	T source;
	T sep;
};

template <typename T>
auto collect(Tokenizer<T> &&tokens) -> std::vector<T> {
	std::vector<T> v;
	while (tokens.resume()) {
		v.push_back(tokens.getValue());
	}
	return v;
}

int main() {
	Scanner<std::string> scan("test one two three", " ");
	auto i{scan.tokenizer()};
	int index = 0;
	while (i.resume()) {
		++index;
		std::cout << "calling index: " << index << " -> " << i.getValue() << "\n";
	}
	scan.set("test::cpp::plus", "::");
	auto s{scan.tokenizer()};
	index = 0;
	while (s.resume()) {
		++index;
		std::cout << "index: " << index << " -> " << s.getValue() << "\n";
	}
	scan.set("test;one;two;three;;;", ";");
	auto v{collect(scan.tokenizer())};
	for (const auto &i : std::views::all(v)) {
		std::cout << "vector: " << i << "\n";
	}
	scan.set("abc->one->two->apples", "->");
	auto t{scan.tokenizer()};
	auto v2{collect(std::move(t))};
	for (const auto &i : std::views::all(v2)) {
		std::cout << i << "\n";
	}
	return 0;
}