#include <array>
#include <iostream>

template <size_t S>
constexpr auto sum(const std::array<int, S> &a) {
	size_t index = 0;
	for(size_t i = 0; i < a.size(); ++i) {
		index += a[i];
	}
	return index;
}

int main() {
	constexpr static std::array<int, 4> values{0, 1, 2, 4};
	constexpr static auto index = sum(values);
	std::cout << "Caculated at compile time: " << index << "\n";
}

// Compiles to:
/*
.LC0:
	   .string "Caculated at compile time: "
.LC1:
	   .string "\n"
main:
	   push    rbp
	   mov     rbp, rsp
	   mov     esi, OFFSET FLAT:.LC0
	   mov     edi, OFFSET FLAT:_ZSt4cout
	   call    std::basic_ostream<char, std::char_traits<char> >&
std::operator<< <std::char_traits<char> >(std::basic_ostream<char,
std::char_traits<char> >&, char const*) mov     esi, 7 mov     rdi, rax call
std::basic_ostream<char, std::char_traits<char> >::operator<<(unsigned long) mov
esi, OFFSET FLAT:.LC1 mov     rdi, rax call    std::basic_ostream<char,
std::char_traits<char> >& std::operator<< <std::char_traits<char>
>(std::basic_ostream<char, std::char_traits<char> >&, char const*) mov     eax,
0 pop     rbp ret
*/