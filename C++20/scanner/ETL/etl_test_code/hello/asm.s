.section .data
str0: .asciz "Hello, World!"
num0: .quad 0
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
init:
    pushq %rbp
    movq %rsp, %rbp
    subq $32, %rsp
    leaq str0(%rip), %rax
    movq %rax, -8(%rbp)
    movq -8(%rbp), %rdi
    movq $0, %rax
    call puts
    movq %rax, -16(%rbp)
    movq num0(%rip), %rax
    movq %rax, -24(%rbp)
    leave
    ret

