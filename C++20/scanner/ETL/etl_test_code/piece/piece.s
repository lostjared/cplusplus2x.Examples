.section __TEXT,__cstring
t50: .asciz "MasterPiece"
.section __TEXT,__text
.globl _start
_start:
    xor %rbp, %rbp
    movq %rsp, %rdi
    andq $-16, %rsp
    subq $8, %rsp 
    call _main
    movq %rax, %rdi
    movl $0, %edi
    call _exit

.globl _main
_main:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp
    call _init
    movq $0, %rax
    leave
    ret
.globl _draw_grid
_draw_grid:
    pushq %rbp
    movq %rsp, %rbp
    subq $496, %rsp
    movq $0, %rcx
    movq $0, %rax
    movq %rax, -8(%rbp)
    movq $0, %rax
    movq %rax, -16(%rbp)
    movq $0, %rax
    movq %rax, -24(%rbp)
    movq $255, %rax
    movq %rax, -32(%rbp)
    movq -8(%rbp), %rdi # t0 # t0
    movq -16(%rbp), %rsi # t1 # t1
    movq -24(%rbp), %rdx # t2 # t2
    movq -32(%rbp), %rcx # t3 # t3
    movq $0, %rax
    call _sdl_setcolor
    movq %rax, -40(%rbp)
    movq $0, %rax
    movq %rax, -48(%rbp)
    movq $0, %rax
    movq %rax, -56(%rbp)
    movq $1440, %rax
    movq %rax, -64(%rbp)
    movq $1080, %rax
    movq %rax, -72(%rbp)
    movq -48(%rbp), %rdi # t5 # t5
    movq -56(%rbp), %rsi # t6 # t6
    movq -64(%rbp), %rdx # t7 # t7
    movq -72(%rbp), %rcx # t8 # t8
    movq $0, %rax
    call _sdl_fillrect
    movq %rax, -80(%rbp)
    movq $8, %rax
    movq %rax, -88(%rbp)
    movq $17, %rax
    movq %rax, -96(%rbp)
    movq $32, %rax
    movq %rax, -104(%rbp)
    movq $4, %rax
    movq %rax, -112(%rbp)
    movq -104(%rbp), %rax # t10 # t10
    imulq -112(%rbp), %rax
    movq %rax, -120(%rbp)
    movq %rax, -128(%rbp)
    movq $16, %rax
    movq %rax, -136(%rbp)
    movq $4, %rax
    movq %rax, -144(%rbp)
    movq -136(%rbp), %rax # t13 # t13
    imulq -144(%rbp), %rax
    movq %rax, -152(%rbp)
    movq %rax, -160(%rbp)
    movq $0, %rax
    movq %rax, -168(%rbp)
for_start_16: 
    movq -168(%rbp), %rax # x # x
    movq %rax, -176(%rbp)
    movq -88(%rbp), %rax # grid_w # grid_w
    movq %rax, -184(%rbp)
    movq -176(%rbp), %rax # t18 # t18
    movq -184(%rbp), %rdi # t19 # t19
    cmpq %rdi, %rax
    setl %cl
    movzbq %cl, %rdx
    movq %rdx, -192(%rbp)
    movq -192(%rbp), %rax # t20 # t20
    cmpq $0, %rax
    je for_end_16
    movq $0, %rax
    movq %rax, -200(%rbp)
