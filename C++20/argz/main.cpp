#include<iostream>
#include"argz.hpp"

int main(int argc, char **argv) {
    Argz<std::string> args;
    args.initArgs(argc, argv);
    args.addOptionSingle('o', "output");
    args.addOptionSingle('c', "code");
    args.addOptionSingle('v', "argument list");
    args.addOptionSingleValue('t', "type info");
    args.addOptionDouble(1, "test", "test arg");
    args.addOptionDoubleValue(2, "test2", "value 2");

    int value = 0;
    Argument<std::string> arg;
    
    try {
        while( (value = args.proc(arg)) != -1) {
            switch(value) {
                case 'o':
                std::cout << "o switch\n";
                break;
                case 'c':
                std::cout << "c switch\n";
                break;
                case 't':
                std::cout << "Type value: " << arg.arg_value << "\n";
                break;
                case 1:
                std::cout << "test switch\n";
                break;
                case 2:
                std::cout << "test 2 : " << arg.arg_value << "\n";
                break;
                case 'v':
                args.help();
                break;

            }
        }
    } catch(const ArgException<std::string> &e) {
        std::cerr << "Syntax Error: " << e.text() << "\n";
    }
    return 0;
}
