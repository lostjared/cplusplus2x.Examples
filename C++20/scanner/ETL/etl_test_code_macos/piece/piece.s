.section __TEXT,__cstring
s: .asciz "test"
t137: .asciz "MasterPiece"
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
    subq $928, %rsp
    movq $0, %rcx
    movq %rdi, -8(%rbp)
    movq %rsi, -16(%rbp)
    movq $0, %rax
    movq %rax, -24(%rbp)
    movq $0, %rax
    movq %rax, -32(%rbp)
    movq $0, %rax
    movq %rax, -40(%rbp)
    movq $255, %rax
    movq %rax, -48(%rbp)
    movq -24(%rbp), %rdi # t0 # t0
    movq -32(%rbp), %rsi # t1 # t1
    movq -40(%rbp), %rdx # t2 # t2
    movq -48(%rbp), %rcx # t3 # t3
    movq $0, %rax
    call _sdl_setcolor
    movq %rax, -56(%rbp)
    movq $0, %rax
    movq %rax, -64(%rbp)
    movq $0, %rax
    movq %rax, -72(%rbp)
    movq $1440, %rax
    movq %rax, -80(%rbp)
    movq $1080, %rax
    movq %rax, -88(%rbp)
    movq -64(%rbp), %rdi # t5 # t5
    movq -72(%rbp), %rsi # t6 # t6
    movq -80(%rbp), %rdx # t7 # t7
    movq -88(%rbp), %rcx # t8 # t8
    movq $0, %rax
    call _sdl_fillrect
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
    movq $20, %rax
    movq %rax, -168(%rbp)
    movq $30, %rax
    movq %rax, -176(%rbp)
    movq $1440, %rax
    movq %rax, -184(%rbp)
    cqto
    idivq -128(%rbp)
    movq %rax, -192(%rbp)
    movq %rax, -200(%rbp)
    movq $1080, %rax
    movq %rax, -208(%rbp)
    cqto
    idivq -160(%rbp)
    movq %rax, -216(%rbp)
    movq %rax, -224(%rbp)
    movq $0, %rax
    movq %rax, -232(%rbp)
for_start_20: 
    movq -232(%rbp), %rax # x # x
    movq %rax, -240(%rbp)
    movq -200(%rbp), %rdi # grid_w # grid_w
    cmpq %rdi, %rax
    setl %cl
    movzbq %cl, %rdx
    movq %rdx, -248(%rbp)
    movq -248(%rbp), %rax # t23 # t23
    cmpq $0, %rax
    je for_end_20
    movq $0, %rax
    movq %rax, -256(%rbp)
for_start_24: 
    movq -256(%rbp), %rax # y # y
    movq %rax, -264(%rbp)
    movq -224(%rbp), %rdi # grid_h # grid_h
    cmpq %rdi, %rax
    setl %cl
    movzbq %cl, %rdx
    movq %rdx, -272(%rbp)
    movq -272(%rbp), %rax # t27 # t27
    cmpq $0, %rax
    je for_end_24
    movq -232(%rbp), %rax # x # x
    movq %rax, -280(%rbp)
    imulq -128(%rbp), %rax
    movq %rax, -288(%rbp)
    movq -168(%rbp), %rax # offset_left # offset_left
    movq %rax, -296(%rbp)
    movq -288(%rbp), %rax # t29 # t29
    addq -296(%rbp), %rax
    movq %rax, -304(%rbp)
    movq %rax, -312(%rbp)
    movq -256(%rbp), %rax # y # y
    movq %rax, -320(%rbp)
    imulq -160(%rbp), %rax
    movq %rax, -328(%rbp)
    movq -176(%rbp), %rax # offset_top # offset_top
    movq %rax, -336(%rbp)
    movq -328(%rbp), %rax # t33 # t33
    addq -336(%rbp), %rax
    movq %rax, -344(%rbp)
    movq %rax, -352(%rbp)
    movq -8(%rbp), %rax # grid # grid
    movq %rax, -360(%rbp)
    movq -232(%rbp), %rax # x # x
    movq %rax, -368(%rbp)
    imulq -200(%rbp), %rax
    movq %rax, -376(%rbp)
    movq -256(%rbp), %rax # y # y
    movq %rax, -384(%rbp)
    movq -376(%rbp), %rax # t38 # t38
    addq -384(%rbp), %rax
    movq %rax, -392(%rbp)
    movq -360(%rbp), %rdi # t36 # t36
    movq -392(%rbp), %rsi # t40 # t40
    movq $0, %rax
    call _mematb
    movq %rax, -400(%rbp)
    movq %rax, -408(%rbp)
    movq -16(%rbp), %rax # mode # mode
    movq %rax, -416(%rbp)
    movq $1, %rax
    movq %rax, -424(%rbp)
    movq -416(%rbp), %rsi # t42 # t42
    movq -424(%rbp), %rdi # t43 # t43
    cmpq %rdi, %rsi
    sete %cl
    movzbq %cl, %rdx
    movq %rdx, -432(%rbp)
    movq -432(%rbp), %rax # t44 # t44
    cmpq $0, %rax
    je sublabel_else_45
