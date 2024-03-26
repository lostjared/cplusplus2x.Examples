#include <iostream>
#include <string>

// can now pass constexpr struct to template parameter
struct FValue {
	float val = 0;
	constexpr FValue(float f) : val{f} {}
};

template <FValue value>
void Concat(float val) {
	std::cout << value.val + val << "\n";
}

// auto type
template <auto y>
void increase(double x) {
	std::cout << y + x << "\n";
}

int main() {
	constexpr FValue f{10.5f};
	Concat<f>(10.0);
	return 0;
}