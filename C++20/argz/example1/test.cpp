#include<iostream>
#include"../argz.hpp"


int main(int argc, char **argv) {
    

    Argz<std::string> argz(argc, argv);
    argz.addOptionSingleValue('i', "input data");
    argz.addOptionSingle('h', "print out help");
    
    int value = 0;
    Argument<std::string> arg;
    try {
        while((value = argz.proc(arg)) != -1) {
            switch(value) {
                case 'h':
                    argz.help();
                    break;
                case 'i':
                    std::cout << "got input: " << arg.arg_value << "\n";
                    break;
            }
        }
    } catch(const ArgException<std::string>& e) {
        std::cerr << "Syntax Error: " << e.text() << "\n";
    }
    return 0;
}
