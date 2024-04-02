#include<iostream>
#include<bit>

int main() {
    const unsigned long test = 0xFF00FF;
    for(int i = 0; i < std::bit_width(test); ++i) {
        unsigned long rt_val = std::rotl(test, i);
        unsigned long rt_val2 = std::rotr(test, i);
        std::cout << std::hex << test << ":" << std::hex << rt_val << ":" << std::hex << rt_val2 << "\n";
    }
    return 0;
}