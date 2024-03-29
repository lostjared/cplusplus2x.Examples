#include <iostream>

consteval int len(const char *src) {
	int i{};
	while(src[i] != '\0') {
		++i;
	}
	return i;
}

consteval int find(const char *src, const char *search) {
	bool found = true;
	int s_length = len(search);
	int length = len(src) - s_length;
	for(int i = 0; i < length; ++i) {
		found = true;
		for(int z = 0; z < s_length; ++z) {
			if(src[i + z] != search[z]) {
				found = false;
				break;
			}
		}
		if(found == true)
			return i;
	}
	return -1;
}

int main() {
	constexpr auto search = find("test 1 2 3", "1");
	constexpr auto f = find("apples, bannas, oranges", "or");
	std::cout << "found or: " << f << " 1 at: " << search << "\n";
	return 0;
}

// compiles to with GCC 13.2
// no searching done at compile time
/*
.LC0:
		.string "found or: "
.LC1:
		.string " 1 at: "
.LC2:
		.string "\n"
main:
		push    rbp
		mov     rbp, rsp
		sub     rsp, 16
		mov     DWORD PTR [rbp-4], 5
		mov     DWORD PTR [rbp-8], 16
		mov     esi, OFFSET FLAT:.LC0
		mov     edi, OFFSET FLAT:_ZSt4cout
		call    std::basic_ostream<char, std::char_traits<char> >& std::operator<< <std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*)
		mov     esi, 16
		mov     rdi, rax
		call    std::basic_ostream<char, std::char_traits<char> >::operator<<(int)
		mov     esi, OFFSET FLAT:.LC1
		mov     rdi, rax
		call    std::basic_ostream<char, std::char_traits<char> >& std::operator<< <std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*)
		mov     esi, 5
		mov     rdi, rax
		call    std::basic_ostream<char, std::char_traits<char> >::operator<<(int)
		mov     esi, OFFSET FLAT:.LC2
		mov     rdi, rax
		call    std::basic_ostream<char, std::char_traits<char> >& std::operator<< <std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*)
		mov     eax, 0
		leave
		ret
*/