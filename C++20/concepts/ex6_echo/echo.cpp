#include <iostream>
#include <string>
#include <type_traits>

template <typename T>
concept Object = std::is_class_v<T> && requires(T t) {
	{ t.toString() } -> std::same_as<std::string>;
};

template <Object O>
void echo(const O o) {
	std::cout << o.toString();
}

template <Object O, Object... Obj>
void echo(const O &o, Obj... obj) {
	std::cout << o.toString();
	echo(obj...);
}

class DataType {
public:
	DataType(const std::string &n) : name{n} {}
	std::string toString() const { return name + " -> [value of Type]\n"; }

private:
	std::string name;
};

int main() {
	DataType type1{"hello, "}, type2{"world!"}, type3{" :) "};
	echo(type1, type2, type3);
	// echo(type1, " Hey ", 10); // ERR "Hey" doesn't satisfy requriment
	return 0;
}
