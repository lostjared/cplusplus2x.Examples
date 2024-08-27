.section .data
t19: .quad 0
t17: .quad 0
t11: .asciz "="
t3: .asciz "Enter second number: "
t7: .asciz "+"
t0: .asciz "Enter firsT number: "
.section .bss
    .lcomm tempBufferLHS, 24
    .lcomm tempBufferRHS, 24
.section .text
.globl main
main:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp
    call init
    movq %rax, %rbx
    leave
    movq %rbx, %rax
    ret
add_numbers:
    pushq %rbp
    movq %rsp, %rbp
    subq $144, %rsp
    leaq t0(%rip), %rax
    movq %rax, -8(%rbp)
    movq -8(%rbp), %rdi
    movq $0, %rax
    call puts
    movq %rax, -16(%rbp)
    movq $0, %rax
    call scan_integer
    movq %rax, -24(%rbp)
    movq %rax, -32(%rbp)
    leaq t3(%rip), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rdi
    movq $0, %rax
    call puts
    movq %rax, -48(%rbp)
    movq $0, %rax
    call scan_integer
    movq %rax, -56(%rbp)
    movq %rax, -64(%rbp)
    movq -32(%rbp), %rdi
    movq $0, %rax
    call str
    movq %rax, -72(%rbp)
    leaq t7(%rip), %rax
    movq %rax, -80(%rbp)
    xorq %rcx, %rcx
    movq -72(%rbp), %rdi
    call strlen
    addq %rax, %rcx
    addq $1, %rcx
    addq $1, %rcx
    movq %rcx, %rdi
    xorq %rax, %rax
    call malloc
    movq %rax, -88(%rbp)
    movq %rax, %rdi
    movq -72(%rbp), %rsi
    call strcpy
    movq -80(%rbp), %rsi
    call strcat
    movq -64(%rbp), %rdi
    movq $0, %rax
    call str
    movq %rax, -96(%rbp)
    xorq %rcx, %rcx
    movq -88(%rbp), %rdi
    call strlen
    addq %rax, %rcx
    movq -96(%rbp), %rdi
    call strlen
    addq %rax, %rcx
    addq $1, %rcx
    movq %rcx, %rdi
    xorq %rax, %rax
    call malloc
    movq %rax, -104(%rbp)
    movq %rax, %rdi
    movq -88(%rbp), %rsi
    call strcpy
    movq -96(%rbp), %rsi
    call strcat
    leaq t11(%rip), %rax
    movq %rax, -112(%rbp)
    xorq %rcx, %rcx
    movq -104(%rbp), %rdi
    call strlen
    addq %rax, %rcx
    addq $1, %rcx
    addq $1, %rcx
    movq %rcx, %rdi
    xorq %rax, %rax
    call malloc
    movq %rax, -120(%rbp)
    movq %rax, %rdi
    movq -104(%rbp), %rsi
    call strcpy
    movq -112(%rbp), %rsi
    call strcat
    movq -32(%rbp), %rax
    addq -64(%rbp), %rax
    movq %rax, -128(%rbp)
    movq -128(%rbp), %rdi
    movq $0, %rax
    call str
    movq %rax, -136(%rbp)
    xorq %rcx, %rcx
    movq -120(%rbp), %rdi
    call strlen
    addq %rax, %rcx
    movq -136(%rbp), %rdi
    call strlen
    addq %rax, %rcx
    addq $1, %rcx
    movq %rcx, %rdi
    xorq %rax, %rax
    call malloc
    movq %rax, -144(%rbp)
    movq %rax, %rdi
    movq -120(%rbp), %rsi
    call strcpy
    movq -136(%rbp), %rsi
    call strcat
    movq -144(%rbp), %rdi
    movq $0, %rax
    call puts
    movq %rax, -152(%rbp)
    movq $0, %rax
    movq %rax, -160(%rbp)
    movq -136(%rbp), %rdi
    call free
    movq -104(%rbp), %rdi
    call free
    movq -120(%rbp), %rdi
    call free
    movq -96(%rbp), %rdi
    call free
    movq -144(%rbp), %rdi
    call free
    movq -88(%rbp), %rdi
    call free
    movq -72(%rbp), %rdi
    call free
    leave
    ret
init:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp
    movq $0, %rax
    call add_numbers
    movq %rax, -8(%rbp)
    movq $0, %rax
    movq %rax, -16(%rbp)
    leave
    ret