sublabel_if_45: 
    movq $0, %rax
    movq %rax, -440(%rbp)
    movq -408(%rbp), %rsi # color # color
    movq -440(%rbp), %rdi # t46 # t46
    cmpq %rdi, %rsi
    sete %cl
    movzbq %cl, %rdx
    movq %rdx, -448(%rbp)
    movq -448(%rbp), %rax # t47 # t47
    cmpq $0, %rax
    je sublabel_else_48
sublabel_if_48: 
    movq $0, %rax
    movq %rax, -456(%rbp)
    movq $0, %rax
    movq %rax, -464(%rbp)
    movq $0, %rax
    movq %rax, -472(%rbp)
    movq $255, %rax
    movq %rax, -480(%rbp)
    movq -456(%rbp), %rdi # t49 # t49
    movq -464(%rbp), %rsi # t50 # t50
    movq -472(%rbp), %rdx # t51 # t51
    movq -480(%rbp), %rcx # t52 # t52
    movq $0, %rax
    call _sdl_setcolor
    movq %rax, -488(%rbp)
    jmp sublabel_end_48
sublabel_else_48: 
sublabel_end_48: 
    movq $1, %rax
    movq %rax, -496(%rbp)
    movq -408(%rbp), %rsi # color # color
    movq -496(%rbp), %rdi # t54 # t54
    cmpq %rdi, %rsi
    sete %cl
    movzbq %cl, %rdx
    movq %rdx, -504(%rbp)
    movq -504(%rbp), %rax # t55 # t55
    cmpq $0, %rax
    je sublabel_else_56
sublabel_if_56: 
    movq $255, %rax
    movq %rax, -512(%rbp)
    movq $0, %rax
    movq %rax, -520(%rbp)
    movq $0, %rax
    movq %rax, -528(%rbp)
    movq $255, %rax
    movq %rax, -536(%rbp)
    movq -512(%rbp), %rdi # t57 # t57
    movq -520(%rbp), %rsi # t58 # t58
    movq -528(%rbp), %rdx # t59 # t59
    movq -536(%rbp), %rcx # t60 # t60
    movq $0, %rax
    call _sdl_setcolor
    movq %rax, -544(%rbp)
    jmp sublabel_end_56
sublabel_else_56: 
sublabel_end_56: 
    movq $2, %rax
    movq %rax, -552(%rbp)
    movq -408(%rbp), %rsi # color # color
    movq -552(%rbp), %rdi # t62 # t62
    cmpq %rdi, %rsi
    sete %cl
    movzbq %cl, %rdx
    movq %rdx, -560(%rbp)
    movq -560(%rbp), %rax # t63 # t63
    cmpq $0, %rax
    je sublabel_else_64
sublabel_if_64: 
    movq $0, %rax
    movq %rax, -568(%rbp)
    movq $255, %rax
    movq %rax, -576(%rbp)
    movq $0, %rax
    movq %rax, -584(%rbp)
    movq $255, %rax
    movq %rax, -592(%rbp)
    movq -568(%rbp), %rdi # t65 # t65
    movq -576(%rbp), %rsi # t66 # t66
    movq -584(%rbp), %rdx # t67 # t67
    movq -592(%rbp), %rcx # t68 # t68
    movq $0, %rax
    call _sdl_setcolor
    movq %rax, -600(%rbp)
    jmp sublabel_end_64
sublabel_else_64: 
sublabel_end_64: 
    movq $3, %rax
    movq %rax, -608(%rbp)
    movq -408(%rbp), %rsi # color # color
    movq -608(%rbp), %rdi # t70 # t70
    cmpq %rdi, %rsi
    sete %cl
    movzbq %cl, %rdx
    movq %rdx, -616(%rbp)
    movq -616(%rbp), %rax # t71 # t71
    cmpq $0, %rax
    je sublabel_else_72
