.section .data
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
    subq $16, %rsp
    movq $0, %rcx
    movq $0, %rax
    pushq %rcx
    call hello
    movq %rax, -8(%rbp)
    popq %rcx
    movq $0, %rax
    movq %rax, -16(%rbp)
    movq -16(%rbp), %rax# t1
    leave
    ret

