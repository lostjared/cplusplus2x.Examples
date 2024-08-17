#ifndef _SCANNER_H_
#define _SCANNER_H_

#include<iostream>
#include<string>
#include"string_buffer.hpp"
#include<vector>
#include<string>

namespace scan {

    using TString = buffer::StringBuffer<char>;

    class Scanner {
    public:
        Scanner(const TString &b);

    private:
        TString string_buffer;
    };
    
}


#endif