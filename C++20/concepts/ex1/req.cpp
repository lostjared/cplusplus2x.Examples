#include<iostream>

template<typename T>
requires(!std::is_pointer_v<T>)
T sub(T one, T two) {
	return one - two;
}

int main(int argc, char **argv) {
	int x = sub(3, 1);
	std::cout << "value: " << x << "\n";
	int y = 20;
	// Following in a compile time error:
	// std::cout << sub(&x, &y) << "\n";
	return 0;
}
