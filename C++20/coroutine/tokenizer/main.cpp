/* Use GCC 13.2  or greater */
// Help from Chapter 15 C++ 20 The Complete Guide
#include "tokenizer.hpp"

int main() {
	Scanner<std::string> scan("test one two three", " ");
	auto i{scan.tokenizer()};
	int index = 0;
	while (i.next()) {
		++index;
		std::cout << "calling index: " << index << " -> " << i.getToken() << "\n";
	}
	scan.set("test::cpp::plus", "::");
	auto s{scan.tokenizer()};
	index = 0;
	while (s.next()) {
		++index;
		std::cout << "index: " << index << " -> " << s.getToken() << "\n";
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