#ifndef _TYPES_X_H_
#define _TYPES_X_H_
  
#include<iostream>


namespace types {
    enum class TokenType { TT_ID, TT_SYM, TT_STR, TT_NUM, TT_NULL };
    enum class CharType { TT_NULL, TT_CHAR, TT_DIGIT, TT_SYMBOL, TT_STRING };

    void print_type_TokenType(std::ostream &out, const TokenType &tt);
    void print_type_CharType(std::ostream &out, const CharType &c);
}

#endif