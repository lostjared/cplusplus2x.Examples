#ifndef _STR_BUF_X
#define _STR_BUF_X

#include<string>
#include<sstream>
#include<cstdint>
#include<optional>

namespace scan {
    namespace buffer {
        template<typename Ch = char, typename String = std::basic_string<Ch, std::char_traits<Ch>>>
        class StringBuffer {
        public:
            using ch_type = Ch;
            using string_type = String;
            StringBuffer(const string_type &buf) : buffer_{buf}, index{0} {}
            StringBuffer(const StringBuffer<Ch,String> &sb) :  buffer_{sb.buffer_}, index{0} {}   
            StringBuffer(StringBuffer<Ch,String> &&sb) :  buffer_{std::move(sb.buffer_)}, index{0} {}
            StringBuffer<Ch,String> &operator=(const StringBuffer<Ch,String> &sb);                                      
            StringBuffer<Ch,String> &operator=(StringBuffer<Ch,String> &&b);
            StringBuffer<Ch,String> &operator=(const String &b);

            std::optional<ch_type> getch();
            std::optional<ch_type> curch();
            std::optional<ch_type> peekch();
            std::optional<ch_type> forward_step(int num=1);
            std::optional<ch_type> backward_step(int num=1);

            bool eof(uint64_t pos);
            void reset(uint64_t pos = 0);

            private:
               string_type buffer_;           
               uint64_t index{};
        };

        template<typename Ch, typename String>
        StringBuffer<Ch,String> &StringBuffer<Ch,String>::operator=(const StringBuffer<Ch,String> &sb) {
            buffer_ = sb.buffer_;
            index = sb.index;
            return *this;
        }            
        template<typename Ch, typename String>         
        StringBuffer<Ch,String> &StringBuffer<Ch,String>::operator=(StringBuffer<Ch,String> &&b) { //rval
            buffer_ = std::move(b.buffer_);
            index = b.index;
                return *this;
        }
        template<typename Ch, typename String>         
        StringBuffer<Ch,String> &StringBuffer<Ch,String>::operator=(const String &buf) {
            buffer_ =  buf;
            index = 0;
            return *this;
        }

        template<typename Ch, typename String>         
        std::optional<Ch> StringBuffer<Ch,String>::getch() {
            if(index + 1 < buffer_.length()) {
                return buffer_[index++];
            }
            return std::nullopt;
        }
        template<typename Ch, typename String>
        std::optional<Ch> StringBuffer<Ch,String>::curch() {
            if(index < buffer_.length())
                return buffer_[index];
            return std::nullopt;
        }
        template<typename Ch, typename String>
        std::optional<Ch> StringBuffer<Ch, String>::peekch() {
            if(index + 1 < buffer_.length())
                return buffer_[index+1];
            return std::nullopt;
        }

        template<typename Ch, typename String> 
        std::optional<Ch> StringBuffer<Ch, String>::forward_step(int num) {
            if(index + num < buffer_.length()) {
                index = index + num;
                return buffer_[index];
            }
            return std::nullopt;
        }
        template<typename Ch, typename String> 
        std::optional<Ch> StringBuffer<Ch, String>::backward_step(int num) {
            if(index - num > 0) {
                index = index - num;
                return buffer_[index];
            }
            return std::nullopt;
        }

        template<typename Ch, typename String>
        bool StringBuffer<Ch,String>::eof(uint64_t pos) {
            if(pos+index > buffer_.size()) {
                return true;
            } else {
                return false;
            }
        }
        template<typename Ch, typename String>
        void StringBuffer<Ch,String>::reset(uint64_t pos) {
            index = pos;
        }
    }

    namespace token {
        enum class TokenType { TT_ID, TT_SYM, TT_STR, TT_NUM, TT_NULL };
        enum class CharType { TT_NULL, TT_CHAR, TT_DIGIT, TT_SYMBOL, TT_STRING };
        template<int MAX_CHARS=256>
        class TokenMap {
        public:
            TokenMap();
            std::optional<CharType> lookup_int8(int8_t c);
        private:
            CharType token_map[MAX_CHARS+1];
        };

