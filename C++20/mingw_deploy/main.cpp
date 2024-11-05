#include<iostream>
#include<sstream>
#include<string>
#include<cstdio>
#include<cstdlib>
#include<algorithm>
#include<optional>
#include"argz.hpp"


std::optional<std::string> extractFilename(const std::string &path) {
    auto pos = path.find("=>");
    if(pos != std::string::npos) {
        std::string rightof = path.substr(pos+2);
        auto pos2 = rightof.rfind("(");
        if(pos2 != std::string::npos) {
            std::string leftof = rightof.substr(0, pos2);
            return leftof;
        }
    }
    return std::nullopt;
}

int main(int argc, char **argv) {
    if(argc != 2) {
        std::cerr << "Error requires one argument, EXE name.\n";
        std::cerr.flush();
        exit(EXIT_FAILURE);
    }
    std::ostringstream stream;
    stream << "ldd " << argv[1] << " | grep mingw ";
#ifdef _WIN32
    FILE *fptr = _popen(stream.str().c_str(), "r");
#else
    FILE *fptr = popen(stream.str().c_str(), "r");
#endif
    if(!fptr) {
        std::cerr << "An Error has occoured..\n";
        std::cerr.flush();
        exit(EXIT_FAILURE);
    }
    std::string output;
    while(!feof(fptr)) {
        char buf[1024];
        std::size_t bytes = fread(buf, 1, 1024, fptr);
        buf[bytes] = 0;
        output += buf;
    }

#ifdef _WIN32
    _pclose(fptr);
#else
    pclose(fptr);
#endif
    std::istringstream istream(output);
    while(!istream.eof()) {
        std::string line;
        std::getline(istream, line);
        auto filename = extractFilename(line);
        if(filename.has_value()) {
            std::ostringstream stream;
            stream << "cp " << filename.value() << " . ";
            system(stream.str().c_str());
            std::cout << stream.str() << "\n";
        }
    }
    return 0;
}