for_start_21: 
    movq -200(%rbp), %rax # y # y
    movq %rax, -208(%rbp)
    movq -96(%rbp), %rax # grid_h # grid_h
    movq %rax, -216(%rbp)
    movq -208(%rbp), %rax # t23 # t23
    movq -216(%rbp), %rdi # t24 # t24
    cmpq %rdi, %rax
    setl %cl
    movzbq %cl, %rdx
    movq %rdx, -224(%rbp)
    movq -224(%rbp), %rax # t25 # t25
    cmpq $0, %rax
    je for_end_21
    movq -168(%rbp), %rax # x # x
    movq %rax, -232(%rbp)
    imulq -128(%rbp), %rax
    movq %rax, -240(%rbp)
    movq %rax, -248(%rbp)
    movq -200(%rbp), %rax # y # y
    movq %rax, -256(%rbp)
    imulq -160(%rbp), %rax
    movq %rax, -264(%rbp)
    movq %rax, -272(%rbp)
    movq $0, %rax
    call _rand
    movq %rax, -280(%rbp)
    movq $255, %rax
    movq %rax, -288(%rbp)
    movq -280(%rbp), %rax # t30 # t30
    cqto
    movq -288(%rbp), %rdi # t31 # t31
    idivq %rdi
    movq %rdx, -296(%rbp)
    movq $0, %rax
    call _rand
    movq %rax, -304(%rbp)
    movq $255, %rax
    movq %rax, -312(%rbp)
    movq -304(%rbp), %rax # t33 # t33
    cqto
    movq -312(%rbp), %rdi # t34 # t34
    idivq %rdi
    movq %rdx, -320(%rbp)
    movq $0, %rax
    call _rand
    movq %rax, -328(%rbp)
    movq $255, %rax
    movq %rax, -336(%rbp)
    movq -328(%rbp), %rax # t36 # t36
    cqto
    movq -336(%rbp), %rdi # t37 # t37
    idivq %rdi
    movq %rdx, -344(%rbp)
    movq $255, %rax
    movq %rax, -352(%rbp)
    movq -296(%rbp), %rdi # t32 # t32
    movq -320(%rbp), %rsi # t35 # t35
    movq -344(%rbp), %rdx # t38 # t38
    movq -352(%rbp), %rcx # t39 # t39
    movq $0, %rax
    call _sdl_setcolor
    movq %rax, -360(%rbp)
    movq -248(%rbp), %rdi # grid_x # grid_x
    movq -272(%rbp), %rsi # grid_y # grid_y
    movq -128(%rbp), %rdx # block_size_w # block_size_w
    movq -160(%rbp), %rcx # block_size_h # block_size_h
    movq $0, %rax
    call _sdl_fillrect
    movq %rax, -368(%rbp)
for_post_22: 
    movq -200(%rbp), %rax # y # y
    movq %rax, -376(%rbp)
    movq $1, %rax
    movq %rax, -384(%rbp)
    movq -376(%rbp), %rax # t42 # t42
    addq -384(%rbp), %rax
    movq %rax, -392(%rbp)
    movq -392(%rbp), %rdx # t44 # t44
    movq %rdx, -200(%rbp)
    jmp for_start_21
for_end_21: 
for_post_17: 
    movq -168(%rbp), %rax # x # x
    movq %rax, -400(%rbp)
    movq $1, %rax
    movq %rax, -408(%rbp)
    movq -400(%rbp), %rax # t45 # t45
    addq -408(%rbp), %rax
    movq %rax, -416(%rbp)
    movq -416(%rbp), %rdx # t47 # t47
    movq %rdx, -168(%rbp)
    jmp for_start_16
for_end_16: 
    movq $0, %rax
    movq %rax, -424(%rbp)
    leave
    ret
.globl _init
_init:
    pushq %rbp
    movq %rsp, %rbp
    subq $160, %rsp
    movq $0, %rcx
    movq $0, %rax
    call _sdl_init
    movq %rax, -8(%rbp)
    leaq t50(%rip), %rax
    movq %rax, -16(%rbp)
    movq $1440, %rax
    movq %rax, -24(%rbp)
    movq $1080, %rax
    movq %rax, -32(%rbp)
    movq -16(%rbp), %rdi # t50 # t50
    movq -24(%rbp), %rsi # t51 # t51
    movq -32(%rbp), %rdx # t52 # t52
    movq $0, %rax
    call _sdl_create
    movq %rax, -40(%rbp)
while_start_54: 
    movq $0, %rax
    call _sdl_pump
    movq %rax, -48(%rbp)
    cmpq $0, %rax
    je while_end_54
    movq $0, %rax
    call _sdl_clear
    movq %rax, -56(%rbp)
    movq $0, %rax
    call _draw_grid
    movq %rax, -64(%rbp)
    movq $0, %rax
    call _sdl_flip
    movq %rax, -72(%rbp)
    jmp while_start_54
while_end_54: 
    movq $0, %rax
    call _sdl_release
    movq %rax, -80(%rbp)
    movq $0, %rax
    call _sdl_quit
    movq %rax, -88(%rbp)
    movq $0, %rax
    movq %rax, -96(%rbp)
    leave
    ret

