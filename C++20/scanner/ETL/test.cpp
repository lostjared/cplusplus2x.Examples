#include<iostream>
#include<string>
#include"parser.hpp"
#include<fstream>
#include<sstream>



void test_parse(const std::string &filename) {
    std::fstream file;
    file.open(filename, std::ios::in);
    if(!file.is_open()) {
        std::cerr << "Could not open: " << filename << ".\n";
        exit(EXIT_FAILURE);
    }
    std::ostringstream stream;
    stream << file.rdbuf() << " \n";
    file.close();
    if(stream.str().length()>0) {
        parse::Parser parser(new scan::Scanner(scan::TString(stream.str())));
        parser.parse();
    } else {
        std::cerr << " Zero bytes to read..\n";
        exit(EXIT_FAILURE);
    }
}