sublabel_if_72: 
    movq $0, %rax
    movq %rax, -624(%rbp)
    movq $0, %rax
    movq %rax, -632(%rbp)
    movq $255, %rax
    movq %rax, -640(%rbp)
    movq $255, %rax
    movq %rax, -648(%rbp)
    movq -624(%rbp), %rdi # t73 # t73
    movq -632(%rbp), %rsi # t74 # t74
    movq -640(%rbp), %rdx # t75 # t75
    movq -648(%rbp), %rcx # t76 # t76
    movq $0, %rax
    call _sdl_setcolor
    movq %rax, -656(%rbp)
    jmp sublabel_end_72
sublabel_else_72: 
sublabel_end_72: 
    movq $4, %rax
    movq %rax, -664(%rbp)
    movq -408(%rbp), %rsi # color # color
    movq -664(%rbp), %rdi # t78 # t78
    cmpq %rdi, %rsi
    sete %cl
    movzbq %cl, %rdx
    movq %rdx, -672(%rbp)
    movq -672(%rbp), %rax # t79 # t79
    cmpq $0, %rax
    je sublabel_else_80
sublabel_if_80: 
    movq $255, %rax
    movq %rax, -680(%rbp)
    movq $0, %rax
    movq %rax, -688(%rbp)
    movq $255, %rax
    movq %rax, -696(%rbp)
    movq $25, %rax
    movq %rax, -704(%rbp)
    movq -680(%rbp), %rdi # t81 # t81
    movq -688(%rbp), %rsi # t82 # t82
    movq -696(%rbp), %rdx # t83 # t83
    movq -704(%rbp), %rcx # t84 # t84
    movq $0, %rax
    call _sdl_setcolor
    movq %rax, -712(%rbp)
    jmp sublabel_end_80
sublabel_else_80: 
sublabel_end_80: 
    jmp sublabel_end_45
sublabel_else_45: 
    movq $0, %rax
    call _rand
    movq %rax, -720(%rbp)
    movq $255, %rax
    movq %rax, -728(%rbp)
    movq -720(%rbp), %rax # t86 # t86
    cqto
    movq -728(%rbp), %rdi # t87 # t87
    idivq %rdi
    movq %rdx, -736(%rbp)
    movq $0, %rax
    call _rand
    movq %rax, -744(%rbp)
    movq $255, %rax
    movq %rax, -752(%rbp)
    movq -744(%rbp), %rax # t89 # t89
    cqto
    movq -752(%rbp), %rdi # t90 # t90
    idivq %rdi
    movq %rdx, -760(%rbp)
    movq $0, %rax
    call _rand
    movq %rax, -768(%rbp)
    movq $255, %rax
    movq %rax, -776(%rbp)
    movq -768(%rbp), %rax # t92 # t92
    cqto
    movq -776(%rbp), %rdi # t93 # t93
    idivq %rdi
    movq %rdx, -784(%rbp)
    movq $0, %rax
    call _rand
    movq %rax, -792(%rbp)
    movq $255, %rax
    movq %rax, -800(%rbp)
    movq -792(%rbp), %rax # t95 # t95
    cqto
    movq -800(%rbp), %rdi # t96 # t96
    idivq %rdi
    movq %rdx, -808(%rbp)
    movq -736(%rbp), %rdi # t88 # t88
    movq -760(%rbp), %rsi # t91 # t91
    movq -784(%rbp), %rdx # t94 # t94
    movq -808(%rbp), %rcx # t97 # t97
    movq $0, %rax
    call _sdl_setcolor
    movq %rax, -816(%rbp)
sublabel_end_45: 
    movq -312(%rbp), %rdi # grid_x # grid_x
    movq -352(%rbp), %rsi # grid_y # grid_y
    movq -128(%rbp), %rdx # block_size_w # block_size_w
    movq -160(%rbp), %rcx # block_size_h # block_size_h
    movq $0, %rax
    call _sdl_fillrect
    movq %rax, -824(%rbp)
for_post_25: 
    movq -256(%rbp), %rax # y # y
    movq %rax, -832(%rbp)
    movq $1, %rax
    movq %rax, -840(%rbp)
    movq -832(%rbp), %rax # t100 # t100
    addq -840(%rbp), %rax
    movq %rax, -848(%rbp)
    movq -848(%rbp), %rdx # t102 # t102
    movq %rdx, -256(%rbp)
    jmp for_start_24
