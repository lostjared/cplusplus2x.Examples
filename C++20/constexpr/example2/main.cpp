#include <array>
#include <cstdio>
#include <iostream>
#include <string>

template <size_t N>
constexpr static const int find(const std::array<int, N> &values, const int item) {
	for(size_t i = 0; i < values.size(); ++i) {
		if(values[i] == item)
			return i;
	}
	constexpr const int not_found = -1;
	return not_found;
}

int main() {
	constexpr static std::array<int, 10> values{1, 2, 3, 4, 4, 6, 1, 2, 5, 10};
	constexpr static const int item = 5;
	constexpr static int pos = find(values, item);
	std::printf("indxx: %d\n", pos);
	return 0;
}

// compiles to without optimizations

/*
.LC0:
	   .string "indxx: %d\n"
main:
	   push    rbp
	   mov     rbp, rsp
	   mov     esi, 8
	   mov     edi, OFFSET FLAT:.LC0
	   mov     eax, 0
	   call    printf
	   mov     eax, 0
	   pop     rbp
	   ret
*/