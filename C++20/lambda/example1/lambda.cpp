#include <iostream>
#include <string>

int main() {
	auto hash = [](const char *src) consteval {
		int total{};
		for (int i = 0; src[i] != '\0'; ++i) {
			total += src[i];
		}
		const int buckets = 255;
		return total % buckets;
	};
	constexpr static auto h = hash("the key");
	constexpr static const char *key = "the key";
	constexpr static auto k = hash(key);
	std::cout << h << ":" << k << "\n";
	if (h != k) {
		std::cerr << "Error\n";
	}
	return 0;
}

/* compiles to in GCC 13.2
notice there is no .string "the key" its all done at compile time
*/

/*
.LC1:
		.string ":"
.LC2:
		.string "\n"
main:
		push    rbp
		mov     rbp, rsp
		sub     rsp, 16
		mov     esi, 172
		mov     edi, OFFSET FLAT:_ZSt4cout
		call    std::basic_ostream<char, std::char_traits<char> >::operator<<(int)
		mov     esi, OFFSET FLAT:.LC1
		mov     rdi, rax
		call    std::basic_ostream<char, std::char_traits<char> >& std::operator<< <std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*)
		mov     esi, 172
		mov     rdi, rax
		call    std::basic_ostream<char, std::char_traits<char> >::operator<<(int)
		mov     esi, OFFSET FLAT:.LC2
		mov     rdi, rax
		call    std::basic_ostream<char, std::char_traits<char> >& std::operator<< <std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*)
		mov     eax, 0
		leave
		ret
*/