/*
 use g++-13 not support on Clang yet
 $ CXX=g++-13 make
*/

#include <format>
#include <iostream>

int main(int argc, char **argv) {
	std::cout << std::format("Hello, World {:d}\n", 10);
	for (int i = 0; i < 10; ++i) {
		std::cout << std::format("{:d} -> Hello Index\n", i);
	}
	return 0;
}