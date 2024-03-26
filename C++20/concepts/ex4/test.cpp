#include <iostream>
#include <string>
#include <type_traits>

template <typename T>
concept Object = std::is_class_v<T>;

template <Object O>
void echo(const O &o) {
	std::cout << "Object: " << o << "\n";
}

template <typename T>
void echo(const T &t) {
	std::cout << "Non Object: " << t << "\n";
}

class Obj {
public:
	Obj() = default;
	Obj(int valuex) : value{valuex} {}
	int getValue() const { return value; }
	friend std::ostream &operator<<(std::ostream &out, const Obj &o) {
		out << "{" << o.getValue() << "} ";
		return out;
	}

private:
	int value;
};

int main() {
	Obj object{255};
	echo(object);
	echo(10);
	echo(25.0f);
	return 0;
}
