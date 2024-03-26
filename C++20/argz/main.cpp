#include "argz.hpp"
#include <iostream>

int main(int argc, char **argv) {
	Argz<std::string> args;
	args.initArgs(argc, argv)
	    .addOptionSingle('o', "output")
	    .addOptionSingle('c', "code")
	    .addOptionSingle('v', "argument list")
	    .addOptionSingleValue('t', "type info")
	    .addOptionDouble('T', "test", "test arg")
	    .addOptionDoubleValue('I', "test2", "value 2");

	int value{};
	Argument<std::string> arg;
	try {
		while ((value = args.proc(arg)) != -1) {
			switch (value) {
			case 'o':
				std::cout << "o switch\n";
				break;
			case 'c':
				std::cout << "c switch\n";
				break;
			case 't':
				std::cout << "Type value: " << arg.arg_value << "\n";
				break;
			case 'T':
				std::cout << "test switch\n";
				break;
			case 'I':
				std::cout << "test2: " << arg.arg_value << "\n";
				break;
			case '-':
				std::cout << "No arg: " << arg.arg_value << "\n";
				break;
			case 'v':
				args.help(std::cout);
				break;
			}
		}
	} catch (const ArgException<std::string> &e) {
		std::cerr << "Syntax Error: " << e.text() << "\n";
	}
	return 0;
}
