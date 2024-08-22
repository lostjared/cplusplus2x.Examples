#include "parser.hpp"
#include <iostream>
#include<memory>

namespace parse {

    Parser::Parser(scan::Scanner *scanner) : scan{scanner} {}

    Parser::~Parser() {}

    void Parser::match(const types::TokenType &type) {
        if (token_index < scan->size()) {
            scan::TToken &token = scan->operator[](token_index);
            if (token.getTokenType() != type) {
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
        if (token_index < scan->size()) {
            scan::TToken &token = scan->operator[](token_index);
            const std::string &s = token.getTokenValue();
            if (s != t) {
                std::cerr << "Error: Token value mismatch found: ";
                std::cerr << token.getTokenValue() << " expected: " << t << "\n";
                exit(EXIT_FAILURE);
            } else {
                token_index++;
            }
        } 
    }

    void Parser::match(const types::OperatorType op) {
        if (token_index < scan->size()) {
            scan::TToken &token = scan->operator[](token_index);
            auto op_t = types::lookUp(token.getTokenValue());
            if (op_t.has_value()) {
                if (op == *op_t) {
                    token_index++;
                } else {
                    std::cerr << "Error: Operator type mismatch found: " << token.getTokenValue() << " expected: " << types::opStrings[static_cast<int>(op)] << "\n";
                    exit(EXIT_FAILURE);
                }
            }
        }
    }

    void Parser::match(const types::KeywordType kw) {
        if (token_index < scan->size()) {
            scan::TToken &token = scan->operator[](token_index);
            auto op_t = types::lookUp_Keyword(token.getTokenValue());
            if (op_t.has_value()) {
                if (kw == *op_t) {
                    token_index++;
                } else {
                    std::cerr << "Error: Keyword type mismatch found: " << token.getTokenValue() << " expected: " << types::kwStr[static_cast<int>(kw)] << "\n";
                    exit(EXIT_FAILURE);
                }
            }
        }
    } 

    bool Parser::test(const types::TokenType &type) {
        scan::TToken &token = scan->operator[](token_index);
        return token.getTokenType() == type;
    }

    bool Parser::test(const string_type &t) {
        scan::TToken &token = scan->operator[](token_index);
        return token.getTokenValue() == t;
    }

    bool Parser::test(const types::OperatorType &op) {
        scan::TToken &token = scan->operator[](token_index);
        auto op_t = types::lookUp(token.getTokenValue());
        return op_t.has_value() && op == *op_t;
    }

    bool Parser::test(const types::KeywordType &kw) {
        scan::TToken &token = scan->operator[](token_index);
        auto op_t = types::lookUp_Keyword(token.getTokenValue());
        return op_t.has_value() && kw == *op_t;
    }

    void Parser::inc(const uint64_t num) {
        token_index += num;
    }

    void Parser::dec(const uint64_t num) {
        if (token_index >= num) {
            token_index -= num;
        }
    }

    void Parser::parse() {
        uint64_t num = scan->scan();
        if (num > 0) {
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
        auto program = std::make_unique<ast::Program>();
        while (token_index < scan->size()) {
            if (test(types::KeywordType::KW_PROC)) {
                inc();
                auto function = parseFunction(); 
                program->body.push_back(std::move(function));
            } else if (test(types::KeywordType::KW_LET)) {
                inc();
                auto assignment = parseAssignment();
                program->body.push_back(std::move(assignment));
            } else {
                std::cerr << "Unknown token: " << scan->operator[](token_index).getTokenValue() << " at index " << token_index << "\n";
                exit(EXIT_FAILURE);
            }
        }
        root = std::move(program);
    }

    std::unique_ptr<ast::Expression> Parser::parseExpression() {
        auto left = parseTerm();

        while (test(types::OperatorType::OP_PLUS) || test(types::OperatorType::OP_MINUS)) {
            auto op = types::lookUp(scan->operator[](token_index).getTokenValue());
            inc();  
            auto right = parseTerm();
            left = std::make_unique<ast::BinaryOp>(*op, std::move(left), std::move(right));
        }

        return left;
    }

    std::unique_ptr<ast::Expression> Parser::parseTerm() {
        auto left = parseFactor();

        while (test(types::OperatorType::OP_MUL) || test(types::OperatorType::OP_DIV)) {
            auto op = types::lookUp(scan->operator[](token_index).getTokenValue());
            inc();  
            auto right = parseFactor();
            left = std::make_unique<ast::BinaryOp>(*op, std::move(left), std::move(right));
        }

        return left;
    }

    std::unique_ptr<ast::Expression> Parser::parseFactor() {
        if (test(types::OperatorType::OP_LPAREN)) {
            inc();  
            auto expr = parseExpression();
            match(types::OperatorType::OP_RPAREN);
            return expr;
        } else {
            return parsePrimary();
        }
    }

    std::unique_ptr<ast::Expression> Parser::parsePrimary() {
        if (test(types::TokenType::TT_NUM) || test(types::TokenType::TT_STR)) {
            auto token = scan->operator[](token_index);
            inc();
            return std::make_unique<ast::Literal>(token.getTokenValue());
        } else if (test(types::TokenType::TT_ID)) {
            auto token = scan->operator[](token_index);
            inc();
            return std::make_unique<ast::Identifier>(token.getTokenValue());
        }
        return nullptr;
    }

    std::unique_ptr<ast::Assignment> Parser::parseAssignment() {
        if(test(types::TokenType::TT_ID)) {
            auto lhs = parsePrimary();
            if (test(types::OperatorType::OP_ASSIGN)) {
                inc();
                auto rhs = parseExpression();
                match(types::OperatorType::OP_SEMICOLON);
                return std::make_unique<ast::Assignment>(std::move(lhs), std::move(rhs));
            }
        }
        return nullptr;
    }

    std::unique_ptr<ast::Function> Parser::parseFunction() {
        if (test(types::TokenType::TT_ID)) {
            std::string name = scan->operator[](token_index).getTokenValue();
            inc();
            match(types::OperatorType::OP_LPAREN);
            match(types::OperatorType::OP_RPAREN);
            match(types::OperatorType::OP_LBRACE);
            auto function = std::make_unique<ast::Function>(name);
            while (!test(types::OperatorType::OP_RBRACE)) {
                    if(test(types::KeywordType::KW_LET)) {
                        inc();
                        auto stmt = parseAssignment();  
                        if (stmt) {
                            function->body.push_back(std::move(stmt));
                        } 
                    } else break;
            }
            match(types::OperatorType::OP_RBRACE);
            return function;
        }
        return nullptr;
    }

}