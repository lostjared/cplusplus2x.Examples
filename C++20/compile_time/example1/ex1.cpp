#include <iostream>

consteval int len(const char *src) {
	int i = 0;
	while(src[i] != '\0') {
		i++;
	}
	return i;
}

int main(int argc, char **argv) {
	constexpr int l = len("test of getting the length of this long string");
	std::cout << "len: " << l << "\n";
	return 0;
}

// compiles to: doesn't even include the string in the code!
/*

.LC0:
		.string "len: "
.LC1:
		.string "\n"
main:
		push    rbp
		mov     rbp, rsp
		sub     rsp, 32
		mov     DWORD PTR [rbp-20], edi
		mov     QWORD PTR [rbp-32], rsi
		mov     DWORD PTR [rbp-4], 46
		mov     esi, OFFSET FLAT:.LC0
		mov     edi, OFFSET FLAT:_ZSt4cout
		call    std::basic_ostream<char, std::char_traits<char> >& std::operator<< <std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*)
		mov     esi, 46
		mov     rdi, rax
		call    std::basic_ostream<char, std::char_traits<char> >::operator<<(int)
		mov     esi, OFFSET FLAT:.LC1
		mov     rdi, rax
		call    std::basic_ostream<char, std::char_traits<char> >& std::operator<< <std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*)
		mov     eax, 0
		leave
		ret

*/