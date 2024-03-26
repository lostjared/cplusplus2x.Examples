#include "../argz.hpp"

int main(int argc, char **argv) {
	Argz<std::string> argz(argc, argv);
	argz.addOptionSingle('x', "test");
	Argz<std::string> argzx;
	argzx = argz;
	argzx.help(std::cout);
	Argz<std::string> argz3{std::move(argzx)};
	argz3.help(std::cout);
	Argz<std::string> test4;
	test4 = std::move(argz3);
	test4.help(std::cout);
	return 0;
}