for_end_24: 
for_post_21: 
    movq -232(%rbp), %rax # x # x
    movq %rax, -856(%rbp)
    movq $1, %rax
    movq %rax, -864(%rbp)
    movq -856(%rbp), %rax # t103 # t103
    addq -864(%rbp), %rax
    movq %rax, -872(%rbp)
    movq -872(%rbp), %rdx # t105 # t105
    movq %rdx, -232(%rbp)
    jmp for_start_20
for_end_20: 
    movq $0, %rax
    movq %rax, -880(%rbp)
    leave
    ret
.globl _allocate_grid
_allocate_grid:
    pushq %rbp
    movq %rsp, %rbp
    subq $288, %rsp
    movq $0, %rcx
    movq %rdi, -8(%rbp)
    movq %rsi, -16(%rbp)
    movq -8(%rbp), %rax # w # w
    movq %rax, -24(%rbp)
    movq -16(%rbp), %rax # h # h
    movq %rax, -32(%rbp)
    movq -24(%rbp), %rax # t107 # t107
    imulq -32(%rbp), %rax
    movq %rax, -40(%rbp)
    movq -40(%rbp), %rdi # t109 # t109
    movq $0, %rax
    call _malloc
    movq %rax, -48(%rbp)
    movq %rax, -56(%rbp)
    movq -8(%rbp), %rax # w # w
    movq %rax, -64(%rbp)
    movq -16(%rbp), %rax # h # h
    movq %rax, -72(%rbp)
    movq -64(%rbp), %rax # t111 # t111
    imulq -72(%rbp), %rax
    movq %rax, -80(%rbp)
    movq -56(%rbp), %rdi # grid # grid
    movq -80(%rbp), %rsi # t113 # t113
    movq $0, %rax
    call _memclr
    movq %rax, -88(%rbp)
    movq $0, %rax
    movq %rax, -96(%rbp)
for_start_115: 
    movq -8(%rbp), %rax # w # w
    movq %rax, -104(%rbp)
    movq -96(%rbp), %rax # x # x
    movq -104(%rbp), %rdi # t117 # t117
    cmpq %rdi, %rax
    setl %cl
    movzbq %cl, %rdx
    movq %rdx, -112(%rbp)
    movq -112(%rbp), %rax # t118 # t118
    cmpq $0, %rax
    je for_end_115
    movq $0, %rax
    movq %rax, -120(%rbp)
for_start_119: 
    movq -16(%rbp), %rax # h # h
    movq %rax, -128(%rbp)
    movq -120(%rbp), %rax # y # y
    movq -128(%rbp), %rdi # t121 # t121
    cmpq %rdi, %rax
    setl %cl
    movzbq %cl, %rdx
    movq %rdx, -136(%rbp)
    movq -136(%rbp), %rax # t122 # t122
    cmpq $0, %rax
    je for_end_119
    movq -8(%rbp), %rax # w # w
    movq %rax, -144(%rbp)
    movq -96(%rbp), %rax # x # x
    imulq -144(%rbp), %rax
    movq %rax, -152(%rbp)
    addq -120(%rbp), %rax
    movq %rax, -160(%rbp)
    movq $1, %rax
    movq %rax, -168(%rbp)
    movq $0, %rax
    call _rand
    movq %rax, -176(%rbp)
    movq $4, %rax
    movq %rax, -184(%rbp)
    movq -176(%rbp), %rax # t127 # t127
    cqto
    movq -184(%rbp), %rdi # t128 # t128
    idivq %rdi
    movq %rdx, -192(%rbp)
    movq -168(%rbp), %rax # t126 # t126
    addq -192(%rbp), %rax
    movq %rax, -200(%rbp)
    movq -56(%rbp), %rdi # grid # grid
    movq -160(%rbp), %rsi # t125 # t125
    movq -200(%rbp), %rdx # t130 # t130
    movq $0, %rax
    call _memstoreb
    movq %rax, -208(%rbp)
for_post_120: 
    movq $1, %rax
    movq %rax, -216(%rbp)
    movq -120(%rbp), %rax # y # y
    addq -216(%rbp), %rax
    movq %rax, -224(%rbp)
    movq -224(%rbp), %rdx # t133 # t133
    movq %rdx, -120(%rbp)
    jmp for_start_119
