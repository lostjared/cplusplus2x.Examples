#include"scanner.hpp"
#include<fstream>
#include<sstream>
#include<iostream>
#include<memory>

extern int html_main(const char *filename, const char *outfilename);

int scanFile(const std::string &contents) {
    try {
        std::unique_ptr<scan::Scanner> scan(new scan::Scanner(scan::TString(contents)));
        uint64_t tokens = scan->scan();
        std::cout << "Sucessfull scan returned: " << tokens << " token(s)...\n";

        for(size_t i = 0; i < scan->size(); ++i) {
            scan->operator[](i).print(std::cout);
        }
        return static_cast<int>(tokens);
    } catch(scan::ScanExcept &e) {
        std::cerr << "Fatal error: " << e.why() << "\n";
    }
    return 0;
}

int main(int argc, char **argv) {
 
    if(argc != 2 && argc != 3) {
        std::cerr << "Error: requires one argument filename\n";
        exit(EXIT_FAILURE);
    }

    std::fstream file;
    file.open(argv[1], std::ios::in);
    std::ostringstream stream;
    stream << file.rdbuf();
    file.close();

    if(argc == 2) {
        if(stream.str().length()>0) {
            return scanFile(stream.str());
        }
    } else if(argc == 3) {
        html_main(argv[1], argv[2]);
    }
    return 0;
}