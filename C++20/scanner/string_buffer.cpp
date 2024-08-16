#include"string_buffer.hpp"
#include<utility>
#include<optional>

namespace scan {
    namespace buffer {
            StringBuffer &StringBuffer::operator=(const StringBuffer &sb) {
                buffer_ = sb.buffer_;
                index = sb.index;
                return *this;
            }                     
            StringBuffer &StringBuffer::operator=(StringBuffer &&b) { //rval
                buffer_ = std::move(b.buffer_);
                index = b.index;
                 return *this;
            }
            StringBuffer &StringBuffer::operator=(const std::basic_string<ch_type> &bb) {
                buffer_ =  buffer_;
                index = 0;
                return *this;
            }

            std::optional<StringBuffer::ch_type> StringBuffer::getch() {

                return std::nullopt;
            }
            std::optional<StringBuffer::ch_type> StringBuffer::curch() {

                return std::nullopt;
            }
            std::optional<StringBuffer::ch_type> StringBuffer::peekch() {

                return std::nullopt;
            }

            bool StringBuffer::eof(uint64_t pos) {

                if(pos+index > buffer_.size()) {
                    return true;
                } else {
                    return false;
                }

            }

    }
}