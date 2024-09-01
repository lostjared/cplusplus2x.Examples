.section .data
t0: .asciz "Hey this is a number: "
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
    subq $128, %rsp
    movq $0, %rcx
    leaq t0(%rip), %rax
    movq %rax, -40(%rbp)
    movq $10, %rax
    movq %rax, -48(%rbp)
    movq $10, %rax
    movq %rax, -56(%rbp)
    movq -48(%rbp), %rax # t1 # t1
    imulq -56(%rbp), %rax
    movq %rax, -64(%rbp)
    movq -64(%rbp), %rdi # t3 # t3
    movq $0, %rax
    call str
    movq %rax, -72(%rbp)
    addq $22, -80(%rbp)
    movq $0, %rcx
    movq %rcx, -80(%rbp)
     addq $25, -80(%rbp)
    movq -72(%rbp), %rdi # t4 # t4
    call strlen # t4
    addq %rax, -80(%rbp)
    addq $1, -80(%rbp)
    movq $1, %rsi
    xorq %rax, %rax
    movq -80(%rbp), %rdi # counter # counter
    call calloc
    movq %rax, %rdi
    movq %rdi, -88(%rbp)
    movq -40(%rbp), %rsi # t0 # t0
    call strcpy
    movq -72(%rbp), %rsi # t4 # t4
    call strcat
    movq -88(%rbp), %rdi # t5 # t5
    movq $0, %rax
    call puts
    movq %rax, -96(%rbp)
    movq $0, %rax
    movq %rax, -104(%rbp)
    movq -88(%rbp), %rdi # t5 # t5
    call free #t5
    movq -72(%rbp), %rdi # t4 # t4
    call free #t4
    movq -104(%rbp), %rax # t7 # t7
    leave
    ret
.section .note.GNU-stack,"",@progbits

