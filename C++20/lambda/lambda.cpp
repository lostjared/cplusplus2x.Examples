#include <iostream>
#include <string>

template <typename F>
void Fill(F func) {
	func(0);
	func("test");
	func(13.4);
	func(25.2f);
}

int main() {
	auto test = []<typename T>(const T &type) { std::cout << "template typename: " << type << "\n"; };
	Fill(test);
	Fill([](const auto &t) {
		if constexpr (std::is_same<decltype(t), const float &>::value) {
			std::cout << "filtered out floating point: " << t << "\n";
			return;
		}
		std::cout << "auto: " << t << "\n";
	});
	return 0;
}