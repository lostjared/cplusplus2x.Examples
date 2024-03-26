#include <iostream>
#include <string>
#include <type_traits>

template <typename T>
concept Output = requires(T type) {
	{ std::cout << type } -> std::same_as<std::ostream &>;
};

template <Output T>
void writeln(const T &type) {
	std::cout << type << "\n";
}

template <Output T, Output... Args>
void writeln(const T &type, Args... args) {
	std::cout << type;
	writeln(args...);
}

class Err {};

class Type {
public:
	Type(int t) : type{t} {}
	int getType() const { return type; }

private:
	int type;
};

std::ostream &operator<<(std::ostream &out, const Type &type) {
	out << "[" << type.getType() << "]";
	return out;
}

int main() {
	writeln("Hello, ", "World! value is: ", 25);
	// writeln("Hello Error:", Err{}); // ERR
	writeln("Hello, ", " Foo! ", Type{100});
	return 0;
}
