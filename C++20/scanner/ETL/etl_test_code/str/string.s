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
    movq %rax, %rbx
    leave
    movq %rbx, %rax
    ret
.globl init
init:
    pushq %rbp
    movq %rsp, %rbp
    subq $192, %rsp
    movq $0, %rcx
    movq $200, %rax
    movq %rax, -8(%rbp)
    movq $300, %rax
    movq %rax, -16(%rbp)
    movq -8(%rbp), %rax# x
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rdi# t0
    movq $0, %rax
    pushq %rcx
    call str
    movq %rax, -32(%rbp)
    popq %rcx
    addq $22, %rcx
    leaq t2(%rip), %rax
    movq %rax, -40(%rbp)
    movq -32(%rbp), %rdi# t1
    pushq %rcx
    call strlen # t1
    popq %rcx
    addq %rax, %rcx
    addq $1, %rcx
    addq $1, %rcx
    movq %rcx, %rdi
    movq $1, %rsi
    xorq %rax, %rax
    pushq %rcx
    call calloc
    movq %rax, %rdi
    movq %rdi, -48(%rbp)
    movq -32(%rbp), %rsi# t1
    call strcpy
    movq -40(%rbp), %rsi# t2
    call strcat
    popq %rcx
    movq -16(%rbp), %rax# y
    movq %rax, -56(%rbp)
    movq -56(%rbp), %rdi# t4
    movq $0, %rax
    pushq %rcx
    call str
    movq %rax, -64(%rbp)
    popq %rcx
    addq $22, %rcx
    movq -48(%rbp), %rdi# t3
    pushq %rcx
    call strlen # t3
    popq %rcx
    addq %rax, %rcx
    movq -64(%rbp), %rdi# t5
    pushq %rcx
    call strlen # t5
    popq %rcx
    addq %rax, %rcx
    addq $1, %rcx
    movq %rcx, %rdi
    movq $1, %rsi
    xorq %rax, %rax
    pushq %rcx
    call calloc
    movq %rax, %rdi
    movq %rdi, -72(%rbp)
    movq -48(%rbp), %rsi# t3
    call strcpy
    movq -64(%rbp), %rsi# t5
    call strcat
    popq %rcx
    leaq t7(%rip), %rax
    movq %rax, -80(%rbp)
    movq -72(%rbp), %rdi# t6
    pushq %rcx
    call strlen # t6
    popq %rcx
    addq %rax, %rcx
    addq $1, %rcx
    addq $1, %rcx
    movq %rcx, %rdi
    movq $1, %rsi
    xorq %rax, %rax
    pushq %rcx
    call calloc
    movq %rax, %rdi
    movq %rdi, -88(%rbp)
    movq -72(%rbp), %rsi# t6
    call strcpy
    movq -80(%rbp), %rsi# t7
    call strcat
    popq %rcx
    movq -8(%rbp), %rax# x
    movq %rax, -96(%rbp)
    movq -16(%rbp), %rax# y
    movq %rax, -104(%rbp)
    movq -96(%rbp), %rax# t9
    addq -104(%rbp), %rax
    movq %rax, -112(%rbp)
    movq -112(%rbp), %rdi# t11
    movq $0, %rax
    pushq %rcx
    call str
    movq %rax, -120(%rbp)
    popq %rcx
    addq $22, %rcx
    movq -88(%rbp), %rdi# t8
    pushq %rcx
    call strlen # t8
    popq %rcx
    addq %rax, %rcx
    movq -120(%rbp), %rdi# t12
    pushq %rcx
    call strlen # t12
    popq %rcx
    addq %rax, %rcx
    addq $1, %rcx
    movq %rcx, %rdi
    movq $1, %rsi
    xorq %rax, %rax
    pushq %rcx
    call calloc
    movq %rax, %rdi
    movq %rdi, -128(%rbp)
    movq -88(%rbp), %rsi# t8
    call strcpy
    movq -120(%rbp), %rsi# t12
    call strcat
    popq %rcx
    movq -128(%rbp), %rdi# t13
    movq $0, %rax
    pushq %rcx
    call puts
    movq %rax, -136(%rbp)
    popq %rcx
    movq -8(%rbp), %rax# x
    movq %rax, -144(%rbp)
    movq -8(%rbp), %rax# x
    movq %rax, -152(%rbp)
    movq -144(%rbp), %rax# t15
    imulq -152(%rbp), %rax
    movq %rax, -160(%rbp)
    movq -160(%rbp), %rdi# t17
    movq $0, %rax
    pushq %rcx
    call str
    movq %rax, -168(%rbp)
    popq %rcx
    addq $22, %rcx
    movq -168(%rbp), %rdi# t18
    movq $0, %rax
    pushq %rcx
    call puts
    movq %rax, -176(%rbp)
    popq %rcx
    movq $0, %rax
    movq %rax, -184(%rbp)
    movq -184(%rbp), %rax# t20
    movq -168(%rbp), %rdi# t18
    call free #t18
    movq -128(%rbp), %rdi# t13
    call free #t13
    movq -120(%rbp), %rdi# t12
    call free #t12
    movq -88(%rbp), %rdi# t8
    call free #t8
    movq -64(%rbp), %rdi# t5
    call free #t5
    movq -48(%rbp), %rdi# t3
    call free #t3
    movq -72(%rbp), %rdi# t6
    call free #t6
    movq -32(%rbp), %rdi# t1
    call free #t1
    leave
    ret
.section .note.GNU-stack,"",@progbits

