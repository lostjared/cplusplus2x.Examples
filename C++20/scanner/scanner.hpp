#ifndef _SCANNER_H_
#define _SCANNER_H_

#include<iostream>
#include<string>
#include"string_buffer.hpp"
#include<vector>
#include<string>
#include<optional>

namespace scan {

    using TString = buffer::StringBuffer<char>;
    using TToken = token::Token<char>;
    using TMap = token::TokenMap<char>;

    class Scanner {
    public:
        Scanner(const TString &b);
        uint64_t scan();
        std::optional<TToken> grabId();
        std::optional<TToken> grabDigits();
        std::optional<TToken> grabSymbols();
        std::optional<TToken> grabString();

        TToken &operator[](size_t index);
        size_t size() const;
    private:
        TString string_buffer;
        TMap token_map;
        std::vector<TToken> tokens;
    };

    
}

std::ostream &operator<<(std::ostream &out, const types::CharType &c);
std::ostream &operator<<(std::ostream &out, const types::TokenType &t);



#endif