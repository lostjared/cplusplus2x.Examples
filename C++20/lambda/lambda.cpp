#include <iostream>
#include <string>

template <typename F>
void Fill(F func) {
	func(0);
	func("test");
	func(13.4);
}

int main() {
	auto test = []<typename T>(const T &type) { std::cout "template typename: " << type << "\n"; };
	Fill(test);
	Fill([](const auto &t) { std::cout << "auto: " << t << "\n"; });
	return 0;
}