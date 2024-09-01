.section .data
t7: .asciz "="
t2: .asciz "+"
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
    subq $256, %rsp
    movq $0, %rcx
    movq $200, %rax
    movq %rax, -40(%rbp)
    movq $300, %rax
    movq %rax, -48(%rbp)
    movq -40(%rbp), %rax # x # x
    movq %rax, -56(%rbp)
    movq -56(%rbp), %rdi # t0 # t0
    movq $0, %rax
    call str
    movq %rax, -64(%rbp)
    addq $22, -72(%rbp)
    leaq t2(%rip), %rax
    movq %rax, -80(%rbp)
    movq $0, %rcx
    movq %rcx, -72(%rbp)
    movq -64(%rbp), %rdi # t1 # t1
    call strlen # t1
    addq %rax, -72(%rbp)
    addq $4, -72(%rbp)
    addq $1, -72(%rbp)
    movq $1, %rsi
    xorq %rax, %rax
    movq -72(%rbp), %rdi # counter # counter
    call calloc
    movq %rax, %rdi
    movq %rdi, -88(%rbp)
    movq -64(%rbp), %rsi # t1 # t1
    call strcpy
    movq -80(%rbp), %rsi # t2 # t2
    call strcat
    movq -48(%rbp), %rax # y # y
    movq %rax, -96(%rbp)
    movq -96(%rbp), %rdi # t4 # t4
    movq $0, %rax
    call str
    movq %rax, -104(%rbp)
    addq $22, -72(%rbp)
    movq $0, %rcx
    movq -88(%rbp), %rdi # t3 # t3
    call strlen # t3
    addq %rax, -72(%rbp)
    movq -104(%rbp), %rdi # t5 # t5
    call strlen # t5
    addq %rax, -72(%rbp)
    addq $1, -72(%rbp)
    movq $1, %rsi
    xorq %rax, %rax
    movq -72(%rbp), %rdi # counter # counter
    call calloc
    movq %rax, %rdi
    movq %rdi, -112(%rbp)
    movq -88(%rbp), %rsi # t3 # t3
    call strcpy
    movq -104(%rbp), %rsi # t5 # t5
    call strcat
    leaq t7(%rip), %rax
    movq %rax, -120(%rbp)
    movq $0, %rcx
    movq -112(%rbp), %rdi # t6 # t6
    call strlen # t6
    addq %rax, -72(%rbp)
    addq $4, -72(%rbp)
    addq $1, -72(%rbp)
    movq $1, %rsi
    xorq %rax, %rax
    movq -72(%rbp), %rdi # counter # counter
    call calloc
    movq %rax, %rdi
    movq %rdi, -128(%rbp)
    movq -112(%rbp), %rsi # t6 # t6
    call strcpy
    movq -120(%rbp), %rsi # t7 # t7
    call strcat
    movq -40(%rbp), %rax # x # x
    movq %rax, -136(%rbp)
    movq -48(%rbp), %rax # y # y
    movq %rax, -144(%rbp)
    movq -136(%rbp), %rax # t9 # t9
    addq -144(%rbp), %rax
    movq %rax, -152(%rbp)
    movq -152(%rbp), %rdi # t11 # t11
    movq $0, %rax
    call str
    movq %rax, -160(%rbp)
    addq $22, -72(%rbp)
    movq $0, %rcx
    movq -128(%rbp), %rdi # t8 # t8
    call strlen # t8
    addq %rax, -72(%rbp)
    movq -160(%rbp), %rdi # t12 # t12
    call strlen # t12
    addq %rax, -72(%rbp)
    addq $1, -72(%rbp)
    movq $1, %rsi
    xorq %rax, %rax
    movq -72(%rbp), %rdi # counter # counter
    call calloc
    movq %rax, %rdi
    movq %rdi, -168(%rbp)
    movq -128(%rbp), %rsi # t8 # t8
    call strcpy
    movq -160(%rbp), %rsi # t12 # t12
    call strcat
    movq -168(%rbp), %rdi # t13 # t13
    movq $0, %rax
    call puts
    movq %rax, -176(%rbp)
    movq -40(%rbp), %rax # x # x
    movq %rax, -184(%rbp)
    movq -40(%rbp), %rax # x # x
    movq %rax, -192(%rbp)
    movq -184(%rbp), %rax # t15 # t15
    imulq -192(%rbp), %rax
    movq %rax, -200(%rbp)
    movq -200(%rbp), %rdi # t17 # t17
    movq $0, %rax
    call str
    movq %rax, -208(%rbp)
    addq $22, -72(%rbp)
    movq -208(%rbp), %rdi # t18 # t18
    movq $0, %rax
    call puts
    movq %rax, -216(%rbp)
    movq $0, %rax
    movq %rax, -224(%rbp)
    movq -208(%rbp), %rdi # t18 # t18
    call free #t18
    movq -168(%rbp), %rdi # t13 # t13
    call free #t13
    movq -160(%rbp), %rdi # t12 # t12
    call free #t12
    movq -128(%rbp), %rdi # t8 # t8
    call free #t8
    movq -104(%rbp), %rdi # t5 # t5
    call free #t5
    movq -88(%rbp), %rdi # t3 # t3
    call free #t3
    movq -112(%rbp), %rdi # t6 # t6
    call free #t6
    movq -64(%rbp), %rdi # t1 # t1
    call free #t1
    movq -224(%rbp), %rax # t20 # t20
    leave
    ret
.section .note.GNU-stack,"",@progbits

