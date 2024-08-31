.section .data
s: .asciz "Jared Says: "
t18: .asciz "Hey two numers: "
t11: .asciz "="
t6: .asciz "+"
t1: .asciz ": "
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
.globl format
format:
    pushq %rbp
    movq %rsp, %rbp
    subq $192, %rsp
    movq $0, %rcx
    movq %rdi, -8(%rbp)
    movq %rsi, -16(%rbp)
    movq %rdx, -24(%rbp)
    movq -8(%rbp), %rax # hello # hello
    movq %rax, -32(%rbp)
    leaq t1(%rip), %rax
    movq %rax, -40(%rbp)
    movq -32(%rbp), %rdi # t0 # t0
    pushq %rcx
    call strlen # t0
    popq %rcx
    addq %rax, %rcx
    addq $5, %rcx
    addq $1, %rcx
    movq %rcx, %rdi
    movq $1, %rsi
    xorq %rax, %rax
    pushq %rcx
    call calloc
    popq %rcx
    movq %rax, %rdi
    movq %rdi, -48(%rbp)
    movq -32(%rbp), %rsi # t0 # t0
    pushq %rcx
    call strcpy
    movq -40(%rbp), %rsi # t1 # t1
    call strcat
    popq %rcx
    movq -16(%rbp), %rax # x # x
    movq %rax, -56(%rbp)
    movq -56(%rbp), %rdi # t3 # t3
    pushq %rcx
    movq $0, %rax
    call str
    movq %rax, -64(%rbp)
    popq %rcx
    addq $22, %rcx
    movq -48(%rbp), %rdi # t2 # t2
    pushq %rcx
    call strlen # t2
    popq %rcx
    addq %rax, %rcx
    movq -64(%rbp), %rdi # t4 # t4
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
    popq %rcx
    movq %rax, %rdi
    movq %rdi, -72(%rbp)
    movq -48(%rbp), %rsi # t2 # t2
    pushq %rcx
    call strcpy
    movq -64(%rbp), %rsi # t4 # t4
    call strcat
    popq %rcx
    leaq t6(%rip), %rax
    movq %rax, -80(%rbp)
    movq -72(%rbp), %rdi # t5 # t5
    pushq %rcx
    call strlen # t5
    popq %rcx
    addq %rax, %rcx
    addq $4, %rcx
    addq $1, %rcx
    movq %rcx, %rdi
    movq $1, %rsi
    xorq %rax, %rax
    pushq %rcx
    call calloc
    popq %rcx
    movq %rax, %rdi
    movq %rdi, -88(%rbp)
    movq -72(%rbp), %rsi # t5 # t5
    pushq %rcx
    call strcpy
    movq -80(%rbp), %rsi # t6 # t6
    call strcat
    popq %rcx
    movq -24(%rbp), %rax # y # y
    movq %rax, -96(%rbp)
    movq -96(%rbp), %rdi # t8 # t8
    pushq %rcx
    movq $0, %rax
    call str
    movq %rax, -104(%rbp)
    popq %rcx
    addq $22, %rcx
    movq -88(%rbp), %rdi # t7 # t7
    pushq %rcx
    call strlen # t7
    popq %rcx
    addq %rax, %rcx
    movq -104(%rbp), %rdi # t9 # t9
    pushq %rcx
    call strlen # t9
    popq %rcx
    addq %rax, %rcx
    addq $1, %rcx
    movq %rcx, %rdi
    movq $1, %rsi
    xorq %rax, %rax
    pushq %rcx
    call calloc
    popq %rcx
    movq %rax, %rdi
    movq %rdi, -112(%rbp)
    movq -88(%rbp), %rsi # t7 # t7
    pushq %rcx
    call strcpy
    movq -104(%rbp), %rsi # t9 # t9
    call strcat
    popq %rcx
    leaq t11(%rip), %rax
    movq %rax, -120(%rbp)
    movq -112(%rbp), %rdi # t10 # t10
    pushq %rcx
    call strlen # t10
    popq %rcx
    addq %rax, %rcx
    addq $4, %rcx
    addq $1, %rcx
    movq %rcx, %rdi
    movq $1, %rsi
    xorq %rax, %rax
    pushq %rcx
    call calloc
    popq %rcx
    movq %rax, %rdi
    movq %rdi, -128(%rbp)
    movq -112(%rbp), %rsi # t10 # t10
    pushq %rcx
    call strcpy
    movq -120(%rbp), %rsi # t11 # t11
    call strcat
    popq %rcx
    movq -16(%rbp), %rax # x # x
    movq %rax, -136(%rbp)
    movq -24(%rbp), %rax # y # y
    movq %rax, -144(%rbp)
    movq -136(%rbp), %rax # t13 # t13
    addq -144(%rbp), %rax
    movq %rax, -152(%rbp)
    movq -152(%rbp), %rdi # t15 # t15
    pushq %rcx
    movq $0, %rax
    call str
    movq %rax, -160(%rbp)
    popq %rcx
    addq $22, %rcx
    movq -128(%rbp), %rdi # t12 # t12
    pushq %rcx
    call strlen # t12
    popq %rcx
    addq %rax, %rcx
    movq -160(%rbp), %rdi # t16 # t16
    pushq %rcx
    call strlen # t16
    popq %rcx
    addq %rax, %rcx
    addq $1, %rcx
    movq %rcx, %rdi
    movq $1, %rsi
    xorq %rax, %rax
    pushq %rcx
    call calloc
    popq %rcx
    movq %rax, %rdi
    movq %rdi, -168(%rbp)
    movq -128(%rbp), %rsi # t12 # t12
    pushq %rcx
    call strcpy
    movq -160(%rbp), %rsi # t16 # t16
    call strcat
    popq %rcx
    movq -160(%rbp), %rdi # t16 # t16
    call free #t16
    movq -128(%rbp), %rdi # t12 # t12
    call free #t12
    movq -104(%rbp), %rdi # t9 # t9
    call free #t9
    movq -112(%rbp), %rdi # t10 # t10
    call free #t10
    movq -88(%rbp), %rdi # t7 # t7
    call free #t7
    movq -72(%rbp), %rdi # t5 # t5
    call free #t5
    movq -64(%rbp), %rdi # t4 # t4
    call free #t4
    movq -48(%rbp), %rdi # t2 # t2
    call free #t2
    movq -168(%rbp), %rax # t17 # t17
    leave
    ret
