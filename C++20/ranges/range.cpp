// as of Sat Mar 23 2024 only works on GCC not Clang

#include<iostream>
#include<algorithm>
#include<string>
#include<ranges>
#include<vector>

int main() {
    std::vector<std::string> v{"one ", "two", "apples", "bannas", "Turkey"};
    std::ranges::sort(v);
    for(const auto &i : std::views::take(v, 3)) {
        std::cout << i << "\n";
    }
    std::ranges::for_each(v, [](auto s) {
        std::cout << s << "\n";
    });
    return 0;
}