        template<int MAX_CHARS>
        TokenMap<MAX_CHARS>::TokenMap() {
            std::size_t i = 0;
            for(i = 0;  i < MAX_CHARS; ++i) {
                token_map[i] = CharType::TT_NULL;
            }
            for(i = 'a'; i <= 'z'; ++i) {
                token_map[i] = CharType::TT_CHAR;
            }
            for(i = 'A'; i <= 'Z'; ++i) {
                token_map[i] = CharType::TT_CHAR;
            }
            for(i = '0'; i <= '9'; ++i) {
                token_map[i] = CharType::TT_DIGIT;
            }
            token_map['+'] = CharType::TT_SYMBOL;
            token_map['-'] = CharType::TT_SYMBOL;
            token_map['*'] = CharType::TT_SYMBOL;
            token_map['/'] = CharType::TT_SYMBOL;
            token_map['%'] = CharType::TT_SYMBOL;
            token_map['&'] = CharType::TT_SYMBOL;
            token_map['|'] = CharType::TT_SYMBOL;
            token_map['^'] = CharType::TT_SYMBOL;
            token_map['~'] = CharType::TT_SYMBOL;
            token_map['!'] = CharType::TT_SYMBOL;
            token_map['='] = CharType::TT_SYMBOL;
            token_map['<'] = CharType::TT_SYMBOL;
            token_map['>'] = CharType::TT_SYMBOL;
            token_map['('] = CharType::TT_SYMBOL;
            token_map[')'] = CharType::TT_SYMBOL;
            token_map['['] = CharType::TT_SYMBOL;
            token_map[']'] = CharType::TT_SYMBOL;
            token_map['{'] = CharType::TT_SYMBOL;
            token_map['}'] = CharType::TT_SYMBOL;
            token_map[';'] = CharType::TT_SYMBOL;
            token_map[':'] = CharType::TT_SYMBOL;
            token_map[','] = CharType::TT_SYMBOL;
            token_map['.'] = CharType::TT_SYMBOL;
            token_map['?'] = CharType::TT_SYMBOL;
            token_map['#'] = CharType::TT_SYMBOL;
            token_map['_'] = CharType::TT_SYMBOL;
            token_map['\\'] = CharType::TT_SYMBOL;
            token_map['\''] = CharType::TT_STRING;
            token_map['\"'] = CharType::TT_STRING;  
            token_map['@'] = CharType::TT_SYMBOL;   
        }
        template<int MAX_CHARS>
        std::optional<CharType> TokenMap<MAX_CHARS>::lookup_int8(int8_t c) {
            if(c >= 0 && c < MAX_CHARS) {
                return token_map[c];
            }
            return std::nullopt;
        }
        
        template<typename Ch = char, typename String = std::basic_string<Ch, std::char_traits<Ch>>>
        class Token {
        public:
            using ch_type = Ch;
            using string_type = String;
            Token() = default;
            Token(const TokenType &t) : type{t} {}
            Token(const Token<Ch,String> &t) : type{t.type}, value{t.value} {}
            Token(Token<Ch,String>  &&t) : type{t.type}, value{std::move(t.value)} {}
            Token<Ch,String> &operator=(const TokenType &type);
            Token<Ch,String> &operator=(const Token<Ch,String> &t);
            Token<Ch,String> &operator=(Token<Ch,String> &&t);
            void setToken(const TokenType &type, const String &value);
            string_type getTokenValue() const { return value; }
            TokenType getTokenType() const { return type; }
        private:
            TokenType type;
            string_type value;
        };
        
        template<typename Ch, typename String>
        void Token<Ch, String>::setToken(const TokenType &type, const String &value) {
            this->type = type;
            this->value = value;
        }

        template<typename Ch, typename String>
        Token<Ch,String> &Token<Ch,String>::operator=(const TokenType &type) {

            return *this;
        }
        template<typename Ch, typename String>
        Token<Ch,String> &Token<Ch,String>::operator=(const Token<Ch,String> &t) {

            return *this;
        }
        template<typename Ch, typename String>
        Token<Ch,String> &Token<Ch,String>::operator=(Token<Ch,String> &&t) {

            return *this;
        }
    }
}

#endif