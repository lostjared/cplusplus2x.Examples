.section .data
str0: .asciz "The Value is: %d\n"
num6: .quad 0
num4: .quad 4
num3: .quad 2
num2: .quad 50
num1: .quad 100
num0: .quad 25
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
print_numbers:
    pushq %rbp
    movq %rsp, %rbp
    subq $96, %rsp
    movq num0(%rip), %rax
    movq %rax, -8(%rbp)
    movq num1(%rip), %rax
    movq %rax, -16(%rbp)
    movq num2(%rip), %rax
    movq %rax, -24(%rbp)
    movq -8(%rbp), %rax
    movq %rax, -32(%rbp)
    movq -16(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -32(%rbp), %rax
    imulq -40(%rbp), %rax
    movq %rax, -48(%rbp)
    movq -24(%rbp), %rax
    movq %rax, -56(%rbp)
    movq -48(%rbp), %rax
    cqto
    idivq -56(%rbp)
    movq %rax, -64(%rbp)
    movq num3(%rip), %rax
    movq %rax, -72(%rbp)
    movq num4(%rip), %rax
    movq %rax, -80(%rbp)
    movq -72(%rbp), %rax
    addq -80(%rbp), %rax
    movq %rax, -88(%rbp)
    movq -64(%rbp), %rax
    addq -88(%rbp), %rax
    movq %rax, -96(%rbp)
    movq %rax, -104(%rbp)
    leaq str0(%rip), %rax
    movq %rax, -112(%rbp)
    movq -112(%rbp), %rdi
    movq -104(%rbp), %rsi
    movq $0, %rax
    call printf
    movq %rax, -120(%rbp)
    movq num6(%rip), %rax
    movq %rax, -128(%rbp)
    leave
    ret
init:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp
    movq $0, %rax
    call print_numbers
    movq %rax, -136(%rbp)
    movq num6(%rip), %rax
    movq %rax, -144(%rbp)
    leave
    ret

