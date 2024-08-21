#ifndef _PARSER_H__
#define _PARSER_H__

#include"scanner.hpp"
#include<memory>

namespace parse {
    class Parser {
    public:
        using string_type = scan::token::Token<char>::string_type;
        Parser(scan::Scanner *scanner) : scan{scanner} {}
        ~Parser();
        void parse();

    private:
        std::unique_ptr<scan::Scanner> scan;
        uint64_t token_index = 0;

        void match(const types::TokenType &type);
        void match(const string_type &t);
        bool test(const types::TokenType &type);
        bool test(const string_type &t);
        void inc(const uint64_t num=1);
        void dec(const uint64_t num=1);
        void proc_tokens();

    };
}

#endif
