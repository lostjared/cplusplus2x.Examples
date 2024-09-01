.section .data
t0: .asciz "Hello,$### \" World!"
.section .text
.globl main
main:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp
    call init
    movq $0, %rax
    leave
    ret
    movq %rdi, -8(%rbp)
    movq %rsi, -8(%rbp)
    movq %rdx, -16(%rbp)
    movq %rcx, -24(%rbp)
    movq %r8, -32(%rbp)
    movq %r9, -8(%rbp)
.globl init
init:
    pushq %rbp
    movq %rsp, %rbp
    subq $96, %rsp
    movq $0, %rcx
    leaq t0(%rip), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rdi # t0 # t0
    movq $0, %rax
    call puts
    movq %rax, -48(%rbp)
    movq $0, %rax
    movq %rax, -56(%rbp)
    leave
    ret
.section .note.GNU-stack,"",@progbits

