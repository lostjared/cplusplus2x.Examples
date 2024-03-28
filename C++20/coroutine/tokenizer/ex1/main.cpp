/* use tokenizer with wchar_t */

#include"../tokenizer.hpp"
#include<iostream>
#include<string>
#include<algorithm>
#include<ranges>

int main(int argc, char **argv) {
    Scanner<std::wstring> scanner;
    bool active = true;
    while(active) {
        std::wcout << L"Enter a sentence..\n";
        std::wstring s;
        std::getline(std::wcin, s);
        scanner.set(s, L" ");
        auto tokens{collect(scanner.tokenizer())};
        int index = 0;
        for(const auto &token : std::views::all(tokens)) {
            std::wcout << L"token: " << index++ << L" " << token << L"\n";
        }
        if(tokens.size() > 0 && tokens.at(0) == L"quit") {
            active = false;
        }
    }
    return 0;
}