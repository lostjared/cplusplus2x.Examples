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
    std::ranges::sort(v, [](auto v1, auto v2) {
        return v2 < v1;
    });
    std::ranges::for_each(v, [](auto s) {
        std::cout << "reverse: " << s << "\n";
    });
    for(const auto &i : std::views::all(v)) {
        std::cout << "X: "<< i << "\n";
    }

    std::vector v2 { 0, 1, 2, 3, 4 };

    auto pos = std::ranges::find(v2, 1);
    auto p1 = std::views::counted(pos, 2);
    for(const auto &i : p1) {
        std::cout << "counted: " << i << "\n";
    }
    return 0;
}