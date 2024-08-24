.section .data
str0: .asciz "Hello, World!"
.section .text
.globl main
main:
    pushq %rbp
    movq %rsp, %rbp
    subq $0, %rsp
    call init
    movq %rax, %rbx
    leave
    movq %rbx, %rax
    ret
init:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp
    leaq str0(%rip), %rax
    movq %rax, -8(%rbp)
    movq -8(%rbp), %rdi
    movq $0, %rax
    call puts
    movq %rax, -16(%rbp)
    movq $0, %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    leave
    ret

