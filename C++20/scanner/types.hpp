#ifndef _TYPES_X__
#define _TYPES_X__
  
#include<iostream>
#include<vector>
#include<string>

namespace types {
    enum class TokenType { TT_ID, TT_SYM, TT_STR, TT_NUM, TT_NULL };
    enum class CharType { TT_CHAR, TT_DIGIT, TT_SYMBOL, TT_STRING, TT_NULL };
    const unsigned int max_types = 4;

    // quick Look up
    extern std::vector<std::string> strTokenType;
    extern std::vector<std::string> sstrCharType;
    void print_type_TokenType(std::ostream &out, const TokenType &tt);
    void print_type_CharType(std::ostream &out, const CharType &c);
}

#endif