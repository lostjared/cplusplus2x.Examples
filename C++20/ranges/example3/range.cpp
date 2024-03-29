// use g++ 13

#include <iostream>
#include <ranges>
#include <sstream>
#include <string>

int main() {
	std::ranges::iota_view v_range{1, 100};
	for(auto val : v_range) { // 1-99
		std::cout << val << "\n";
	}
	for(auto val2 : std::ranges::iota_view('a', 'z' + 1)) {
		std::cout << val2 << "\n";
	}
	std::istringstream values{"25 25 100"};
	for(const auto &i : std::ranges::istream_view<int>(values)) {
		std::cout << i << " + 1 = " << i + 1 << "\n";
	}
	std::string_view s{"test1"}; // string view is a view
	for(auto &i : s) {
		std::cout << "ch: " << i << "\n";
	}
	for(auto &i : std::views::take(s, 2)) {
		std::cout << "ch index:" << i << "\n";
	}
	return 0;
}