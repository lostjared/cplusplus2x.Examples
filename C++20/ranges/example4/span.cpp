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
    std::vector<std::string> v2 {"perl", "python", "java", "javascript", "c++" };
    // last 3 elements
    std::span<const std::string, 3> s2{v2.end() - 3, v2.end()};
    for(const auto &e : s2) {
        std::cout << "last 3: " << e << "\n";
    }
    auto c = v2.capacity();
    v2.push_back("Lisp");
    if(v2.capacity() != c) {
        s2 = std::span<const std::string, 3>{v2.end()-3, v2.end()};
    }
    for(auto &e : s2) {
        std::cout << "after capacity change: " << e << "\n";
    }
    return 0;
}