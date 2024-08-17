#include"scanner.hpp"


namespace scan {

    Scanner::Scanner(const TString &b) : string_buffer{b} {

    }

    uint64_t Scanner::scan() {
        tokens.clear();
        while(!string_buffer.eof(0)) {
            auto ch = string_buffer.getch();
            if(ch.has_value() && *ch == '/') {
                auto ch2 = string_buffer.curch();
                if(ch2.has_value() && *ch2 == '/') {
                    auto ch_ln = string_buffer.curch();
                    do {
                        ch_ln = string_buffer.getch();    
                        std::cout << "eat chr: " << *ch_ln << "\n";
                    } while(ch_ln.has_value() && *ch_ln != '\n');
                    continue;
                } else if(ch2.has_value() && *ch2 == '*') {
                    string_buffer.getch(); 
                    while(true) {
                        auto ch_in = string_buffer.getch();
                        if(!ch_in.has_value()) break;
                        if(*ch_in == '*') {
                            auto ch_ex = string_buffer.curch();
                            if(ch_ex.has_value() && *ch_ex == '/') {
                                string_buffer.getch(); 
                                break;
                            }
                        }
                    }
                    continue;
                }
            }
            if(ch.has_value()) {
                 auto t_ch = token_map.lookup_int8(*ch);
                if(t_ch.has_value()) {
                    switch(*t_ch) {
                        case types::CharType::TT_CHAR: {
                            auto tok = grabId();
                            if(tok.has_value()) {
                                tokens.push_back(*tok);
                            }
                        }
                        break;
                        case types::CharType::TT_DIGIT: {
                            auto tok = grabDigits();
                            if(tok.has_value()) {
                                tokens.push_back(*tok);
                            }
                        }
                        break;
                        case types::CharType::TT_SYMBOL: {
                            auto tok = grabSymbols();
                            if(tok.has_value()) {
                                tokens.push_back(*tok);
                            }
                        }
                        break;
                        case types::CharType::TT_STRING: {
                            auto tok = grabString();
                            if(tok.has_value()) {
                                tokens.push_back(*tok);
                            }
                        }
                        break;
                        case types::CharType::TT_NULL:
                        std::cout << "skip char: " << *ch << "\n";
                        continue;
                    }
                }
            } else break;
        }
        return tokens.size();
    }

    std::optional<TToken> Scanner::grabId() {
        auto ch = string_buffer.backward_step(1);
        TToken token;

        if(ch.has_value()) {
            auto ch_t = token_map.lookup_int8(*ch);
            decltype(token.getTokenValue()) tok_value;

            while(true) {
                ch = string_buffer.getch();
                if(!ch.has_value()) break;
                ch_t = token_map.lookup_int8(*ch);
                if(!ch_t.has_value() || (*ch_t != types::CharType::TT_CHAR && *ch_t != types::CharType::TT_DIGIT)) break;
                tok_value += *ch;    
            }

            token.setToken(types::TokenType::TT_ID, tok_value);
            std::cout << "grabbed id: " << tok_value << "\n";
            return token;
        }

        return std::nullopt;
    }

    std::optional<TToken> Scanner::grabDigits() {
        auto ch = string_buffer.backward_step(1);
        TToken token;

        if(ch.has_value()) {
            auto ch_t = token_map.lookup_int8(*ch);
            decltype(token.getTokenValue()) tok_value;
            
            while(true) {
                ch = string_buffer.getch();
                if(!ch.has_value()) break;
                ch_t = token_map.lookup_int8(*ch);
                if(!ch_t.has_value() || *ch_t != types::CharType::TT_DIGIT) break;
                tok_value += *ch;    
            }

            token.setToken(types::TokenType::TT_NUM, tok_value);
            std::cout << "grabbed number: " << tok_value << "\n";
            return token;
        }
        return std::nullopt;
    }

    std::optional<TToken> Scanner::grabSymbols() {
        return std::nullopt;
    }
        
    std::optional<TToken> Scanner::grabString() {
        return std::nullopt;
    }

    
    TToken &Scanner::operator[](size_t index) { return tokens[index]; }
    size_t Scanner::size() const { return tokens.size(); }
}



using types::TokenType;
using types::CharType;


std::ostream &operator<<(std::ostream &out, const TokenType &tt) {
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

    return out;
}       

std::ostream &operator<<(std::ostream &out, const CharType &c) {
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
    return out;
}
