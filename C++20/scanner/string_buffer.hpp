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
            using ch_type = uint8_t;

            StringBuffer(const std::string &buf) : buffer_{buf}, index{0} {}
            StringBuffer(const StringBuffer &sb) :  buffer_{sb.buffer_}, index{0} {}   
            StringBuffer(StringBuffer &&sb) :  buffer_{std::move(sb.buffer_)}, index{0} {}
            StringBuffer &operator=(const StringBuffer &sb);                                      
            StringBuffer &operator=(StringBuffer &&b);
            StringBuffer &operator=(const std::basic_string<ch_type> &b);

            std::optional<ch_type> getch();
            std::optional<ch_type> curch();
            std::optional<ch_type> peekch();

            bool eof(uint64_t pos);

            private:
               std::string buffer_;           
               uint64_t index{};
        };
    }

}


#endif