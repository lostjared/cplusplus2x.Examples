#include"scanner.hpp"
#include<fstream>
#include<sstream>
#include<iostream>
#include<memory>




int scanFile(const std::string &contents) {
    try {
        std::unique_ptr<scan::Scanner> scan(new scan::Scanner(scan::TString(contents+"\n")));
        uint64_t tokens = scan->scan();
        std::cout << "scan returned: " << tokens << "\n";

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
    if(argc != 2) {
        std::cerr << "Error: requires one argument filename\n";
        exit(EXIT_FAILURE);
    }

    std::fstream file;
    file.open(argv[1], std::ios::in);
    std::ostringstream stream;
    stream << file.rdbuf();
    file.close();

    if(stream.str().length()>0) {
        return scanFile(stream.str());
    }
    return 0;
}
