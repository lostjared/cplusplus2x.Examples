#include "types.hpp"


namespace types {

    std::vector<std::string> strTokenType {"Identifier", "Symbols", "String", "Numeric", "NULL"};
    std::vector<std::string> strCharType  {"Characters", "Digits", "Symbols", "String", "NULL"};

    void print_type_TokenType(std::ostream &out, const TokenType &tt) {
       unsigned int t_type = static_cast<unsigned int>(tt);
       if(t_type < strTokenType.size())
            out << strTokenType[t_type];
    }       

    void print_type_CharType(std::ostream &out, const CharType &c) {
        unsigned int t_type = static_cast<unsigned int>(c);
        if(t_type < strCharType.size())
           out << strCharType[t_type];
    }
}