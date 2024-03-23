// 
// $ CXX=g++13 make


#include<iostream>
#include<string>
#include<ranges>
#include<algorithm>
#include<cstdlib>
#include<vector>


int scanInteger() {
    std::string s;
    std::getline(std::cin, s);
    if(s.length() > 0) {
        int value = atoi(s.c_str());
        if(value > 0) return value;
        std::cout << "Invalid input try again..\n";
        return scanInteger();
    }
    else {
        std::cout << "Invalid input try again..\n";
        return scanInteger();
    }
}

void echoString(std::string_view s) {
    for(const auto &i : std::views::all(s)) {
        std::cout << i << ',';
    }
    for(const auto &s : std::views::take(s, 1)) {
        std::cout << static_cast<char>(toupper(s)) << "!" << ",";
    }
    std::cout << "0;\n";
}

int main() {
    std::cout << "Enter how many lines to enter.: ";
    int numLines = scanInteger();
    std::vector<std::string> v;
    for(int i = 0; i < numLines; ++i) {
        std::string s;
        std::getline(std::cin, s);
        if(s.length() > 0) {
            v.push_back(s);
        }
    }
    std::ranges::sort(v);
    std::cout << "sorted -: [\n";
    for(const auto &i : std::views::all(v)) {
        echoString(i);
    }
    std::cout << "0 ];\n\n";
    return 0;

}