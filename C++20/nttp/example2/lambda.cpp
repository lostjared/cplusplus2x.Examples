#include <iostream>

template <std::invocable auto rt>
void echoIndex() {
	std::cout << rt() << " is index.\n";
}

int main() {
	auto value = []() { return 255; };
	// use lambda as nttp
	echoIndex<value>();
	return 0;
}