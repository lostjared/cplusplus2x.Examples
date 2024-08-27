.section .data
t13: .quad 0
t9: .asciz "The Value is: %d\n"
t6: .quad 4
t5: .quad 2
z: .quad 50
t11: .quad 0
y: .quad 100
x: .quad 25
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
print_numbers:
    pushq %rbp
    movq %rsp, %rbp
    subq $96, %rsp
    movq $25, %rax
    movq %rax, -8(%rbp)
    movq $100, %rax
    movq %rax, -16(%rbp)
    movq $50, %rax
    movq %rax, -24(%rbp)
    movq -8(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -16(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -32(%rbp), %rax
    imulq -40(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -24(%rbp), %rax
    movq %rax, -56(%rbp)
    movq -48(%rbp), %rax
    cqto
    idivq -56(%rbp)
    movq %rax, -64(%rbp)
    movq $2, %rax
    movq %rax, -72(%rbp)
    movq $4, %rax
    movq %rax, -80(%rbp)
    movq -72(%rbp), %rax
    addq -80(%rbp), %rax
    movq %rax, -88(%rbp)
    movq -64(%rbp), %rax
    addq -88(%rbp), %rax
    movq %rax, -96(%rbp)
    movq %rax, -104(%rbp)
    leaq t9(%rip), %rax
    movq %rax, -112(%rbp)
    movq -112(%rbp), %rdi
    movq -104(%rbp), %rsi
    movq $0, %rax
    call printf
    movq %rax, -120(%rbp)
    movq $0, %rax
    movq %rax, -128(%rbp)
    leave
    ret
init:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp
    movq $0, %rax
    call print_numbers
    movq %rax, -8(%rbp)
    movq $0, %rax
    movq %rax, -16(%rbp)
    leave
    ret

