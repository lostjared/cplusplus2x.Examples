#include <iostream>
#include <type_traits>
#include <vector>

template <typename C, typename T>
	requires std::same_as<typename C::value_type, T>
void push_back(C &cont, const T &value) {
	cont.push_back(value);
}

int main(int argc, char **argv) {
	std::vector v{0, 1, 2};
	push_back(v, 10);
	// push_back(v, 10.1); error narrow not same type
	std::cout << "vector with: \n";
	for (const auto &i : v) {
		std::cout << i << "\n";
	}
	return 0;
}
