
#include<iostream>
#include"../argz.hpp"
#include<string>
#include<cstring>

int main(int argc, char **argv) {
    std::locale::global(std::locale(""));
    std::wcout.imbue(std::locale());
    Argz<std::wstring> argz(argc, argv);
    argz.addOptionSingle('h', L"Help message \u2665");
    argz.addOptionSingle('v', L"Version Info \u263A");
    argz.addOptionDouble('j', L"jared", L"j\u00C6red");
    
    if(argc == 1) {
        argz.help(std::wcout);
        return 0;
    }
    
    int value{};
    Argument<std::wstring> arg;
    try {
    while ((value = argz.proc(arg)) != -1) {
        switch(value) {
            case 'h':
                argz.help(std::wcout);
                break;
            case 'v':
                std::wcout << L" \u263A version 0.1.0\n";
                break;
            case 'j':
                std::wcout << arg.arg_name << L" -> " << arg.desc << L" Bruni\n";
                break;
        }
    }
    } catch(const ArgException<std::wstring> &e) {
        std::wcout << L"Syntax Error: "  << e.text() << "\n";
    }
    return 0;
}

