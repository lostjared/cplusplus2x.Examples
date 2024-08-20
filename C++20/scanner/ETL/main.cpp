#include"scanner.hpp"
#include"argz.hpp"
#include"types.hpp"


extern void test_parse(const std::string &);

int main(int argc, char **argv)  {
    Argz<std::string> argz(argc, argv);
    argz.addOptionSingleValue('i', "input text").addOptionSingleValue('o', "output file").addOptionSingle('h', "help").addOptionSingle('v', "help");
    std::string in_file, out_file;
    int value = 0;

    Argument<std::string> arg;
    try {
        while((value = argz.proc(arg)) != -1) {
            switch(value) {
                case 'h':
                case 'v':
                    argz.help(std::cout);
                    exit(EXIT_SUCCESS);
                    break;
                case 'i':
                    in_file = arg.arg_value;
                    break;
                case 'o':
                    out_file = arg.arg_value;
                    break;
            }
        }
    } catch(const ArgException<std::string> &e) {
        std::cerr << "Syntax Error: " << e.text() << "\n";
    }

    if(in_file.length() == 0) {
        std::cerr << "Input file must be provided...use -i \n";
        exit(EXIT_FAILURE);
    }

    test_parse(in_file);

    return 0;
}