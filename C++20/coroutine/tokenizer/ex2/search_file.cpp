#include<iostream>
#include<string>
#include<algorithm>
#include<fstream>
#include<cstdlib>
#include<memory>
#include "../tokenizer.hpp"

std::string readStringFromFile(const std::string &filename) {
    std::fstream file;
    file.open(filename, std::ios::in);
    if(!file.is_open()) {
        std::cout << "Error could not open file: " << filename << "\n";
        exit(EXIT_FAILURE);
    }
    file.seekg(0, std::ios::end);
    long long int len {file.tellg()};
    file.seekg(0, std::ios::beg);
    std::unique_ptr<char[]> buffer(new char [len+1]);
    file.read(buffer.get(), len);
    buffer[len] = 0;
    file.close();
    return std::string(buffer.get());
}

int main(int argc, char **argv) {
    if(argc != 3) {
        std::cerr << "Error requires one argument, file to search.\n";
        exit(EXIT_FAILURE);
    }
    std::string data{readStringFromFile(argv[1])};
    std::string search{argv[2]};
    Scanner<std::string> scanner(search, " ");
    auto tokens{collect(scanner.tokenizer())};
    if(tokens.size() > 0) {
        for(const auto &s : std::views::all(tokens)) {
            if(data.find(s) != std::string::npos) {
                std::cout << "Found search substring: " << s << "\n";
            }
        }
    }
    return 0;
}