#include"string_buffer.hpp"
#include<utility>
#include<optional>

namespace scan {

    void print_type_CharType(std::ostream &out, const types::TokenType &tt) {
        switch(tt) {
            case types::TokenType::TT_ID:
            out << "Identifier";
            break;
            case types::TokenType::TT_NUM:
            out << "Numeeic Data";
            break;
            case types::TokenType::TT_STR:
            out << "String Data";
            break;
            case types::TokenType::TT_SYM:
            out << "Symbol(s)";
            break;
            default:
                out << "Null Type";
        }     
    }       

    void print_type_TokenType(std::ostream &out, const types::CharType &c) {
        switch(c) {
            case types::CharType::TT_CHAR:
            out << "Characters";
            break;
            case types::CharType::TT_DIGIT:
            out << "Digits";
            break;
            case types::CharType::TT_STRING:
            out << "String";
            break;
            case types::CharType::TT_SYMBOL:
            out << "Symbol";
            break;
            default:
            out << "[Null Type]";
        }
    }

}