.globl init
init:
    pushq %rbp
    movq %rsp, %rbp
    subq $160, %rsp
    movq $0, %rcx
    movq $25, %rax
    movq %rax, -8(%rbp)
    movq $25, %rax
    movq %rax, -16(%rbp)
    leaq t18(%rip), %rax
    movq %rax, -24(%rbp)
    movq -8(%rbp), %rax # x # x
    movq %rax, -32(%rbp)
    movq -16(%rbp), %rax # y # y
    movq %rax, -40(%rbp)
    movq -24(%rbp), %rdi # t18 # t18
    movq -32(%rbp), %rsi # t19 # t19
    movq -40(%rbp), %rdx # t20 # t20
    pushq %rcx
    movq $0, %rax
    call format
    movq %rax, -48(%rbp)
    popq %rcx
    movq -48(%rbp), %rdi # t21 # t21
    pushq %rcx
    movq $0, %rax
    call puts
    movq %rax, -56(%rbp)
    popq %rcx
    leaq s(%rip), %rax
    movq %rax, -64(%rbp)
    movq %rax, -72(%rbp)
    movq $10, %rax
    movq %rax, -80(%rbp)
    movq $10, %rax
    movq %rax, -88(%rbp)
    movq -72(%rbp), %rdi # t23 # t23
    movq -80(%rbp), %rsi # t24 # t24
    movq -88(%rbp), %rdx # t25 # t25
    pushq %rcx
    movq $0, %rax
    call format
    movq %rax, -96(%rbp)
    popq %rcx
    movq -96(%rbp), %rdi # t26 # t26
    pushq %rcx
    movq $0, %rax
    call puts
    movq %rax, -104(%rbp)
    popq %rcx
    movq $0, %rax
    movq %rax, -112(%rbp)
    movq -96(%rbp), %rdi # t26 # t26
    call free #t26
    movq -48(%rbp), %rdi # t21 # t21
    call free #t21
    movq -112(%rbp), %rax # t28 # t28
    leave
    ret
.section .note.GNU-stack,"",@progbits

