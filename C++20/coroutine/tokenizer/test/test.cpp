#include "../tokenizer.hpp"
#include <iostream>
#include <random>
#include <string>

void fillBuffer(auto &en, auto &char_dist, auto &length, std::string &s) {
	auto len{length(en)};
	for(int i = 0; i < len; ++i) {
		s += char_dist(en);
		if(length(en) > 100)
			s += " ";
		if(length(en) > 200)
			s += " ";
	}
}

void testScanner(auto &en, auto &char_dist, auto &length, Scanner<std::string> &scan) {
	std::string random_string;
	fillBuffer(en, char_dist, length, random_string);
	scan.set(random_string, " ");
	auto tokens{collect(scan.tokenizer())};
	int index{};
	std::cout << "random string: " << random_string << "\n";
	for(const auto &i : tokens) {
		++index;
		std::cout << "index[" << index << "] -> " << i << "\n";
	}
}

int main() {
	try {
		std::mt19937 en{std::random_device{}()};
		std::uniform_int_distribution<int> char_dist{'a', 'z'};
		std::uniform_int_distribution<int> length{0, 255};

		Scanner<std::string> scanner;
		constexpr static int count{5000};
		for(int index = 0; index < count; ++index) {
			std::cout << "running test index: " << index << "\n";
			testScanner(en, char_dist, length, scanner);
		}
		std::cout << "Passed test no exceptions thrown or errors occoured.\n";
	} catch(const std::runtime_error &e) {
		std::cerr << "Runtime Error: " << e.what() << "\n";
	}
	return 0;
}