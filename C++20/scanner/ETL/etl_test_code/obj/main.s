.section .data
t1: .asciz "Goodbye \n"
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
.globl init
init:
    pushq %rbp
    movq %rsp, %rbp
    subq $32, %rsp
    movq $0, %rcx
    movq $0, %rax
    pushq %rcx
    call program
    movq %rax, -8(%rbp)
    popq %rcx
    leaq t1(%rip), %rax
    movq %rax, -16(%rbp)
    movq -16(%rbp), %rdi# t1
    movq $0, %rax
    pushq %rcx
    call printf
    movq %rax, -24(%rbp)
    popq %rcx
    movq $0, %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rax# t3
    leave
    ret