for_end_119: 
for_post_116: 
    movq $1, %rax
    movq %rax, -232(%rbp)
    movq -96(%rbp), %rax # x # x
    addq -232(%rbp), %rax
    movq %rax, -240(%rbp)
    movq -240(%rbp), %rdx # t135 # t135
    movq %rdx, -96(%rbp)
    jmp for_start_115
for_end_115: 
    movq -56(%rbp), %rax # grid # grid
    leave
    ret
.globl _init
_init:
    pushq %rbp
    movq %rsp, %rbp
    subq $320, %rsp
    movq $0, %rcx
    movq $0, %rax
    call _sdl_init
    movq %rax, -8(%rbp)
    leaq t137(%rip), %rax
    movq %rax, -16(%rbp)
    movq $1440, %rax
    movq %rax, -24(%rbp)
    movq $1080, %rax
    movq %rax, -32(%rbp)
    movq -16(%rbp), %rdi # t137 # t137
    movq -24(%rbp), %rsi # t138 # t138
    movq -32(%rbp), %rdx # t139 # t139
    movq $0, %rax
    call _sdl_create
    movq %rax, -40(%rbp)
    movq $1440, %rax
    movq %rax, -48(%rbp)
    movq $32, %rax
    movq %rax, -56(%rbp)
    movq $4, %rax
    movq %rax, -64(%rbp)
    movq -56(%rbp), %rax # t142 # t142
    imulq -64(%rbp), %rax
    movq %rax, -72(%rbp)
    movq -48(%rbp), %rax # t141 # t141
    cqto
    idivq -72(%rbp)
    movq %rax, -80(%rbp)
    movq $1080, %rax
    movq %rax, -88(%rbp)
    movq $16, %rax
    movq %rax, -96(%rbp)
    movq $4, %rax
    movq %rax, -104(%rbp)
    movq -96(%rbp), %rax # t147 # t147
    imulq -104(%rbp), %rax
    movq %rax, -112(%rbp)
    movq -88(%rbp), %rax # t146 # t146
    cqto
    idivq -112(%rbp)
    movq %rax, -120(%rbp)
    movq -80(%rbp), %rdi # t145 # t145
    movq -120(%rbp), %rsi # t150 # t150
    movq $0, %rax
    call _allocate_grid
    movq %rax, -128(%rbp)
    movq %rax, -136(%rbp)
    movq $1, %rax
    movq %rax, -144(%rbp)
while_start_152: 
    movq $0, %rax
    call _sdl_pump
    movq %rax, -152(%rbp)
    cmpq $0, %rax
    je while_end_152
    movq $0, %rax
    call _sdl_clear
    movq %rax, -160(%rbp)
    movq -144(%rbp), %rax # mode # mode
    movq %rax, -168(%rbp)
    movq -136(%rbp), %rdi # grid # grid
    movq -168(%rbp), %rsi # t155 # t155
    movq $0, %rax
    call _draw_grid
    movq %rax, -176(%rbp)
    movq $0, %rax
    call _sdl_flip
    movq %rax, -184(%rbp)
    leaq s(%rip), %rax
    movq %rax, -192(%rbp)
    movq $40, %rax
    movq %rax, -200(%rbp)
    movq -200(%rbp), %rdi # t158 # t158
    movq $0, %rax
    call _sdl_keydown
    movq %rax, -208(%rbp)
    cmpq $0, %rax
    je sublabel_else_160
sublabel_if_160: 
    movq $1, %rcx # here
    movq %rcx, -144(%rbp)
    jmp sublabel_end_160
sublabel_else_160: 
    movq $44, %rax
    movq %rax, -216(%rbp)
    movq -216(%rbp), %rdi # t161 # t161
    movq $0, %rax
    call _sdl_keydown
    movq %rax, -224(%rbp)
    cmpq $0, %rax
    je sublabel_else_163
sublabel_if_163: 
    movq $0, %rcx # here
    movq %rcx, -144(%rbp)
    jmp sublabel_end_163
sublabel_else_163: 
sublabel_end_163: 
sublabel_end_160: 
    jmp while_start_152
while_end_152: 
    movq -136(%rbp), %rdi # grid # grid
    movq $0, %rax
    call _free
    movq %rax, -232(%rbp)
    movq $0, %rax
    call _sdl_release
    movq %rax, -240(%rbp)
    movq $0, %rax
    call _sdl_quit
    movq %rax, -248(%rbp)
    movq $0, %rax
    movq %rax, -256(%rbp)
    leave
    ret

