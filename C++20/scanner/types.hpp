#ifndef _TYPES_X__
#define _TYPES_X__
  
#include<iostream>
#include<vector>
#include<string>
#include<optional>

namespace types {
    enum class TokenType { TT_ID, TT_SYM, TT_STR, TT_NUM, TT_NULL };
    enum class CharType { TT_CHAR, TT_DIGIT, TT_SYMBOL, TT_STRING, TT_SINGLE, TT_SPACE, TT_NULL };
    enum class OperatorType {
        OP_INC,             // '++'
        OP_DEC,             // '--'
        OP_LSHIFT_ASSIGN,   // '<<='
        OP_RSHIFT_ASSIGN,   // '>>='
        OP_PLUS_ASSIGN,     // '+='
        OP_MINUS_ASSIGN,    // '-='
        OP_MUL_ASSIGN,      // '*='
        OP_DIV_ASSIGN,      // '/='
        OP_MOD_ASSIGN,      // '%='
        OP_AND_ASSIGN,      // '&='
        OP_OR_ASSIGN,       // '|='
        OP_XOR_ASSIGN,      // '^='
        OP_LSHIFT,          // '<<'
        OP_RSHIFT,          // '>>'
        OP_EQ,              // '=='
        OP_NEQ,             // '!='
        OP_LE,              // '<='
        OP_GE,              // '>='
        OP_AND_AND,         // '&&'
        OP_OR_OR,           // '||'
        OP_ARROW,           // '->'
        OP_PLUS,            // '+'
        OP_MINUS,           // '-'
        OP_MUL,             // '*'
        OP_DIV,             // '/'
        OP_MOD,             // '%'
        OP_ASSIGN,          // '='
        OP_AND,             // '&'
        OP_OR,              // '|'
        OP_XOR,             // '^'
        OP_NOT,             // '!'
        OP_LT,              // '<'
        OP_GT,              // '>'
        OP_LPAREN,          // '('
        OP_RPAREN,          // ')'
        OP_LBRACKET,        // '['
        OP_RBRACKET,        // ']'
        OP_LBRACE,          // '{'
        OP_RBRACE,          // '}'
        OP_COMMA,           // ','
        OP_SEMICOLON,       // ';'
        OP_COLON,           // ':'
        OP_DOT,             // '.'
        OP_QUESTION,        // '?'
        OP_TILDE            // '~'
    };

    inline std::vector<std::string> opName = {
        "Increment",                // OP_INC
        "Decrement",                // OP_DEC
        "Left Shift Assignment",    // OP_LSHIFT_ASSIGN
        "Right Shift Assignment",   // OP_RSHIFT_ASSIGN
        "Plus Assignment",          // OP_PLUS_ASSIGN
        "Minus Assignment",         // OP_MINUS_ASSIGN
        "Multiplication Assignment",// OP_MUL_ASSIGN
        "Division Assignment",      // OP_DIV_ASSIGN
        "Modulus Assignment",       // OP_MOD_ASSIGN
        "Bitwise AND Assignment",   // OP_AND_ASSIGN
        "Bitwise OR Assignment",    // OP_OR_ASSIGN
        "Bitwise XOR Assignment",   // OP_XOR_ASSIGN
        "Left Shift",               // OP_LSHIFT
        "Right Shift",              // OP_RSHIFT
        "Equal",                    // OP_EQ
        "Not Equal",                // OP_NEQ
        "Less Than or Equal",       // OP_LE
        "Greater Than or Equal",    // OP_GE
        "Logical AND",              // OP_AND_AND
        "Logical OR",               // OP_OR_OR
        "Arrow",                    // OP_ARROW
        "Plus",                     // OP_PLUS
        "Minus",                    // OP_MINUS
        "Multiplication",           // OP_MUL
        "Division",                 // OP_DIV
        "Modulus",                  // OP_MOD
        "Assignment",               // OP_ASSIGN
        "Bitwise AND",              // OP_AND
        "Bitwise OR",               // OP_OR
        "Bitwise XOR",              // OP_XOR
        "Logical NOT",              // OP_NOT
        "Less Than",                // OP_LT
        "Greater Than",             // OP_GT
        "Left Parenthesis",         // OP_LPAREN
        "Right Parenthesis",        // OP_RPAREN
        "Left Bracket",             // OP_LBRACKET
        "Right Bracket",            // OP_RBRACKET
        "Left Brace",               // OP_LBRACE
        "Right Brace",              // OP_RBRACE
        "Comma",                    // OP_COMMA
        "Semicolon",                // OP_SEMICOLON
        "Colon",                    // OP_COLON
        "Dot",                      // OP_DOT
        "Question Mark",            // OP_QUESTION
        "Bitwise NOT"               // OP_TILDE
    };

    inline std::vector<std::string> opStrings = {
        "++",       // OP_INC
        "--",       // OP_DEC
        "<<=",      // OP_LSHIFT_ASSIGN
        ">>=",      // OP_RSHIFT_ASSIGN
        "+=",       // OP_PLUS_ASSIGN
        "-=",       // OP_MINUS_ASSIGN
        "*=",       // OP_MUL_ASSIGN
        "/=",       // OP_DIV_ASSIGN
        "%=",       // OP_MOD_ASSIGN
        "&=",       // OP_AND_ASSIGN
        "|=",       // OP_OR_ASSIGN
        "^=",       // OP_XOR_ASSIGN
        "<<",       // OP_LSHIFT
        ">>",       // OP_RSHIFT
        "==",       // OP_EQ
        "!=",       // OP_NEQ
        "<=",       // OP_LE
        ">=",       // OP_GE
        "&&",       // OP_AND_AND
        "||",       // OP_OR_OR
        "->",       // OP_ARROW
        "+",        // OP_PLUS
        "-",        // OP_MINUS
        "*",        // OP_MUL
        "/",        // OP_DIV
        "%",        // OP_MOD
        "=",        // OP_ASSIGN
        "&",        // OP_AND
        "|",        // OP_OR
        "^",        // OP_XOR
        "!",        // OP_NOT
        "<",        // OP_LT
        ">",        // OP_GT
        "(",        // OP_LPAREN
        ")",        // OP_RPAREN
        "[",        // OP_LBRACKET
        "]",        // OP_RBRACKET
        "{",        // OP_LBRACE
        "}",        // OP_RBRACE
        ",",        // OP_COMMA
        ";",        // OP_SEMICOLON
        ":",        // OP_COLON
        ".",        // OP_DOT
        "?",        // OP_QUESTION
        "~"         // OP_TILDE
    };
    
    extern std::vector<std::string> strTokenType;
    extern std::vector<std::string> sstrCharType;
    void print_type_TokenType(std::ostream &out, const TokenType &tt);
    void print_type_CharType(std::ostream &out, const CharType &c);
    std::optional<OperatorType> lookUp(const std::string &op);
}

#endif