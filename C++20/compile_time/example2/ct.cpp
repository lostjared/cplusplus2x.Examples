#include <array>
#include <iostream>
#include <ranges>

template <std::size_t N>
consteval int indexOf(std::array<int, N> v, int s) {
	for(int i = 0; i < v.size(); ++i)
		if(v[i] == s)
			return i;
	return -1;
}

template <typename T, std::size_t N>
consteval auto sum(std::array<T, N> values) {
	T sum{};
	for(auto &i : std::views::all(values)) {
		sum += i;
	}
	return sum;
}

int main() {
	constexpr std::array v{0, 3, 4, 8, 6, 1};
	constexpr auto i = indexOf(v, 8);
	constexpr auto s = sum(v);
	std::cout << "index is: " << i << "\n";
	std::cout << "sum is: " << s << "\n";
	return 0;
}

// compiles to with GCC 13.2
/*
.LC0:
		.string "index is: "
.LC1:
		.string "\n"
.LC2:
		.string "sum is: "
main:
		push    rbp
		mov     rbp, rsp
		sub     rsp, 32
		mov     DWORD PTR [rbp-32], 0
		mov     DWORD PTR [rbp-28], 3
		mov     DWORD PTR [rbp-24], 4
		mov     DWORD PTR [rbp-20], 8
		mov     DWORD PTR [rbp-16], 6
		mov     DWORD PTR [rbp-12], 1
		mov     DWORD PTR [rbp-4], 3
		mov     DWORD PTR [rbp-8], 22
		mov     esi, OFFSET FLAT:.LC0
		mov     edi, OFFSET FLAT:_ZSt4cout
		call    std::basic_ostream<char, std::char_traits<char> >& std::operator<< <std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*)
		mov     esi, 3
		mov     rdi, rax
		call    std::basic_ostream<char, std::char_traits<char> >::operator<<(int)
		mov     esi, OFFSET FLAT:.LC1
		mov     rdi, rax
		call    std::basic_ostream<char, std::char_traits<char> >& std::operator<< <std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*)
		mov     esi, OFFSET FLAT:.LC2
		mov     edi, OFFSET FLAT:_ZSt4cout
		call    std::basic_ostream<char, std::char_traits<char> >& std::operator<< <std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*)
		mov     esi, 22
		mov     rdi, rax
		call    std::basic_ostream<char, std::char_traits<char> >::operator<<(int)
		mov     esi, OFFSET FLAT:.LC1
		mov     rdi, rax
		call    std::basic_ostream<char, std::char_traits<char> >& std::operator<< <std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*)
		mov     eax, 0
		leave
		ret

*/