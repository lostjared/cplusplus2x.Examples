.section .data
t7: .asciz "%d * %d / 100 = %d\n"
t0: .asciz "Hello, World"
.section .text
    movq %rdi, -8(%rbp)
    movq %rsi, -8(%rbp)
    movq %rdx, -16(%rbp)
    movq %rcx, -24(%rbp)
    movq %r8, -32(%rbp)
    movq %r9, -8(%rbp)
.globl program
program:
    pushq %rbp
    movq %rsp, %rbp
    subq $192, %rsp
    movq $0, %rcx
    leaq t0(%rip), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rdi # t0 # t0
    movq $0, %rax
    call puts
    movq %rax, -48(%rbp)
    movq $25, %rax
    movq %rax, -56(%rbp)
    movq $25, %rax
    movq %rax, -64(%rbp)
    movq -56(%rbp), %rax # x # x
    movq %rax, -72(%rbp)
    movq -64(%rbp), %rax # y # y
    movq %rax, -80(%rbp)
    movq -72(%rbp), %rax # t2 # t2
    imulq -80(%rbp), %rax
    movq %rax, -88(%rbp)
    movq $100, %rax
    movq %rax, -96(%rbp)
    movq -88(%rbp), %rax # t4 # t4
    cqto
    idivq -96(%rbp)
    movq %rax, -104(%rbp)
    movq %rax, -112(%rbp)
    leaq t7(%rip), %rax
    movq %rax, -120(%rbp)
    movq -56(%rbp), %rax # x # x
    movq %rax, -128(%rbp)
    movq -64(%rbp), %rax # y # y
    movq %rax, -136(%rbp)
    movq -120(%rbp), %rdi # t7 # t7
    movq -128(%rbp), %rsi # t8 # t8
    movq -136(%rbp), %rdx # t9 # t9
    movq -112(%rbp), %rcx # z # z
    movq $0, %rax
    call printf
    movq %rax, -144(%rbp)
    movq $0, %rax
    movq %rax, -152(%rbp)
    leave
    ret
.section .note.GNU-stack,"",@progbits

