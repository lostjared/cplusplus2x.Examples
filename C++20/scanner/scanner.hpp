#ifndef _SCANNER_H_
#define _SCANNER_H_

#include<iostream>
#include<string>
#include"string_buffer.hpp"
#include<vector>
#include<string>

namespace scan {

    using buffer::StringBuffer;

    class Scanner {
    public:
        Scanner(const StringBuffer &b);

    private:
        StringBuffer string_buffer;
    };
    
}


#endif
