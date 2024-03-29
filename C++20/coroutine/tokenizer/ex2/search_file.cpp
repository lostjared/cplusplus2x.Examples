#include "../tokenizer.hpp"
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

std::string readStringFromFile(const std::string &name) {
	std::ifstream file{name, std::ios::in | std::ios::binary | std::ios::ate};
	if(!file.is_open()) {
		throw std::runtime_error("Could not open file: " + name);
	}
	auto len{file.tellg()};
	if(len == -1) {
		throw std::runtime_error("Could not get length of the file: " + name);
	}
	std::string temp;
	temp.resize(len);
	file.seekg(0, std::ios::beg);
	file.read(&temp[0], len);
	if(!file) {
		throw std::runtime_error("Error reading file: " + name);
	}
	return temp;
}

int main(int argc, char **argv) {
	if(argc != 3) {
		std::cerr << "Error requires one argument, file to search.\n";
		exit(EXIT_FAILURE);
	}
	std::string data{readStringFromFile(argv[1])};
	std::string search{argv[2]};
	Scanner<std::string> scanner(search, " ");
	auto tokens{collect(scanner.tokenizer())};
	if(tokens.size() > 0) {
		for(const auto &s : std::views::all(tokens)) {
			if(data.find(s) != std::string::npos) {
				std::cout << argv[1] << ": Found search substring: " << s << "\n";
			}
		}
	}
	return 0;
}