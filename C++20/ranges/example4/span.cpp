// use gcc 13 or greater
#include<iostream>
#include<ranges>
#include<span>
#include<vector>
#include<algorithm>

int main() {
    std::vector<std::string> v { "apples", "bannnas", "pairs", "oranges" };
    std::span<const std::string, 3> s{v.begin(), 3};
    for(const auto &e : s) {
        std::cout << "index: " << e << "\n";
    }
    std::cout << "sorted same span now points to: \n";
    std::ranges::sort(v);
    for(const auto &e : s) {
        std::cout << "index: " << e << "\n";
    }
    return 0;
}