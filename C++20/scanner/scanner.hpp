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
    using TMap = token::TokenMap<>;

    class Scanner {
    public:
        Scanner(const TString &b);
        uint64_t scan();
        std::optional<TToken> grabId();
        std::optional<TToken> grabDigits();
        std::optional<TToken> grabSymbols();
        std::optional<TToken> grabString();
    private:
        TString string_buffer;
        TMap token_map;
        std::vector<TToken> tokens;
    };
    
}


#endif