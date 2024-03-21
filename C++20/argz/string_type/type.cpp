
#include<iostream>
#include"../argz.hpp"
#include<string>
#include<cstring>

class StringWrapper {
public:
    using value_type = wchar_t;
    using size_type = std::wstring::size_type;
    StringWrapper() = default;
    StringWrapper(const char *src) {
        std::wstring s;
        for(int i = 0; i < strlen(src); ++i) {
            s += static_cast<wchar_t>(src[i]);
        }
        this->src = s;
    }
    StringWrapper(const wchar_t *src) {
        this->src = src;
    }
    size_type length() const {
        return src.length();
    }
    StringWrapper &operator=(const StringWrapper &w) {
        src = w.src;
        return *this;
    }
    StringWrapper &operator+=(const StringWrapper &w) {
        src += w.src;
        return *this;
    }
    value_type &operator[](const int &pos) {
        return src[pos];
    }
    const value_type &operator[](const int &pos) const {
        return src[pos];
    }
    
    friend std::ostream &operator<<(std::ostream &out, const StringWrapper &w) {
        for(int i = 0; i < w.length(); ++i) {
            out << static_cast<char>(w[i]);
        }
        return out;
    }
    friend std::wostream &operator<<(std::wostream &out, const StringWrapper &w) {
        out << w.src;
        return out;
    }
    
private:
    std::wstring src;
};


int main(int argc, char **argv) {
    std::locale::global(std::locale(""));
    Argz<StringWrapper> argz(argc, argv);
    argz.addOptionSingle('h', "Help meessage \u2665 ");
    argz.help(std::cout);
    return 0;
}
