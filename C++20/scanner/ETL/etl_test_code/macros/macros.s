.section .data
t0: .asciz "Hey this is a number: "
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
    subq $64, %rsp
    movq $0, %rcx
    leaq t0(%rip), %rax
    movq %rax, -8(%rbp)
    movq $10, %rax
    movq %rax, -16(%rbp)
    movq $10, %rax
    movq %rax, -24(%rbp)
    movq -16(%rbp), %rax# t1
    imulq -24(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -32(%rbp), %rdi# t3
    movq $0, %rax
    pushq %rcx
    call str
    movq %rax, -40(%rbp)
    popq %rcx
    addq $22, %rcx
     addq $25, %rcx
    movq -40(%rbp), %rdi# t4
    pushq %rcx
    call strlen # t4
    popq %rcx
    addq %rax, %rcx
    addq $1, %rcx
    movq %rcx, %rdi
    movq $1, %rsi
    xorq %rax, %rax
    pushq %rcx
    call calloc
    movq %rax, %rdi
    movq %rdi, -48(%rbp)
    movq -8(%rbp), %rsi# t0
    call strcpy
    movq -40(%rbp), %rsi# t4
    call strcat
    popq %rcx
    movq -48(%rbp), %rdi# t5
    movq $0, %rax
    pushq %rcx
    call puts
    movq %rax, -56(%rbp)
    popq %rcx
    movq $0, %rax
    movq %rax, -64(%rbp)
    movq -64(%rbp), %rax# t7
    movq -48(%rbp), %rdi# t5
    call free #t5
    movq -40(%rbp), %rdi# t4
    call free #t4
    leave
    ret
.section .note.GNU-stack,"",@progbits

