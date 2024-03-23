#include<iostream>
#include<array>
#include<string>

template<typename String>
void printOutput(const String &s) {
    if constexpr(std::is_same<typename String::value_type, char>::value) {
        std::cout << s << "\n";
    }
    if constexpr(std::is_same<typename String::value_type, wchar_t>::value) {
        std::wcout << s << L"\n";
    }
}

int main() {
    printOutput(std::string("Test 1 2 3"));
    printOutput(std::wstring(L"test 3 4 5"));
    return 0;
}