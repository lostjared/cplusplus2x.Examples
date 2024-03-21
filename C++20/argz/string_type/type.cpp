
#include<iostream>
#include"../argz.hpp"
#include<string>
#include<cstring>

int main(int argc, char **argv) {
    std::locale::global(std::locale(""));
    Argz<std::wstring> argz(argc, argv);
    argz.addOptionSingle('h', L"Help message \u2665");
    argz.help(std::wcout);
    return 0;
}

