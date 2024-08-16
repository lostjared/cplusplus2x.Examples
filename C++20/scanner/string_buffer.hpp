#ifndef _STR_BUF_X
#define _STR_BUF_X

#include<string>
#include<sstream>
#include<cstdint>
#include<optional>

namespace scan {

    namespace buffer {
        class StringBuffer {
        public:
            using ch_type = int8_t;
            using string_type = std::basic_string<ch_type, std::char_traits<ch_type>>;

            StringBuffer(const string_type &buf) : buffer_{buf}, index{0} {}
            StringBuffer(const StringBuffer &sb) :  buffer_{sb.buffer_}, index{0} {}   
            StringBuffer(StringBuffer &&sb) :  buffer_{std::move(sb.buffer_)}, index{0} {}
            StringBuffer &operator=(const StringBuffer &sb);                                      
            StringBuffer &operator=(StringBuffer &&b);
            StringBuffer &operator=(const string_type &b);

            std::optional<ch_type> getch();
            std::optional<ch_type> curch();
            std::optional<ch_type> peekch();

            bool eof(uint64_t pos);

            private:
               string_type buffer_;           
               uint64_t index{};
        };
    }

}


#endif
