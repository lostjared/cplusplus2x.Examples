#include<iostream>
#include<string>
#include"test.hpp"

int main() {
    for(const auto &i : value1_arr) {
        std::cout << i;
    }
    return 0;
}