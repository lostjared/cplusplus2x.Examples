#include<iostream>
#include<string>
#include<random>
#include"../tokenizer.hpp"


void fillBuffer(std::string &s) {
    std::mt19937 en(std::random_device{}());
    std::uniform_int_distribution<int> char_dist('a', 'z');
    std::uniform_int_distribution<int> length(0, 255);
    auto len{length(en)};
    for(int i = 0; i < len; ++i) {
        s += char_dist(en);
        if(length(en) > 100) s += " ";
        if(length(en) > 200) s += " ";
    }
}

void testScanner(Scanner<std::string> &scan) {
    std::string random_string;
    fillBuffer(random_string);
    scan.set(random_string, " ");
    auto tokens{collect(scan.tokenizer())};
    int index{};
    std::cout << "random string: " << random_string << "\n";
    for(const auto &i : tokens) {
        ++index;
        std::cout << "index[" << index << "] -> " << i << "\n";
    }
}

int main() {
    try {
        Scanner<std::string> scanner;
        constexpr static int count = 5000;
        for(int index = 0; index < count; ++index) {
            std::cout << "running test index: " << index << "\n";
            testScanner(scanner);
        }
        std::cout << "Passed test no exceptions thrown or errors occoured.\n";
    } catch(const std::runtime_error &e) {
        std::cerr << "Runtime Error: " << e.what() << "\n";
    }
    return 0;
}