#include<iostream>
#include<array>

template<size_t S>
constexpr auto sum(const std::array<int, S> &a) {
    size_t index = 0;
    for(size_t i = 0; i < a.size(); ++i) {
        index += a[i];
    }
    return index;
}

int main() {
    constexpr std::array<int, 4> values{0, 1, 2, 4};
    constexpr static auto index = sum(values);
    std::cout << "Caculated at compile time: " << index << "\n";
}

/* without constexpr static

# Compilation provided by Compiler Explorer at https://godbolt.org/
.LC0:
        .string "Caculated at compile time: "
.LC1:
        .string "\n"
main:
        push    rbp
        mov     rbp, rsp
        sub     rsp, 32
        mov     DWORD PTR [rbp-32], 0
        mov     DWORD PTR [rbp-28], 1
        mov     DWORD PTR [rbp-24], 2
        mov     DWORD PTR [rbp-20], 4
        mov     QWORD PTR [rbp-8], 7
        mov     esi, OFFSET FLAT:.LC0
        mov     edi, OFFSET FLAT:_ZSt4cout
        call    std::basic_ostream<char, std::char_traits<char> >& std::operator<< <std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*)
        mov     rdx, rax
        mov     rax, QWORD PTR [rbp-8]
        mov     rsi, rax
        mov     rdi, rdx
        call    std::basic_ostream<char, std::char_traits<char> >::operator<<(unsigned long)
        mov     esi, OFFSET FLAT:.LC1
        mov     rdi, rax
        call    std::basic_ostream<char, std::char_traits<char> >& std::operator<< <std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*)
        mov     eax, 0
        leave
        ret
*/
/* with cosntexpr static 
# Compilation provided by Compiler Explorer at https://godbolt.org/
.LC0:
        .string "Caculated at compile time: "
.LC1:
        .string "\n"
main:
        push    rbp
        mov     rbp, rsp
        sub     rsp, 16
        mov     DWORD PTR [rbp-16], 0
        mov     DWORD PTR [rbp-12], 1
        mov     DWORD PTR [rbp-8], 2
        mov     DWORD PTR [rbp-4], 4
        mov     esi, OFFSET FLAT:.LC0
        mov     edi, OFFSET FLAT:_ZSt4cout
        call    std::basic_ostream<char, std::char_traits<char> >& std::operator<< <std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*)
        mov     esi, 7
        mov     rdi, rax
        call    std::basic_ostream<char, std::char_traits<char> >::operator<<(unsigned long)
        mov     esi, OFFSET FLAT:.LC1
        mov     rdi, rax
        call    std::basic_ostream<char, std::char_traits<char> >& std::operator<< <std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*)
        mov     eax, 0
        leave
        ret
*/