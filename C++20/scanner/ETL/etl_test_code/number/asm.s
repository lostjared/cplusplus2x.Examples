.section .data
str0: .asciz "The Value is: %d\n"
.section .text
.globl main
main:
    pushq %rbp
    movq %rsp, %rbp
    subq $0, %rsp
    call init
    movq %rax, %rbx
    leave
    movq %rbx, %rax
    ret
print_numbers:
    pushq %rbp
    movq %rsp, %rbp
    subq $120, %rsp
    movq $25, %rax
    movq %rax, -8(%rbp)
    movq -8(%rbp), %rax
    movq %rax, -16(%rbp)
    movq $100, %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movq %rax, -32(%rbp)
    movq $50, %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -16(%rbp), %rax
    movq %rax, -56(%rbp)
    movq -32(%rbp), %rax
    movq %rax, -64(%rbp)
    movq -56(%rbp), %rax
    imulq -64(%rbp), %rax
    movq %rax, -72(%rbp)
    movq -48(%rbp), %rax
    movq %rax, -80(%rbp)
    movq -72(%rbp), %rax
    cqto
    idivq -80(%rbp)
    movq %rax, -88(%rbp)
    movq $2, %rax
    movq %rax, -96(%rbp)
    movq $4, %rax
    movq %rax, -104(%rbp)
    movq -96(%rbp), %rax
    addq -104(%rbp), %rax
    movq %rax, -112(%rbp)
    movq -88(%rbp), %rax
    addq -112(%rbp), %rax
    movq %rax, -120(%rbp)
    movq -120(%rbp), %rax
    movq %rax, -128(%rbp)
    leaq str0(%rip), %rax
    movq %rax, -136(%rbp)
    movq -128(%rbp), %rax
    movq %rax, -144(%rbp)
    movq -136(%rbp), %rdi
    movq -144(%rbp), %rsi
    movq $0, %rax
    call printf
    movq %rax, -152(%rbp)
    movq $0, %rax
    movq %rax, -160(%rbp)
    movq -160(%rbp), %rax
    leave
    ret
init:
    pushq %rbp
    movq %rsp, %rbp
    subq $8, %rsp
    movq $0, %rax
    call print_numbers
    movq %rax, -168(%rbp)
    movq $0, %rax
    movq %rax, -176(%rbp)
    movq -176(%rbp), %rax
    leave
    ret

