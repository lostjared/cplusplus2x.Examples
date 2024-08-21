#include "parser.hpp"
namespace parse {
 
    Parser::~Parser() {
    
    }

    void Parser::match(const types::TokenType &type) {
        if(token_index < scan->size()) {
            scan::TToken &token = scan->operator[](token_index);
            if(token.getTokenType() != type) {
                std::cerr << "Error: Token type mismatch found: ";
                types::print_type_TokenType(std::cerr, token.getTokenType());
                std::cerr << " expected: ";
                types::print_type_TokenType(std::cerr, type);
                std::cerr << "\n";
                exit(EXIT_FAILURE);
            } else {
                token_index++;
            }
        } 
    }

    void Parser::match(const string_type &t) {
        if(token_index < scan->size()) {
            scan::TToken &token = scan->operator[](token_index);
            const std::string &s = token.getTokenValue();
            if(s != t) {
                std::cerr << "Error: Token value mismatch found: ";
                std::cerr << token.getTokenValue() << " expected: " << token.getTokenValue() << "\n";
                exit(EXIT_FAILURE);
            } else {
                token_index++;
            }
        } 
    }

    void Parser::inc(const uint64_t num) {
        token_index += num;
    }
    void Parser::dec(const uint64_t num) {
        token_index -= num;
    }

    void Parser::parse() {
        uint64_t num = scan->scan();
        if(num > 0) {
            std::cout << "ETL: Scanned " << num << " tokens.\n";
            std::cout << "ETL: parsing ... ";
            proc_tokens();
            std::cout << " [complete]\n";

        } else {
            std::cerr << "ETL: Error zero tokens or failure...\n";
            exit(EXIT_FAILURE);
        }
    }

    void Parser::proc_tokens() {

    }
}