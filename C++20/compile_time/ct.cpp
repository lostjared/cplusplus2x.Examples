#include <array>
#include <iostream>

int indexValue() {
	static constinit int index = 0;
	;
	return ++index;
}

constexpr bool is_prime(int value) {
	for(int i = 2; i < value / 2; ++i) {
		if(value % i == 0) {
			return false;
		}
	}
	return value > 1;
}

template <int N>
consteval std::array<int, N> prime_numbers() {
	std::array<int, N> primes;
	int z = 0;
	for(int i = 0; z < N; ++i) {
		if(is_prime(i)) {
			primes[z++] = i;
		}
	}
	return primes;
}
int main() {
	auto primes = prime_numbers<100>();
	for(int i = 0; i < primes.size(); ++i) {
		std::cout << i << ": -> " << primes[i] << " is prime\n";
	}
}

// compiles to generating the prime numbers at compile time
/*

.LC1:
		.string ": -> "
.LC2:
		.string " is prime\n"
.LC0:
		.long   2
		.long   3
		.long   4
		.long   5
		.long   7
		.long   11
		.long   13
		.long   17
		.long   19
		.long   23
		.long   29
		.long   31
		.long   37
		.long   41
		.long   43
		.long   47
		.long   53
		.long   59
		.long   61
		.long   67
		.long   71
		.long   73
		.long   79
		.long   83
		.long   89
		.long   97
		.long   101
		.long   103
		.long   107
		.long   109
		.long   113
		.long   127
		.long   131
		.long   137
		.long   139
		.long   149
		.long   151
		.long   157
		.long   163
		.long   167
		.long   173
		.long   179
		.long   181
		.long   191
		.long   193
		.long   197
		.long   199
		.long   211
		.long   223
		.long   227
		.long   229
		.long   233
		.long   239
		.long   241
		.long   251
		.long   257
		.long   263
		.long   269
		.long   271
		.long   277
		.long   281
		.long   283
		.long   293
		.long   307
		.long   311
		.long   313
		.long   317
		.long   331
		.long   337
		.long   347
		.long   349
		.long   353
		.long   359
		.long   367
		.long   373
		.long   379
		.long   383
		.long   389
		.long   397
		.long   401
		.long   409
		.long   419
		.long   421
		.long   431
		.long   433
		.long   439
		.long   443
		.long   449
		.long   457
		.long   461
		.long   463
		.long   467
		.long   479
		.long   487
		.long   491
		.long   499
		.long   503
		.long   509
		.long   521
		.long   523
main:
		push    rbp
		mov     rbp, rsp
		push    rbx
		sub     rsp, 424
		lea     rax, [rbp-432]
		mov     esi, OFFSET FLAT:.LC0
		mov     edx, 50
		mov     rdi, rax
		mov     rcx, rdx
		rep movsq
		mov     DWORD PTR [rbp-20], 0
		jmp     .L4
.L6:
		mov     eax, DWORD PTR [rbp-20]
		mov     esi, eax
		mov     edi, OFFSET FLAT:_ZSt4cout
		call    std::basic_ostream<char, std::char_traits<char> >::operator<<(int)
		mov     esi, OFFSET FLAT:.LC1
		mov     rdi, rax
		call    std::basic_ostream<char, std::char_traits<char> >& std::operator<< <std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*)
		mov     rbx, rax
		mov     eax, DWORD PTR [rbp-20]
		movsx   rdx, eax
		lea     rax, [rbp-432]
		mov     rsi, rdx
		mov     rdi, rax
		call    std::array<int, 100ul>::operator[](unsigned long)
		mov     eax, DWORD PTR [rax]
		mov     esi, eax
		mov     rdi, rbx
		call    std::basic_ostream<char, std::char_traits<char> >::operator<<(int)
		mov     esi, OFFSET FLAT:.LC2
		mov     rdi, rax
		call    std::basic_ostream<char, std::char_traits<char> >& std::operator<< <std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*)
		add     DWORD PTR [rbp-20], 1
.L4:
		mov     eax, DWORD PTR [rbp-20]
		cdqe
		mov     edx, 100
		cmp     rax, rdx
		jb      .L6
		mov     eax, 0
		mov     rbx, QWORD PTR [rbp-8]
		leave
		ret

*/
